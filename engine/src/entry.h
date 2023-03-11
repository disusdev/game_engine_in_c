#ifndef __ENTRY_H__
#define __ENTRY_H__

#include "core/engine.h"

extern void
app_create(t_app* out_app);

int
main()
{
  t_app app;
  app_create(&app);
  
  engine_create(&app);
  return engine_run();
}

#endif
