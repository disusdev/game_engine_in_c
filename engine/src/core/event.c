#include "event.h"

#include <platform/platform.h>

#define MAX_EVENT_LISTENERS 16

static t_event event;
static t_event_func_data event_funcs[MAX_EVENT_LISTENERS];
static u32 event_func_counter;

void
event_init()
{
  platform_mem_set(&event, 0, sizeof(t_event));
  event_func_counter = 0;
}

b8
event_register(event_fn event_func,
               e_event_type type)
{
  if (event_func_counter < ARRAY_SIZE(event_funcs))
  {
    event_funcs[event_func_counter++] = (t_event_func_data){ type, event_func };
    
    return true;
  }
  return false;
}

t_event*
event_get_handle()
{
  return &event;
}

void
event_dispatch_ext(t_event* e, e_event_type type)
{
  e->type = type;
  
  for(u64 i = 0; i < event_func_counter; i++)
  {
    if (event_funcs[i].type == type)
    {
      event_funcs[i].func(e);
    }
  }
}

void
event_dispatch(e_event_type type)
{
  event_dispatch_ext(&event, type);
}