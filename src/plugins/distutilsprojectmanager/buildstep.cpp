/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010-2011 Openismus GmbH.
**   Authors: Peter Penz (ppenz@openismus.com)
**            Patricia Santana Cruz (patriciasantanacruz@gmail.com)
**
** Contact: Nokia Corporation (info@qt.nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at info@qt.nokia.com.
**
**************************************************************************/

#include "buildstep.h"
#include "distutilsproject.h"
#include "distutilsprojectconstants.h"
#include "distutilsbuildconfiguration.h"
#include "distutilstarget.h"

#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/toolchain.h>
#include <projectexplorer/gnumakeparser.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <utils/qtcprocess.h>

#include <QVariantMap>
#include <QLineEdit>
#include <QFormLayout>

using namespace DistutilsProjectManager;
using namespace DistutilsProjectManager::Internal;
using namespace DistutilsProjectManager::Constants;

const char BUILD_STEP_ID[] = "DistutilsProjectManager.BuildStep";
const char BUILD_STEP_ADDITIONAL_ARGUMENTS_KEY[] = "DistutilsProjectManager.BuildStep.AdditionalArguments";
const char CLEAN_KEY[]  = "DistutilsProjectManager.MakeStep.Clean";
const char BUILD_TARGETS_KEY[] = "DistutilsProjectManager.MakeStep.BuildTargets";

//////////////////////////
// MakeStepFactory class
//////////////////////////
BuildStepFactory::BuildStepFactory(QObject *parent) :
    IBuildStepFactory(parent)
{
}

QStringList BuildStepFactory::availableCreationIds(ProjectExplorer::BuildStepList *parent) const
{
    if (parent->target()->project()->id() == QLatin1String(DISTUTILS_PROJECT_ID))
        return QStringList() << QLatin1String(BUILD_STEP_ID);
    return QStringList();
}

QString BuildStepFactory::displayNameForId(const QString &id) const
{
    if (id == QLatin1String(BUILD_STEP_ID))
        return tr("Make", "Display name for DistutilsProjectManager::MakeStep id.");
    return QString();
}

bool BuildStepFactory::canCreate(ProjectExplorer::BuildStepList *parent, const QString &id) const
{
    if (parent->target()->project()->id() != QLatin1String(DISTUTILS_PROJECT_ID))
        return false;

    if (parent->id() != QLatin1String(ProjectExplorer::Constants::BUILDSTEPS_BUILD))
        return false;

    return QLatin1String(BUILD_STEP_ID) == id;
}

ProjectExplorer::BuildStep *BuildStepFactory::create(ProjectExplorer::BuildStepList *parent, const QString &id)
{
    if (!canCreate(parent, id))
        return 0;
    return new BuildStep(parent);
}

bool BuildStepFactory::canClone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *source) const
{
    return canCreate(parent, source->id());
}

ProjectExplorer::BuildStep *BuildStepFactory::clone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *source)
{
    if (!canClone(parent, source))
        return 0;
    return new BuildStep(parent, static_cast<BuildStep *>(source));
}

bool BuildStepFactory::canRestore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map) const
{
    QString id = ProjectExplorer::idFromMap(map);
    return canCreate(parent, id);
}

ProjectExplorer::BuildStep *BuildStepFactory::restore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return 0;
    BuildStep *bs = new BuildStep(parent);
    if (bs->fromMap(map))
        return bs;
    delete bs;
    return 0;
}

/////////////////////
// MakeStep class
/////////////////////
BuildStep::BuildStep(ProjectExplorer::BuildStepList* bsl) :
    AbstractProcessStep(bsl, QLatin1String(BUILD_STEP_ID)),
    m_clean(false)
{
    ctor();
}

BuildStep::BuildStep(ProjectExplorer::BuildStepList *bsl, const QString &id) :
    AbstractProcessStep(bsl, id),
    m_clean(false)
{
    ctor();
}

BuildStep::BuildStep(ProjectExplorer::BuildStepList *bsl, BuildStep *bs) :
    AbstractProcessStep(bsl, bs),
    m_buildTargets(bs->m_buildTargets),
    m_additionalArguments(bs->additionalArguments()),
    m_clean(bs->m_clean)
{
    ctor();
}

void BuildStep::ctor()
{
    setDefaultDisplayName(tr("Make"));
}

DistutilsBuildConfiguration *BuildStep::distutilsBuildConfiguration() const
{
    return static_cast<DistutilsBuildConfiguration *>(buildConfiguration());
}

void BuildStep::setClean(bool clean)
{
    m_clean = clean;
}

bool BuildStep::init()
{
    DistutilsBuildConfiguration *bc = distutilsBuildConfiguration();

    QString arguments = Utils::QtcProcess::joinArgs(m_buildTargets);
    Utils::QtcProcess::addArgs(&arguments, additionalArguments());

    setIgnoreReturnValue(m_clean);

    ProjectExplorer::ProcessParameters *pp = processParameters();
    pp->setMacroExpander(bc->macroExpander());
    pp->setEnvironment(bc->environment());
    pp->setWorkingDirectory(bc->buildDirectory());
    pp->setCommand(bc->toolChain()->makeCommand());
    pp->setArguments(arguments);

    /*
    setOutputParser(new GnuMakeParser());
    if (bc->distutilsTarget()->distutilsProject()->toolChain())
        appendOutputParser(bc->distutilsTarget()->distutilsProject()->toolChain()->outputParser());
    outputParser()->setWorkingDirectory(pp->effectiveWorkingDirectory());
    */

    return AbstractProcessStep::init();
}

void BuildStep::run(QFutureInterface<bool> &interface)
{
    AbstractProcessStep::run(interface);
}

ProjectExplorer::BuildStepConfigWidget *BuildStep::createConfigWidget()
{
    return new MakeStepConfigWidget(this);
}

bool BuildStep::immutable() const
{
    return false;
}

void BuildStep::setBuildTarget(const QString &target, bool on)
{
    QStringList old = m_buildTargets;
    if (on && !old.contains(target))
         old << target;
    else if (!on && old.contains(target))
        old.removeOne(target);

    m_buildTargets = old;
}

void BuildStep::setAdditionalArguments(const QString &list)
{
    if (list == m_additionalArguments)
        return;

    m_additionalArguments = list;

    emit additionalArgumentsChanged(list);
}

QString BuildStep::additionalArguments() const
{
    return m_additionalArguments;
}

QVariantMap BuildStep::toMap() const
{
    QVariantMap map = AbstractProcessStep::toMap();

    map.insert(QLatin1String(BUILD_TARGETS_KEY), m_buildTargets);
    map.insert(QLatin1String(BUILD_STEP_ADDITIONAL_ARGUMENTS_KEY), m_additionalArguments);
    map.insert(QLatin1String(CLEAN_KEY), m_clean);
    return map;
}

bool BuildStep::fromMap(const QVariantMap &map)
{
    m_buildTargets = map.value(QLatin1String(BUILD_TARGETS_KEY)).toStringList();
    m_additionalArguments = map.value(QLatin1String(BUILD_STEP_ADDITIONAL_ARGUMENTS_KEY)).toString();
    m_clean = map.value(QLatin1String(CLEAN_KEY)).toBool();

    return ProjectExplorer::BuildStep::fromMap(map);
}

///////////////////////////////
// MakeStepConfigWidget class
///////////////////////////////
MakeStepConfigWidget::MakeStepConfigWidget(BuildStep *makeStep) :
    m_makeStep(makeStep),
    m_summaryText(),
    m_additionalArguments(0)
{
    QFormLayout *fl = new QFormLayout(this);
    fl->setMargin(0);
    fl->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
    setLayout(fl);

    m_additionalArguments = new QLineEdit(this);
    fl->addRow(tr("Arguments:"), m_additionalArguments);
    m_additionalArguments->setText(m_makeStep->additionalArguments());

    updateDetails();

    connect(m_additionalArguments, SIGNAL(textChanged(QString)),
            makeStep, SLOT(setAdditionalArguments(QString)));
    connect(makeStep, SIGNAL(additionalArgumentsChanged(QString)),
            this, SLOT(updateDetails()));
}

QString MakeStepConfigWidget::displayName() const
{
    return tr("Make", "DistutilsProjectManager::MakeStepConfigWidget display name.");
}

QString MakeStepConfigWidget::summaryText() const
{
    return m_summaryText;
}

void MakeStepConfigWidget::updateDetails()
{
    DistutilsBuildConfiguration *bc = m_makeStep->distutilsBuildConfiguration();
    ProjectExplorer::ToolChain *tc = bc->toolChain();

    if (tc) {
        QString arguments = Utils::QtcProcess::joinArgs(m_makeStep->m_buildTargets);
        Utils::QtcProcess::addArgs(&arguments, m_makeStep->additionalArguments());

        ProjectExplorer::ProcessParameters param;
        param.setMacroExpander(bc->macroExpander());
        param.setEnvironment(bc->environment());
        param.setWorkingDirectory(bc->buildDirectory());
        param.setCommand(tc->makeCommand());
        param.setArguments(arguments);
        m_summaryText = param.summary(displayName());
    } else {
        m_summaryText = tr("<b>Unknown tool chain</b>");
    }

    emit updateSummary();
}
