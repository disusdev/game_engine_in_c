#include "cvec.h"

#include <core/asserts.h>

void*
_cvec_create(const u64 length,
             const u64 stride)
{
  const u64 header_size = CVEC_FIELD_LENGTH * sizeof(u64);
  const u64 vec_size = length * stride;
  u64* vec = mem_alloc(header_size + vec_size, MEM_TAG_CVEC);
  mem_zero(vec, header_size + vec_size);
  vec += CVEC_FIELD_LENGTH;
  vec[CVEC_CAPACITY] = length;
  vec[CVEC_SIZE] = 0;
  vec[CVEC_STRIDE] = stride;
  return vec;
}

void*
_cvec_create_cpy(const u64 length,
                 const u64 stride,
                 const void* src)
{
  ASSERT(src);
  void* vec = _cvec_create(length, stride);
  mem_copy(vec, src, length * stride);
  return vec;
}

void
cvec_destroy(void* vec)
{
  ASSERT(vec);
  u64* header = (u64*)vec - CVEC_FIELD_LENGTH;
  const u64 header_size = CVEC_FIELD_LENGTH * sizeof(u64);
  const u64 total_size = header_size + ((u64*)vec)[CVEC_CAPACITY] * ((u64*)vec)[CVEC_STRIDE];
  mem_free(header, total_size, MEM_TAG_CVEC);
}

void*
_cvec_push(void* vec,
           const void* value_ptr)
{
  ASSERT(vec);
  const u64 length = cvec_size(vec);
  const u64 stride = cvec_stride(vec);
  if (length >= cvec_capacity(vec))
  {
    vec = _cvec_resize(vec);
  }
  u8* p_vec = (u8*)vec;
  p_vec += length * stride;
  mem_copy(p_vec, value_ptr, stride);
  ((u64*)vec)[CVEC_SIZE] = length + 1;
  return vec;
}

void*
_cvec_push_new(void* vec)
{
  ASSERT(vec);
  const u64 length = cvec_size(vec);
  const u64 stride = cvec_stride(vec);
  if (length >= cvec_capacity(vec))
  {
    vec = _cvec_resize(vec);
  }
  ((u64*)vec)[CVEC_SIZE] = length + 1;
  return vec;
}

void
cvec_pop(void* vec,
         void* value_ptr)
{
  ASSERT(vec);
  ASSERT(cvec_size(vec) != 0);
  const u64 length = cvec_size(vec);
  const u64 stride = cvec_stride(vec);
  u64 addr = (u64)vec;
  addr += ((length - 1) * stride);
  if (value_ptr)
  {
    mem_copy(value_ptr, (void*)addr, stride);
  }
}

void
cvec_clear(void* vec)
{
  ASSERT(vec);
  u64* header = (u64*)vec;
  u64 cap = header[CVEC_CAPACITY];
  u64 stride = header[CVEC_STRIDE];
  header -= (CVEC_FIELD_LENGTH - 2);
  const u64 header_size = (CVEC_FIELD_LENGTH - 2) * sizeof(u64);
  const u64 total_size = header_size + cap * stride;
  mem_zero(header, total_size);
}

void
cvec_erase(void* vec,
           u32 index)
{
  ASSERT(vec);
  const u64 stride = cvec_stride(vec);
  u64 length = cvec_size(vec);
  const u64 elem_index = index;
  
  if (length == 0)
  {
    return;
  }
  
  ((u64*)vec)[CVEC_SIZE] = length - 1;
  
  length = cvec_size(vec);
  if (length == 0)
  {
    return;
  }
  
  for(u64 i = 0; i < length - elem_index; i++)
  {
    mem_copy(vec + (elem_index + i),
             vec + (elem_index + i + 1),
             stride);
  }
}

u64
cvec_find(void* vec,
          const void* value_ptr)
{
  ASSERT(vec);
  u64 stride = cvec_stride(vec);
  u64 index = 0;
  for(u8* ptr = vec; ptr; ptr += stride)
  {
    if (mem_cmp(ptr, value_ptr, stride))
    {
      return index;
    }
    index++;
  }
}

void*
_cvec_ncreate_ncpy(const u64 length,
                   const u64 stride,
                   const void* src,
                   const u64 src_length)
{
  ASSERT(src);
  void* vec = _cvec_create(length, stride);
  mem_copy(vec, src, src_length * stride);
  return vec;
}

void*
cvec_merge_vecvec(const void* vec1,
                  const void* vec2)
{
  return cvec_merge_nvecvec(vec1, cvec_size(vec1),
                            vec2, cvec_size(vec2));
}

void*
_cvec_merge_arrvec(const void* arr,
                   const u64 arr_size,
                   const void* vec)
{
  return cvec_merge_narrvec(arr, arr_size,
                            vec, cvec_size(vec));
}

void*
_cvec_merge_vecarr(const void* vec,
                   const void* arr,
                   const u64 arr_size)
{
  return cvec_merge_nvecarr(vec, cvec_size(vec),
                            arr, arr_size);
}

void*
cvec_merge_nvecvec(const void* vec1, u64 size1,
                   const void* vec2, u64 size2)
{
  ASSERT(vec1);
  ASSERT(vec2);
  
  const u64 vec1_length = size1;
  const u64 vec1_stride = cvec_stride(vec1);
  
  const u64 vec2_length = size2;
  const u64 vec2_stride = cvec_stride(vec2);
  
  ASSERT(vec1_stride == vec2_stride);
  
  void* n_vec = _cvec_ncreate_ncpy(vec1_length + vec2_length,
                                   vec1_stride,
                                   vec1, vec1_length);
  
  u8* insert_ptr = (u8*)n_vec;
  insert_ptr += vec1_length * vec1_stride;
  
  mem_copy((void*)insert_ptr, vec2, (vec2_length * vec1_stride));
  
  return n_vec;
}

void*
cvec_merge_narrvec(const void* arr, u64 arr_size,
                   const void* vec, u64 vec_size)
{
  ASSERT(arr);
  ASSERT(vec);
  
  const u64 stride = cvec_stride(vec);
  const u64 vec_length = vec_size;
  
  void* n_vec = _cvec_ncreate_ncpy(vec_length + arr_size,
                                   stride,
                                   arr, arr_size);
  
  u8* insert_ptr = (u8*)n_vec;
  insert_ptr += arr_size * stride;
  
  mem_copy((void*)insert_ptr, vec, (vec_length * stride));
  
  return n_vec;
}

void*
cvec_merge_nvecarr(const void* vec, u64 vec_size,
                   const void* arr, u64 arr_size)
{
  ASSERT(vec);
  ASSERT(arr);
  
  const u64 stride = cvec_stride(vec);
  const u64 vec_length = vec_size;
  
  void* n_vec = _cvec_ncreate_ncpy(vec_length + arr_size,
                                   stride,
                                   vec, vec_length);
  
  u8* insert_ptr = (u8*)n_vec;
  insert_ptr += vec_length * stride;
  
  mem_copy((void*)insert_ptr, arr, (arr_size * stride));
  
  return n_vec;
}

void*
_cvec_resize(void* vec)
{
  ASSERT(vec);
  const u64 length = cvec_size(vec);
  const u64 stride = cvec_stride(vec);
  void* temp = _cvec_create((CVEC_RESIZE_FACTOR * cvec_capacity(vec)),
                            stride);
  u64* p_temp = (u64*)temp;
  mem_copy(p_temp, vec, length * stride);
  ((u64*)vec)[CVEC_SIZE] = length;
  cvec_destroy(vec);
  return temp;
}
