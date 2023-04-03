#ifndef __CVEC_H__
#define __CVEC_H__

#include <defines.h>

#include <core/mem.h>

enum
{
  CVEC_SIZE = -1,
  CVEC_CAPACITY = -2,
  CVEC_STRIDE = -3,
  CVEC_FIELD_LENGTH = 3,
  CVEC_DEFAULT_CAPACITY = 1,
  CVEC_RESIZE_FACTOR = 2,
};

#define cvec(type) type*

#define cvec_capacity(vec)\
((vec) ? ((u64*)vec)[CVEC_CAPACITY] : (u64)0)

#define cvec_size(vec)\
((vec) ? ((u64*)vec)[CVEC_SIZE] : (u64)0)

#define cvec_stride(vec)\
((vec) ? ((u64*)vec)[CVEC_STRIDE] : (u64)0)

#define cvec_back(vec)\
((vec) && cvec_size(vec) > 0 ? &vec[cvec_size(vec) - 1] : 0)

#define cvec_empty(vec)\
(cvec_size(vec) == 0)

// resize
void*
_cvec_resize(void* vec);
#define cvec_resize(vec, size) do {\
((u64*)vec)[CVEC_SIZE] = size;\
vec = _cvec_resize(vec); } while(0)

// create
void*
_cvec_create(const u64 length,
             const u64 stride);
#define cvec_create(type)\
(type*)_cvec_create(CVEC_DEFAULT_CAPACITY, sizeof(type))

// create_cpy
void*
_cvec_create_cpy(const u64 length,
                 const u64 stride,
                 const void* src);
#define cvec_create_cpy(type, src)\
(type*)_cvec_create_cpy(CVEC_DEFAULT_CAPACITY, sizeof(type), src)

// ncreate
#define cvec_ncreate(type, size)\
(type*)_cvec_create(size, sizeof(type))

// ncreate_cpy
#define cvec_ncreate_cpy(type, size, src)\
(type*)_cvec_create_cpy(size, sizeof(type), src)

// ncreate_ncpy
void*
_cvec_ncreate_ncpy(const u64 length,
                   const u64 stride,
                   const void* src,
                   const u64 src_length);

// destroy
void
cvec_destroy(void* vec);

// push
void*
_cvec_push(void* vec,
           const void* value_ptr);
#define cvec_push(vec, value) do {\
u64 stride = cvec_stride(vec);\
u64 type_stride = sizeof(TYPEOF(value));\
ASSERT(type_stride == stride);\
TYPEOF(value) temp = value;\
vec = _cvec_push(vec, &temp); } while(0)

void*
_cvec_push_new(void* vec);
#define cvec_push_new(vec, out_ptr) do {\
vec = _cvec_push_new(vec);\
u64 stride = cvec_stride(vec);\
u64 length = cvec_size(vec);\
u8* p_vec = (u8*)vec;\
p_vec += stride * (length - 1);\
out_ptr = p_vec;\
} while(0)

// pop
void
cvec_pop(void* vec,
         void* value_ptr);

// clear
void
cvec_clear(void* vec);

// erase
void
cvec_erase(void* vec,
           u32 index);

// find
u64
cvec_find(void* vec,
          const void* value_ptr);

// merge_vecvec
void*
cvec_merge_vecvec(const void* vec1,
                  const void* vec2);
// merge_arrvec
void*
_cvec_merge_arrvec(const void* arr,
                   const u64 arr_size,
                   const void* vec);
#define cvec_merge_arrvec(arr, vec)\
_cvec_merge_arrvec(arr, ARRAY_SIZE(arr), vec)

// merge_vecarr
void*
_cvec_merge_vecarr(const void* vec,
                   const void* arr,
                   const u64 arr_size);
#define cvec_merge_vecarr(vec, arr)\
_cvec_merge_vecarr(vec, arr, ARRAY_SIZE(arr))

// merge_nvecvec
void*
cvec_merge_nvecvec(const void* vec1, u64 size1,
                   const void* vec2, u64 size2);
// merge_narrvec
void*
cvec_merge_narrvec(const void* arr, u64 arr_size,
                   const void* vec, u64 vec_size);
// merge_nvecarr
void*
cvec_merge_nvecarr(const void* vec, u64 vec_size,
                   const void* arr, u64 arr_size);

#endif