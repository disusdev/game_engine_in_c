#ifndef __EVENT_H__
#define __EVENT_H__

#include <defines.h>

typedef enum
{
#define EVENT_TYPE(name) name,
#include <event_types.h>
#undef EVENT_TYPE
  EVENT_TYPE_COUNT
}
e_event_type;

typedef struct
t_event
{
  e_event_type type;
  
  union
  {
    u64 u64[2];
    i64 i64[2];
    f64 f64[2];
    u32 u32[4];
    i32 i32[4];
    f32 f32[4];
    u16 u16[8];
    i16 i16[8];
    u8 u8[16];
    i8 i8[16];
    char c[16];
  } ctx;
}
t_event;

typedef void (*event_fn)(t_event* event);

typedef struct
t_event_func_data
{
  e_event_type type;
  event_fn func;
}
t_event_func_data;

void event_init();
b8
event_register(event_fn, e_event_type type);
t_event*
event_get_handle();
void
event_dispatch(e_event_type type);
void
event_dispatch_ext(t_event* e, e_event_type type);

#endif