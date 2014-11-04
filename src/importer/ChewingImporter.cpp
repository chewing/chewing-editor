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

#include "ChewingImporter.h"

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

ChewingImporter::ChewingImporter(const QString& path)
    :UserphraseImporter(path)
{
    UserphraseSet result;

    QFile file{path_};
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Cannot open file" << path_;
        return;
    }

    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(file.readAll(), &err);
    if (doc.isNull()) {
        qWarning() << "parse json error in " << path_ << err.errorString();
        return;
    }

    if (!doc.isObject()) {
        qWarning() << "root is not an object in" << path_;
        return;
    }

    auto root = doc.object();
    if (!root["userphrase"].isArray()) {
        qWarning() << "userphrase is not an array in " << path_;
        return;
    }

    auto array = root["userphrase"].toArray();
    for (auto i = array.begin(); i != array.end(); ++i) {
        if (!(*i).isObject()) {
            qWarning() << "skip non object in " << path_;
            continue;
        }

        auto obj = (*i).toObject();
        if (!obj["phrase"].isString() || !obj["bopomofo"].isString()) {
            qWarning() << "phrase or bopomofo is not a string in" << path_;
            continue;
        }

        result.insert(Userphrase{
            obj["phrase"].toString(),
            obj["bopomofo"].toString()
        });
    }

    std::swap(userphrase_, result);
    supportedFormat_ = true;
}
