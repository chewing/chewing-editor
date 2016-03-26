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

#include <gtest/gtest.h>

#include <QDebug>
#include <QDir>

#include "HashImporter.h"

class TestHashImporter : public ::testing::Test {
protected:
    TestHashImporter() = default;
    virtual ~TestHashImporter() = default;
    virtual void SetUp() {}
    virtual void TearDown() {}
};

TEST_F(TestHashImporter, ReadUserphrase)
{
    QString path{TESTDATA "/import/uhash.dat/uhash.dat"};
    HashImporter importer{path};

    EXPECT_TRUE(importer.isSupportedFormat());
    EXPECT_EQ(QFileInfo{path}.fileName(), importer.getPath());

    auto result = importer.getUserphraseSet();

    EXPECT_EQ(2, result.size());

    EXPECT_EQ(0, QString::compare(
        QString("\xE6\xB8\xAC\xE8\xA9\xA6" /* 測試 */),
        result[0].phrase_));

    EXPECT_EQ(0, QString::compare(
        QString("\xE3\x84\x98\xE3\x84\x9C\xCB\x8B \xE3\x84\x95\xCB\x8B" /* ㄘㄜˋ ㄕˋ */),
        result[0].bopomofo_));

    EXPECT_EQ(0, QString::compare(
        QString("\xE4\xB8\x80\xE4\xB8\x8B" /* 一下 */),
        result[1].phrase_));

    EXPECT_EQ(0, QString::compare(
        QString("\xE3\x84\xA7\xCB\x8A \xE3\x84\x92\xE3\x84\xA7\xE3\x84\x9A\xCB\x8B" /* ㄧˊ ㄒㄧㄚˋ */),
        result[1].bopomofo_));
}

TEST_F(TestHashImporter, ReadBrokenHash)
{
    auto list = QDir{TESTDATA "/import/uhash.dat/broken"}.entryList(QDir::Files);

    ASSERT_GT(list.size(), 0);

    foreach(auto file, list) {
        auto path = QString("%1/%2").arg(TESTDATA "/import/uhash.dat/broken").arg(file);
        HashImporter importer{path};
        EXPECT_FALSE(importer.isSupportedFormat());
        EXPECT_TRUE(importer.getUserphraseSet().empty());
    }
}

TEST_F(TestHashImporter, PathError)
{
    HashImporter importer{TESTDATA "/NoSuchPath/uhash.dat"};

    EXPECT_FALSE(importer.isSupportedFormat());
    EXPECT_TRUE(importer.getUserphraseSet().empty());
}
