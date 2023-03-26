#ifndef __INPUT_H__
#define __INPUT_H__

#include "defines.h"

// should be max 256
typedef enum
e_key_code
{
#include "input_keys.h"
}
e_key_code;

typedef enum
e_button_code
{
  BUTTON_MOUSE_LEFT = 0,
  BUTTON_MOUSE_RIGHT = 1,
}
e_button_code;

typedef struct
t_input_state
{
  u64 keys_down[4];
  u64 keys_up[4];
  u64 keys_hold[4];
  u64 keys_prev_hold[4];
  
  u64 buttons_down[4];
  u64 buttons_up[4];
  u64 buttons_hold[4];
  u64 buttons_prev_hold[4];
  
  // one per frame, as appears?
  u8 char_trigger;
  
  i32 mouse_x;
  i32 mouse_y;
}
t_input_state;

void
input_init();

void
input_pre_update();

void
input_update();

b8
input_get_key_down(e_key_code key);

b8
input_get_key_up(e_key_code key);

b8
input_get_key(e_key_code key);

b8
input_get_button_down(e_button_code button);

b8
input_get_button_up(e_button_code button);

b8
input_get_button(e_button_code button);

void
input_get_mouse_pos(i32* out_x, i32* out_y);

b8
input_get_char(char* ch);

#endif
