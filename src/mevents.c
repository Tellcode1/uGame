#include "../module.h"

void
umod_send(umodsys* sys, const umod_event* ev)
{
  int next_tail = (sys->tail + 1) % u_MODULE_EVENT_RING_BUFFER_SIZE;
  if (next_tail == sys->head)
  {
    // drop events if event ring buffer overflow
#ifdef DEBUG
    umod_log(sys, "event queue overflow, dropping event %u\n", ev->event_id);
#endif
    return;
  }

  sys->events[sys->tail] = *ev;

  sys->tail = next_tail;
}

int
umod_recv(umodsys* sys, const char* module_name, void (*handler)(umod* mod, const umod_event* ev))
{
  umod* mod       = umodsys_find_module_by_name(sys, module_name);
  int   processed = 0;
  int   i         = sys->head;

  while (i != sys->tail)
  {
    umod_event* e = &sys->events[i];
    if (e->sender_mask & mod->desc.receive_mask)
    {
      if (handler) handler(mod, e);
      processed++;
    }
    i = (i + 1) % u_MODULE_EVENT_RING_BUFFER_SIZE;
  }

  return processed;
}