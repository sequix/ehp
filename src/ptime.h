#pragma once

// ptime_t stands for precised-time_t
typedef struct {
    int  year;        // eg: 2018
    int  month;       // eg: 12
    int  day;         // eg: 31
    int  hour;        // eg: 23
    int  minute;      // eg: 59
    int  second;      // eg: 59
    int  millisecond; // eg: 999
    long nanosecond;  // eg: 42134318
} ptime_st;

typedef ptime_st* ptime_t;

ptime_t ptime(void);
