#ifndef __APP_H__
#define __APP_H__

typedef struct
t_app
{
  char* name;
  void (*init)();
  void (*update)(double);
  void (*term)();
}
t_app;

#endif
