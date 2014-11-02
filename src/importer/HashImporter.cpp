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

#include <QByteArray>
#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QString>

const size_t SIGNATURE_SIZE        = 4;
const size_t CHEWING_LIFETIME_SIZE = 4;
const size_t USERPHRASE_SIZE       = 125;

const size_t USERPHRASE_LEN_OFFSET   = 16;

const size_t BOPOMOFO_SIZE = 4;
const int BOPOMOFO_SHIFT[] = { 9, 7, 3, 0 };
const int BOPOMOFO_MASK[] = { 0x1F, 0x3, 0xF, 0x7 };
const int BOPOMOFO_LEN[] = { 3, 3, 3, 2 };
const char *const BOPOMOFO_TABLE[] = {
    // ㄅㄆㄇㄈㄉㄊㄋㄌㄍㄎㄏㄐㄑㄒㄓㄔㄕㄖㄗㄘㄙ
    "\xE3\x84\x85\xE3\x84\x86\xE3\x84\x87\xE3\x84\x88\xE3\x84\x89"
    "\xE3\x84\x8A\xE3\x84\x8B\xE3\x84\x8C\xE3\x84\x8D\xE3\x84\x8E"
    "\xE3\x84\x8F\xE3\x84\x90\xE3\x84\x91\xE3\x84\x92\xE3\x84\x93"
    "\xE3\x84\x94\xE3\x84\x95\xE3\x84\x96\xE3\x84\x97\xE3\x84\x98"
    "\xE3\x84\x99",

    // ㄧㄨㄩ
    "\xE3\x84\xA7\xE3\x84\xA8\xE3\x84\xA9",

    // ㄚㄛㄜㄝㄞㄟㄠㄡㄢㄣㄤㄥㄦ
    "\xE3\x84\x9A\xE3\x84\x9B\xE3\x84\x9C\xE3\x84\x9D\xE3\x84\x9E"
    "\xE3\x84\x9F\xE3\x84\xA0\xE3\x84\xA1\xE3\x84\xA2\xE3\x84\xA3"
    "\xE3\x84\xA4\xE3\x84\xA5\xE3\x84\xA6",

    // ˙ˊˇˋ
    "\xCB\x99\xCB\x8A\xCB\x87\xCB\x8B"
};

static uint16_t getUint16(const char *ptr)
{
    auto uptr = reinterpret_cast<const unsigned char *>(ptr);

    return (uptr[0] << 0) | (uptr[1] << 8);
}

static QString getBopomofoFromPhoneArray(const std::vector<uint16_t> &phone_array)
{
    // FIXME: Not implemented.
    QByteArray buf;

    for (auto it = phone_array.begin(); it != phone_array.end(); ++it) {
        qDebug() << "*it = " << *it;
        for (size_t i = 0; i < BOPOMOFO_SIZE; ++i) {
            size_t idx = ((*it >> BOPOMOFO_SHIFT[i]) & BOPOMOFO_MASK[i]);
            if (idx == 0) {
                continue;
            }

            buf.append(&BOPOMOFO_TABLE[i][(idx - 1)* BOPOMOFO_LEN[i]], BOPOMOFO_LEN[i]);
        }
        buf.append(' ');
    }
    buf[buf.length()-1] = 0;

    qDebug() << "buf = " << buf;

    return QString(buf);
}

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

    std::vector<char> buffer(USERPHRASE_SIZE);

    ret = data.readRawData(&buffer[0], SIGNATURE_SIZE);
    if (ret != SIGNATURE_SIZE) {
        qWarning() << "Cannot read signature" << path_;
        return;
    }

    if (memcmp(&buffer[0], "CBiH", CHEWING_LIFETIME_SIZE) != 0) {
        qWarning() << "Cannot find signature `CBiH`" << path_;
        return;
    }

    ret = data.readRawData(&buffer[0], CHEWING_LIFETIME_SIZE);
    if (ret != CHEWING_LIFETIME_SIZE) {
        qWarning() << "Cannot read chewing_lifetime" << path_;
        return;
    }

    UserphraseSet result;

    while (data.readRawData(&buffer[0], USERPHRASE_SIZE) == USERPHRASE_SIZE) {
        size_t pos = USERPHRASE_LEN_OFFSET;

        int phone_len = static_cast<unsigned char>(buffer[pos]);
        ++pos;

        std::vector<uint16_t> phone_array;
        for (int i = 0; i < phone_len; ++i) {
            phone_array.push_back(getUint16(&buffer[pos]));
            pos += 2;
        }

        int phrase_len = static_cast<unsigned char>(buffer[pos]);
        ++pos;

        buffer[pos + phrase_len] = 0;

        result.insert(Userphrase{
            QString{&buffer[pos]},
            getBopomofoFromPhoneArray(phone_array)
        });
    }

    std::swap(userphrase_, result);
    supportedFormat_ = true;
}
