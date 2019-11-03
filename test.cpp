#include <gtest/gtest.h>

TEST(test_reg, regularity_check) {
    ASSERT_EQ ("ERROR", ForTest("a.", "a"));
    ASSERT_EQ ("ERROR", ForTest("ab.+*", "f"));
    ASSERT_EQ ("ERROR", ForTest("ab.c^", "b"));
    ASSERT_EQ ("ERROR", ForTest("*ab+", "a"));
    ASSERT_EQ ("ERROR", ForTest("20+n.", "b"));
    ASSERT_EQ ("ERROR", ForTest("ada*.cd*..", "aa"));
    ASSERT_EQ ("ERROR", ForTest("ab+cd+*aab", "aba"));
}

TEST(test_reg, check_answer_1) {
    EXPECT_EQ ("4", ForTest("ab+c.aba.*.bac.+.+*", "abacb"));
    EXPECT_EQ ("2", ForTest("ab+c.aba.*.bac.+.+*", "acbaabc"));
    EXPECT_EQ ("0", ForTest("ab+c.aba.*.bac.+.+*", "cbba"));
    EXPECT_EQ ("0", ForTest("ab+c.aba.*.bac.+.+*", "aa"));
    EXPECT_EQ ("7", ForTest("ab+c.aba.*.bac.+.+*", "aacbcab"));

}
TEST(test_reg, check_answer_2) {
    EXPECT_EQ ("0", ForTest("acb..bab.c.*.ab.ba.+.+*a.", "cb"));
    EXPECT_EQ ("1", ForTest("acb..bab.c.*.ab.ba.+.+*a.", "acb"));
    EXPECT_EQ ("1", ForTest("acb..bab.c.*.ab.ba.+.+*a.", "abcacbc"));
    EXPECT_EQ ("4", ForTest("acb..bab.c.*.ab.ba.+.+*a.", "bababbc"));
    EXPECT_EQ ("1", ForTest("acb..bab.c.*.ab.ba.+.+*a.", "a"));
    EXPECT_EQ ("7", ForTest("acb..bab.c.*.ab.ba.+.+*a.", "acbbbaa"));
}

TEST(test_reg, check_answer_3) {
    EXPECT_EQ ("0", ForTest("aba*.b.cb*.c.+.", "bb"));
    EXPECT_EQ ("0", ForTest("aba*.b.cb*.c.+.", "baa"));
    EXPECT_EQ ("3", ForTest("aba*.b.cb*.c.+.", "abbac"));
    EXPECT_EQ ("6", ForTest("aba*.b.cb*.c.+.", "acbbbccb"));
}

TEST(test_reg, check_answer_4) {
    EXPECT_EQ ("5", ForTest("b*", "bbbbb"));
    EXPECT_EQ ("0", ForTest("b*", "ab"));
}

TEST(test_reg, check_answer_5) {
    EXPECT_EQ ("6", ForTest("ab*.", "abbbbb"));
    EXPECT_EQ ("2", ForTest("ab*.", "abab"));
}