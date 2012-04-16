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

#include "distutilsprojectfile.h"
#include "distutilsproject.h"
#include "distutilsprojectconstants.h"

using namespace DistutilsProjectManager;
using namespace DistutilsProjectManager::Internal;

DistutilsProjectFile::DistutilsProjectFile(DistutilsProject *project, const QString &fileName) :
    Core::IDocument(project),
    m_project(project),
    m_fileName(fileName)
{
}

bool DistutilsProjectFile::save(QString *errorString, const QString &fileName, bool autoSave)
{
    Q_UNUSED(errorString);
    Q_UNUSED(fileName);
    Q_UNUSED(autoSave);

    return false;
}

QString DistutilsProjectFile::fileName() const
{
    return m_fileName;
}

QString DistutilsProjectFile::defaultPath() const
{
    return QString();
}

QString DistutilsProjectFile::suggestedFileName() const
{
    return QString();
}

QString DistutilsProjectFile::mimeType() const
{
    return QLatin1String(Constants::SETUP_PY_MIMETYPE);
}

bool DistutilsProjectFile::isModified() const
{
    return false;
}

bool DistutilsProjectFile::isSaveAsAllowed() const
{
    return false;
}

bool DistutilsProjectFile::reload(QString *errorString, ReloadFlag flag, ChangeType type)
{
    Q_UNUSED(errorString);
    Q_UNUSED(flag);
    Q_UNUSED(type);

    return false;
}

void DistutilsProjectFile::rename(const QString &newName)
{
    Q_UNUSED(newName);
}
