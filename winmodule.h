#ifndef U_WINDOW_MODULE_H
#define U_WINDOW_MODULE_H

/**
 * In uGame, the window is responsible for both the windowing and (low level) input msgs.
 * winmodule->module_data = SDL_Window *
 */

#include "module.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_video.h>

/*
 * The sender mask of the messages.
 * Your receiver mask must contain this bit to receive messages from the inputs/window module.
 */
#define U_WIN_MESSAGE_MASK (0x2)

typedef enum uwin_msg_type
{
  U_WIN_MSG_EXIT,               // Called to signal user to free window module and free (User is trying to free window)
  U_WIN_MSG_KEY_DOWN,           // Key pressed.
  U_WIN_MSG_KEY_HELD,           // Key repeat
  U_WIN_MSG_KEY_UP,             // Key unpressed.
  U_WIN_MSG_MOUSE_MOVE,         // Mouse moved.
  U_WIN_MSG_MOUSE_CLICK,        // Mouse clicked.
  U_WIN_MSG_MOUSE_DOUBLE_CLICK, // Mouse double clicked. Both the CLICK and DOUBLE_CLICK events are posted.
  U_WIN_MSG_MOUSE_SCROLL,       // Scroll wheel used.
  U_WIN_MSG_WIN_RESIZE,         // Window resized
} uwin_msg_type;

typedef struct uwin_msg_key
{
  SDL_Scancode scancode;
} uwin_msg_key;
typedef struct uwin_msg_mouse_move
{
  // Window space
  // <0...width, 0...height>
  float x, y;
} uwin_msg_mouse_move;
typedef struct uwin_msg_mouse_btn
{
  // Window space
  // <0...width, 0...height>
  float x, y;
  u32   button;
} uwin_msg_mouse_btn;

typedef struct uwin_msg_wind_resized
{
  u32 width;
  u32 height;
} uwin_msg_wind_resized;

typedef struct uwin_msg_scroll
{
  float x; // + right, - left
  float y; // + up, - down
} uwin_msg_scroll;

typedef union uwin_msg_payload
{
  uwin_msg_key          key;
  uwin_msg_mouse_move   mouse_move;
  uwin_msg_mouse_btn    mouse_bton;
  uwin_msg_wind_resized win_resized;
  uwin_msg_scroll       scroll;
} uwin_msg_payload;

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
    .name         = "uWindow",
    .send_mask    = U_WIN_MESSAGE_MASK,
    .receive_mask = 0,
    .init         = uwin_init,
    .free         = uwin_free,
    .tick         = uwin_tick,
  };
}

#endif