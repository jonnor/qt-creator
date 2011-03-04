/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** Author: Milian Wolff, KDAB (milian.wolff@kdab.com)
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** No Commercial Usage
**
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#include "modelhelpers.h"

#include <QString>
#include <QDir>
#include <QPair>

#include "frame.h"

namespace Valgrind {
namespace XmlProtocol {

QString toolTipForFrame(const Frame &frame)
{
        QString location;
        if (!frame.file().isEmpty()) {
            location = frame.directory() + QDir::separator() + frame.file();
            if (frame.line() > 0)
                location += ':' + QString::number(frame.line());
        }

        typedef QPair<QString, QString> StringPair;
        QList<StringPair> lines;

        if (!frame.functionName().isEmpty())
            lines << qMakePair(QObject::tr("Function:"), frame.functionName());
        if (!location.isEmpty())
            lines << qMakePair(QObject::tr("Location:"), location);
        if (frame.instructionPointer())
            lines << qMakePair(QObject::tr("Instruction pointer:"),
                               QString("0x%1").arg(frame.instructionPointer(), 0, 16));
        if (!frame.object().isEmpty())
            lines << qMakePair(QObject::tr("Object:"), frame.object());

        QString html = "<html>"
                       "<head>"
                       "<style>dt { font-weight:bold; } dd { font-family: monospace; }</style>\n"
                       "<body><dl>";

        foreach(const StringPair &pair, lines)
            html += "<dt>" + pair.first + "</dt><dd>" + pair.second + "</dd>\n";

        html += "</dl></body></html>";
        return html;
}

}
}