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

static void logger(void *data, int level, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    std::vector<char> buffer(len + 1);

    va_start(ap, fmt);
    vsnprintf(&buffer[0], buffer.size(), fmt, ap);
    va_end(ap);

    if (level <= CHEWING_LOG_INFO) {
        qDebug() << &buffer[0];
    } else if (level <= CHEWING_LOG_WARN) {
        qWarning() << &buffer[0];
    } else if (level <= CHEWING_LOG_ERROR) {
        qCritical() << &buffer[0];
    }
}

UserphraseModel::UserphraseModel(QObject *parent, const char *path)
    :QAbstractListModel(parent)
    ,ctx_(chewing_new2(nullptr, path, logger, nullptr), chewing_delete)
    ,userphrase_()
{
    if (!ctx_) {
        // FIXME: Report error here
        qCritical() << "chewing_new2() fails";
    }
}

int UserphraseModel::rowCount(const QModelIndex &parent) const
{
    return userphrase_.size();
}

QVariant UserphraseModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return userphrase_[index.row()].display_;
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

    // XXX: indexList is in revsrsed order, so first is actual last, and vice
    // verse.
    auto first = indexList.last().row();
    auto last = indexList.first().row();

    emit beginRemoveRows(indexList.first().parent(), first, last);

    foreach(auto item, indexList) {
        auto index = item.row();

        qDebug() << FUNC_NAME
            << userphrase_[index].phrase_
            << userphrase_[index].bopomofo_;
        auto ret = chewing_userphrase_remove(
            ctx_.get(),
            userphrase_[index].phrase_.toUtf8().constData(),
            userphrase_[index].bopomofo_.toUtf8().constData());
        if (ret == 0) {
            // FIXME: std::vector::erase is an inefficient operation.
            userphrase_.erase(userphrase_.begin() + index);
        } else {
            qWarning() << "chewing_userphrase_remove() returns" << ret;
        }
        // FIXME: Handle chewing_userphrase_remove fails.
    }

    emit endRemoveRows();
}

void UserphraseModel::refresh()
{
    qDebug() << FUNC_NAME;

    UserphraseSet userphrase;

    std::vector<char> phrase;
    unsigned int phrase_len;

    std::vector<char> bopomofo;
    unsigned int bopomofo_len;

    chewing_userphrase_enumerate(ctx_.get());
    while (chewing_userphrase_has_next(ctx_.get(), &phrase_len, &bopomofo_len)) {
        phrase.resize(phrase_len);
        bopomofo.resize(bopomofo_len);

        int ret = chewing_userphrase_get(ctx_.get(),
            &phrase[0], phrase.size(),
            &bopomofo[0], bopomofo.size());
        if (ret == -1) {
            qWarning() << "chewing_userphrase_get() returns" << ret;
            continue;
        }

        qDebug() << "Get userphrase:" << &phrase[0] << &bopomofo[0];
        userphrase.insert(Userphrase{
            QString::fromUtf8(&phrase[0]),
            QString::fromUtf8(&bopomofo[0])
        });

    }

    emit beginResetModel();
    userphrase_.swap(userphrase);
    emit endResetModel();

    qDebug() << "Total userphrase" << userphrase_.size();
    emit refreshCompleted(userphrase_.size());
}

bool UserphraseModel::add(const QString &phrase, const QString &bopomofo)
{

    auto ret = chewing_userphrase_add(
        ctx_.get(),
        phrase.toUtf8().constData(),
        bopomofo.toUtf8().constData());

    if (ret == 0) {
        emit beginResetModel();
        userphrase_.insert(Userphrase{
            phrase,
            bopomofo
        });
        emit endResetModel();
    } else {
        qWarning() << "chewing_userphrase_add() returns" << ret;
    }

    return ret == 0;
}

void UserphraseModel::importUserphrase(UserphraseImporter& importer)
{
    qDebug() << FUNC_NAME;
    size_t old_count = userphrase_.size();

    for (auto& i: importer.load()) {
        // FIXME: UserphraseModel shall provide a API to support Userphrase directly.
        add(i.phrase_, i.bopomofo_);
    }

    size_t new_count = userphrase_.size();

    // FIXME: How to know if import file is supported?
    emit importCompleted(true, importer.getPath(), new_count - old_count, new_count);
}

void UserphraseModel::exportUserphrase(UserphraseExporter& exporter)
{
    qDebug() << FUNC_NAME;
    for (auto& i: userphrase_) {
        exporter.addUserphrase(i.phrase_, i.bopomofo_);
    }
}
