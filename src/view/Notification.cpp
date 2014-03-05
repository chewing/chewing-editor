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

#include "Notification.h"

#include <QDebug>

Notification::Notification(QWidget* parent)
    :QLabel(parent)
{
}

void Notification::notifyImportCompleted(
    bool result,
    const QString& path,
    size_t imported,
    size_t total)
{
    qDebug() << FUNC_NAME << result << path << imported << total;

    if (result) {
        setText(
            tr("%1 %2 %3")
                .arg(tr("Import %1 success.").arg(path))
                .arg(tr("%1 phrase(s) are imported.", 0, imported).arg(imported))
                .arg(tr("Total %1 user phrase(s).", 0, total).arg(total)));
    } else {
        setText(tr("Import %1 failed."));
    }
}

void Notification::notifyExportCompleted(
    bool result,
    const QString& path,
    size_t exported)
{
    qDebug() << FUNC_NAME << result << path << exported;

    if (result) {
        setText(
            tr("%1 %2 %3")
                .arg(tr("Export %1 success.").arg(path))
                .arg(tr("%1 phrase(s) are exported.", 0, exported).arg(exported)));
    } else {
        setText(tr("Export %1 failed."));
    }
}

void Notification::notifyAddNewPhraseCompleted(const Userphrase& userphrase)
{
    qDebug() << FUNC_NAME << userphrase.display_;

    setText(tr("Add new phrase %1 success.").arg(userphrase.display_));
}


void Notification::notifyRemovePhraseCompleted(const Userphrase& userphrase)
{
    qDebug() << FUNC_NAME << userphrase.display_;

    setText(tr("Remove phrase %1 success.").arg(userphrase.display_));
}

void Notification::notifyRefreshCompleted(size_t count)
{
    qDebug() << FUNC_NAME << count;

    setText(tr("Refresh completed. Total %1 user phrase(s).", 0, count).arg(count));
}
