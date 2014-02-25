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

#include "UserphraseData.h"

#include <QDebug>

static void logger(void *data, int level, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    size_t len = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    std::vector<char> buffer(len + 1);

    va_start(ap, fmt);
    vsnprintf(&buffer[0], buffer.size(), fmt, ap);
    va_end(ap);

    if (level <= CHEWING_LOG_INFO) {
        qDebug() << &buffer[0];
    } else if (level <= CHEWING_LOG_WARN) {
        qWarning() << &buffer[0];
    } else if (level <= CHEWING_LOG_ERROR) {
        qCritical() << &buffer[0];
    }
}

UserphraseData::UserphraseData(const char* path)
:ctx_(chewing_new2(nullptr, path, logger, nullptr), chewing_delete)
{
    if (!ctx_) {
        // FIXME: Report error here
        qCritical() << "chewing_new2() fails";
    }
    refresh();
}

size_t UserphraseData::size() const
{
    return userphrase_.size();
}

const Userphrase &UserphraseData::get(size_t index)
{
    return userphrase_[index];
}

void UserphraseData::add(std::string &&phrase_, std::string &&bopomofo_)
{
    userphrase_.push_back(Userphrase{
        std::move(phrase_),
        std::move(bopomofo_),
    });
}

void UserphraseData::remove(size_t index)
{
    qDebug() << FUNC_NAME << index;
    auto ret = chewing_userphrase_remove(
        ctx_.get(),
        userphrase_[index].phrase_.c_str(),
        userphrase_[index].bopomofo_.c_str());

    if (ret == 0) {
        // FIXME: std::vector::erase is an inefficient operation.
        userphrase_.erase(userphrase_.begin() + index);
    } else {
        qWarning() << "chewing_userphrase_remove() returns" << ret;
    }
    // FIXME: Handle chewing_userphrase_remove fails.
}

void UserphraseData::refresh()
{
    qDebug() << FUNC_NAME;
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
            qWarning() << "chewing_userphrase_get() returns" << ret;
            continue;
        }

        qDebug() << "Get userphrase:" << &phrase[0] << &bopomofo[0];
        userphrase.push_back(Userphrase{
            std::string(&phrase[0]),
            std::string(&bopomofo[0])
        });

    }

    userphrase_.swap(userphrase);
    qDebug() << "Total userphrase" << userphrase_.size();
}
