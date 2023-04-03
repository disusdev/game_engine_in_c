#include <entry.h>

#include <core/logger.h>
#include <core/asserts.h>
#include <core/input.h>
#include <core/event.h>
#include <containers/cvec.h>

void
simple_cvec_test()
{
  cvec(u32) indices = cvec_create(u32);
  
  for (u32 i = 0; i < 10000; i += 1)
  {
    cvec_push(indices, i);
  }
  
  for (u32 i = 0; i < cvec_size(indices); i += 1)
  {
    LOG_DEBUG("Is $u == $u ? $s",
              i,
              indices[i],
              (i == indices[i]) ? "Yes" : "No");
    ASSERT(i == indices[i]);
  }
  
  cvec_destroy(indices);
}

typedef struct
{
  const char* name;
  f32 pos_x;
  f32 pos_y;
  u32 health;
  u32 charges;
} Enemy;

void
log_enemy_stats(Enemy* enemy)
{
  LOG_DEBUG("o $s", enemy->name);
  LOG_DEBUG("|- o pos");
  LOG_DEBUG("   |- x: $f", to_u64(enemy->pos_x));
  LOG_DEBUG("   |- y: $f", to_u64(enemy->pos_y));
  LOG_DEBUG("|- health: $u", enemy->health);
  LOG_DEBUG("|- charge: $u\n", enemy->charges);
}

void
complex_cvec_test()
{
  cvec(Enemy) enemies = cvec_create(Enemy);
  
  Enemy enemy =
  {
    "Zorg",
    0, 0,
    100, 2
  };
  cvec_push(enemies, enemy);
  
  enemy.name = "Sigon";
  enemy.pos_x = 10;
  enemy.pos_y = 10;
  enemy.health = 60;
  enemy.charges = 1;
  
  cvec_push(enemies, enemy);
  
  enemy.name = "Mrock";
  enemy.pos_x = 5;
  enemy.pos_y = 5;
  enemy.health = 200;
  enemy.charges = 5;
  
  cvec_push(enemies, enemy);
  
  enemy.name = "Woron";
  enemy.pos_x = 15;
  enemy.pos_y = 16;
  enemy.health = 20;
  enemy.charges = 0;
  
  cvec_push(enemies, enemy);
  
  for (u32 i = 0; i < cvec_size(enemies); i += 1)
  {
    log_enemy_stats(&enemies[i]);
  }
  
  cvec_destroy(enemies);
}

void
game_init()
{
  LOG_DEBUG("Simple:");
  simple_cvec_test();
  LOG_DEBUG(" ");
  LOG_DEBUG("Complex:");
  complex_cvec_test();
}

void
game_update(double dt)
{
  if (input_get_key_down(KEY_CODE_ESC))
  {
    event_dispatch(APP_QUIT);
  }
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

