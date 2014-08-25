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

#include "CommitHistoryUploader.h"

#include <QDebug>
#include <QFile>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QHttpMultiPart>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

#include <cstdio>

CommitHistoryUploader::CommitHistoryUploader()
    :ctx_{chewing_new2(nullptr, nullptr, nullptr, nullptr), chewing_delete}
{
}

bool CommitHistoryUploader::save()
{
    QTemporaryFile file;
    if (file.open()) {
        chewing_commit_history_export(ctx_.get(), file.fileName().toStdString().c_str());
    }
    else {
        qDebug() << "Cannot open file.";
        return false;
    }
    file.close();

    file.open();

    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart jsonPart;
    jsonPart.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data;name=\"file\";filename=\""+file.fileName()+"\""));
    jsonPart.setBodyDevice(&file);

    multiPart->append(jsonPart);

    QUrl url("http://localhost/test.php");
    QNetworkRequest request(url);

    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.post(request, multiPart);

    printf("Reply:\n%s\n", ((QString)reply->readAll()).toStdString().c_str());
    printf("BEEN HERE\n");

    return true;
}
