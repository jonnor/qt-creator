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

#include "distutilsmanager.h"
#include "distutilsproject.h"
#include "distutilsprojectconstants.h"

#include <coreplugin/icore.h>
#include <coreplugin/idocument.h>
#include <coreplugin/messagemanager.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/session.h>

using namespace ProjectExplorer;

namespace DistutilsProjectManager {
namespace Internal {

Project *DistutilsManager::openProject(const QString &fileName, QString *errorString)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    if (canonicalFilePath.isEmpty()) {
        if (errorString)
            *errorString = tr("Failed opening project '%1': Project file does not exist")
                             .arg(QDir::toNativeSeparators(fileName));
        return 0;
    }

    // Check whether the project is already open or not.
    ProjectExplorerPlugin *projectExplorer = ProjectExplorerPlugin::instance();
    foreach (Project *pi, projectExplorer->session()->projects()) {
        if (canonicalFilePath == pi->document()->fileName()) {
            *errorString = tr("Failed opening project '%1': Project already open")
                             .arg(QDir::toNativeSeparators(canonicalFilePath));
            return 0;
        }
    }

    return new DistutilsProject(this, canonicalFilePath);
}

QString DistutilsManager::mimeType() const
{
    return QLatin1String(Constants::SETUP_PY_MIMETYPE);
}

} // namespace Internal
} // namespace DistutilsProjectManager
