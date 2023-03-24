#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <defines.h>

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

u32
compose_to_buffer(u8* buffer,
                  const char* format,
                  t_log_data arr_data[]);

u64
to_u64(f64 value);

#define LOG_DEBUG(format, ...) do {\
u8 log_buffer[1024];\
t_log_data darr[] = { __VA_ARGS__ };\
u32 l = compose_to_buffer(log_buffer, "[DEBUG] " format "\n", darr);\
platform_console_write(log_buffer, l, LOG_TYPE_DEBUG); } while (0)
#define LOG_EMPTY(format, ...) do {\
u8 log_buffer[1024];\
t_log_data darr[] = { __VA_ARGS__ };\
u32 l = compose_to_buffer(log_buffer, format "\n", darr);\
platform_console_write(log_buffer, l, LOG_TYPE_EMPTY); } while (0)
#define LOG_WARN(format, ...) do {\
u8 log_buffer[1024];\
t_log_data darr[] = { __VA_ARGS__ };\
u32 l = compose_to_buffer(log_buffer, "[WARN]  " format "\n", darr);\
platform_console_write(log_buffer, l, LOG_TYPE_WARN); } while (0)
#define LOG_ERROR(format, ...) do {\
u8 log_buffer[1024];\
t_log_data darr[] = { __VA_ARGS__ };\
u32 l = compose_to_buffer(log_buffer, "[ERROR] " format "\n", darr);\
platform_console_write(log_buffer, l, LOG_TYPE_ERROR); } while (0)

#endif