#include "platform.h"

#include <windows.h>

#include <stdio.h>

#include <core/event.h>

typedef struct
system_state
{
  HINSTANCE instance;
  HWND handle;
}
system_state;

static system_state state;

static f64 clock_frequency;
static f64 awake_time;

LRESULT CALLBACK win32_process_msg(HWND window,
                                   u32 msg,
                                   WPARAM w_param,
                                   LPARAM l_param);

void
platform_init(const char* app_name,
              i32 x,
              i32 y,
              i32 width,
              i32 height)
{
  state.instance = GetModuleHandleA(0);
  
  HICON icon = LoadIcon(state.instance,
                        IDI_APPLICATION);
  WNDCLASS wc;
  platform_mem_set(&wc, 0, sizeof(wc));
  wc.style = CS_DBLCLKS;
  wc.lpfnWndProc = win32_process_msg;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = state.instance;
  wc.hIcon = icon;
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = NULL;
  wc.lpszClassName = "window_class";
  
  if (!RegisterClassA(&wc))
  {
    printf("Window registration failed!\n");
    exit(1);
  }
  
  u32 client_x = x;
  u32 client_y = y;
  u32 client_w = width;
  u32 client_h = height;
  
  u32 window_x = client_x;
  u32 window_y = client_y;
  u32 window_w = client_w;
  u32 window_h = client_h;
  
  u32 window_style = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
  u32 window_ex_style = WS_EX_APPWINDOW;
  
  window_style |= WS_MAXIMIZEBOX;
  window_style |= WS_MINIMIZEBOX;
  window_style |= WS_THICKFRAME;
  
  RECT border_rect = { 0, 0, 0, 0 };
  AdjustWindowRectEx(&border_rect, window_style, 0, window_ex_style);
  
  window_x += border_rect.left;
  window_y += border_rect.top;
  
  window_w += border_rect.right - border_rect.left;
  window_h += border_rect.bottom - border_rect.top;
  
  HWND handle = CreateWindowExA(window_ex_style,
                                "window_class",
                                app_name,
                                window_style,
                                window_x,
                                window_y,
                                window_w,
                                window_h,
                                0, 0,
                                state.instance, 0);
  
  if (handle == NULL)
  {
    printf("Window creation failed!\n");
    exit(2);
  }
  else
  {
    state.handle = handle;
  }
  
  i32 should_activate = 1;
  i32 show_window_command_flags = should_activate ? SW_SHOW : SW_SHOWNOACTIVATE;
  
  ShowWindow(state.handle,
             show_window_command_flags);
  
  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);
  clock_frequency = 1.0 / (f64)frequency.QuadPart;
  awake_time = platform_get_absolute_time();
}

void
platform_term()
{
  DestroyWindow(state.handle);
  state.handle = 0;
}

void
platform_pump_msg()
{
  MSG message;
  while(PeekMessageA(&message, NULL, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
}

void*
platform_alloc(u64 size,
               b8 aligned)
{
  return malloc(size);
}

void
platform_free(void* block,
              b8 aligned)
{
  return free(block);
}

void*
platform_mem_zero(void* block,
                  u64 size)
{
  return memset(block, 0, size);
}

void*
platform_mem_copy(void* dst,
                  const void* src,
                  u64 size)
{
  return memcpy(dst, src, size);
}

void*
platform_mem_set(void* dst,
                 i32 value,
                 u64 size)
{
  return memset(dst, value, size);
}

f64
platform_get_absolute_time()
{
  LARGE_INTEGER time;
  QueryPerformanceCounter(&time);
  return (f64)time.QuadPart * clock_frequency;
}

const u8 colors[] =
{
  FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
  FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED,
  FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
  FOREGROUND_RED | FOREGROUND_INTENSITY
};

void
platform_console_write(const char* msg,
                       u32 msg_length,
                       u8 color_index)
{
  HANDLE console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(console_handle, colors[color_index]);
  OutputDebugStringA(msg);
  LPDWORD number_written = 0;
  WriteConsoleA(console_handle,
                msg,
                (DWORD)msg_length,
                number_written, 0);
  SetConsoleTextAttribute(console_handle, colors[0]);
}

LRESULT CALLBACK win32_process_msg(HWND window,
                                   u32 msg,
                                   WPARAM w_param,
                                   LPARAM l_param)
{
  if (w_param == SC_KEYMENU && (l_param >> 16) <= 0) return false;
  
  switch(msg)
  {
    case WM_ERASEBKGND:
    {
      return true;
    } break;
    
    case WM_DESTROY:
    {
      PostQuitMessage(0);
      return false;
    } break;
    
    case WM_CLOSE:
    {
      event_dispatch(APP_QUIT);
      return false;
    } break;
    
    case WM_SIZE:
    {
      t_event e;
      e.ctx.i32[0] = LOWORD(l_param);
      e.ctx.i32[1] = HIWORD(l_param);
      
      event_dispatch_ext(&e, APP_RESIZE);
    } break;
    
    case WM_KEYDOWN:
    {
      t_event e;
      e.ctx.u32[0] = (u32)w_param;
      event_dispatch_ext(&e, INPUT_KEY_DOWN);
    } break;
    
    case WM_KEYUP:
    {
      t_event e;
      e.ctx.u32[0] = (u32)w_param;
      event_dispatch_ext(&e, INPUT_KEY_UP);
    } break;
    
    case WM_CHAR:
    {
      t_event e;
      e.ctx.u8[0] = (u8)w_param;
      event_dispatch_ext(&e, INPUT_CHAR);
    } break;
    
    case WM_LBUTTONDOWN:
    {
      t_event e;
      e.ctx.u32[0] = 0;
      event_dispatch_ext(&e, INPUT_BUTTON_DOWN);
    } break;
    
    case WM_LBUTTONUP:
    {
      t_event e;
      e.ctx.u32[0] = 0;
      event_dispatch_ext(&e, INPUT_BUTTON_UP);
    } break;
    
    case WM_RBUTTONDOWN:
    {
      t_event e;
      e.ctx.u32[0] = 1;
      event_dispatch_ext(&e, INPUT_BUTTON_DOWN);
    } break;
    
    case WM_RBUTTONUP:
    {
      t_event e;
      e.ctx.u32[0] = 1;
      event_dispatch_ext(&e, INPUT_BUTTON_UP);
    } break;
    
    case WM_MOUSEMOVE:
    {
      t_event e;
      e.ctx.i32[0] = LOWORD(l_param);
      e.ctx.i32[1] = HIWORD(l_param);
      event_dispatch_ext(&e, INPUT_MOUSE_MOVE);
    } break;
  }
  
  return DefWindowProcA(window, msg, w_param, l_param);
}

