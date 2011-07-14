/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include "uploadandinstalltarpackagestep.h"

#include "maemoglobal.h"
#include "maemopackagecreationstep.h"
#include "maemopackageinstaller.h"
#include "qt4maemodeployconfiguration.h"

#include <projectexplorer/buildsteplist.h>

using namespace ProjectExplorer;

namespace RemoteLinux {
namespace Internal {
class UploadAndInstallTarPackageActionPrivate
{
public:
    UploadAndInstallTarPackageActionPrivate() : installer(new MaemoTarPackageInstaller) { }
    ~UploadAndInstallTarPackageActionPrivate() { delete installer; }

    MaemoTarPackageInstaller * const installer;
};
} // namespace Internal

using namespace Internal;

UploadAndInstallTarPackageService::UploadAndInstallTarPackageService(QObject *parent)
    : AbstractUploadAndInstallPackageService(parent),
      m_d(new UploadAndInstallTarPackageActionPrivate)
{
}

UploadAndInstallTarPackageService::~UploadAndInstallTarPackageService()
{
    delete m_d;
}

AbstractMaemoPackageInstaller *UploadAndInstallTarPackageService::packageInstaller() const
{
    return m_d->installer;
}


UploadAndInstallTarPackageStep::UploadAndInstallTarPackageStep(BuildStepList *bsl)
    : AbstractRemoteLinuxDeployStep(bsl, stepId())
{
    ctor();
}

UploadAndInstallTarPackageStep::UploadAndInstallTarPackageStep(BuildStepList *bsl,
        UploadAndInstallTarPackageStep *other)
    : AbstractRemoteLinuxDeployStep(bsl, other)
{
    ctor();
}

void UploadAndInstallTarPackageStep::ctor()
{
    m_deployService = new UploadAndInstallTarPackageService(this);
    setDefaultDisplayName(displayName());
}

bool UploadAndInstallTarPackageStep::isDeploymentPossible(QString *whyNot) const
{
    const AbstractMaemoPackageCreationStep * const pStep
        = MaemoGlobal::earlierBuildStep<MaemoTarPackageCreationStep>(deployConfiguration(), this);
    if (!pStep) {
        if (whyNot)
            *whyNot = tr("No tarball creation step found.");
        return false;
    }
    m_deployService->setPackageFilePath(pStep->packageFilePath());
    return AbstractRemoteLinuxDeployStep::isDeploymentPossible(whyNot);
}

QString UploadAndInstallTarPackageStep::stepId()
{
    return QLatin1String("MaemoUploadAndInstallTarPackageStep");
}

QString UploadAndInstallTarPackageStep::displayName()
{
    return tr("Deploy tarball via SFTP upload");
}

} //namespace RemoteLinux