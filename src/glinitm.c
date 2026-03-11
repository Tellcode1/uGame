#include "../glad.h"
#include "../glmodule.h"

#include <SDL3/SDL_video.h>
#include <stddef.h>
#include <stdlib.h>

int
ugl_init(umod* mod, void* init_info)
{
  const ugl_init_info* ini = init_info;

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, ini->major_version);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, ini->minor_version);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, ini->profile);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, ini->double_buffering);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

  mod->module_data = malloc(sizeof(ugl_module));
  if (!mod->module_data) return -1;

  ugl_module* ugl = mod->module_data;
  SDL_Window* win = (SDL_Window*)umodsys_find_module_by_name(mod->sys, "uWindow")->module_data;

  ugl->context = SDL_GL_CreateContext(win);
  if (!ugl->context)
  {
    umod_log(mod->sys, "SDL_GL_CreateContext failed: %s\n", SDL_GetError());
    return -1;
  }

  SDL_GL_MakeCurrent(win, ugl->context);

  ugl->window = win;

  if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) return -1;
  // if (gladLoadGL()) { return -1; }

  // SDL_GL_LoadLibrary(NULL);

  return 0;
}

void
ugl_exit(umod* mod)
{
  ugl_module* ugl = mod->module_data;
  SDL_GL_DestroyContext(ugl->context);
  free(ugl);
}

void
ugl_tick(umod* mod)
{
  ugl_module* ugl = mod->module_data;

  glClear(GL_COLOR_BUFFER_BIT);
  SDL_GL_SwapWindow(ugl->window);
}
