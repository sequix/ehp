#include "array.h"
#include "mem.h"
#include <gtest/gtest.h>

TEST(array_t, basic)
{
    len_t init_cap = 5;
    array_t arr = array_new(init_cap);
    EXPECT_EQ(init_cap, arr->cap);

    int n = init_cap * 3;

    for (int i = 1; i <= n; ++i) {
        int *p = talloc(int);
        *p = i;
        array_push_back(arr, p);
    }

    EXPECT_EQ(n, arr->len);
    EXPECT_EQ(init_cap << 2, arr->cap);

    int **pa = (int**)arr->array;
    for (int i = 1; i <= n; ++i) {
        EXPECT_EQ(i, *pa[i-1]);
    }

    for (int i = 1; i <= n; ++i) {
        EXPECT_EQ(i, array_get_int(arr, i-1));
    }
}
