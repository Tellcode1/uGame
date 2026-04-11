#include "../ESL/rwhelp.h"
#include "../ESL/script.h"
#include "../ESL/struct.h"
#include "../ESL/var.h"
#include "../camera.h"
#include "../cmdbuf.h"
#include "../fralloc.h"
#include "../glmodule.h"
#include "../input.h"
#include "../time.h"
#include "../winmodule.h"

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_scancode.h>
#include <SDL3/SDL_video.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

static bool run = true;

static inline void
process_events(const umod_msg* msg, void* user_data)
{
  ucamera* cam                  = (ucamera*)user_data;

  const uwin_msg_payload* pload = (uwin_msg_payload*)msg->data;
  switch ((uwin_msg_type)msg->msg_id) {
    case U_WIN_MSG_EXIT: run = false; break;
    case U_WIN_MSG_KEY_DOWN:
      printf("Key %u down\n", pload->key.scancode);
      if (pload->key.scancode == SDL_SCANCODE_ESCAPE) run = false;

      vec2 move = { 0 };
      if (pload->key.scancode == SDL_SCANCODE_S) { move[1] += -0.5F; }
      if (pload->key.scancode == SDL_SCANCODE_W) { move[1] += 0.5F; }
      if (pload->key.scancode == SDL_SCANCODE_A) { move[0] += -0.5F; }
      if (pload->key.scancode == SDL_SCANCODE_D) { move[0] += 0.5F; }
      ucamera_move(cam, move);
      break;
    case U_WIN_MSG_KEY_UP: printf("Key %u up\n", pload->key.scancode); break;
    // case U_WIN_MSG_MOUSE_MOVE: printf("Mouse moved: %f %f\n", pload->mouse_move.x, pload->mouse_move.y); break;
    case U_WIN_MSG_MOUSE_CLICK: printf("Mouse button %u clicked %f %f\n", pload->mouse_bton.button, pload->mouse_bton.x, pload->mouse_bton.y); break;
    case U_WIN_MSG_MOUSE_DOUBLE_CLICK: printf("Mouse button %u double clicked %f %f\n", pload->mouse_bton.button, pload->mouse_bton.x, pload->mouse_bton.y); break;
    case U_WIN_MSG_WIN_RESIZE: printf("Window resized %ix%i\n", pload->win_resized.width, pload->win_resized.height); break;
    case U_WIN_MSG_MOUSE_SCROLL:
      printf("Scroll %f %f\n", pload->scroll.x, pload->scroll.y);
      ucamera_set_zoom(cam, cam->zoom + pload->scroll.y * 0.2F);
      break;
    default: break;
  }
}

static inline int
load_script(const char* path, e_script* s)
{
  FILE* f = fopen(path, "rb");
  if (!f) {
    puts(path);
    perror("Failed to open file");
    return -1;
  }

  void* root_allocation  = nullptr;

  e_compilation_result r = { 0 };
  int                  e = e_file_load(f, &root_allocation, &r.ninstructions, &r.instructions, &r.nliterals, &r.literals, &r.nfunctions, &r.functions);
  if (e) return e;

  e = e_script_init(&r, nullptr, 0, s);
  if (e) {
    fclose(f);
    return e;
  }

  fclose(f);

  return 0;
}

static inline e_var*
get_struct_member(const e_var* v, const char* member)
{ return e_struct_get_member(e_hash_fnv(member, strlen(member)), E_VAR_AS_STRUCT(v)); }

int
main(void)
{
  if (e_refdobj_pool_init(8, &ge_pool)) return -1;

  e_script test = { 0 };
  if (load_script("build/test.b", &test)) return -1;

  e_var obj = e_script_call(&test, "init", nullptr, 0);
  eb_println(&obj, 1);

  umodsys sys;
  if (umodsys_init(&sys)) return -1;

  uwin_init_info winit_info = {
    .width  = 800,
    .height = 600,
    .title  = "Poot dispenser here!!!",
    .flags  = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE,
  };

  umod_desc desc = uwin_module_info();

  int uwin_id    = umod_init(&desc, &winit_info, &sys);
  if (uwin_id < 0) return -1;

  umod_desc input = {
    .name         = "uInput",
    .send_mask    = U_INPUT_MESSAGE_MASK,
    .receive_mask = U_WIN_MESSAGE_MASK,
    .init         = uinput_init,
    .free         = uinput_free,
    .tick         = uinput_tick,
  };
  int uinput_id = umod_init(&input, nullptr, &sys);
  if (uinput_id < 0) return -1;

  ugl_init_info glinit_info = {
    .major_version    = 3,
    .minor_version    = 3,
    .profile          = SDL_GL_CONTEXT_PROFILE_CORE,
    .double_buffering = 1,
  };

  desc       = ugl_module_desc();

  int ugl_id = -1;
  if ((ugl_id = umod_init(&desc, &glinit_info, &sys)) < 0) return -1;

  printf("GL vendor:   %s\n", glGetString(GL_VENDOR));
  printf("GL renderer: %s\n", glGetString(GL_RENDERER));
  printf("GL version:  %s\n", glGetString(GL_VERSION));

  ucamera cam;
  if (ucamera_init(&cam)) return -1;

  ugl_set_active_camera((ugl_module*)umodsys_get_module(&sys, ugl_id)->module_data, &cam);

  float angle = 0.0F;

  u64    prev = utime_hwclock_nsecs();
  double dt   = 0.0;
  while (run) {
    u64 now         = utime_hwclock_nsecs();
    dt              = UTIME_NSECS_TO_SECS(now - prev);
    prev            = now;

    ugl_module* ugl = (ugl_module*)umodsys_get_module(&sys, ugl_id)->module_data;

    umodsys_tick(&sys);
    umodsys_recv(&sys, U_WIN_MESSAGE_MASK, process_events, &cam);

    ucamera_rebuild(&cam);

    e_var n = e_script_call(&test, "update", (e_var[]){ obj, e_var_from_float(dt) }, 2);
    if (n.type != E_VARTYPE_NULL) obj = n;

    const float PI  = 3.1415926535F;

    e_vec3 position = { 0 };
    e_var* access   = get_struct_member(&obj, "position");
    if (access) position = access->val.vec3;

    ugl_command cmd = {
      .type       = UGL_COMMAND_DRAW_RECT,
      .value.rect = { .rect = { .x = (float)position.x, .y = (float)position.y, .w = 0.5F, .h = 0.5F }, .color = { .r = 0.F, .g = 1.0F, 1.0F, 1.0F }, .layer = 0, },
    };
    ugl_cmd_push(ugl, &cmd);

    angle += 0.008F;
    if (angle >= 2.0f * PI) { angle -= 2.0f * PI; }

    float x               = cosf(angle) * 3.F;
    float y               = sinf(angle) * 3.F;

    cmd.value.rect.color  = (ucolor){ 1, 1, 1, 1 };
    cmd.value.rect.rect.w = 1;
    cmd.value.rect.rect.h = 1;
    cmd.value.rect.rect.x = x;
    cmd.value.rect.rect.y = y;
    ugl_cmd_push(ugl, &cmd);

    ugl_draw(ugl);

    ugl_cmd_flush(ugl);
    ufr_clear();
    umodsys_clear_queue(&sys);
  }

  ucamera_free(&cam);
  umod_free(&sys, "uWindow");
  umod_free(&sys, "uGL");
  return 0;
}
