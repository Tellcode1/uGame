#ifndef U_WINDOW_MODULE_H
#define U_WINDOW_MODULE_H

/**
 * In uGame, the window is responsible for both the windowing and input msgs.
 */

#include "module.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_video.h>

/*
 * The sender mask of the messages.
 * Your receiver mask must contain this bit to receive messages from the inputs module.
 */
#define U_WIN_MESSAGE_MASK (0x2)

typedef enum uwin_msg_type
{
  U_WIN_MSG_EXIT,               // Called to signal user to free window module and exit (User is trying to exit window)
  U_WIN_MSG_INIT,               // Called when window module is initialized.
  U_WIN_MSG_KEY_DOWN,           // Key pressed / repeat.
  U_WIN_MSG_KEY_UP,             // Key unpressed.
  U_WIN_MSG_MOUSE_MOVE,         // Mouse moved.
  U_WIN_MSG_MOUSE_CLICK,        // Mouse clicked.
  U_WIN_MSG_MOUSE_DOUBLE_CLICK, // Mouse double clicked. Both the CLICK and DOUBLE_CLICK events are posted.
} uwin_msg_type;

typedef struct
{
  u32 scancode;
} uev_key;
typedef struct
{
  float x, y;
} uev_mouse_move;
typedef struct
{
  u32   button;
  float x, y;
} uev_mouse_btn;

typedef union uwin_msg_payload
{
  uev_key        key;
  uev_mouse_move mouse_move;
  uev_mouse_btn  mouse_bton;
} uwin_msg_payload;

typedef struct uwin_module
{
  SDL_Window* win;
} uwin_module;

typedef struct uwin_init_info
{
  int             width;
  int             height;
  const char*     title;
  SDL_WindowFlags flags;
} uwin_init_info;

/**
 * Initialize a window.
 * user_data = &uwin_module_desc
 */
int  uwin_init(umod* mod, void* user_data);
void uwin_tick(umod* mod);
void uwin_free(umod* mod);

static inline umod_desc
uwin_module_info(void)
{
  return (umod_desc){
    .name         = "UWindow",
    .send_mask    = U_WIN_MESSAGE_MASK,
    .receive_mask = 0,
    .init         = uwin_init,
    .exit         = uwin_free,
    .tick         = uwin_tick,
  };
}

#endif