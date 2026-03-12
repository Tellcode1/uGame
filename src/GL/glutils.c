#include "../../glad.h"
#include "../../glmodule.h"

static void
debug_log_shaderinfo(u32 shader)
{
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char log[512];
    glGetShaderInfoLog(shader, 512, NULL, log);
    fprintf(stderr, "shader compile error: %s\n", log);
  }
}

static void
debug_log_programinfo(u32 prog)
{
  int success;
  glGetProgramiv(prog, GL_LINK_STATUS, &success);
  if (!success)
  {
    char log[512];
    glGetProgramInfoLog(prog, 512, NULL, log);
    fprintf(stderr, "shader link error: %s\n", log);
  }
}

u32
ugl_load_shader(const char* vertex_source, const char* fragment_source)
{
  // compile vertex
  u32 vert = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert, 1, (const char**)&vertex_source, NULL);
  glCompileShader(vert);
  debug_log_shaderinfo(vert);

  // compile fragment
  u32 frag = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag, 1, (const char**)&fragment_source, NULL);
  glCompileShader(frag);
  debug_log_shaderinfo(frag);

  // link
  u32 prog = glCreateProgram();
  glAttachShader(prog, vert);
  glAttachShader(prog, frag);
  glLinkProgram(prog);

  glDeleteShader(vert);
  glDeleteShader(frag);

  // #ifndef NDEBUG
  debug_log_programinfo(prog);
  // #endif

  return prog;
}