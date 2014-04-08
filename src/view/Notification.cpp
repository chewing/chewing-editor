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
    qDebug() << result << path << imported << total;

    if (result) {
        setText(
            tr("%1 %2 %3")
                .arg(tr("Import %1 successful.").arg(path))
                .arg(tr("%n phrase(s) are imported.", 0, imported))
                .arg(tr("Total %n user phrase(s).", 0, total)));
    } else {
        setText(tr("Import %1 failed.").arg(path));
    }
}

void Notification::notifyExportCompleted(
    bool result,
    const QString& path,
    size_t exported)
{
    qDebug() << result << path << exported;

    if (result) {
        setText(
            tr("%1 %2")
                .arg(tr("Export %1 successful.").arg(path))
                .arg(tr("%n phrase(s) are exported.", 0, exported)));
    } else {
        setText(tr("Export %1 failed.").arg(path));
    }
}

void Notification::notifyAddNewPhraseCompleted(const Userphrase& userphrase)
{
    qDebug() << userphrase.display_;

    setText(tr("Add new phrase %1 successful.").arg(userphrase.display_));
}


void Notification::notifyRemovePhraseCompleted(size_t count)
{
    qDebug() << count;

    setText(tr("Remove %n phrase(s) successful.", 0, count));
}

void Notification::notifyRefreshCompleted(size_t count)
{
    qDebug() << count;

    setText(tr("Refresh completed. Total %n user phrase(s).", 0, count));
}
