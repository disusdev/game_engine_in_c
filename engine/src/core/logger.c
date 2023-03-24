#include "logger.h"

#include <platform/platform.h>

static u32
digit_count(u64 n)
{
  if (n == 0) return 1;
  u32 count = 0;
  while(n != 0)
  {
    n = n / 10;
    count++;
  }
  return count;
}

static i32
to_norm(f64 *val)
{
  i32 exponent = 0;
  f64 value = *val;
  
  while (value >= 1.0)
  {
    value /= 10.0;
    ++exponent;
  }
  
  while (value < 0.1)
  {
    value *= 10.0;
    --exponent;
  }
  *val = value;
  return exponent;
}

static u32
u2s(u8* buffer,
    u32 value)
{
  u8* p_buffer = buffer;
  i32 places = 0;
  if (value == 0)
  {
    p_buffer[0] = '0';
    p_buffer[1] = '\0';
    return 1;
  }
  
  f64 f_val = (f64)value;
  i32 exponent = to_norm(&f_val);
  while (exponent > 0)
  {
    i32 digit = (f_val + 1E-9) * 10;
    *p_buffer++ = digit + '0';
    f_val = f_val * 10 - digit;
    ++places;
    --exponent;
  }
  
  if (places == 0)
  {
    *p_buffer++ = '0';
  }
  
  return (p_buffer - buffer);
}

static u32
i2s(u8* buffer,
    i32 value)
{
  u8* p_buffer = buffer;
  
  i32 places = 0;
  if (value == 0)
  {
    p_buffer[0] = '0';
    p_buffer[1] = '\0';
    return 1;
  }
  
  if (value < 0)
  {
    *p_buffer++ = '-';
    value = -value;
  }
  
  f64 f_val = (f64)value;
  i32 exponent = to_norm(&f_val);
  
  while (exponent > 0)
  {
    i32 digit = (f_val + 1E-9) * 10;
    *p_buffer++ = digit + '0';
    f_val = f_val * 10 - digit;
    ++places;
    --exponent;
  }
  
  if (places == 0)
  {
    *p_buffer++ = '0';
  }
  
  return (p_buffer - buffer);
}

static u32
f2s(u8* buffer,
    f64 value,
    u32 fraction)
{
  u8* p_buffer = buffer;
  i32 exponent = 0;
  i32 places = 0;
  
  if (value == 0.0)
  {
    p_buffer[0] = '0';
    p_buffer[1] = '\0';
    return 1;
  }       
  
  u32 d_count = 0;
  
  if (value < 0.0)
  {
    *p_buffer++ = '-';
    value = -value;
    d_count++;
  }
  
  d_count = digit_count((u64)value);
  
  exponent = to_norm(&value);
  u32 width = d_count + fraction;
  
  while (exponent > 0)
  {
    i32 digit = (value + 1E-9) * 10;
    *p_buffer++ = digit + '0';
    value = value * 10 - digit;
    ++places;
    --exponent;
  }
  
  if (places == 0)
  {
    *p_buffer++ = '0';
  }
  
  *p_buffer++ = '.';
  
  while (exponent < 0 && places < width)
  {
    *p_buffer++ = '0';
    --exponent;
    ++places;
  }
  
  while (places < width)
  {
    i32 digit = (value + 1E-9) * 10;
    *p_buffer++ = digit + '0';
    value = value * 10.0 - digit;
    ++places;
  }
  
  return (p_buffer - buffer);
}

u32
compose_to_buffer(u8* buffer,
                  const char* format,
                  t_log_data arr_data[])
{
  u32 data_index = 0;
  u8* p_buffer = buffer;
  const char* p_view = format;
  while(*p_view != '\0')
  {
    if (*p_view == '$')
    {
      p_view++;
      
      u8 type = *p_view;
      
      if (type == 'i')
      {
        i32 di = arr_data[data_index++].value.i;
        u32 size = i2s(p_buffer, di);
        p_buffer += size;
      }
      else if (type == 'u')
      {
        u32 du = arr_data[data_index++].value.u;
        u32 size = u2s(p_buffer, du);
        p_buffer += size;
      }
      else if (type == 'f')
      {
        f32 df = arr_data[data_index++].value.f;
        u32 size = f2s(p_buffer, df, 2);
        p_buffer += size;
      }
      else if (type == 's')
      {
        const char* ds = arr_data[data_index++].value.c;
        u32 size = str_length(ds);
        mem_copy(p_buffer, ds, size);
        p_buffer += size;
      }
    }
    else
    {
      *p_buffer = *p_view;
      p_buffer++;
    }
    
    p_view++;
  }
  
  *p_buffer = '\0';
  
  return (p_buffer - buffer);
}

u64
to_u64(f64 value)
{
  return *(u64*)&value;
}
