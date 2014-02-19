/*
 * chewing-editor: Chewing userphrase editor
 * Copyright (C) 2014 ChangZhuo Chen

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

#include "DefaultUserphraseData.h"

class UserphraseModel final: public QAbstractListModel {
    Q_OBJECT

public:
    UserphraseModel(QObject * parent = 0)
        :QAbstractListModel(parent), data_(new DefaultUserphraseData()) {}
    UserphraseModel(const UserphraseModel& x) = delete;
    UserphraseModel& operator=(const UserphraseModel& x) = delete;
    virtual ~UserphraseModel() = default;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    bool remove(QModelIndexList &&indexList);

protected:
    std::unique_ptr<UserphraseData> data_;
};
