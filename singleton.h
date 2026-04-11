#ifndef U_SINGLETONS_H
#define U_SINGLETONS_H

#include "module.h"

extern umodsys* g_umodsys;

static inline umodsys*
g_umodsys_set(umodsys* sys)
{
  umodsys* old = g_umodsys;
  g_umodsys    = sys;
  return old;
}

static inline umodsys*
g_umodsys_get()
{ return g_umodsys; }

static inline umod*
g_umodsys_find(const char* module)
{ return umodsys_find_module_by_name(g_umodsys, module); }

#endif // U_SINGLETONS_H