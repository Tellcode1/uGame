#ifndef U_FRAME_ALLOC_H
#define U_FRAME_ALLOC_H

#include "../std/include/types.h"

#include <stddef.h>

/**
 * Size in bytes of the frame arena
 */
#ifndef U_FRAME_ARENA_SIZE
#  define U_FRAME_ARENA_SIZE (1024 * 1024) /* 1MB */
#endif

typedef struct uframe_arena
{
  u8  buffer[U_FRAME_ARENA_SIZE];
  u32 offset;
} uframe_arena;

extern uframe_arena ufr;

static inline void*
ufr_alloc(size_t size) // Allocate from frame arena. Pointer invalidated on frame end.
{
  if (ufr.offset + size > U_FRAME_ARENA_SIZE) return NULL;
  void* buf = ufr.buffer + ufr.offset;
  ufr.offset += size;
  ufr.offset = (ufr.offset + 7) & ~7; // align to 8 bytes
  return buf;
}

// NOOP
static inline void
ufr_free(void* p)
{
  (void)p;
}

static inline void
ufr_clear()
{
  ufr.offset = 0;
}

#endif // U_FRAME_ALLOC_H
