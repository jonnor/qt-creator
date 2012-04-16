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

#ifndef AUTOTOOLSBUILDCONFIGURATION_H
#define AUTOTOOLSBUILDCONFIGURATION_H

#include <projectexplorer/buildconfiguration.h>

namespace DistutilsProjectManager {
namespace Internal {

class DistutilsTarget;
class DistutilsBuildConfigurationFactory;

class DistutilsBuildConfiguration : public ProjectExplorer::BuildConfiguration
{
    Q_OBJECT
    friend class DistutilsBuildConfigurationFactory;

public:
    explicit DistutilsBuildConfiguration(DistutilsTarget *parent);

    DistutilsTarget *distutilsTarget() const;
    QString buildDirectory() const;
    void setBuildDirectory(const QString &buildDirectory);
    QVariantMap toMap() const;
    ProjectExplorer::IOutputParser *createOutputParser() const;
    BuildType buildType() const;

protected:
    DistutilsBuildConfiguration(DistutilsTarget *parent, const QString &id);
    DistutilsBuildConfiguration(DistutilsTarget *parent, DistutilsBuildConfiguration *source);

    bool fromMap(const QVariantMap &map);

private:
    QString m_buildDirectory;
};

class DistutilsBuildConfigurationFactory : public ProjectExplorer::IBuildConfigurationFactory
{
    Q_OBJECT

public:
    explicit DistutilsBuildConfigurationFactory(QObject *parent = 0);

    QStringList availableCreationIds(ProjectExplorer::Target *parent) const;
    QString displayNameForId(const QString &id) const;

    bool canCreate(ProjectExplorer::Target *parent, const QString &id) const;
    DistutilsBuildConfiguration *create(ProjectExplorer::Target *parent, const QString &id);
    DistutilsBuildConfiguration *createDefaultConfiguration(DistutilsTarget *target) const;
    bool canClone(ProjectExplorer::Target *parent, ProjectExplorer::BuildConfiguration *source) const;
    DistutilsBuildConfiguration *clone(ProjectExplorer::Target *parent, ProjectExplorer::BuildConfiguration *source);
    bool canRestore(ProjectExplorer::Target *parent, const QVariantMap &map) const;
    DistutilsBuildConfiguration *restore(ProjectExplorer::Target *parent, const QVariantMap &map);
};

} // namespace Internal
} // namespace DistutilsProjectManager
#endif // AUTOTOOLSBUILDCONFIGURATION_H
