#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>

struct timing {
  uint64_t minutes;
  uint32_t hours;
  uint8_t daysofweek;
};

int timing_compare_timing(struct timing *t1, struct timing *t2);

#endif // TIMING_H
