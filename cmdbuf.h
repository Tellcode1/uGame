#ifndef U_COMMAND_BUFFER_H
#define U_COMMAND_BUFFER_H

#include "types.h"

typedef enum ugl_command_type
{
  UGL_COMMAND_DRAW_RECT,
  UGL_COMMAND_DRAW_SPRITE, // Textured rect
} ugl_command_type;

typedef struct urect2
{
  float x;
  float y;
  float w;
  float h;
} urect2;

typedef struct ucolor
{
  float r;
  float g;
  float b;
  float a;
} ucolor;

typedef union ugl_command_value
{
  struct
  {
    urect2 rect;
    ucolor color;
    u32    texture;
    u32    layer;
  } rect; // for sprites and rects
} ugl_command_value;

typedef struct ugl_command
{
  ugl_command_type  type;
  ugl_command_value value;
} ugl_command;

#endif // U_COMMAND_BUFFER_H
