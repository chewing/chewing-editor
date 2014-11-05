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

#include "UserphraseImporter.h"

#include "ChewingImporter.h"
#include "HashImporter.h"

UserphraseImporter::UserphraseImporter(const QString& path)
    :path_{path}
    ,supportedFormat_{false}
    ,userphrase_{}
{
}

bool UserphraseImporter::isSupportedFormat()
{
    return supportedFormat_;
}

const UserphraseSet& UserphraseImporter::getUserphraseSet()
{
    return userphrase_;
}

const QString UserphraseImporter::getPath() {
    QFileInfo info{path_};
    return info.fileName();
}

std::shared_ptr<UserphraseImporter> createUserphraseImporter(const QString& path)
{
    std::shared_ptr<UserphraseImporter> ptr;

    /*
     * FIXME: We shall check file type before creating UserphraseImporter
     * object.
     */

    ptr.reset(new ChewingImporter(path));
    if (ptr.get()->isSupportedFormat()) {
        return ptr;
    }

    ptr.reset(new HashImporter(path));
    if (ptr.get()->isSupportedFormat()) {
        return ptr;
    }

    return ptr;
}
