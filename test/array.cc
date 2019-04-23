#include "array.h"
#include "mem.h"
#include <gtest/gtest.h>

TEST(array_t, basic)
{
    array_t *arr = array_new();
    EXPECT_EQ(ARRAY_INIT_CAPACITY, arr->cap);

    int n = ARRAY_INIT_CAPACITY * 3;

    for (int i = 1; i <= n; ++i) {
        int *p = talloc(int);
        *p = i;
        array_push_back(arr, p);
    }

    EXPECT_EQ(n, arr->len);
    EXPECT_EQ(ARRAY_INIT_CAPACITY << 2, arr->cap);

    int **pa = (int**)arr->array;
    for (int i = 1; i <= n; ++i) {
        EXPECT_EQ(i, *pa[i-1]);
    }
}
