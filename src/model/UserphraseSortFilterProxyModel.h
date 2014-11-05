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

#include <QSortFilterProxyModel>

#include "UserphraseModel.h"

class UserphraseSortFilterProxyModel final : public QSortFilterProxyModel {
    Q_OBJECT

public:
    explicit UserphraseSortFilterProxyModel(QObject * parent = 0):QSortFilterProxyModel(parent) {}
    UserphraseSortFilterProxyModel(const UserphraseSortFilterProxyModel& x) = delete;
    UserphraseSortFilterProxyModel& operator=(const UserphraseSortFilterProxyModel& x) = delete;
    virtual ~UserphraseSortFilterProxyModel() = default;


    virtual void setSourceModel(UserphraseModel* model) {
        QSortFilterProxyModel::setSourceModel(model);
    }

    UserphraseModel* sourceModel () const {
        return dynamic_cast<UserphraseModel*>(QSortFilterProxyModel::sourceModel());
    }

public slots:
    void add(const QString &phrase, const QString &bopomofo);
    void remove(QModelIndexList indexList);
};
