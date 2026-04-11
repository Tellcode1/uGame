#ifndef UESL_TIME_H
#define UESL_TIME_H

#include "../../ESL/bfunc.h"
#include "../../time.h"

static inline e_var
uesl_time_now(e_var* args, u32 nargs)
{
  (void)args;
  (void)nargs;
  return e_var_from_float(UTIME_NSECS_TO_SECS(utime_hwclock_nsecs()));
}

static const e_builtin_func uesl_timer_funcs[] = {
  { "u::time::now", "Get current time in seconds, since a fixed time point (hardware/system dependant)", "fn u::time::now() -> float", 0xFFFFFFFF, 0, 0, uesl_time_now },

};

#endif // UESL_TIME_H