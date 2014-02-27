#include "gtest/gtest.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QTemporaryDir>

#include "ChewingExporter.h"

class ChewingExporterTest : public ::testing::Test {
protected:
    ChewingExporterTest();
    virtual ~ChewingExporterTest() = default;
    virtual void SetUp() {}
    virtual void TearDown() {}

    QTemporaryDir dir_;
    QString path_;
};

ChewingExporterTest::ChewingExporterTest()
    :dir_()
    ,path_()
{
    path_ = QString("%1/chewing.json").arg(dir_.path());
}

TEST_F(ChewingExporterTest, WriteNoUserphrase)
{
    ASSERT_TRUE(dir_.isValid());

    ChewingExporter exporter(path_.toStdString().c_str());
    ASSERT_TRUE(exporter.save());

    QFile file{QString(path_)};
    ASSERT_TRUE(file.open(QIODevice::ReadOnly | QIODevice::Text));

    auto root = QJsonDocument::fromJson(file.readAll()).object();

    auto userphrase = root.value("userphrase");
    ASSERT_TRUE(userphrase.isArray());

    auto array = userphrase.toArray();
    ASSERT_EQ(0, array.size());
}

TEST_F(ChewingExporterTest, WriteOneUserphrase)
{
    ASSERT_TRUE(dir_.isValid());

    ChewingExporter exporter(path_.toStdString().c_str());
    exporter.addUserphrase(
        std::string("\xE6\xB8\xAC\xE8\xA9\xA6" /* 測試 */),
        std::string("\xE3\x84\x98\xE3\x84\x9C\xCB\x8B \xE3\x84\x95\xCB\x8B" /* ㄘㄜˋ ㄕˋ */)
    );

    ASSERT_TRUE(exporter.save());

    QFile file{QString(path_)};
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

TEST_F(ChewingExporterTest, PathError)
{
    ASSERT_TRUE(dir_.isValid());

    ChewingExporter exporter("/NoSuchPath.json");
    ASSERT_FALSE(exporter.save());
}
