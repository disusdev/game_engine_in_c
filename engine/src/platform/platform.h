#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#include <defines.h>

void
platform_init(const char* app_name,
              i32 x,
              i32 y,
              i32 width,
              i32 height);

void
platform_term();

void
platform_pump_msg();

void*
platform_alloc(u64 size,
               b8 aligned);

void
platform_free(void* block,
              b8 aligned);

void*
platform_mem_zero(void* block,
                  u64 size);

void*
platform_mem_copy(void* dst,
                  const void* src,
                  u64 size);

void*
platform_mem_set(void* dst,
                 i32 value,
                 u64 size);

f64
platform_get_absolute_time();

void
platform_console_write(const char* msg,
                       u32 msg_length,
                       u8 color_index);

#endif