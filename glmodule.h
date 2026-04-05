#ifndef U_VK_MODULE_H
#define U_VK_MODULE_H

#include "module.h"
#include "types.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#define U_GL_MESSAGE_MASK (0x4)
#define UGL_MAX_COMMANDS 4096

struct ugl_command;
struct ucamera;

typedef struct ugl_module
{
  SDL_Window*   window;
  SDL_GLContext context;

  u32                 ncmds;
  struct ugl_command* cmds;

  u32 vao;
  u32 vbo;
  u32 shader;

  i32 u_pos_loc;
  i32 u_size_loc;
  i32 u_color_loc;
  i32 u_resolution_loc;
  i32 u_mvp_loc;

  struct ucamera* main_cam;
} ugl_module;

typedef struct ugl_init_info
{
  int major_version;
  int minor_version;
  int profile;
  int double_buffering;
} ugl_init_info;

int  ugl_init(umod* mod, void* init_info);
void ugl_free(umod* mod);
void ugl_tick(umod* mod);

static inline struct ucamera*
ugl_set_active_camera(ugl_module* ugl, struct ucamera* cam)
{
  struct ucamera* last = ugl->main_cam;
  ugl->main_cam        = cam;
  return last;
}

/**
 * Returns shader ID.
 */
u32 ugl_load_shader(const char* vertex_source, const char* fragment_source);

void ugl_cmd_push(ugl_module* mod, const struct ugl_command* cmd);
void ugl_cmd_flush(ugl_module* mod);

int  ugl_init_draw_sys(ugl_module* mod);
void ugl_free_draw_sys(ugl_module* mod);

/**
 * Convert and submit all the OpenGL calls from the command buffer
 */
void ugl_draw(ugl_module* mod);

static inline umod_desc
ugl_module_desc(void)
{
  return (umod_desc){
    .name         = "uGL",
    .send_mask    = U_GL_MESSAGE_MASK,
    .receive_mask = 0,
    .init         = ugl_init,
    .free         = ugl_free,
    .tick         = ugl_tick,
  };
}

static inline const char*
ugl_error_str(GLenum err)
{
  switch (err)
  {
    case GL_NO_ERROR: return "GL_NO_ERROR";
    case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
    case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
    default: return "unknown";
  }
}

#endif