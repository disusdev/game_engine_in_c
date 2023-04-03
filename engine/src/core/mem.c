#include "mem.h"

#include <platform/platform.h>

void*
mem_alloc(u64 size,
          e_mem_tag tag)
{
  return platform_alloc(size, false);
}

void
mem_free(void* block,
         u64 size,
         e_mem_tag tag)
{
  platform_free(block, false);
}

void*
mem_set(void* dst,
        i32 value,
        u64 size)
{
  u8* ptr = (u8*)dst;
  while (size > 0)
  {
    *ptr = value;
    ptr++;
    size--;
  }
  return dst;
}

void*
mem_zero(void* block,
         u64 size)
{
  return mem_set(block, 0, size);
}

void*
mem_copy(void* dst,
         const void* src,
         u64 size)
{
  u8* ptr_src = (u8*)src;
  u8* ptr_dst = (u8*)dst;
  while (size > 0)
  {
    *ptr_dst = *ptr_src;
    ptr_dst++;
    ptr_src++;
    size--;
  }
  return dst;
}

b8
mem_cmp(const void* p1,
        const void* p2,
        u64 size)
{
  u8* ptr_src = (u8*)p1;
  u8* ptr_dst = (u8*)p2;
  while (size > 0)
  {
    if (*ptr_dst != *ptr_src)
    {
      return false;
    }
    ptr_dst++;
    ptr_src++;
    size--;
  }
  return true;
}
