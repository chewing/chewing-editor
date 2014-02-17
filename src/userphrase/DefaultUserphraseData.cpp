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

#include "DefaultUserphraseData.h"

#include <cstdio>

#include <QDebug>

static void logger(void *data, int level, const char *fmt, ...)
{
    va_list list;
    int len;

    va_start(list, fmt);
    len = vsnprintf(NULL, 0, fmt, list);
    va_end(list);

    std::vector<char> buf(len);

    va_start(list, fmt);
    len = vsnprintf(&buf[0], buf.size(), fmt, list);
    va_end(list);

    qDebug() << QString::fromUtf8(&buf[0]);
}

DefaultUserphraseData::DefaultUserphraseData(const char* path)
:ctx_(chewing_new2(nullptr, path, logger, nullptr), chewing_delete)
{
    if (!ctx_) {
        // FIXME: Report error here
        qDebug() << "chewing_new2 fails";
    }
    refresh();
}


void DefaultUserphraseData::refreshImpl()
{
    std::vector<Userphrase> userphrase;

    std::vector<char> phrase;
    unsigned int phrase_len;

    std::vector<char> bopomofo;
    unsigned int bopomofo_len;

    chewing_userphrase_enumerate(ctx_.get());
    while (chewing_userphrase_has_next(ctx_.get(), &phrase_len, &bopomofo_len)) {
        phrase.resize(phrase_len);
        bopomofo.resize(bopomofo_len);

        int ret = chewing_userphrase_get(ctx_.get(),
            &phrase[0], phrase.size(),
            &bopomofo[0], bopomofo.size());
        if (ret == -1) {
            qDebug() << "chewing_userphrase_get() returns " << ret;
            continue;
        }

        userphrase.push_back(Userphrase{
            std::string(&phrase[0]),
            std::string(&bopomofo[0])
        });

    }

    userphrase_.swap(userphrase);
}

void DefaultUserphraseData::saveImpl()
{
}

bool DefaultUserphraseData::removeImpl(size_t index)
{
    auto ret = chewing_userphrase_remove(
        ctx_.get(),
        userphrase_[index].phrase_.c_str(),
        userphrase_[index].bopomofo_.c_str());
    qDebug() << "chewing_userphrase_remove returns = " << ret;

    if (ret == 0) {
        // FIXME: std::vector::erase is an inefficient operation.
        userphrase_.erase(userphrase_.begin() + index);
    }
    // FIXME: Handle chewing_userphrase_remove fails.

    return ret == 0;
}
