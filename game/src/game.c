#include <entry.h>

#include <stdio.h>

void
game_init()
{
  printf("Game Init!\n");
}

void
game_update(double dt)
{
  // printf("Game Update! dt:%f \n", dt);
}

void
game_term()
{
  printf("Game Term!\n");
}

void
app_create(t_app* out_app)
{
  out_app->name = "Game";
  out_app->init = &game_init;
  out_app->update = &game_update;
  out_app->term = &game_term;
}

