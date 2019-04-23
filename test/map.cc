#include "map.h"
#include "mem.h"
#include <gtest/gtest.h>

TEST(array_t, basic)
{
    map_t *map = map_new(32);
    EXPECT_EQ(32, map->nbuckets);

    map_set_c(map, "k1", "v1");
}
