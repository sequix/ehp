#pragma once

#include "map.h"

// /[path][?query][#fragment]
// /baidu.com/subpath?k1=v1&k2=v2#loc
//    path: /baidu.com/subpath
//    query: map{k1:v1, k2:v2}
//    raw_query: ?k1=v1&k2=v2
//    fragment: loc
typedef struct {
    map_t query;
    str_t path;
    str_t raw_query;
    str_t fragment;
} url_st;

typedef url_st* url_t;

url_t url_parse(str_t s);

void url_free(url_t url);
