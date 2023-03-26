#include <entry.h>

#include <core/logger.h>

#include <core/input.h>

typedef enum
{
  INPUT_DEFAULT,
  INPUT_FIELD,
  INPUT_MODE_COUNT
}
e_input_mode;

static e_input_mode mode;

void
game_init()
{
  mode = INPUT_DEFAULT;
}

void
game_update(double dt)
{
  if (input_get_key_down(KEY_CODE_RETURN))
  {
    mode = (mode + 1) % INPUT_MODE_COUNT;
  }
  
  switch (mode)
  {
    case INPUT_FIELD:
    {
      char ch[2] = { ' ', '\0' };
      if (input_get_char(&ch[0]))
      {
        LOG_DEBUG("$s pressed!", ch);
      }
    } break;
    
    case INPUT_DEFAULT:
    {
      if (input_get_key_down(KEY_CODE_W))
      {
        LOG_DEBUG("Move forward! $f ns\n\n\n", to_u64(dt * 1000.0));
      }
      
      if (input_get_key_up(KEY_CODE_W))
      {
        LOG_DEBUG("Stop moving forward! $f ns\n\n\n", to_u64(dt * 1000.0));
      }
      
      if (input_get_key(KEY_CODE_SPACE))
      {
        LOG_DEBUG("Let me fly! $f", to_u64(dt * 1000.0));
      }
    } break;
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

