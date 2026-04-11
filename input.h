#ifndef U_INPUT_MODULE_H
#define U_INPUT_MODULE_H

#include "module.h"

#include <SDL3/SDL_scancode.h>
#include <cglm/cglm.h>

#define U_INPUT_MESSAGE_MASK (0x4)

typedef enum umouse_button {
  UMOUSE_BUTTON_LEFT   = 0,
  UMOUSE_BUTTON_MIDDLE = 1,
  UMOUSE_BUTTON_RIGHT  = 2,
  UMOUSE_BUTTON_SIDE0, // Not necessarily side buttons.
  UMOUSE_BUTTON_SIDE1, // just the non normal ones.
  UMOUSE_BUTTON_SIDE2,
} umouse_button;

typedef struct uinput_module {
  bool last_key_state[SDL_SCANCODE_COUNT];
  bool key_state[SDL_SCANCODE_COUNT];

  u32 mouse_state;      // Bitmask. Clicked? 0 is left mouse button, 2 is middle, 3 is right
  u32 last_mouse_state; // Bitmask. Clicked? 0 is left mouse button, 2 is middle, 3 is right

  // Window space <0..width, 0...height>
  vec2 mouse_pos;
  vec2 mouse_move;   // How much the mouse has moved this frame
  vec2 mouse_scroll; // How much the mouse has scrolled this frame
} uinput_module;

/**
 * Initialize input module.
 */
int  uinput_init(umod* mod, void* user_data);
void uinput_tick(umod* mod);
void uinput_free(umod* mod);

static inline void uinput_mouse_position(uinput_module* input, vec2 out_position);
static inline bool uinput_mouse_pressed(uinput_module* input, umouse_button button);

static inline bool uinput_key_pressed(uinput_module* input, SDL_Scancode key);
static inline bool uinput_key_held(uinput_module* input, SDL_Scancode key);
static inline bool uinput_key_released(uinput_module* input, SDL_Scancode key);

// static inline umod_desc
// uinput_module_info(void)
// {
//   return (umod_desc){
//     .name         = "uWindow",
//     .send_mask    = U_WIN_MESSAGE_MASK,
//     .receive_mask = 0,
//     .init         = uwin_init,
//     .free         = uwin_free,
//     .tick         = uwin_tick,
//   };
// }

#endif // U_INPUT_MODULE_H
