#include "../time.h"

#include <stdbool.h>
#include <stdio.h>

int
main(void)
{
  utimer tmr = utimer_init();
  while (true)
  {
    // utime tm;
    // utime_get_time(&tm);
    printf("%.3f\n", (double)utimer_elapsed_nsecs(tmr) / 1e9);

    // printf("%i/%i/%i %i:%i:%i %.3f\n", tm.day, tm.month, tm.year, tm.hour, tm.min, tm.sec, (double)utime_hwclock_nsecs() / 1e9);
  }
  return 0;
}