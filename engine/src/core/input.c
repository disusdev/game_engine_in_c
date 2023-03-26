#include "input.h"

#include "event.h"

static t_input_state state;

void
input_register_char(t_event* e);

void
input_register_mouse_pos(t_event* e);

void
input_register_key_down(t_event* e);

void
input_register_key_up(t_event* e);

void
input_register_button_down(t_event* e);

void
input_register_button_up(t_event* e);

void
input_init()
{
  event_register(input_register_char, INPUT_CHAR);
  event_register(input_register_mouse_pos, INPUT_MOUSE_MOVE);
  event_register(input_register_key_down, INPUT_KEY_DOWN);
  event_register(input_register_key_up, INPUT_KEY_UP);
  event_register(input_register_button_down, INPUT_BUTTON_DOWN);
  event_register(input_register_button_up, INPUT_BUTTON_UP);
}

void
input_pre_update()
{
  state.char_trigger = 0;
}

void
input_update()
{
  state.keys_down[0] = (state.keys_hold[0] ^ state.keys_prev_hold[0]) & state.keys_hold[0];
  state.keys_down[1] = (state.keys_hold[1] ^ state.keys_prev_hold[1]) & state.keys_hold[1];
  state.keys_down[2] = (state.keys_hold[2] ^ state.keys_prev_hold[2]) & state.keys_hold[2];
  state.keys_down[3] = (state.keys_hold[3] ^ state.keys_prev_hold[3]) & state.keys_hold[3];
  
  state.keys_up[0] = (state.keys_hold[0] ^ state.keys_prev_hold[0]) & state.keys_prev_hold[0];
  state.keys_up[1] = (state.keys_hold[1] ^ state.keys_prev_hold[1]) & state.keys_prev_hold[1];
  state.keys_up[2] = (state.keys_hold[2] ^ state.keys_prev_hold[2]) & state.keys_prev_hold[2];
  state.keys_up[3] = (state.keys_hold[3] ^ state.keys_prev_hold[3]) & state.keys_prev_hold[3];
  
  state.keys_prev_hold[0] = state.keys_hold[0];
  state.keys_prev_hold[1] = state.keys_hold[1];
  state.keys_prev_hold[2] = state.keys_hold[2];
  state.keys_prev_hold[3] = state.keys_hold[3];
  
  //#########################################
  
  state.buttons_down[0] = (state.buttons_hold[0] ^ state.buttons_prev_hold[0]) & state.buttons_hold[0];
  state.buttons_down[1] = (state.buttons_hold[1] ^ state.buttons_prev_hold[1]) & state.buttons_hold[1];
  state.buttons_down[2] = (state.buttons_hold[2] ^ state.buttons_prev_hold[2]) & state.buttons_hold[2];
  state.buttons_down[3] = (state.buttons_hold[3] ^ state.buttons_prev_hold[3]) & state.buttons_hold[3];
  
  state.buttons_up[0] = (state.buttons_hold[0] ^ state.buttons_prev_hold[0]) & state.buttons_prev_hold[0];
  state.buttons_up[1] = (state.buttons_hold[1] ^ state.buttons_prev_hold[1]) & state.buttons_prev_hold[1];
  state.buttons_up[2] = (state.buttons_hold[2] ^ state.buttons_prev_hold[2]) & state.buttons_prev_hold[2];
  state.buttons_up[3] = (state.buttons_hold[3] ^ state.buttons_prev_hold[3]) & state.buttons_prev_hold[3];
  
  state.buttons_prev_hold[0] = state.buttons_hold[0];
  state.buttons_prev_hold[1] = state.buttons_hold[1];
  state.buttons_prev_hold[2] = state.buttons_hold[2];
  state.buttons_prev_hold[3] = state.buttons_hold[3];
}

void
input_register_char(t_event* e)
{
  state.char_trigger = e->ctx.u8[0];
}

void
input_register_mouse_pos(t_event* e)
{
  state.mouse_x = e->ctx.i32[0];
  state.mouse_y = e->ctx.i32[1];
}

void
input_register_key_down(t_event* e)
{
  u32 key = e->ctx.u32[0];
  state.keys_hold[key / 64] |= BIT(key % 64);
}

void
input_register_key_up(t_event* e)
{
  u32 key = e->ctx.u32[0];
  state.keys_hold[key / 64] &= !BIT(key % 64);
}

void
input_register_button_down(t_event* e)
{
  u32 button = e->ctx.u32[0];
  state.buttons_hold[button / 64] |= BIT(button % 64);
}

void
input_register_button_up(t_event* e)
{
  u32 button = e->ctx.u32[0];
  state.buttons_hold[button / 64] &= !BIT(button % 64);
}

b8
input_get_key_down(e_key_code key)
{
  return (state.keys_down[key / 64] & BIT(key % 64));
}

b8
input_get_key_up(e_key_code key)
{
  return (state.keys_up[key / 64] & BIT(key % 64));
}

b8
input_get_key(e_key_code key)
{
  return (state.keys_hold[key / 64] & BIT(key % 64));
}

b8
input_get_button_down(e_button_code button)
{
  return (state.buttons_down[button / 64] & BIT(button % 64));
}

b8
input_get_button_up(e_button_code button)
{
  return (state.buttons_up[button / 64] & BIT(button % 64));
}

b8
input_get_button(e_button_code button)
{
  return (state.buttons_hold[button / 64] & BIT(button % 64));
}

void
input_get_mouse_pos(i32* out_x,
                    i32* out_y)
{
  *out_x = state.mouse_x;
  *out_y = state.mouse_y;
}

b8
input_get_char(char* ch)
{
  *ch = state.char_trigger;
  return state.char_trigger;
}