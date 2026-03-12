#include "../camera.h"

#include <cglm/mat4.h>
#include <cglm/vec2.h>
#include <cglm/vec3.h>
#include <string.h>

int
ucamera_init(ucamera* cam)
{
  cam->ortho_size[0] = 16;
  cam->ortho_size[1] = 9;

  glm_vec2_zero(cam->position);

  cam->zoom     = 1.F;
  cam->rotation = 0.F;

  glm_mat4_zero(cam->vp_matrix);

  return 0;
}

void
ucamera_free(ucamera* cam)
{
  memset(cam, 0, sizeof(ucamera));
}

const mat4*
ucamera_vp_matrix(const ucamera* cam)
{
  return &cam->vp_matrix;
}

void
ucamera_set_zoom(ucamera* cam, float zoom)
{
  cam->zoom = zoom;
}

void
ucamera_set_position(ucamera* cam, vec2 new_position)
{
  glm_vec2_copy(new_position, cam->position);
}

void
ucamera_move(ucamera* cam, vec2 by)
{
  glm_vec2_add(cam->position, by, cam->position);
}

void
ucamera_rebuild(ucamera* cam)
{
  float half_w = (cam->ortho_size[0] / 2.0f) / cam->zoom;
  float half_h = (cam->ortho_size[1] / 2.0f) / cam->zoom;

  mat4 proj, view;

  glm_ortho(-half_w, half_w, -half_h, half_h, -1.0f, 1.0f, proj);

  glm_mat4_identity(view);
  glm_translate(view, (vec3){ -cam->position[0], -cam->position[1], 0.0f });
  glm_rotate(view, cam->rotation, (vec3){ 0.0f, 0.0f, 1.0f });

  glm_mat4_mul(proj, view, cam->vp_matrix);
}
