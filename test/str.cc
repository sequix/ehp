#include "str.h"
#include <gtest/gtest.h>

TEST(str_t, new)
{
    const char *expected = "hello";
    str_t *s = str_new(expected);
    EXPECT_EQ(5, s->len);
    EXPECT_STREQ(expected, s->s);

    s = str_new("");
    EXPECT_EQ(0, s->len);
    EXPECT_STREQ("", s->s);
}

TEST(str_t, new_n)
{
    const char *expected = "hello";
    str_t *s = str_new_n(expected, 3);
    EXPECT_STREQ("hel", s->s);

    s = str_new_n("123", 0);
    EXPECT_EQ(0, s->len);
    EXPECT_STREQ("", s->s);
}

TEST(str_t, trim)
{
    // NULL
    str_t *s = str_trim(NULL);
    EXPECT_EQ(NULL, s);

    // empty string
    s = str_trim(str_new(""));
    EXPECT_EQ(0, s->len);
    EXPECT_STREQ("", s->s);

    // all spaces
    s = str_trim(str_new(" \t \r \n "));
    EXPECT_EQ(0, s->len);
    EXPECT_STREQ("", s->s);

    // only left
    s = str_trim(str_new(" \t\r\nehp"));
    EXPECT_EQ(3, s->len);
    EXPECT_STREQ("ehp", s->s);

    // only right
    s = str_trim(str_new("ehp\n \t\r"));
    EXPECT_EQ(3, s->len);
    EXPECT_STREQ("ehp", s->s);

    // both side
    s = str_trim(str_new(" \n\t\r  ehp\n \t\r"));
    EXPECT_EQ(3, s->len);
    EXPECT_STREQ("ehp", s->s);
}

TEST(str_t, trim_left)
{
    // NULL
    str_t *s = str_trim_left(NULL);
    EXPECT_EQ(NULL, s);

    // empty string
    s = str_trim_left(str_new(""));
    EXPECT_EQ(0, s->len);
    EXPECT_STREQ("", s->s);

    // all spaces
    s = str_trim_left(str_new(" \t \r \n "));
    EXPECT_EQ(0, s->len);
    EXPECT_STREQ("", s->s);

    // only left
    s = str_trim_left(str_new(" \t\r\nehp"));
    EXPECT_EQ(3, s->len);
    EXPECT_STREQ("ehp", s->s);

    // only right
    s = str_trim_left(str_new("ehp\n \t\r"));
    EXPECT_EQ(7, s->len);
    EXPECT_STREQ("ehp\n \t\r", s->s);

    // both side
    s = str_trim_left(str_new(" \n\t\r  ehp\n \t\r"));
    EXPECT_EQ(7, s->len);
    EXPECT_STREQ("ehp\n \t\r", s->s);
}

TEST(str_t, trim_right)
{
    // NULL
    str_t *s = str_trim_right(NULL);
    EXPECT_EQ(NULL, s);

    // empty string
    s = str_trim_right(str_new(""));
    EXPECT_EQ(0, s->len);
    EXPECT_STREQ("", s->s);

    // all spaces
    s = str_trim_right(str_new(" \t \r \n "));
    EXPECT_EQ(0, s->len);
    EXPECT_STREQ("", s->s);

    // only right
    s = str_trim_right(str_new(" \t\r\nehp"));
    EXPECT_EQ(7, s->len);
    EXPECT_STREQ(" \t\r\nehp", s->s);

    // only right
    s = str_trim_right(str_new("ehp\n \t\r"));
    EXPECT_EQ(3, s->len);
    EXPECT_STREQ("ehp", s->s);

    // both side
    s = str_trim_right(str_new(" \n\t\r  ehp\n \t\r"));
    EXPECT_EQ(9, s->len);
    EXPECT_STREQ(" \n\t\r  ehp", s->s);
}

TEST(str_t, split)
{
    // NULL
    array_t *arr = str_split_n(NULL, 0, -1);
    EXPECT_EQ(NULL, arr);

    // split zero
    arr = str_split_n(str_new("aaa:bb:c"), ':', 0);
    EXPECT_EQ(0, arr->len);

    // split one
    arr = str_split_n(str_new("aaa:bb:c"), ':', 1);
    EXPECT_EQ(1, arr->len);
    EXPECT_STREQ("aaa:bb:c", ((str_t*)arr->array[0])->s);

    // split two
    arr = str_split_n(str_new("aaa:bb:c"), ':', 2);
    EXPECT_EQ(2, arr->len);
    EXPECT_STREQ("aaa", ((str_t*)(char*)arr->array[0])->s);
    EXPECT_STREQ("bb:c", ((str_t*)(char*)arr->array[1])->s);

    // split three
    arr = str_split_n(str_new("aaa:bb:c"), ':', 3);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ("aaa", ((str_t*)(char*)arr->array[0])->s);
    EXPECT_STREQ("bb", ((str_t*)(char*)arr->array[1])->s);
    EXPECT_STREQ("c", ((str_t*)(char*)arr->array[2])->s);

    // split more
    arr = str_split_n(str_new("aaa:bb:c"), ':', 10);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ("aaa", ((str_t*)(char*)arr->array[0])->s);
    EXPECT_STREQ("bb", ((str_t*)(char*)arr->array[1])->s);
    EXPECT_STREQ("c", ((str_t*)(char*)arr->array[2])->s);

    // split all
    arr = str_split_n(str_new("aaa:bb:c"), ':', -1);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ("aaa", ((str_t*)(char*)arr->array[0])->s);
    EXPECT_STREQ("bb", ((str_t*)(char*)arr->array[1])->s);
    EXPECT_STREQ("c", ((str_t*)(char*)arr->array[2])->s);

    // special
    arr = str_split_n(str_new(":"), ':', -1);
    EXPECT_EQ(2, arr->len);
    EXPECT_STREQ("", ((str_t*)(char*)arr->array[0])->s);
    EXPECT_STREQ("", ((str_t*)(char*)arr->array[1])->s);

    // special
    arr = str_split_n(str_new(":"), ':', 1);
    EXPECT_EQ(1, arr->len);
    EXPECT_STREQ(":", ((str_t*)(char*)arr->array[0])->s);

    // special
    arr = str_split_n(str_new("::"), ':', -1);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ("", ((str_t*)(char*)arr->array[0])->s);
    EXPECT_STREQ("", ((str_t*)(char*)arr->array[1])->s);
    EXPECT_STREQ("", ((str_t*)(char*)arr->array[2])->s);

    // special
    arr = str_split_n(str_new("::"), ':', 1);
    EXPECT_EQ(1, arr->len);
    EXPECT_STREQ("::", ((str_t*)(char*)arr->array[0])->s);

    // special
    arr = str_split_n(str_new("::"), ':', 2);
    EXPECT_EQ(2, arr->len);
    EXPECT_STREQ("", ((str_t*)(char*)arr->array[0])->s);
    EXPECT_STREQ(":", ((str_t*)(char*)arr->array[1])->s);

    // special
    arr = str_split_n(str_new("aaa::b"), ':', -1);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ("aaa", ((str_t*)(char*)arr->array[0])->s);
    EXPECT_STREQ("", ((str_t*)(char*)arr->array[1])->s);
    EXPECT_STREQ("b", ((str_t*)(char*)arr->array[2])->s);

    // special
    arr = str_split_n(str_new("aaa::"), ':', -1);
    EXPECT_EQ(3, arr->len);
    EXPECT_STREQ("aaa", ((str_t*)(char*)arr->array[0])->s);
    EXPECT_STREQ("", ((str_t*)(char*)arr->array[1])->s);
    EXPECT_STREQ("", ((str_t*)(char*)arr->array[1])->s);
}
