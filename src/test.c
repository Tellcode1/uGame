#include "../glmodule.h"
#include "../winmodule.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>

static bool run = true;

static inline void
process_events(const umod_msg* msg)
{
  const uwin_msg_payload* pload = (uwin_msg_payload*)msg->data;
  switch ((uwin_msg_type)msg->msg_id)
  {
    case U_WIN_MSG_EXIT: run = false; break;
    case U_WIN_MSG_INIT: run = true; break;
    case U_WIN_MSG_KEY_DOWN:
      printf("Key %u down\n", pload->key.scancode);
      if (pload->key.scancode == SDL_SCANCODE_ESCAPE) run = false;
      break;
    case U_WIN_MSG_KEY_UP: printf("Key %u up\n", pload->key.scancode); break;
    case U_WIN_MSG_MOUSE_MOVE: printf("Mouse moved: %f %f\n", pload->mouse_move.x, pload->mouse_move.y); break;
    case U_WIN_MSG_MOUSE_CLICK: printf("Mouse button %u clicked %f %f\n", pload->mouse_bton.button, pload->mouse_bton.x, pload->mouse_bton.y); break;
    case U_WIN_MSG_MOUSE_DOUBLE_CLICK: printf("Mouse button %u double clicked %f %f\n", pload->mouse_bton.button, pload->mouse_bton.x, pload->mouse_bton.y); break;
  }
}

int
main(void)
{
  umodsys sys;
  if (umodsys_init(&sys)) return -1;

  uwin_init_info winit_info = {
    .width  = 800,
    .height = 600,
    .title  = "Poot dispenser here!!!",
    .flags  = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE,
  };

  umod_desc desc = uwin_module_info();
  if (umod_init(&desc, &winit_info, &sys)) return -1;

  ugl_init_info glinit_info = {
    .major_version    = 4,
    .minor_version    = 5,
    .profile          = SDL_GL_CONTEXT_PROFILE_CORE,
    .double_buffering = 1,
  };

  desc = ugl_module_desc();
  if (umod_init(&desc, &glinit_info, &sys)) return -1;

  while (run)
  {
    umodsys_tick(&sys);
    umodsys_recv(&sys, U_WIN_MESSAGE_MASK, process_events);
    umodsys_clear_queue(&sys);
  }

  umod_free(&sys, "uWindow");
  umod_free(&sys, "uGL");
  return 0;
}