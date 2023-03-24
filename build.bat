@echo off

setlocal enabledelayedexpansion

set SCRIPT_PATH=%~dp0

if not exist .bin mkdir .bin
if not exist .lib mkdir .lib
if not exist .obj mkdir .obj

set CC=clang
set AR=ar

set COMPILE_OPTIONS=-std=c99 -w -g -O0 -Wall -Wextra -DASSERTS_ENABLE

set SYSTEM_LIBS=-luser32
set ENGINE_INCLUDES=-I%SCRIPT_PATH%engine\src

for /R "%SCRIPT_PATH%engine\src" %%f in (*.c) do (
call :compile_2obj %%f, .obj\%%~nf
SET ENGINE_OBJS=!ENGINE_OBJS! %SCRIPT_PATH%.obj\%%~nf.o )

%AR% rc %SCRIPT_PATH%.lib\engine.lib !ENGINE_OBJS!

call :error_check .lib\engine.lib

for /R "%SCRIPT_PATH%game\src" %%f in (*.c) do (
SET GAME_SRC=%GAME_SRC% %%f )

%CC% -o %SCRIPT_PATH%.bin\game.exe^
 %GAME_SRC% %ENGINE_INCLUDES% -L%SCRIPT_PATH%.lib^
 %SYSTEM_LIBS% %COMPILE_OPTIONS% -lengine

call :error_check .bin\game.exe

pause

exit 0

:compile_2obj (
  %CC% -c %~1 -o %SCRIPT_PATH%%~2.o %ENGINE_INCLUDES% %COMPILE_OPTIONS%
  call :error_check %~2.o
  goto :eof
)

:error_check (
  if %ERRORLEVEL% equ 0 (
  echo %~1 : success
  ) else (
  echo Error code: %ERRORLEVEL%
  pause
  exit %ERRORLEVEL%
  )
  goto :eof
)

endlocal
