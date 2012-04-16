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

#include "distutilsbuildconfiguration.h"
#include "distutilsproject.h"
#include "distutilstarget.h"
#include "distutilsprojectconstants.h"
#include "buildstep.h"

#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/toolchain.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/customexecutablerunconfiguration.h>
#include <utils/qtcassert.h>

#include <QInputDialog>

using namespace DistutilsProjectManager;
using namespace DistutilsProjectManager::Constants;
using namespace Internal;
using namespace ProjectExplorer;
using namespace ProjectExplorer::Constants;

//////////////////////////////////////
// DistutilsBuildConfiguration class
//////////////////////////////////////
DistutilsBuildConfiguration::DistutilsBuildConfiguration(DistutilsTarget *parent)
    : BuildConfiguration(parent, QLatin1String(DISTUTILS_BUILDCCONFIG_ID))
{
    m_buildDirectory = distutilsTarget()->defaultBuildDirectory();
}

DistutilsBuildConfiguration::DistutilsBuildConfiguration(DistutilsTarget *parent, const QString &id)
    : BuildConfiguration(parent, id)
{
}

DistutilsBuildConfiguration::DistutilsBuildConfiguration(DistutilsTarget *parent, DistutilsBuildConfiguration *source)
    : BuildConfiguration(parent, source),
      m_buildDirectory(source->m_buildDirectory)
{
    cloneSteps(source);
}

QVariantMap DistutilsBuildConfiguration::toMap() const
{
    QVariantMap map = BuildConfiguration::toMap();
    map.insert(QLatin1String(BUILD_DIRECTORY_KEY), m_buildDirectory);
    return map;
}

bool DistutilsBuildConfiguration::fromMap(const QVariantMap &map)
{
    if (!BuildConfiguration::fromMap(map))
        return false;

    m_buildDirectory = map.value(QLatin1String(BUILD_DIRECTORY_KEY), distutilsTarget()->defaultBuildDirectory()).toString();
    return true;
}

QString DistutilsBuildConfiguration::buildDirectory() const
{
    return m_buildDirectory;
}

void DistutilsBuildConfiguration::setBuildDirectory(const QString &buildDirectory)
{
    if (m_buildDirectory == buildDirectory)
        return;
    m_buildDirectory = buildDirectory;
    emit buildDirectoryChanged();
}

DistutilsTarget *DistutilsBuildConfiguration::distutilsTarget() const
{
    return static_cast<DistutilsTarget *>(target());
}

IOutputParser *DistutilsBuildConfiguration::createOutputParser() const
{
    ToolChain *tc = distutilsTarget()->distutilsProject()->toolChain();
    if (tc)
        return tc->outputParser();
    return 0;
}

//////////////////////////////////////
// DistutilsBuildConfiguration class
//////////////////////////////////////
DistutilsBuildConfigurationFactory::DistutilsBuildConfigurationFactory(QObject *parent) :
    IBuildConfigurationFactory(parent)
{
}

QStringList DistutilsBuildConfigurationFactory::availableCreationIds(Target *parent) const
{
    if (!qobject_cast<DistutilsTarget *>(parent))
        return QStringList();
    return QStringList() << QLatin1String(DISTUTILS_BUILDCCONFIG_ID);
}

QString DistutilsBuildConfigurationFactory::displayNameForId(const QString &id) const
{
    if (id == QLatin1String(DISTUTILS_BUILDCCONFIG_ID))
        return tr("Build");
    return QString();
}

bool DistutilsBuildConfigurationFactory::canCreate(Target *parent, const QString &id) const
{
    if (!qobject_cast<DistutilsTarget *>(parent))
        return false;
    if (id == QLatin1String(DISTUTILS_BUILDCCONFIG_ID))
        return true;
    return false;
}

DistutilsBuildConfiguration *DistutilsBuildConfigurationFactory::create(Target *parent, const QString &id)
{
    if (!canCreate(parent, id))
        return 0;

    DistutilsTarget *t = static_cast<DistutilsTarget *>(parent);
    DistutilsBuildConfiguration *bc = createDefaultConfiguration(t);

    bool ok;
    QString buildConfigurationName = QInputDialog::getText(0,
                          tr("New Configuration"),
                          tr("New configuration name:"),
                          QLineEdit::Normal,
                          QString(),
                          &ok);

    if (!ok || buildConfigurationName.isEmpty())
        return 0;
    bc->setDisplayName(buildConfigurationName);

    t->addBuildConfiguration(bc);
    t->addDeployConfiguration(t->createDeployConfiguration(QLatin1String(DEFAULT_DEPLOYCONFIGURATION_ID)));
    // User needs to choose where the executable file is.
    // TODO: Parse the file in *Anjuta style* to be able to add custom RunConfigurations.
    t->addRunConfiguration(new CustomExecutableRunConfiguration(t));

    return bc;
}

DistutilsBuildConfiguration *DistutilsBuildConfigurationFactory::createDefaultConfiguration(DistutilsTarget *target) const
{
    DistutilsBuildConfiguration *bc = new DistutilsBuildConfiguration(target);
    BuildStepList *buildSteps = bc->stepList(QLatin1String(BUILDSTEPS_BUILD));

    // ### Build Steps Build ###

    // build
    DistutilsProjectManager::Internal::BuildStep *buildStep = new DistutilsProjectManager::Internal::BuildStep(buildSteps);
    buildSteps->insertStep(2, buildStep);
    buildStep->setBuildTarget(QLatin1String("all"),  /*on =*/ true);

    return bc;
}

bool DistutilsBuildConfigurationFactory::canClone(Target *parent, BuildConfiguration *source) const
{
    return canCreate(parent, source->id());
}

DistutilsBuildConfiguration *DistutilsBuildConfigurationFactory::clone(Target *parent, BuildConfiguration *source)
{
    if (!canClone(parent, source))
        return 0;

    DistutilsBuildConfiguration *origin = static_cast<DistutilsBuildConfiguration *>(source);
    DistutilsTarget *target(static_cast<DistutilsTarget *>(parent));
    return new DistutilsBuildConfiguration(target, origin);
}

bool DistutilsBuildConfigurationFactory::canRestore(Target *parent, const QVariantMap &map) const
{
    QString id = idFromMap(map);
    return canCreate(parent, id);
}

DistutilsBuildConfiguration *DistutilsBuildConfigurationFactory::restore(Target *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return 0;
    DistutilsTarget *target(static_cast<DistutilsTarget *>(parent));
    DistutilsBuildConfiguration *bc = new DistutilsBuildConfiguration(target);
    if (bc->fromMap(map))
        return bc;
    delete bc;
    return 0;
}

BuildConfiguration::BuildType DistutilsBuildConfiguration::buildType() const
{
    // TODO: Should I return something different from Unknown?
    return Unknown;
}
