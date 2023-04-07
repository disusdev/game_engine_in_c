#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <defines.h>

#include <containers/cstr.h>

typedef enum
e_log_type
{
  LOG_TYPE_EMPTY,
  LOG_TYPE_DEBUG,
  LOG_TYPE_WARN,
  LOG_TYPE_ERROR
}
e_log_type;

typedef struct
t_log_data
{
  union
  {
    u64 u;
    i64 i;
    f64 f;
    const char* c;
  } value;
}
t_log_data;

void
logger_init();

void
logger_term();

u32
compose_to_buffer(const char* format,
                  t_log_data arr_data[]);

u64
to_u64(f64 value);

u64
composition_length(const char* format,
                   t_log_data arr_data[]);

void
logger_log_buffer(u64 length,
                  e_log_type type);

#define LOG_DEBUG(format, ...) do {\
t_log_data darr[] = { __VA_ARGS__ };\
u32 l = compose_to_buffer("[DEBUG] " format "\n", darr);\
logger_log_buffer(l, LOG_TYPE_DEBUG); } while (0)
#define LOG_EMPTY(format, ...) do {\
t_log_data darr[] = { __VA_ARGS__ };\
u32 l = compose_to_buffer(format "\n", darr);\
logger_log_buffer(l, LOG_TYPE_EMPTY); } while (0)
#define LOG_WARN(format, ...) do {\
t_log_data darr[] = { __VA_ARGS__ };\
u32 l = compose_to_buffer("[WARN] " format "\n", darr);\
logger_log_buffer(l, LOG_TYPE_WARN); } while (0)
#define LOG_ERROR(format, ...) do {\
t_log_data darr[] = { __VA_ARGS__ };\
u32 l = compose_to_buffer("[ERROR] " format "\n", darr);\
logger_log_buffer(l, LOG_TYPE_ERROR); } while (0)

#endif