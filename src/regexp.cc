#include "regexp.h"
#include "log.h"
#include "util.h"
#include <regex.h>
#include <stdio.h>
#include <string.h>

regexp_t *regexp_url = NULL;
regexp_t *regexp_http_req_first = NULL;

int regexp_init(void)
{
    regexp_url = regexp_new(REGEXP_PTRN_URL);
    if (regexp_url == NULL) {
        return -1;
    }

    regexp_http_req_first = regexp_new(REGEXP_PTRN_HTTP_REQ_FIRST);
    if (regexp_http_req_first == NULL) {
        return -2;
    }
    return 0;
}

regexp_t *regexp_new(char *pattern)
{
    regexp_t *reg = talloc(regexp_t);
    if (reg == NULL) {
        return NULL;
    }

    int err = regcomp(reg, pattern, 0);
    if (err != 0) {
        free(reg);
        return NULL;
    }
    return reg;
}

int regexp_match(regexp_t *reg, const char *text, int nsubs,
    regexp_submatch_t *subs)
{
    return regexec(reg, text, nsubs, subs, 0);
    //char buf[BUFSIZ];
    //int ret = regexec(reg, text, nsubs, subs, 0);
    //regerror(ret, reg, buf, BUFSIZ);
    //log_debug("regexp_match: %s", buf);
    //return ret;
}
