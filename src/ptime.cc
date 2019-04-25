#define _POSIX_C_SOURCE 200809L

#include "ptime.h"
#include "log.h"
#include "mem.h"
#include <time.h>
#include <math.h>
#include <time.h>

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
    pt->nanosecond = spec.tv_nsec;
    return pt;
}
