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

#include "gtest/gtest.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTemporaryDir>

#include "ChewingExporter.h"

class TestChewingExporter : public ::testing::Test {
protected:
    TestChewingExporter();
    virtual ~TestChewingExporter() = default;
    virtual void SetUp() {}
    virtual void TearDown() {}

    QTemporaryDir dir_;
    QString path_;
};

TestChewingExporter::TestChewingExporter()
    :dir_()
    ,path_()
{
    path_ = QString("%1/chewing.json").arg(dir_.path());
    //dir_.setAutoRemove(false);
}

TEST_F(TestChewingExporter, WriteNoUserphrase)
{
    ASSERT_TRUE(dir_.isValid());

    ChewingExporter exporter(path_);
    ASSERT_TRUE(exporter.save());

    QFile file{QString(path_)};
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    auto root = QJsonDocument::fromJson(file.readAll()).object();

    auto userphrase = root.value("userphrase");
    ASSERT_TRUE(userphrase.isArray());

    auto array = userphrase.toArray();
    ASSERT_EQ(0, array.size());
}

TEST_F(TestChewingExporter, WriteOneUserphrase)
{
    ASSERT_TRUE(dir_.isValid());

    ChewingExporter exporter(path_.toStdString().c_str());
    exporter.addUserphrase(
        QString("\xE6\xB8\xAC\xE8\xA9\xA6" /* 測試 */),
        QString("\xE3\x84\x98\xE3\x84\x9C\xCB\x8B \xE3\x84\x95\xCB\x8B" /* ㄘㄜˋ ㄕˋ */)
    );

    ASSERT_TRUE(exporter.save());

    QFile file{path_};
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    auto root = QJsonDocument::fromJson(file.readAll()).object();

    auto userphrase = root.value("userphrase");
    ASSERT_TRUE(userphrase.isArray());

    auto array = userphrase.toArray();
    ASSERT_EQ(1, array.size());
    ASSERT_TRUE(array.at(0).isObject());

    auto obj = array.at(0).toObject();
    EXPECT_EQ(
        QJsonValue(QString("\xE6\xB8\xAC\xE8\xA9\xA6" /* 測試 */)),
        obj["phrase"]);
    EXPECT_EQ(QJsonValue(
        QString("\xE3\x84\x98\xE3\x84\x9C\xCB\x8B \xE3\x84\x95\xCB\x8B" /* ㄘㄜˋ ㄕˋ */)),
        obj["bopomofo"]);
}

TEST_F(TestChewingExporter, PathError)
{
    ASSERT_TRUE(dir_.isValid());

    ChewingExporter exporter{TESTDATA "/NoSuchPath/chewing.json"};
    ASSERT_FALSE(exporter.save());
}
