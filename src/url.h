#pragma once

#include "map.h"

// /[path][?query][#fragment]
// /baidu.com/subpath?k1=v1&k2=v2#loc
//    path: /baidu.com/subpath
//    query: map{k1:v1, k2:v2}
//    raw_query: ?k1=v1&k2=v2
//    fragment: loc
typedef struct {
    char*  path;
    map_t* query;
    char*  raw_query;
    char*  fragment;
} url_t;

url_t *url_parse(char *s);

void url_free(url_t *url);
