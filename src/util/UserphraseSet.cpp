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

#include "UserphraseSet.h"

#include <QDebug>

bool UserphraseSet::insert(Userphrase&& userphrase)
{
    auto ret = dedup_.insert(userphrase.display_);
    if (!ret.second) {
        // Duplicate item shall not be stored.
        qDebug() << "Duplicate phrase:" << userphrase.phrase_
                 << "bopomofo:" << userphrase.bopomofo_;
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

size_t UserphraseSet::size() const
{
    return userphrase_.size();
}

const Userphrase& UserphraseSet::operator[](size_t index) const
{
    return userphrase_[index];
}

void UserphraseSet::swap(UserphraseSet& set) noexcept
{
    userphrase_.swap(set.userphrase_);
    dedup_.swap(set.dedup_);
}
