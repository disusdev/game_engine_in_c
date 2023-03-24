#include <entry.h>

#include <core/logger.h>

void
game_init()
{
  LOG_EMPTY("uint: $u, int: $i, float: $f, str: $s", 12, 12, to_u64(12.12f), "hello");
  LOG_DEBUG("uint: $u, int: $i, float: $f, str: $s", 256, -5, to_u64(256.34f), "hello");
  LOG_WARN("uint: $u, int: $i, float: $f, str: $s", 256, -5, to_u64(256.34f), "hello");
  LOG_ERROR("uint: $u, int: $i, float: $f, str: $s", 256, -5, to_u64(256.34f), "hello");
}

void
game_update(double dt)
{
}

void
game_term()
{
}

void
app_create(t_app* out_app)
{
  out_app->name = "Game";
  out_app->init = &game_init;
  out_app->update = &game_update;
  out_app->term = &game_term;
}

