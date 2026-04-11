#include "input.h"

#include "../../ESL/operate.h"
#include "../../input.h"
#include "../../singleton.h"

static inline uinput_module*
get_input_mod()
{
  umod* mod = g_umodsys_find("uInput");
  if (mod) return (uinput_module*)mod->module_data;
  return NULL;
}

e_var
uesl_input_mouse_position(e_var* args, u32 nargs)
{
  (void)args;
  (void)nargs;
  vec2 pos;
  glm_vec2_copy(get_input_mod()->mouse_pos, pos);
  return e_make_vec2(pos[0], pos[1]);
}
e_var
uesl_input_mouse_delta(e_var* args, u32 nargs)
{
  (void)args;
  (void)nargs;
  vec2 pos;
  glm_vec2_copy(get_input_mod()->mouse_move, pos);
  return e_make_vec2(pos[0], pos[1]);
}
e_var
uesl_input_mouse_scroll_delta(e_var* args, u32 nargs)
{
  (void)args;
  (void)nargs;
  vec2 pos;
  glm_vec2_copy(get_input_mod()->mouse_scroll, pos);
  return e_make_vec2(pos[0], pos[1]);
}
e_var
uesl_input_key_pressed(e_var* args, u32 nargs)
{
  (void)args;
  (void)nargs;
  return (e_var){ .type = E_VARTYPE_BOOL, .val.b = uinput_key_pressed(get_input_mod(), evar_to_int(args[0])) };
}
e_var
uesl_input_key_released(e_var* args, u32 nargs)
{
  (void)nargs;
  return (e_var){ .type = E_VARTYPE_BOOL, .val.b = uinput_key_released(get_input_mod(), evar_to_int(args[0])) };
}
e_var
uesl_input_key_held(e_var* args, u32 nargs)
{
  (void)nargs;
  return (e_var){ .type = E_VARTYPE_BOOL, .val.b = uinput_key_held(get_input_mod(), evar_to_int(args[0])) };
}