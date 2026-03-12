#include "../getns.h"
#include "../module.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

int
umodsys_init(umodsys* sys)
{
  if (!sys) return -1;

  memset(sys, 0, sizeof(*sys));

  int mod_cap = 16;

  sys->cmodules = mod_cap;
  sys->nmodules = 0;
  sys->modules  = malloc(sizeof(umod) * mod_cap);
  sys->msgs     = malloc(sizeof(umod_msg) * u_MODULE_EVENT_RING_BUFFER_SIZE);
  sys->logfile  = stderr;

  return 0;
}

void
umodsys_free(umodsys* sys)
{
  free(sys->msgs);
  free(sys->modules);
}

umod*
umodsys_find_module_by_name(const umodsys* sys, const char* name)
{
  for (int i = 0; i < sys->nmodules; i++)
  {
    if (strcmp(name, sys->modules[i].desc.name) == 0) { return &sys->modules[i]; }
  }
  return NULL;
}

umod*
umodsys_find_module(const umodsys* sys, int id)
{
  if (id >= sys->nmodules) return NULL;
  return &sys->modules[id];
}

int
umod_init(const umod_desc* desc, void* init_function_arg, umodsys* sys)
{
  if (!desc || !sys) return -1;

  if (sys->nmodules >= sys->cmodules)
  {
    int   newcap     = sys->cmodules == 0 ? 1 : (sys->cmodules * 2);
    umod* newmodules = malloc(sizeof(umod) * newcap);
    if (sys->modules)
    {
      memcpy(newmodules, sys->modules, sizeof(umod) * sys->nmodules);
      free(sys->modules);
    }

    sys->cmodules = newcap;
    sys->modules  = newmodules;
  }

  int id = sys->nmodules++;

  umod* modp = &sys->modules[id];
  memset(modp, 0, sizeof(umod));

  // Copy description, name is char buffer so this is safe.
  modp->desc = *desc;

  modp->sys           = sys;
  modp->ticks_per_sec = u_MODULE_DEFAULT_TICKS_PER_SECOND;
  modp->dt_ns_accum   = 0;
  modp->module_data   = NULL;
  modp->name_hash     = u_name_hash(desc->name);

  /**
   * If we don't set this, the tick function tries to call tick for
   * each tick from the start of time (epoch but ok).
   */
  modp->last_tick_ns = getns();

  int e = 0;
  if (desc->init) e = desc->init(modp, init_function_arg);

  return e ? -1 : id;
}

void
umod_free(umodsys* sys, const char* name)
{
  umod* modp = umodsys_find_module_by_name(sys, name);
  if (modp && modp->desc.free) modp->desc.free(modp);
}

size_t
umod_log(umodsys* sys, const char* fmt, ...)
{
  va_list ap;
  va_start(ap, fmt);

  size_t w = vfprintf(sys->logfile, fmt, ap);

  va_end(ap);

  return w;
}

int
umodsys_tick_all(umodsys* sys)
{
  u64 now = getns();

  int nticked = 0;
  for (int i = 0; i < sys->nmodules; i++)
  {
    umod* mod = &sys->modules[i];
    if (mod->desc.tick)
    {
      mod->desc.tick(mod);
      mod->last_tick_ns = now;
      nticked++;
    }
  }
  return nticked;
}

static inline int
mod_tick(u64 now, umod* mod)
{
  int nticked = 0;

  u64 interval_ns = 1000000000ULL / mod->ticks_per_sec;

  mod->dt_ns_accum += now - mod->last_tick_ns;
  mod->last_tick_ns = now;

  while (mod->dt_ns_accum >= interval_ns)
  {
    if (mod->desc.tick)
    {
      mod->desc.tick(mod);
      nticked++;
    }
    mod->dt_ns_accum -= interval_ns;
  }

  return nticked;
}

int
umodsys_tick(umodsys* sys)
{
  int nticked = 0;
  u64 now     = getns();

  for (int i = 0; i < sys->nmodules; i++)
  {
    umod* mod = &sys->modules[i];
    nticked += mod_tick(now, mod);
  }

  return nticked;
}

int
umodsys_clear_queue(umodsys* sys)
{
  int ndropped = 0;
  for (int i = sys->head; i != sys->tail; i = (i + 1) % u_MODULE_EVENT_RING_BUFFER_SIZE) { ndropped++; }

  sys->head = sys->tail;

  return ndropped;
}

int
umod_set_tick_rate(umodsys* sys, const char* module_name, int ticks_per_sec)
{
  umod* mod = umodsys_find_module_by_name(sys, module_name);

  int nticked = mod_tick(getns(), mod);

  if (mod) mod->ticks_per_sec = ticks_per_sec;

  return nticked;
}