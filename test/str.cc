#include "str.h"
#include <gtest/gtest.h>

// TODO: table test & customized equality

TEST(str_t, trim)
{
    // NULL
    str_t s = str_trim(NULL);
    EXPECT_EQ(0, str_len(s));
    EXPECT_EQ(STR_EMPTY, s);

    // empty string
    s = str_trim(STR_EMPTY);
    EXPECT_EQ(0, str_len(s));
    EXPECT_EQ(STR_EMPTY, s);

    // WARN: NEVER use "" for CORD, use STR_EMPTY instead
    s = str_trim(str_from(""));
    EXPECT_EQ(0, str_len(s));
    EXPECT_STREQ(STR_EMPTY, s);

    // all spaces
    s = str_trim(" \t \r \n ");
    EXPECT_EQ(0, str_len(s));
    EXPECT_STREQ(STR_EMPTY, s);

    // only left
    s = str_trim(" \t\r\nehp");
    EXPECT_EQ(3, str_len(s));
    EXPECT_STREQ("ehp", s);

    // only right
    s = str_trim("ehp\n \t\r");
    EXPECT_EQ(3, str_len(s));
    EXPECT_STREQ("ehp", s);

    // both side
    s = str_trim(" \n\t\r  ehp\n \t\r");
    EXPECT_EQ(3, str_len(s));
    EXPECT_STREQ("ehp", s);
}

TEST(str_t, trim_left)
{
    // NULL
    str_t s = str_ltrim(NULL);
    EXPECT_EQ(0, str_len(s));
    EXPECT_EQ(NULL, s);

    // empty string
    s = str_ltrim(STR_EMPTY);
    EXPECT_EQ(0, str_len(s));
    EXPECT_EQ(STR_EMPTY, s);

    // empty string
    s = str_ltrim(str_from(""));
    EXPECT_EQ(0, str_len(s));
    EXPECT_STREQ(STR_EMPTY, s);

    // all spaces
    s = str_ltrim(str_from(" \t \r \n "));
    EXPECT_EQ(0, str_len(s));
    EXPECT_STREQ(STR_EMPTY, s);

    // only left
    s = str_ltrim(str_from(" \t\r\nehp"));
    EXPECT_EQ(3, str_len(s));
    EXPECT_STREQ("ehp", s);

    // only right
    s = str_ltrim(str_from("ehp\n \t\r"));
    EXPECT_EQ(7, str_len(s));
    EXPECT_STREQ("ehp\n \t\r", s);

    // both side
    s = str_ltrim(str_from(" \n\t\r  ehp\n \t\r"));
    EXPECT_EQ(7, str_len(s));
    EXPECT_STREQ("ehp\n \t\r", s);
}

TEST(str_t, trim_right)
{
    // NULL
    str_t s = str_rtrim(NULL);
    EXPECT_EQ(0, str_len(s));
    EXPECT_EQ(NULL, s);

    // empty string
    s = str_ltrim(STR_EMPTY);
    EXPECT_EQ(0, str_len(s));
    EXPECT_EQ(STR_EMPTY, s);

    // empty string
    s = str_rtrim(str_from(""));
    EXPECT_EQ(0, str_len(s));
    EXPECT_STREQ(STR_EMPTY, s);

    // all spaces
    s = str_rtrim(str_from(" \t \r \n "));
    EXPECT_EQ(0, str_len(s));
    EXPECT_STREQ(STR_EMPTY, s);

    // only right
    s = str_rtrim(str_from(" \t\r\nehp"));
    EXPECT_EQ(7, str_len(s));
    EXPECT_STREQ(" \t\r\nehp", s);

    // only right
    s = str_rtrim(str_from("ehp\n \t\r"));
    EXPECT_EQ(3, str_len(s));
    EXPECT_STREQ("ehp", s);

    // both side
    s = str_rtrim(str_from(" \n\t\r  ehp\n \t\r"));
    EXPECT_EQ(9, str_len(s));
    EXPECT_STREQ(" \n\t\r  ehp", s);
}

TEST(str_t, split)
{
    array_t arr;

    // NULL
    arr = str_nsplit(NULL, 0, -1);
    EXPECT_EQ(NULL, arr);

    // split zero
    arr = str_nsplit(str_from("aaa:bb:c"), ':', 0);
    EXPECT_EQ(0, arr->len);

    // split one
    arr = str_nsplit(str_from("aaa:bb:c"), ':', 1);
    EXPECT_EQ(1, arr->len);
    EXPECT_STREQ("aaa:bb:c", array_get_(str_t, arr, 0));

    // split two
    arr = str_nsplit(str_from("aaa:bb:c"), ':', 2);
    EXPECT_EQ(2, arr->len);
    EXPECT_STREQ("aaa", array_get_(str_t, arr, 0));
    EXPECT_STREQ("bb:c", array_get_(str_t, arr, 1));

    // split three
    arr = str_nsplit(str_from("aaa:bb:c"), ':', 3);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ("aaa", array_get_(str_t, arr, 0));
    EXPECT_STREQ("bb", array_get_(str_t, arr, 1));
    EXPECT_STREQ("c", array_get_(str_t, arr, 2));

    // split more
    arr = str_nsplit(str_from("aaa:bb:c"), ':', 10);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ("aaa", array_get_(str_t, arr, 0));
    EXPECT_STREQ("bb", array_get_(str_t, arr, 1));
    EXPECT_STREQ("c", array_get_(str_t, arr, 2));

    // split all
    arr = str_nsplit(str_from("aaa:bb:c"), ':', -1);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ("aaa", array_get_(str_t, arr, 0));
    EXPECT_STREQ("bb", array_get_(str_t, arr, 1));
    EXPECT_STREQ("c", array_get_(str_t, arr, 2));

    // special
    arr = str_nsplit(str_from(":"), ':', -1);
    EXPECT_EQ(2, arr->len);
    EXPECT_EQ(STR_EMPTY, array_get_(str_t, arr, 0));
    EXPECT_EQ(STR_EMPTY, array_get_(str_t, arr, 1));

    // special
    arr = str_nsplit(str_from(":"), ':', 1);
    EXPECT_EQ(1, arr->len);
    EXPECT_STREQ(":", array_get_(str_t, arr, 0));

    // special
    arr = str_nsplit(str_from("::"), ':', -1);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ(STR_EMPTY, array_get_(str_t, arr, 0));
    EXPECT_STREQ(STR_EMPTY, array_get_(str_t, arr, 1));
    EXPECT_STREQ(STR_EMPTY, array_get_(str_t, arr, 2));

    // special
    arr = str_nsplit(str_from("::"), ':', 1);
    EXPECT_EQ(1, arr->len);
    EXPECT_STREQ("::", array_get_(str_t, arr, 0));

    // special
    arr = str_nsplit(str_from("::"), ':', 2);
    EXPECT_EQ(2, arr->len);
    EXPECT_STREQ(STR_EMPTY, array_get_(str_t, arr, 0));
    EXPECT_STREQ(":", array_get_(str_t, arr, 1));

    // special
    arr = str_nsplit(str_from("aaa::b"), ':', -1);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ("aaa", array_get_(str_t, arr, 0));
    EXPECT_STREQ(STR_EMPTY, array_get_(str_t, arr, 1));
    EXPECT_STREQ("b", array_get_(str_t, arr, 2));

    // special
    arr = str_nsplit(str_from("aaa::"), ':', -1);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ("aaa", array_get_(str_t, arr, 0));
    EXPECT_STREQ(STR_EMPTY, array_get_(str_t, arr, 1));
    EXPECT_STREQ(STR_EMPTY, array_get_(str_t, arr, 2));
}
