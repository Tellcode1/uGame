#ifndef UESL_MSG_H
#define UESL_MSG_H

#include "../../ESL/bstructs.h"
#include "../../ESL/bvar.h"
#include "../../ESL/var.h"

typedef struct e_builtin_struct uesl_msg;

static const uesl_msg uesl_msg_structure = {
  .name         = "uMessage",
  .fields       = (const char*[]){ "type", "data" },
  .fields_count = 2,
};

static const e_builtin_var uesl_msg_constants[] = {
  { .name = "umsg::MESSAGE_MOUSE_MOVE", .type = E_VARTYPE_INT, .value.i = 0 },
  { .name = "umsg::MESSAGE_KEY_PRESS", .type = E_VARTYPE_INT, .value.i = 1 },
  { .name = "umsg::MESSAGE_KEY_HELD", .type = E_VARTYPE_INT, .value.i = 2 },
  { .name = "umsg::MESSAGE_KEY_RELEASE", .type = E_VARTYPE_INT, .value.i = 3 },
};

// umsg::get_mouse_move(event) -> vec2
e_var umsg_get_mouse_move(e_var* args, u32 nargs);

// umsg::get_key_pressed(event, keycode) -> bool
e_var umsg_get_key_pressed(e_var* args, u32 nargs);

// umsg::get_key_held(event, keycode) -> bool
e_var umsg_get_key_held(e_var* args, u32 nargs);

// umsg::get_key_released(event, keycode)
e_var umsg_get_key_released(e_var* args, u32 nargs);

#endif // UESL_MSG_H