#ifndef U_VK_MODULE_H
#define U_VK_MODULE_H

#include "../std/include/types.h"
#include "module.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#define U_GL_MESSAGE_MASK (0x4)

typedef struct ugl_module
{
  SDL_Window*   window;
  SDL_GLContext context;
} ugl_module;

typedef struct ugl_init_info
{
  int major_version;
  int minor_version;
  int profile;
  int double_buffering;
} ugl_init_info;

int  ugl_init(umod* mod, void* init_info);
void ugl_exit(umod* mod);
void ugl_tick(umod* mod);

static inline umod_desc
ugl_module_desc(void)
{
  return (umod_desc){
    .name         = "uGL",
    .send_mask    = U_GL_MESSAGE_MASK,
    .receive_mask = 0,
    .init         = ugl_init,
    .exit         = ugl_exit,
    .tick         = ugl_tick,
  };
}

#endif