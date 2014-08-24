/*
 * chewing-editor: Chewing commitHistory editor
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

#include "CommitHistorySet.h"

#include <QDebug>

bool CommitHistorySet::insert(CommitHistory&& commitHistory)
{
    commitHistory_.push_back(std::move(commitHistory));
    return true;
}

void CommitHistorySet::erase(iterator it)
{
    commitHistory_.erase(it);
}

CommitHistorySet::iterator CommitHistorySet::begin()
{
    return commitHistory_.begin();
}

CommitHistorySet::iterator CommitHistorySet::end()
{
    return commitHistory_.end();
}

size_t CommitHistorySet::size() const
{
    return commitHistory_.size();
}

const CommitHistory& CommitHistorySet::operator[](size_t index) const
{
    return commitHistory_[index];
}

void CommitHistorySet::swap(CommitHistorySet& set) noexcept
{
    commitHistory_.swap(set.commitHistory_);
}
