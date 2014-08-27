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

#include "CommitHistoryModel.h"

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

    if (buffer[len - 1] == '\n') {
        buffer[len - 1] = 0;
    }

    if (level <= CHEWING_LOG_INFO) {
        qDebug() << &buffer[0];
    } else if (level <= CHEWING_LOG_WARN) {
        qWarning() << &buffer[0];
    } else if (level <= CHEWING_LOG_ERROR) {
        qCritical() << &buffer[0];
    }
}

CommitHistoryModel::CommitHistoryModel(QObject *parent, const char *path)
    :QAbstractListModel{parent}
    ,ctx_{chewing_new2(nullptr, path, logger, nullptr), chewing_delete}
    ,commitHistory_{}
{
    if (!ctx_) {
        // FIXME: Report error here
        qCritical() << "chewing_new2() fails";
    }
}

int CommitHistoryModel::rowCount(const QModelIndex &parent) const
{
    return commitHistory_.size();
}

QVariant CommitHistoryModel::data(const QModelIndex &index, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
        return commitHistory_[index.row()].display_;
    case Qt::WhatsThisRole: // FIXME: Provide "What's This?"
        break;
    default:
        break;
    }

    return QVariant();
}

void CommitHistoryModel::remove(QModelIndexList &&indexList)
{
    if (indexList.empty()) {
        qDebug() << "indexList is empty";
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

        qDebug() << commitHistory_[index].phrase_;
        auto ret = chewing_commit_history_remove(
            ctx_.get(),
            commitHistory_[index].phrase_.toUtf8().constData());
    }

    emit endRemoveRows();

    emit removeCommitHistoryCompleted();
}

void CommitHistoryModel::refresh()
{
    CommitHistorySet commitHistory;

    int length;
    char *phrase;
    unsigned short *phones;

    chewing_commit_history_enumerate(ctx_.get());
    while (chewing_commit_history_has_next(ctx_.get())) {
        int ret = chewing_commit_history_get(ctx_.get(), &length, &phrase, &phones);
        if (ret == -1) {
            qWarning() << "chewing_commit_history_get() returns" << ret;
            continue;
        }

        qDebug() << "Get commit history:" << phrase;
        commitHistory.insert(CommitHistory{length, phrase, phones});

        free(phrase);
        free(phones);
    }

    emit beginResetModel();
    commitHistory_.swap(commitHistory);
    emit endResetModel();

    qDebug() << "Total commit history" << commitHistory_.size();
    emit refreshCompleted(commitHistory_.size());
}

/*
void CommitHistoryModel::exportCommitHistory(CommitHistoryExporter& exporter)
{
    size_t exported = commitHistory_.size();

    for (auto& i: commitHistory_) {
        // FIXME
        exporter.addCommitHistory(i.phrase_, i.bopomofo_);
    }

    bool result = exporter.save();

    emit exportCompleted(result, exporter.getPath(), exported);
}
*/
