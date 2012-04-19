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

#include "distutilsprojectplugin.h"
#include "distutilsmanager.h"
#include "distutilstarget.h"
#include "distutilsbuildconfiguration.h"
#include "distutilsprojectconstants.h"
#include "buildstep.h"

#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>

#include <QStringList>
#include <QtPlugin>

using namespace DistutilsProjectManager::Internal;

DistutilsProjectPlugin::DistutilsProjectPlugin()
{
}

void DistutilsProjectPlugin::extensionsInitialized()
{
}

bool DistutilsProjectPlugin::initialize(const QStringList &arguments,
                                        QString *errorString)
{
    Q_UNUSED(arguments);

    if (!Core::ICore::mimeDatabase()->addMimeTypes(QLatin1String(":distutilsproject/DistutilsProject.mimetypes.xml"), errorString))
        return false;

    addAutoReleasedObject(new DistutilsTargetFactory);
    addAutoReleasedObject(new BuildStepFactory);
    addAutoReleasedObject(new DistutilsManager);

    return true;
}

Q_EXPORT_PLUGIN(DistutilsProjectPlugin)
