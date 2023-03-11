#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <defines.h>

#include "app.h"

typedef struct
t_engine
{
  t_app* app;
  b8 is_running;
}
t_engine;

void
engine_create(t_app* app);

b8
engine_run();

#endif