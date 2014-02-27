#include "gtest/gtest.h"

#include <QDebug>
#include <QTemporaryDir>

#include "ChewingExporter.h"

class ChewingExporterTest : public ::testing::Test {
protected:
    ChewingExporterTest() = default;
    virtual ~ChewingExporterTest() = default;
    virtual void SetUp() {}
    virtual void TearDown() {}

    QTemporaryDir dir_;
};

TEST_F(ChewingExporterTest, WriteJson)
{
    ASSERT_TRUE(dir_.isValid());
    auto path = QString("%1/chewing.json").arg(dir_.path());

    ChewingExporter exporter(path.toStdString().c_str());
}
