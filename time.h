#ifndef U_TIME_H
#define U_TIME_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "../std/include/types.h"

#define UTIME_NSECS_TO_SECS(nsec) ((nsec) * 1e-9)
#define UTIME_NSECS_TO_MSECS(nsec) ((nsec) * 1e-6)
#define UTIME_MSECS_TO_SECS(nsec) ((nsec) * 1e-3)
#define UTIME_MSECS_TO_NSECS(msec) ((msec) * 1e6)
#define UTIME_SECS_TO_NSECS(sec) ((sec) * 1e9)
#define UTIME_SECS_TO_MSECS(sec) ((sec) * 1e3)

  typedef struct utime
  {
    int year;  // Year in C.E (2026,etc.)
    int month; // 0 is January, 11 is December
    int day;   // Day of month, 1 to 31
    int hour;  // Hours in 24 hour format
    int min;   // Minutes
    int sec;   // Floored seconds
  } utime;

  typedef struct utimer
  {
    u64 nsecs; // Nanoseconds when the timer was triggered
  } utimer;

  /* Get the nanosecond timer from the highest resolution clock */
  /* Doesn't correspond to a set meaning, use utime_get_time() to get the desktop time */
  u64 utime_hwclock_nsecs(void);
  u64 utime_hwclock_msecs(void);
  u64 utime_hwclock_secs(void);

  void utime_get_time(utime* otime);

  utimer utimer_init();

  u64 utimer_elapsed_nsecs(utimer t);
  u64 utimer_elapsed_msecs(utimer t);
  u64 utimer_elapsed_secs(utimer t);

#ifdef __cplusplus
}
#endif

#endif // U_TIME_H