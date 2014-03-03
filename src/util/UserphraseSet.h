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

#include <set>
#include <vector>

#include <QString>

#include "Userphrase.h"

class UserphraseSet {
public:
    UserphraseSet() = default;
    UserphraseSet(const UserphraseSet&) = default;
    UserphraseSet& operator=(const UserphraseSet&) = default;
    virtual ~UserphraseSet() = default;

    typedef std::vector<Userphrase>::iterator iterator;

    bool insert(Userphrase&& userphrase);
    void erase(iterator it);

    iterator begin();
    iterator end();
    size_t size();

    Userphrase& operator[](size_t index);

private:
    std::vector<Userphrase> userphrase_;
    std::set<QString> dedup_;
};
