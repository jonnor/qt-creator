/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
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
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
**
**************************************************************************/

#ifndef OPENPAGESSWICHER_H
#define OPENPAGESSWICHER_H

#include <QtGui/QWidget>

QT_FORWARD_DECLARE_CLASS(QModelIndex)

namespace Help {
    namespace Internal {

class OpenPagesModel;
class OpenPagesWidget;

class OpenPagesSwicher : public QWidget
{
    Q_OBJECT

public:
    OpenPagesSwicher(OpenPagesModel *model);
    ~OpenPagesSwicher();

    void gotoNextPage();
    void gotoPreviousPage();

    void selectAndHide();
    void selectCurrentPage();

    void setVisible(bool visible);
    void focusInEvent(QFocusEvent *event);
    bool eventFilter(QObject *object, QEvent *event);

signals:
    void closePage(const QModelIndex &index);
    void setCurrentPage(const QModelIndex &index);

private:
    void selectPageUpDown(int summand);

private:
    OpenPagesModel *m_openPagesModel;
    OpenPagesWidget *m_openPagesWidget;
};

    }   // namespace Internal
}   // namespace Help

#endif  // OPENPAGESSWICHER_H
