#ifndef __ASSERTS_H__
#define __ASSERTS_H__

#include <defines.h>

#include "logger.h"

#if __has_builtin(__builtin_debugtrap)
#define DEBUG_TRAP __builtin_debugtrap();
#else
#define DEBUG_TRAP
#endif

#ifdef ASSERTS_ENABLE
#define ASSERT(cond) do {\
if (!cond)\
{\
LOG_ERROR("[ Assert in " __FILE__ ":$u ]\n", __LINE__); DEBUG_TRAP\
}\
} while(0)
#else
#define ASSERT(cond)
#endif

#endif