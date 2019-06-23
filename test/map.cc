#include "map.h"
#include "map.h"
#include <gtest/gtest.h>

TEST(map_t, basic)
{
    map_t map = map_new();

    map_set(map, "k1", "v1");
    EXPECT_STREQ("v1", map_get(map, "k1"));

    map_set(map, "k1", "v11");
    EXPECT_STREQ("v11", map_get(map, "k1"));
    
    map_set(map, "k2", "v2");
    EXPECT_STREQ("v2", map_get(map, "k2"));
}
