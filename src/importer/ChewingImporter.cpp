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
}

std::vector<Userphrase> ChewingImporter::loadImpl()
{
    std::vector<Userphrase> result;

    QFile file(path_);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << FUNC_NAME << "Cannot open file" << path_;
        return result;
    }

    QJsonParseError err;
    auto doc = QJsonDocument::fromJson(file.readAll(), &err);
    if (doc.isNull()) {
        qWarning() << FUNC_NAME << "parse json error:" << err.errorString();
        return result;
    }

    if (!doc.isObject()) {
        qWarning() << FUNC_NAME << "root is not an object";
        return result;
    }

    auto root = doc.object();
    if (!root["userphrase"].isArray()) {
        qWarning() << FUNC_NAME << "userphrase is not an array";
        return result;
    }

    auto array = root["userphrase"].toArray();
    for (auto i = array.begin(); i != array.end(); ++i) {
        if (!(*i).isObject()) {
            qWarning() << FUNC_NAME << "skip non object";
            continue;
        }

        auto obj = (*i).toObject();
        if (!obj["phrase"].isString() || !obj["bopomofo"].isString()) {
            qWarning() << FUNC_NAME << "phrase or bopomofo is not a string";
            continue;
        }

        result.push_back(Userphrase{
            obj["phrase"].toString(),
            obj["bopomofo"].toString()
        });
    }

    return result;
}
