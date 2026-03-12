#ifndef U_CAMERA_H
#define U_CAMERA_H

#include <cglm/cglm.h>
#include <cglm/types.h>

typedef struct ucamera
{
  vec2 position;
  vec2 ortho_size;

  float zoom;     // 1.0 = normal
  float rotation; // rads

  /* View * Projection. Multiple with your model matrix to get final transformation. */
  mat4 vp_matrix;
} ucamera;

/**
 * Initialize a camera.
 * Make sure to set ugl_set_active_camera()!
 */
int  ucamera_init(ucamera* cam);
void ucamera_free(ucamera* cam);

void ucamera_set_zoom(ucamera* cam, float zoom);
void ucamera_set_position(ucamera* cam, vec2 new_position);
void ucamera_move(ucamera* cam, vec2 by);

void ucamera_rebuild(ucamera* cam);

const mat4* ucamera_vp_matrix(const ucamera* cam);

#endif // U_CAMERA_H