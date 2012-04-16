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

#ifndef AUTOTOOLSOPENPROJECTWIZARD_H
#define AUTOTOOLSOPENPROJECTWIZARD_H

#include <utils/wizard.h>
#include <utils/pathchooser.h>

namespace DistutilsProjectManager {
namespace Internal {

class DistutilsManager;

class DistutilsOpenProjectWizard : public Utils::Wizard
{
    Q_OBJECT

public:
    enum PageId
    {
        BuildPathPageId
    };

    DistutilsOpenProjectWizard(DistutilsManager *manager,
                               const QString &sourceDirectory,
                               QWidget *parent = 0);

    QString buildDirectory() const;
    QString sourceDirectory() const;
    void setBuildDirectory(const QString &directory);
    DistutilsManager *distutilsManager() const;

private:
   void init();
   DistutilsManager *m_manager;
   QString m_buildDirectory;
   QString m_sourceDirectory;
};

class BuildPathPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit BuildPathPage(DistutilsOpenProjectWizard *wizard);

private slots:
    void buildDirectoryChanged();

private:
    DistutilsOpenProjectWizard *m_wizard;
    Utils::PathChooser *m_pc;
};

} // namespace Internal
} // namespace DistutilsProjectManager
#endif //AUTOTOOLSOPENPROJECTWIZARD_H
