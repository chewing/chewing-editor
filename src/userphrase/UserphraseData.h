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

#include <memory>
#include <string>
#include <vector>

#include <QModelIndex>

#include <chewing.h>

struct Userphrase final {
    std::string phrase_;
    std::string bopomofo_;
};

class UserphraseData final {
public:
    explicit UserphraseData(const char* path = nullptr);
    UserphraseData(const UserphraseData&) = delete;
    UserphraseData& operator=(const UserphraseData&) = delete;
    virtual ~UserphraseData() = default;

    size_t size() const;
    const Userphrase& get(size_t index);
    void add(std::string &&phrase_, std::string &&bopomofo_);
    void remove(size_t index);
    void refresh();

protected:

    std::unique_ptr<ChewingContext, void (*)(ChewingContext*)> ctx_;
    std::vector<Userphrase> userphrase_;
};
