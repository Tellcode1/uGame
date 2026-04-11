#ifndef UGAME_ESL_INTERFACE_H
#define UGAME_ESL_INTERFACE_H

#include "../../ESL/bstructs.h"
#include "../../ESL/bvar.h"

typedef struct uesl_runtime_builder {
  e_builtin_struct* structs;
  u32               structs_count;
  u32               structs_capacity;

  e_builtin_var* vars;
  u32            vars_count;
  u32            vars_capacity;
} uesl_runtime_builder;

int  uesl_runtime_builder_init(uesl_runtime_builder* b);
void uesl_runtime_builder_free(uesl_runtime_builder* b);

/**
 * Copy (shallow!) entries into the builder.
 */
int uesl_runtime_builder_register_variable(uesl_runtime_builder* b, const e_builtin_var* v);
int uesl_runtime_builder_register_structure(uesl_runtime_builder* b, const e_builtin_struct* s);

#endif