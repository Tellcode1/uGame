#version 330 core
layout (location = 0) in vec2 pos;
uniform vec2 u_pos;
uniform vec2 u_size;
uniform vec2 u_resolution;

void main()
{
  vec2 world = pos * u_size + u_pos;
  gl_Position = vec4(world, 0.0, 1.0);
}