#ifndef __ENTRY_H__
#define __ENTRY_H__

#include "core/app.h"

extern void
app_create(t_app* out_app);

int
main()
{
  t_app app;
  app_create(&app);
  
  app.init();
  app.update(0.001666);
  app.term();
  return 0;
}

#endif
