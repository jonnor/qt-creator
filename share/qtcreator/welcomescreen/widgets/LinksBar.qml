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

import QtQuick 1.0

Row {
    id: tabBar
    height: 25

    property alias model: tabs.model
    property int tabBarWidth

    Repeater {
        id: tabs
        height: tabBar.height
        model: parent.model
        delegate:
            Item {
            width: tabBarWidth / tabs.count
            height: tabBar.height

            Rectangle {
                width: parent.width; height: 1
                anchors { bottom: parent.bottom; bottomMargin: 1 }
                color: "#acb2c2"
            }
            BorderImage {
                id: tabBackground
                anchors.fill: parent
                border { top: 1; bottom: 1}
                source: "qrc:welcome/images/tab_inactive.png"
            }
            Text {
                id: text
                horizontalAlignment: Qt.AlignHCenter; verticalAlignment: Qt.AlignVCenter
                anchors.fill: parent
                text: model.modelData.title
                elide: Text.ElideRight
                color: "white"
            }
            MouseArea {
                id: mouseArea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: tabBar.current = index
            }
            states: [
                State {
                    id: activeState; when: tabBar.current == index
                    PropertyChanges { target: tabBackground; source:"qrc:welcome/images/tab_active.png" }
                    PropertyChanges { target: text; color: "black" }
                },
                State {
                    id: hoverState; when: mouseArea.containsMouse
                    PropertyChanges { target: tabBackground; source:"qrc:welcome/images/tab_hover.png" }
                }

            ]
        }
    }
}