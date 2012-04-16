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

#include "distutilstarget.h"
#include "distutilsproject.h"
#include "distutilsprojectconstants.h"
#include "distutilsbuildsettingswidget.h"
#include "distutilsbuildconfiguration.h"
#include "buildstep.h"

#include <projectexplorer/customexecutablerunconfiguration.h>
#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <extensionsystem/pluginmanager.h>

#include <QApplication>
#include <QStyle>

using namespace DistutilsProjectManager;
using namespace DistutilsProjectManager::Internal;
using namespace ProjectExplorer;

static QString displayNameForId(const QString &id)
{
    if (id == QLatin1String(DistutilsProjectManager::Constants::DEFAULT_DISTUTILS_TARGET_ID))
        return QApplication::translate("DistutilsProjectManager::Internal::DistutilsTarget",
                              "Desktop", "Distutils Default target display name");
    return QString();
}

//////////////////////////
// DistutilsTarget class
//////////////////////////

DistutilsTarget::DistutilsTarget(DistutilsProject *parent) :
    Target(parent, QLatin1String(Constants::DEFAULT_DISTUTILS_TARGET_ID)),
    m_buildConfigurationFactory(new DistutilsBuildConfigurationFactory(this))
{
    setDefaultDisplayName(displayNameForId(id()));
    setIcon(qApp->style()->standardIcon(QStyle::SP_ComputerIcon));
}

BuildConfigWidget *DistutilsTarget::createConfigWidget()
{
    return new DistutilsBuildSettingsWidget(this);
}


DistutilsProject *DistutilsTarget::distutilsProject() const
{
    return static_cast<DistutilsProject *>(project());
}

DistutilsBuildConfiguration *DistutilsTarget::activeBuildConfiguration() const
{
    return static_cast<DistutilsBuildConfiguration *>(Target::activeBuildConfiguration());
}

DistutilsBuildConfigurationFactory *DistutilsTarget::buildConfigurationFactory() const
{
    return m_buildConfigurationFactory;
}

QString DistutilsTarget::defaultBuildDirectory() const
{
    return distutilsProject()->defaultBuildDirectory();
}

bool DistutilsTarget::fromMap(const QVariantMap &map)
{
    return Target::fromMap(map);
}

/////////////////////////////////
// DistutilsTargetFactory class
/////////////////////////////////
DistutilsTargetFactory::DistutilsTargetFactory(QObject *parent) :
    ITargetFactory(parent)
{
}

bool DistutilsTargetFactory::supportsTargetId(const QString &id) const
{
    return id == QLatin1String(Constants::DEFAULT_DISTUTILS_TARGET_ID);
}

QStringList DistutilsTargetFactory::supportedTargetIds() const
{
    return QStringList() << QLatin1String(Constants::DEFAULT_DISTUTILS_TARGET_ID);
}

QString DistutilsTargetFactory::displayNameForId(const QString &id) const
{
    return ::displayNameForId(id);
}

bool DistutilsTargetFactory::canCreate(Project *parent, const QString &id) const
{
    if (!qobject_cast<DistutilsProject *>(parent))
        return false;
    return id == QLatin1String(Constants::DEFAULT_DISTUTILS_TARGET_ID);
}

DistutilsTarget *DistutilsTargetFactory::create(Project *parent, const QString &id)
{
    if (!canCreate(parent, id))
        return 0;

    DistutilsProject *project(static_cast<DistutilsProject *>(parent));
    DistutilsTarget *t = new DistutilsTarget(project);

    // Add default build configuration:
    DistutilsBuildConfigurationFactory *bcf = t->buildConfigurationFactory();
    DistutilsBuildConfiguration *bc = bcf->createDefaultConfiguration(t);
    bc->setDisplayName(tr("Default Build"));

    t->addBuildConfiguration(bc);
    t->addDeployConfiguration(t->createDeployConfiguration(QLatin1String(ProjectExplorer::Constants::DEFAULT_DEPLOYCONFIGURATION_ID)));
    // User needs to choose where the executable file is.
    // TODO: Parse the file in *Anjuta style* to be able to add custom RunConfigurations.
    t->addRunConfiguration(new CustomExecutableRunConfiguration(t));

    return t;
}

bool DistutilsTargetFactory::canRestore(Project *parent, const QVariantMap &map) const
{
    return canCreate(parent, idFromMap(map));
}

DistutilsTarget *DistutilsTargetFactory::restore(Project *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return 0;
    DistutilsProject *distutilsproject(static_cast<DistutilsProject *>(parent));
    DistutilsTarget *target = new DistutilsTarget(distutilsproject);
    if (target->fromMap(map))
        return target;
    delete target;
    return 0;
}
