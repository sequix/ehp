#pragma once

#include <stdint.h>
#include <stdbool.h>

// ptime_t stands for precised-time_t
typedef struct {
    uint64_t year;        // eg: 2018
    uint64_t month;       // eg: 12
    uint64_t day;         // eg: 31
    uint64_t hour;        // eg: 23
    uint64_t minute;      // eg: 59
    uint64_t second;      // eg: 59
    uint64_t millisecond; // eg: 999
    //uint64_t nanosecond;  // eg: 123456
} ptime_st;

typedef ptime_st* ptime_t;

ptime_t ptime(void);

#define ptime_epoch() ptime_to_epoch(ptime())

// return true if t1 < t2
bool ptime_less(ptime_t t1, ptime_t t2);

// the time from t, after millisecond
ptime_t ptime_add(ptime_t t, uint64_t millisecond);

// difference between two ptime_t in milliseconds
uint64_t ptime_sub(ptime_t t1, ptime_t t2);

// milliseconds since epoch
uint64_t ptime_to_epoch(ptime_t t);

ptime_t epoch_to_ptime(uint64_t epoch);
