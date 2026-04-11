#include "../winmodule.h"

#include <SDL3/SDL_error.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>

int
uwin_init(umod* mod, void* user_data)
{
  const uwin_init_info* desc = (uwin_init_info*)user_data;

  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
  {
    umod_log(mod->sys, "SDL initialization failed: %s\n", SDL_GetError());
    return -1;
  }

  mod->module_data = (void*)SDL_CreateWindow(desc->title, desc->width, desc->height, desc->flags);
  if (!mod->module_data)
  {
    umod_log(mod->sys, "SDL Window creation failed: %s\n", SDL_GetError());
    return -1;
  }

  return 0;
}

void
uwin_free(umod* mod)
{
  if (!mod) return;

  SDL_DestroyWindow((SDL_Window*)mod->module_data);
  SDL_Quit();
}

void
uwin_tick(umod* mod)
{
  SDL_Event ev;
  while (SDL_PollEvent(&ev))
  {
    umod_msg msg;
    msg.sender_mask        = U_WIN_MESSAGE_MASK;
    uwin_msg_payload* data = (uwin_msg_payload*)msg.data;

    if (ev.type == SDL_EVENT_QUIT)
    {
      msg.msg_id = U_WIN_MSG_EXIT;
      umod_send(mod->sys, &msg);
    }
    else if (ev.type == SDL_EVENT_KEY_DOWN)
    {
      msg.msg_id         = U_WIN_MSG_KEY_DOWN;
      data->key.scancode = ev.key.scancode;
      umod_send(mod->sys, &msg);
    }
    else if (ev.type == SDL_EVENT_KEY_UP)
    {
      msg.msg_id         = U_WIN_MSG_KEY_UP;
      data->key.scancode = ev.key.scancode;
      umod_send(mod->sys, &msg);
    }
    else if (ev.type == SDL_EVENT_MOUSE_MOTION)
    {
      msg.msg_id         = U_WIN_MSG_MOUSE_MOVE;
      data->mouse_move.x = ev.motion.xrel;
      data->mouse_move.y = ev.motion.yrel;
      umod_send(mod->sys, &msg);
    }
    else if (ev.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
      msg.msg_id              = (ev.button.clicks == 2) ? U_WIN_MSG_MOUSE_DOUBLE_CLICK : U_WIN_MSG_MOUSE_CLICK;
      data->mouse_bton.x      = ev.button.x;
      data->mouse_bton.y      = ev.button.y;
      data->mouse_bton.button = ev.button.button;
      umod_send(mod->sys, &msg);
    }
    else if (ev.type == SDL_EVENT_WINDOW_RESIZED)
    {
      msg.msg_id               = U_WIN_MSG_WIN_RESIZE;
      data->win_resized.width  = ev.window.data1;
      data->win_resized.height = ev.window.data2;
      umod_send(mod->sys, &msg);
    }
    else if (ev.type == SDL_EVENT_MOUSE_WHEEL)
    {
      msg.msg_id     = U_WIN_MSG_MOUSE_SCROLL;
      data->scroll.x = ev.wheel.x;
      data->scroll.y = ev.wheel.y;
      umod_send(mod->sys, &msg);
    }
  }
}
