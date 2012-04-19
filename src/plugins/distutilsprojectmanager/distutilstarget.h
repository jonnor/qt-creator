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

#ifndef AUTOTOOLSTARGET_H
#define AUTOTOOLSTARGET_H

#include "distutilsbuildconfiguration.h"

#include <projectexplorer/target.h>

namespace DistutilsProjectManager {
namespace Internal {

class DistutilsTargetFactory;
class DistutilsBuildConfiguration;
class DistutilsBuildConfigurationFactory;
class DistutilsProject;

///////////////////////////
//// DistutilsTarget class
///////////////////////////
class DistutilsTarget : public ProjectExplorer::Target
{
    Q_OBJECT
    friend class DistutilsTargetFactory;

public:
    explicit DistutilsTarget(DistutilsProject *parent);

    ProjectExplorer::BuildConfigWidget *createConfigWidget();
    DistutilsProject *distutilsProject() const;
    DistutilsBuildConfigurationFactory *buildConfigurationFactory() const;
    DistutilsBuildConfiguration *activeBuildConfiguration() const;
    QString defaultBuildDirectory() const;

protected:
    bool fromMap(const QVariantMap &map);

private:
    DistutilsBuildConfigurationFactory *m_buildConfigurationFactory;
};


//////////////////////////////////
//// DistutilsTargetFactory class
//////////////////////////////////
class DistutilsTargetFactory : public ProjectExplorer::ITargetFactory
{
    Q_OBJECT

public:
    explicit DistutilsTargetFactory(QObject *parent = 0);

    bool supportsTargetId(const QString &id) const;

    QStringList supportedTargetIds() const;
    QString displayNameForId(const QString &id) const;

    bool canCreate(ProjectExplorer::Project *parent, const QString &id) const;
    DistutilsTarget *create(ProjectExplorer::Project *parent, const QString &id);
    bool canRestore(ProjectExplorer::Project *parent, const QVariantMap &map) const;
    DistutilsTarget *restore(ProjectExplorer::Project *parent, const QVariantMap &map);
};

} // namespace Internal
} // namespace DistutilsProjectManager

#endif // AUTOTOOLSTARGET_H
