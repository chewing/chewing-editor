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

#include "ChewingExporter.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

ChewingExporter::ChewingExporter(const char *path)
    :UserphraseExporter(path)
{
}

void ChewingExporter::addUserphraseImpl(
    const std::string& phrase,
    const std::string& bopomofo)
{
    QJsonObject obj;

    obj["phrase"] = QString(phrase.c_str());
    obj["bopomofo"] = QString(bopomofo.c_str());

    array_.append(obj);

    return;
}

bool ChewingExporter::saveImpl()
{
    QJsonObject root;

    root["userphrase"] = array_;

    QJsonDocument doc(root);

    QFile file{QString(path_.c_str())};

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }
    file.write(doc.toJson());

    return true;
}
