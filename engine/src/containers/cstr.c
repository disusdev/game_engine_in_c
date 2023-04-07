#include "cstr.h"

u64
cstr_length(const char* str)
{
  char* p = (char*)str;
  while (*p != '\0') p++;
  return (p - str);
}