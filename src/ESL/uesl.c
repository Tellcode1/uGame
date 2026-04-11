#include "interface.h"

#include <stdlib.h>

int
uesl_runtime_builder_init(uesl_runtime_builder* b)
{
  b->structs_capacity = 8;
  b->vars_capacity    = 8;

  b->vars_count    = 0;
  b->structs_count = 0;

  b->structs = malloc(sizeof(*b->structs) * b->structs_capacity);
  b->vars    = malloc(sizeof(*b->vars) * b->vars_capacity);

  // Non zero if either are null => error
  return b->structs == NULL || b->vars == NULL;
}

void
uesl_runtime_builder_free(uesl_runtime_builder* b)
{
  if (!b) return;
  free(b->structs);
  free(b->vars);
}

int
uesl_runtime_builder_register_variable(uesl_runtime_builder* b, const e_builtin_var* v)
{
  if (b->vars_count >= b->vars_capacity)
  {
    u32            new_capacity = b->vars_capacity * 2;
    e_builtin_var* new_vars     = realloc(b->vars, sizeof(*b->vars) * new_capacity);
    if (!new_vars) return -1;

    b->vars          = new_vars;
    b->vars_capacity = new_capacity;
  }

  memcpy(&b->vars[b->vars_count], v, sizeof(*v));
  b->vars_count++;

  return 0;
}

int
uesl_runtime_builder_register_structure(uesl_runtime_builder* b, const e_builtin_struct* s)
{
  if (b->structs_count >= b->structs_capacity)
  {
    u32               new_capacity = b->structs_capacity * 2;
    e_builtin_struct* new_structs  = realloc(b->structs, sizeof(*b->structs) * new_capacity);
    if (!new_structs) return -1;

    b->structs          = new_structs;
    b->structs_capacity = new_capacity;
  }

  memcpy(&b->structs[b->structs_count], s, sizeof(*s));
  b->structs_count++;

  return 0;
}