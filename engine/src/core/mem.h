#ifndef __MEM_H__
#define __MEM_H__

#include <defines.h>

typedef enum
{
  MEM_TAG_NONE,
  MEM_TAG_CVEC,
  MEM_TAG_COUNT
}
e_mem_tag;

void*
mem_alloc(u64 size,
          e_mem_tag tag);

void
mem_free(void* block,
         u64 size,
         e_mem_tag tag);

void*
mem_zero(void* block,
         u64 size);

void*
mem_copy(void* dst,
         const void* src,
         u64 size);

void*
mem_set(void* dst,
        i32 value,
        u64 size);

b8
mem_cmp(const void* p1,
        const void* p2,
        u64 size);

#endif