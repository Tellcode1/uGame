#ifndef U_TRANSFORM_H
#define U_TRANSFORM_H

#include "glmpch.h"
#include "module.h"
#include "types.h"

#include <cglm/affine.h>
#include <cglm/mat4.h>

typedef struct utransform
{
  /* Cached transformation matrix */
  mat4 transform;

  CGLM_ALIGN(16) versor rotation;
  CGLM_ALIGN(16) vec3 position;
  CGLM_ALIGN(16) vec3 scale;
} utransform;

#endif // U_TRANSFORM_H