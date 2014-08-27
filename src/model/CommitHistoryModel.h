/*
 * chewing-editor: Chewing userphrase editor
 * Copyright (C) 2014 Chewing Development Team

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma once

#include <memory>

#include <QAbstractListModel>
#include <QString>

#include <chewing.h>

#include "CommitHistory.h"
//#include "CommitHistoryExporter.h"
#include "CommitHistorySet.h"

class CommitHistoryModel final: public QAbstractListModel {
    Q_OBJECT

public:
    CommitHistoryModel(QObject * parent = 0, const char *path = nullptr);
    CommitHistoryModel(const CommitHistoryModel& x) = delete;
    CommitHistoryModel& operator=(const CommitHistoryModel& x) = delete;
    virtual ~CommitHistoryModel() = default;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void remove(QModelIndexList &&indexList);

    //void exportCommitHistory(CommitHistoryExporter& exporter);

signals:
    void exportCompleted(
        bool result,
        const QString& file,
        size_t exported);
    void removeCommitHistoryCompleted();
    void refreshCompleted(size_t count);

public slots:
    void refresh();

private:
    std::unique_ptr<ChewingContext, void (*)(ChewingContext*)> ctx_;
    CommitHistorySet commitHistory_;
};
