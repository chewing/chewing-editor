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

#include "HashImporter.h"

#include <cstring>
#include <vector>

#include <QDataStream>
#include <QDebug>
#include <QFile>

const int FIELD_SIZE = 125;

HashImporter::HashImporter(const QString& path)
:UserphraseImporter{path}
{
    QFile file{path};
    int ret;

    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Cannot open file" << path_;
        return;
    }

    QDataStream data{&file};

    std::vector<char> buffer(FIELD_SIZE);

    ret = data.readRawData(&buffer[0], 4);
    if (ret != 4) {
        qWarning() << "File size is less then 4" << path_;
        return;
    }

    if (memcmp(&buffer[0], "CBiH", 4) != 0) {
        qWarning() << "Cannot find signature `CBiH`" << path_;
        return;
    }
}
