/**
MIT License

Copyright (c) 2026 Tellcode1

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef u_MODULE_H
#define u_MODULE_H

#ifndef u_MODULE_DEFAULT_TICKS_PER_SECOND
#  define u_MODULE_DEFAULT_TICKS_PER_SECOND 60
#endif

#ifndef u_MODULE_NAME_MAX_LENGTH
#  define u_MODULE_NAME_MAX_LENGTH 64
#endif

#ifndef u_MODULE_EVENT_RING_BUFFER_SIZE
#  define u_MODULE_EVENT_RING_BUFFER_SIZE 128
#endif

#ifndef u_MODULE_EVENT_DATA_SIZE
#  define u_MODULE_EVENT_DATA_SIZE 64
#endif

#include "../std/include/types.h"

#include <stddef.h>
#include <stdio.h>

typedef struct umodsys umodsys;
typedef struct umod    umod;

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * Called on initialization of module by the system.
   * Return 0 on success, anything else on error.
   * On error, append an entry to the logfile before returning.
   *
   * All members of the module struct will be initialized, excluding the module_data pointer (set to NULL).
   *
   * User must set the module_data pointer (if needed).
   */
  typedef int (*umod_init_fn)(umod* mod, void* user_data);

  /**
   * Free module resources.
   */
  typedef void (*umod_exit_fn)(umod* mod);

  /**
   * "Tick" (update) a module.
   * The module is expected to poll the queue for msgs and respond:
   * either by posting msgs for next frame or something else.
   * By default, it is called u_MODULE_DEFAULT_TICKS_PER_SECOND times a second.
   * However, individual modules may specify their own tick rates at runtime.
   */
  typedef void (*umod_tick_fn)(umod* mod);

  typedef struct umod_msg
  {
    // Values for both of these are found in the module struct
    // DO NOT ADDRESS RANDOM MODULES!!!!
    // For an msg to be passed to a module (and consumed),
    // The msg.sender_mask & module.receiver_mask must not euual 0
    // I.E. The sender_mask specifies which groups (bits) can send msgs to a module
    // And the receiver mask specifies which groups (bits) it can receive from.
    u32 sender_mask;
    int msg_id; // No inherent meaning. Interpreted by the receiving module.

    u8 data[u_MODULE_EVENT_DATA_SIZE];
  } umod_msg;

  typedef struct umod_desc // Module description
  {
    char name[u_MODULE_NAME_MAX_LENGTH];

    u32 send_mask;    // Mask for when this module is sending msgs
    u32 receive_mask; // Mask for when this module is receiving msgs

    /* If any function pointer is NULL, it will not be called. */
    umod_init_fn init;
    umod_exit_fn exit;
    umod_tick_fn tick;
  } umod_desc;

  /**
   * Log an entry to the logfile of the module system.
   * No umod_logfile() function is provided to minimize risk of use after frees in case of changing logfiles.
   */
  size_t umod_log(umodsys* sys, const char* fmt, ...);

  /**
   * Set the logfile of the uModsys.
   * Ensure the file outlives the structure!
   */
  void umod_set_logfile(umodsys* sys, FILE* f);

  /**
   * Initialize a umodsys.
   * Returns 0 on success, any other integer on failure.
   */
  int  umodsys_init(umodsys* sys);
  void umodsys_free(umodsys* sys);

  /**
   * Tick every module in the system forcefully.
   * Only use this on reloads.
   * Returns number of modules ticked.
   */
  int umodsys_tick_all(umodsys* sys);

  /**
   * Tick each module in the system, if needed.
   * Returns number of modules ticked.
   */
  int umodsys_tick(umodsys* sys);

  /**
   * Post an msg to the queue, which will be read by the module on next tick.
   * Thus, all modules here follow this pattern: Post first frame, Poll next frame.
   * Though it does mean all input reactions (module responses) are one frame late.
   */
  void umod_send(umodsys* sys, const umod_msg* ev);

  /**
   * Poll the msg queue and check for all msgs addressed to this module.
   * Returns the number of msgs addressed.
   * handler is allowed to be NULL.
   */
  int umod_recv(umodsys* sys, const char* module_name, void (*handler)(umod* mod, const umod_msg* ev));

  /**
   * Poll the msg queue for all messages that AND with the receive_mask.
   * Returns the number of msgs addressed.
   * handler is allowed to be NULL.
   */
  int umodsys_recv(umodsys* sys, u32 receive_mask, void (*handler)(const umod_msg* ev));

  /**
   * Flush (clear) the msg queue.
   * Call at the end of each game loop iteration.
   * Returns number of msgs dropped from the queue.
   */
  int umodsys_clear_queue(umodsys* sys);

  /**
   * Find the first module with the name.
   */
  umod* umodsys_find_module(const umodsys* sys, int id);
  umod* umodsys_find_module_by_name(const umodsys* sys, const char* name);

  /**
   * Initialize a module using its description.
   * On success, returns 0.
   */
  int umod_init(const umod_desc* desc, void* init_function_arg, umodsys* sys);

  /**
   * Free module from the system.
   * Internally calls the module's exit function.
   */
  void umod_free(umodsys* sys, const char* module_name);

  /**
   * Takes effect instantaneously, flushing every tick needed.
   */
  int umod_set_tick_rate(umodsys* sys, const char* module_name, int ticks_per_sec);

  struct umod
  {
    umodsys* sys;
    void*    module_data;

    umod_desc desc;
    u32       name_hash;

    int ticks_per_sec;

    // Nanosecond delta time accumulator for ticks, handled by msys
    u64 dt_ns_accum;

    // Nanoseconds when the module was ticked last.
    u64 last_tick_ns;
  };

  struct umodsys
  {
    struct umod* modules;
    int          nmodules;
    int          cmodules; // capacity

    /* stderr by default. */
    FILE* logfile;

    // We don't accidentally want to allocate 10KB on the stack. Do we..?
    umod_msg* msgs; // size=u_MODULE_EVENT_RING_BUFFER_SIZE

    int head, tail; // msg queue head=read tail=write
  };

  // FNV1-A 32 bit hash for strings
  // We could use an unrolled loop for 64 byte strings (for names, fixed length)
  // But that'd be a hassle and could only be used a few places.
  // Most of them including hashing the modules name. Which we already cache.
  // So we don't.
  static inline u32
  u_name_hash(const char* data)
  {
    u32 hash = 0x811C9DC5;
    for (size_t i = 0; data[i] != 0; ++i)
    {
      hash ^= data[i];
      hash *= 0x01000193;
    }
    return hash;
  }

#ifdef __cplusplus
}
#endif

#endif // u_MODULE_H
