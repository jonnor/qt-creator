/**************************************************************************
**
** This file is part of Qt Creator Instrumentation Tools
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


#ifndef ANALYZER_INTERNAL_MEMCHECKCONFIGWIDGET_H
#define ANALYZER_INTERNAL_MEMCHECKCONFIGWIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
class QStandardItemModel;

namespace Ui {
class MemcheckConfigWidget;
}
QT_END_NAMESPACE

namespace Analyzer {
namespace Internal {

class AbstractMemcheckSettings;

class MemcheckConfigWidget : public QWidget
{
    Q_OBJECT

public:
    MemcheckConfigWidget(AbstractMemcheckSettings *settings, QWidget *parent);
    virtual ~MemcheckConfigWidget();

    void setSuppressions(const QStringList &files);
    QStringList suppressions() const;

    // ### remove the following?
    int numCallers() const;
    bool trackOrigins() const;

public slots:
    void setNumCallers(int callers);
    void setTrackOrigins(bool enable);

    void slotAddSuppression();
    void slotRemoveSuppression();
    void slotSuppressionsRemoved(const QStringList &files);
    void slotSuppressionsAdded(const QStringList &files);

private:
    AbstractMemcheckSettings *m_settings;
    QStandardItemModel *m_model;
    Ui::MemcheckConfigWidget *m_ui;
};

}
}

#endif // ANALYZER_INTERNAL_MEMCHECKCONFIGWIDGET_H