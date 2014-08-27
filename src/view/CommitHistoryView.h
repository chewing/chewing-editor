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

#include <QListView>

#include "CommitHistoryModel.h"

class CommitHistoryView final : public QListView {
    Q_OBJECT

public:
    explicit CommitHistoryView(QWidget *parent = 0) :QListView(parent) {}
    CommitHistoryView(const CommitHistoryView&) = delete;
    CommitHistoryView& operator=(const CommitHistoryView&) = delete;
    virtual ~CommitHistoryView() = default;
    virtual void setModel(CommitHistoryModel *model) {
        QListView::setModel(model);
    }

public slots:
    void remove();
    void resetSelection();

protected:
    CommitHistoryModel* model() const {
        return dynamic_cast<CommitHistoryModel*>(QListView::model());
    }
};
