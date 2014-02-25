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

#include "UserphraseModel.h"

#include <QDebug>

int UserphraseModel::rowCount(const QModelIndex &parent) const
{
    return data_.get()->size();
}

QVariant UserphraseModel::data(const QModelIndex &index, int role) const
{
    const auto& phrase = data_.get()->get(index.row());

    // FIXME: QString shall be cached.
    QString str = QString("%1 (%2)")
            .arg(QString::fromStdString(phrase.phrase_))
            .arg(QString::fromStdString(phrase.bopomofo_));

    switch (role) {
    case Qt::DisplayRole:
        return str;
    case Qt::WhatsThisRole: // FIXME: Provide "What's This?"
        break;
    default:
        break;
    }

    return QVariant();
}

void UserphraseModel::remove(QModelIndexList &&indexList)
{
    if (indexList.empty()) {
        qDebug() << FUNC_NAME << "indexList is empty";
        return;
    }

    qSort(indexList.begin(), indexList.end(), qGreater<QModelIndex>());

    foreach(auto index, indexList) {
        data_.get()->remove(index.row());
    }
}
