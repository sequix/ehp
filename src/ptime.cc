#define _POSIX_C_SOURCE 200809L

#include "ptime.h"
#include "log.h"
#include "mem.h"
#include <time.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdint.h>

ptime_t ptime(void)
{
	ptime_t pt = talloc(ptime_st);
    if (pt == NULL) {
        return NULL;
    }

    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);

	time_t now_sec = spec.tv_sec;	
    long ms = round(spec.tv_nsec / 1.0e6);
    if (ms > 999) {
        ++now_sec;
        ms = 0;
    }

    struct tm tmt;
    localtime_r(&now_sec, &tmt);

    pt->year = tmt.tm_year + 1900;
    pt->month = tmt.tm_mon + 1;
    pt->day = tmt.tm_mday;
    pt->hour = tmt.tm_hour;
    pt->minute = tmt.tm_min;
    pt->second = tmt.tm_sec;
    pt->millisecond = ms;
    return pt;
}

bool ptime_less(ptime_t t1, ptime_t t2)
{
    if (t1->year != t2->year) {
        return t1->year < t2->year;
    }
    if (t1->month != t2->month) {
        return t1->month < t2->month;
    }
    if (t1->day != t2->day) {
        return t1->day < t2->day;
    }
    if (t1->hour != t2->hour) {
        return t1->hour < t2->hour;
    }
    if (t1->minute != t2->minute) {
        return t1->minute < t2->minute;
    }
    if (t1->second != t2->second) {
        return t1->second < t2->second;
    }
    return t1->millisecond < t2->millisecond;
}

uint64_t ptime_to_epoch(ptime_t t)
{
    struct tm ttm;
    memset(&ttm, 0, sizeof(ttm));
    ttm.tm_sec = t->second;
    ttm.tm_min = t->minute;
    ttm.tm_hour = t->hour;
    ttm.tm_mday = t->day;
    ttm.tm_mon = t->month;
    ttm.tm_year = t->year;
    time_t tepoch_sec = mktime(&ttm);
    uint64_t tepoch_msec = tepoch_sec * 1000 + t->millisecond;
    return tepoch_msec;
}

ptime_t epoch_to_ptime(uint64_t epoch)
{
    struct tm ttm;
    time_t epoch_sec = epoch / 1000;
    gmtime_r(&epoch_sec, &ttm);

    ptime_t t = talloc(ptime_st);
    t->year = ttm.tm_year;
    t->month = ttm.tm_mon;
    t->day = ttm.tm_mday;
    t->hour = ttm.tm_hour;
    t->minute = ttm.tm_min;
    t->second = ttm.tm_sec;
    t->millisecond = epoch % 1000;
    return t;
}

ptime_t ptime_add(ptime_t t, uint64_t millisecond)
{
    uint64_t ntepoch_msec = ptime_to_epoch(t) + millisecond;
    return epoch_to_ptime(ntepoch_msec);
}

uint64_t ptime_sub(ptime_t t1, ptime_t t2)
{
    if (ptime_less(t1, t2)) {
        return ptime_sub(t2, t1);
    }
    return ptime_to_epoch(t1) - ptime_to_epoch(t2);
}
