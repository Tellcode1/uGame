#include "../module.h"

void
umod_send(umodsys* sys, const umod_msg* ev)
{
  int next_tail = (sys->tail + 1) % u_MODULE_EVENT_RING_BUFFER_SIZE;
  if (next_tail == sys->head)
  {
    // drop msgs if msg ring buffer overflow
#ifdef DEBUG
    umod_log(sys, "msg queue overflow, dropping msg %u\n", ev->msg_id);
#endif
    return;
  }

  sys->msgs[sys->tail] = *ev;

  sys->tail = next_tail;
}

int
umod_recv(umodsys* sys, const char* module_name, void (*handler)(umod* mod, const umod_msg* ev))
{
  umod* mod       = umodsys_find_module_by_name(sys, module_name);
  int   processed = 0;
  int   i         = sys->head;

  while (i != sys->tail)
  {
    umod_msg* e = &sys->msgs[i];
    if (e->sender_mask & mod->desc.receive_mask)
    {
      if (handler) handler(mod, e);
      processed++;
    }
    i = (i + 1) % u_MODULE_EVENT_RING_BUFFER_SIZE;
  }

  return processed;
}

int
umodsys_recv(umodsys* sys, u32 receive_mask, void (*handler)(const umod_msg* ev, void* user_data), void* handler_user_data)
{
  int processed = 0;
  int i         = sys->head;

  while (i != sys->tail)
  {
    umod_msg* e = &sys->msgs[i];
    if (e->sender_mask & receive_mask)
    {
      if (handler) handler(e, handler_user_data);
      processed++;
    }
    i = (i + 1) % u_MODULE_EVENT_RING_BUFFER_SIZE;
  }

  return processed;
}