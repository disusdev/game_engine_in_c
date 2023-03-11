#include "engine.h"

#include "platform/platform.h"

#include <stdio.h>

const f64 FIXED_TIME = 1.0 / 60.0;

static t_engine engine;

void
engine_create(t_app* app)
{
  engine.app = app;
  
  platform_init(app->name, 100, 100, 800, 600);
  
  engine.app->init();
}

b8
engine_run()
{
  engine.is_running = true;
  
  f64 timer = 0.0;
  f64 current = platform_get_absolute_time();
  f64 accumulator = 0.0;
  f64 fresh = 0.0;
  f64 delta = 0.0;
  
  while(engine.is_running)
  {
    platform_pump_msg();
    
    if (!engine.is_running) break;
    
    fresh = platform_get_absolute_time();
    delta = fresh - current;
    current = fresh;
    accumulator += delta;
    
    engine.app->update(delta);
    
    while (accumulator >= FIXED_TIME)
    {
      // fixed update
      
      accumulator -= FIXED_TIME;
      timer += FIXED_TIME;
    }
    
    // collect render group
    
    // render
  }
  
  engine.app->term();
  
  platform_term();
  
  return 0;
}