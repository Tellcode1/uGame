#include "../../camera.h"
#include "../../cmdbuf.h"
#include "../../fralloc.h"
#include "../../glad.h"
#include "../../glmodule.h"
#include "../../time.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <cglm/affine-pre.h>
#include <cglm/affine.h>
#include <cglm/mat4.h>
#include <cglm/quat.h>
#include <stdio.h>
#include <stdlib.h>

void
ugl_cmd_push(ugl_module* mod, const ugl_command* cmd)
{
  if (!mod->cmds)
  {
    mod->cmds  = ufr_alloc(sizeof(ugl_command) * UGL_MAX_COMMANDS);
    mod->ncmds = 0;
  }

  mod->cmds[mod->ncmds++] = *cmd;
}

void
ugl_cmd_flush(ugl_module* mod)
{
  /* Clear command buffers. We'll allocate them next frame on the FR */
  mod->ncmds = 0;
  mod->cmds  = NULL;
}

void
ugl_draw(ugl_module* ugl)
{
  glClear(GL_COLOR_BUFFER_BIT);

  glUseProgram(ugl->shader);
  glBindVertexArray(ugl->vao);
  glBindBuffer(GL_ARRAY_BUFFER, ugl->vbo);

  int w, h;
  SDL_GetWindowSize(ugl->window, &w, &h);
  glUniform2f(ugl->u_resolution_loc, (float)w, (float)h);

  mat4 model = GLM_MAT4_IDENTITY_INIT;

  glm_translate(model, (vec3){ 0.F, 0.F, 0.F });
  // glm_rotate(model, (float)utime_hwclock_nsecs() * 1e-9F, (vec3){ 0.F, 0.F, 1.F });
  glm_rotate(model, 0.0F, (vec3){ 0.0F, 0.0F, 0.0F });
  glm_scale(model, (vec3){ 1.F, 1.F, 1.F });

  mat4 mvp;
  glm_mat4_mul(model, ugl->main_cam->vp_matrix, mvp);

  glUniformMatrix4fv(ugl->u_mvp_loc, 1, false, mvp[0]);

  glViewport(0, 0, w, h); //

  for (u32 i = 0; i < ugl->ncmds; i++)
  {
    ugl_command* cmd = &ugl->cmds[i];
    if (cmd->type == UGL_COMMAND_DRAW_RECT)
    {
      glUniform2f(ugl->u_pos_loc, cmd->value.rect.rect.x, cmd->value.rect.rect.y);
      glUniform2f(ugl->u_size_loc, cmd->value.rect.rect.w, cmd->value.rect.rect.h);
      glUniform4f(ugl->u_color_loc, cmd->value.rect.color.r, cmd->value.rect.color.g, cmd->value.rect.color.b, cmd->value.rect.color.a);
      glDrawArrays(GL_TRIANGLES, 0, 6);
    }
  }

  glBindVertexArray(0); // unbind when done
  SDL_GL_SwapWindow(ugl->window);
}

static inline char*
file_read(const char* path)
{
  FILE* f = fopen(path, "r");
  if (!f) return NULL;

  fseek(f, 0, SEEK_END);

  long size = ftell(f);

  fseek(f, 0, SEEK_SET);

  char* s = malloc(size + 1);
  s[size] = 0;

  fread(s, size, 1, f);

  fclose(f);

  return s;
}

int
ugl_init_draw_sys(ugl_module* ugl)
{
  ugl->ncmds = 0;
  ugl->cmds  = NULL;

  // clang-format off
  float quad[] = {
  -0.5F, -0.5F,
   0.5F, -0.5F,
   0.5F,  0.5F,

  -0.5F, -0.5F,
   0.5F,  0.5F,
  -0.5F,  0.5F,
};
  // clang-format on

  glGenVertexArrays(1, &ugl->vao);
  glGenBuffers(1, &ugl->vbo);
  glBindVertexArray(ugl->vao);
  glBindBuffer(GL_ARRAY_BUFFER, ugl->vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
  glEnableVertexAttribArray(0);

  char* vertex_source   = NULL;
  char* fragment_source = NULL;
  vertex_source         = file_read("src/shaders/vert.glsl");
  fragment_source       = file_read("src/shaders/frag.glsl");

  ugl->shader = ugl_load_shader(vertex_source, fragment_source);

  ugl->u_pos_loc        = glGetUniformLocation(ugl->shader, "u_pos");
  ugl->u_size_loc       = glGetUniformLocation(ugl->shader, "u_size");
  ugl->u_color_loc      = glGetUniformLocation(ugl->shader, "u_color");
  ugl->u_resolution_loc = glGetUniformLocation(ugl->shader, "u_resolution");
  ugl->u_mvp_loc        = glGetUniformLocation(ugl->shader, "u_mvp");

  free(vertex_source);
  free(fragment_source);

  return 0;
}

void
ugl_free_draw_sys(ugl_module* ugl)
{
  glDeleteVertexArrays(1, &ugl->vao);
  glDeleteBuffers(1, &ugl->vbo);
  glDeleteShader(ugl->shader);
}
