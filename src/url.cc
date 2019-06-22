#include "mem.h"
#include "url.h"
#include "regexp.h"
#include "file.h"
#include <string.h>

static url_t url_new(void)
{
    url_t url = talloc(url_st);
    if (url == NULL) {
        return NULL;
    }
    url->query = map_new(64);
    if (url->query == NULL) {
        return NULL;
    }
    url->path = NULL;
    url->raw_query = NULL;
    url->fragment = NULL;
    return url;
}

static void url_parse_query(url_t url, const char *os, int left, int right)
{
    int len = right - left + 1;
    char *s = stalloc_a(len + 1, char);
    strncpy(s, os+left, len); 
    s[len] = '\0';

    for (int i = left + 1; i <= right; ) {
        int pequal = i;
        for (; pequal <= right && s[pequal] != '='; ++pequal);

        int psep = pequal + 1;
        for (; psep <= right && s[psep] != '&'; ++psep);

        s[pequal] = '\0';
        s[psep] = '\0';

        char *key = &s[i];
        char *value = &s[pequal + 1];
        map_set_c(url->query, key, value);
        i = psep + 1;
    }
}

url_t url_parse(str_t s)
{
    // 4 sub parts: entire url, path, raw_query, fragment
    regexp_submatch_st mat[4];

    if (regexp_match(regexp_url, s, 4, mat) != 0) {
        return NULL;
    }

    url_t url = url_new();
    if (url == NULL) {
        return NULL;
    }

    // TODO: error check
    int path_len = mat[1].rm_eo - mat[1].rm_so + 1;
    url->path = str_sub(str_from(s), mat[1].rm_so, path_len);
    if (url->path == NULL) {
        return NULL;
    }

    for (int i = 2; i < 4; ++i) {
        int so = mat[i].rm_so;
        if (so == -1) {
            break;
        }
        if (s[so] == '?') {
            url_parse_query(url, s, so, mat[i].rm_eo);
            int query_len = mat[i].rm_eo - mat[i].rm_so + 1;
            url->fragment = str_sub(str_from(s), mat[i].rm_so, query_len);
            if (url->fragment == NULL) {
                return NULL;
            }
        } else if (s[so] == '#') {
            int frag_len = mat[i].rm_eo - mat[i].rm_so + 1;
            url->fragment = str_sub(str_from(s), mat[i].rm_so, frag_len);
            if (url->fragment == NULL) {
                return NULL;
            }
        } else {
            // wrong
        }
    }
    return url;
}
