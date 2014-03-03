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

#include "gtest/gtest.h"

#include "UserphraseSet.h"

#pragma execution_character_set("utf-8")

class UserphraseSetTest : public ::testing::Test {
protected:
    UserphraseSetTest() = default;
    virtual ~UserphraseSetTest() = default;

    UserphraseSet set_;
};

/*
 * Use macro here because we want to know line number when it fails. Function
 * will hide line number.
 */
#define COMPARE_USERPHRASE(it, phrase, bopomofo) do { \
    EXPECT_EQ(0, QString::compare((phrase), (it)->phrase_)); \
    EXPECT_EQ(0, QString::compare((bopomofo), (it)->bopomofo_)); \
} while (0)

TEST_F(UserphraseSetTest, InsertDuplicated)
{
    ASSERT_TRUE(set_.insert(Userphrase{QString("測試"), QString("ㄘㄜˋ ㄕˋ")}));
    ASSERT_FALSE(set_.insert(Userphrase{QString("測試"), QString("ㄘㄜˋ ㄕˋ")}));

    ASSERT_EQ(1, set_.size());

    COMPARE_USERPHRASE(set_.begin(), "測試", "ㄘㄜˋ ㄕˋ");
}

TEST_F(UserphraseSetTest, IndexOperator)
{
    ASSERT_TRUE(set_.insert(Userphrase{QString("測試"), QString("ㄘㄜˋ ㄕˋ")}));

    ASSERT_EQ(1, set_.size());

    EXPECT_EQ(0, QString::compare("測試", set_[0].phrase_));
    EXPECT_EQ(0, QString::compare("ㄘㄜˋ ㄕˋ", set_[0].bopomofo_));
}

TEST_F(UserphraseSetTest, Enumerate)
{
    Userphrase data[] = {
        Userphrase{QString("測試"), QString("ㄘㄜˋ ㄕˋ")},
        Userphrase{QString("一下"), QString("ㄧˊ ㄒㄧㄚˋ")},
    };

    ASSERT_TRUE(set_.insert(Userphrase{data[0]}));
    ASSERT_TRUE(set_.insert(Userphrase{data[1]}));

    ASSERT_EQ(2, set_.size());

    int i = 0;
    for (auto it = set_.begin(); it != set_.end(); ++it, ++i) {
        COMPARE_USERPHRASE(it, data[i].phrase_, data[i].bopomofo_);
    }
}

TEST_F(UserphraseSetTest, Erase)
{
    ASSERT_TRUE(set_.insert(Userphrase{QString("測試"), QString("ㄘㄜˋ ㄕˋ")}));
    ASSERT_TRUE(set_.insert(Userphrase{QString("一下"), QString("ㄧˊ ㄒㄧㄚˋ")}));

    ASSERT_EQ(2, set_.size());

    set_.erase(set_.begin());

    ASSERT_EQ(1, set_.size());

    COMPARE_USERPHRASE(set_.begin(), "一下", "ㄧˊ ㄒㄧㄚˋ");
}

TEST_F(UserphraseSetTest, SamePhraseDifferentBopomofo)
{
    ASSERT_TRUE(set_.insert(Userphrase{QString("什麼"), QString("ㄕㄣˊ ㄇㄜ˙")}));
    ASSERT_TRUE(set_.insert(Userphrase{QString("什麼"), QString("ㄕㄜˊ ㄇㄜ˙")}));

    ASSERT_EQ(2, set_.size());

    COMPARE_USERPHRASE(set_.begin(), "什麼", "ㄕㄣˊ ㄇㄜ˙");
    COMPARE_USERPHRASE(set_.begin() + 1, "什麼", "ㄕㄜˊ ㄇㄜ˙");

    set_.erase(set_.begin());

    ASSERT_EQ(1, set_.size());

    COMPARE_USERPHRASE(set_.begin(), "什麼", "ㄕㄜˊ ㄇㄜ˙");
}

TEST_F(UserphraseSetTest, DifferentPhraseSameBopomofo)
{
    ASSERT_TRUE(set_.insert(Userphrase{QString("測試"), QString("ㄘㄜˋ ㄕˋ")}));
    ASSERT_TRUE(set_.insert(Userphrase{QString("策試"), QString("ㄘㄜˋ ㄕˋ")}));

    ASSERT_EQ(2, set_.size());

    COMPARE_USERPHRASE(set_.begin(), "測試", "ㄘㄜˋ ㄕˋ");
    COMPARE_USERPHRASE(set_.begin() + 1, "策試", "ㄘㄜˋ ㄕˋ");

    set_.erase(set_.begin() + 1);

    ASSERT_EQ(1, set_.size());

    COMPARE_USERPHRASE(set_.begin(), "測試", "ㄘㄜˋ ㄕˋ");
}
