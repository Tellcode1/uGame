#ifndef UESL_INPUT_H
#define UESL_INPUT_H

#include "../../ESL/bfunc.h"
#include "../../ESL/bvar.h"

#include <SDL3/SDL_scancode.h>

e_var uesl_input_mouse_position(e_var* args, u32 nargs);
e_var uesl_input_mouse_delta(e_var* args, u32 nargs);
e_var uesl_input_mouse_scroll_delta(e_var* args, u32 nargs);
e_var uesl_input_key_pressed(e_var* args, u32 nargs);
e_var uesl_input_key_released(e_var* args, u32 nargs);
e_var uesl_input_key_held(e_var* args, u32 nargs);

static const e_builtin_func uesl_input_functions[] = {
  {
      "uinput::get_mouse_position",
      "Get the current mouse position, as a vector2, in window space (0->window_width, 0->window_height) where 0x0 is top left corner of the window",
      "fn uinput::get_mouse_position() -> vec2",
      0xFFFFFFFF,
      0,
      0,
      uesl_input_mouse_position,
  },
  {
      "uinput::get_mouse_delta",
      "How much the mouse has moved since the last frame, window space.",
      "fn uinput::get_mouse_delta() -> vec2",
      0xFFFFFFFF,
      0,
      0,
      uesl_input_mouse_delta,
  },
  {
      "uinput::get_mouse_scroll_delta",
      "How much the user has scrolled since the last frame, window space. You should really only care about the Y component (+ upwards, - downwards)",
      "fn uinput::get_mouse_scroll_delta() -> vec2",
      0xFFFFFFFF,
      0,
      0,
      uesl_input_mouse_scroll_delta,
  },
  {
      "uinput::is_key_pressed",
      "",
      "",
      0xFFFFFFFF,
      1,
      1,
      uesl_input_key_pressed,
  },
  {
      "uinput::is_key_released",
      "",
      "",
      0xFFFFFFFF,
      1,
      1,
      uesl_input_key_released,
  },
  {
      "uinput::is_key_held",
      "",
      "",
      0xFFFFFFFF,
      1,
      1,
      uesl_input_key_held,
  },
};

static const e_builtin_var uesl_input_vars[] = {
  { .name = "uinput::scancode::W", .type = E_VARTYPE_INT, .value = { .i = SDL_SCANCODE_W } },
  { .name = "uinput::scancode::A", .type = E_VARTYPE_INT, .value = { .i = SDL_SCANCODE_A } },
  { .name = "uinput::scancode::S", .type = E_VARTYPE_INT, .value = { .i = SDL_SCANCODE_S } },
  { .name = "uinput::scancode::D", .type = E_VARTYPE_INT, .value = { .i = SDL_SCANCODE_D } },

};

#endif // UESL_INPUT_H