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

#include "UserphraseSet.h"

bool UserphraseSet::insert(Userphrase&& userphrase)
{
    auto ret = dedup_.insert(userphrase.display_);
    if (!ret.second) {
        // Duplicate item shall not be stored.
        return false;
    }

    userphrase_.push_back(std::move(userphrase));
    return true;
}

void UserphraseSet::erase(iterator it)
{
    dedup_.erase(it->display_);
    userphrase_.erase(it);
}

UserphraseSet::iterator UserphraseSet::begin()
{
    return userphrase_.begin();
}

UserphraseSet::iterator UserphraseSet::end()
{
    return userphrase_.end();
}

size_t UserphraseSet::size()
{
    return userphrase_.size();
}

Userphrase& UserphraseSet::operator[](size_t index)
{
    return userphrase_[index];
}
