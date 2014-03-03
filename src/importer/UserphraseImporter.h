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

#pragma once

#include <vector>

#include <QString>

#include "UserphraseSet.h"

class UserphraseImporter {
public:
    explicit UserphraseImporter(const QString& path);
    UserphraseImporter(const UserphraseImporter&) = delete;
    UserphraseImporter& operator=(const UserphraseImporter&) = delete;
    virtual ~UserphraseImporter() = default;

    UserphraseSet load();

protected:
    virtual UserphraseSet loadImpl() = 0;
    QString path_;
};
