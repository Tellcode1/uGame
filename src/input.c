#include "../input.h"

#include <SDL3/SDL_keyboard.h>
#include <SDL3/SDL_mouse.h>
#include <cglm/vec2.h>

int
uinput_init(umod* mod, void* user_data)
{
  (void)user_data;

  mod->module_data = calloc(1, sizeof(uinput_module));
  if (!mod->module_data) return -1;

  // uinput_module* input = (uinput_module*)mod->module_data;

  return 0;
}

void
uinput_tick(umod* mod)
{
  uinput_module* input = (uinput_module*)mod->module_data;

  glm_vec2_zero(input->mouse_scroll);
  glm_vec2_zero(input->mouse_move);

  const bool* sdl_keys = SDL_GetKeyboardState(NULL);
  if (!sdl_keys) return;

  memcpy(input->last_key_state, input->key_state, SDL_SCANCODE_COUNT);
  memcpy(input->key_state, sdl_keys, SDL_SCANCODE_COUNT);

  vec2 old_mouse_pos;
  glm_vec2_copy(input->mouse_pos, old_mouse_pos);

  float x, y;
  u32   mouse_mask        = SDL_GetMouseState(&x, &y);

  input->mouse_move[0]    = x - old_mouse_pos[0];
  input->mouse_move[1]    = y - old_mouse_pos[1];

  input->mouse_pos[0]     = x;
  input->mouse_pos[1]     = y;

  input->last_mouse_state = input->mouse_state;
  input->mouse_state      = mouse_mask;
}

void
uinput_free(umod* mod)
{ free(mod->module_data); }

void
uinput_mouse_position(uinput_module* input, vec2 out_position)
{ glm_vec2_copy(input->mouse_pos, out_position); }

bool
uinput_mouse_pressed(uinput_module* input, umouse_button button)
{ return (bool)(input->mouse_state & (1 << (button))); }

bool
uinput_key_pressed(uinput_module* input, SDL_Scancode key)
{ return (bool)(input->key_state[key] && !input->last_key_state[key]); }

bool
uinput_key_held(uinput_module* input, SDL_Scancode key)
{ return (bool)(input->key_state[key] && input->last_key_state[key]); }

bool
uinput_key_released(uinput_module* input, SDL_Scancode key)
{ return (bool)(!input->key_state[key] && input->last_key_state[key]); }
