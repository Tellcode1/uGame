#include "../time.h"

#include "../getns.h"

#include <sys/time.h>
#include <time.h>

u64
utime_hwclock_nsecs(void)
{
  return getns();
}

u64
utime_hwclock_msecs(void)
{
#ifdef _WIN32
  LARGE_INTEGER frequency;
  LARGE_INTEGER start;
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);
  return (u64)(start.QuadPart * 1000.0 / frequency.QuadPart);
#else
  struct timespec spec;
#  if _POSIX_C_SOURCE >= 200809L
  clock_gettime(CLOCK_REALTIME, &spec);
#  else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  spec.tv_sec  = tv.tv_sec;
  spec.tv_nsec = tv.tv_usec * 1000;
#  endif
  return spec.tv_sec * 1000 + (spec.tv_nsec / 1000000);
#endif
}

u64
utime_hwclock_secs(void)
{
#if _POSIX_C_SOURCE >= 200809L
  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  return spec.tv_sec;
#else
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec;
#endif
}

void
utime_get_time(utime* otime)
{
  time_t now;
  time(&now); // Get the current time as an integer

  // Convert to local time structure
  struct tm* local = localtime(&now);

  if (otime)
  {
    otime->year  = local->tm_year + 1900;
    otime->month = local->tm_mon + 1;
    otime->day   = local->tm_mday;
    otime->hour  = local->tm_hour;
    otime->min   = local->tm_min;
    otime->sec   = local->tm_sec;
  }
}

utimer
utimer_init(void)
{
  return (utimer){ .nsecs = getns() };
}

u64
utimer_elapsed_nsecs(utimer t)
{
  return getns() - t.nsecs;
}

u64
utimer_elapsed_msecs(utimer t)
{
  return (getns() - t.nsecs) * (u64)1e-6;
}

u64
utimer_elapsed_secs(utimer t)
{
  return (getns() - t.nsecs) * (u64)1e-9;
}