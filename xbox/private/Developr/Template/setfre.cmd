@if NOT DEFINED _echo echo off

REM ****************************
REM *** DON'T EDIT THIS FILE ***
REM ****************************

REM *************************************************************************
REM *
REM * SetFre.cmd
REM *
REM * Set up a free Razzle window.
REM *
REM *************************************************************************

REM
REM Build into obj, not objd.
REM
set BUILD_ALT_DIR=

REM
REM Build with no debug info.
REM
set NTDEBUG=
set NTDEBUGTYPE=

REM
REM Set optimization settings.
REM
set MSC_OPTIMIZATION=

REM
REM Set default binplace subdirectory.
REM
if DEFINED _NTWIN64 (
	set _BINPLACE_SUBDIR=fre.64
) else (
	set _BINPLACE_SUBDIR=fre
)

REM
REM Set the binplace directory for this Razzle window.
REM
set _BINPLACE_DIR=%_BINPLACE_ROOT%\%_BINPLACE_SUBDIR%
set %_BINPLACE_VAR%=%_BINPLACE_DIR%

REM
REM Execute private commands for free Razzle windows only.
REM
if EXIST %INIT%\SetFreP.cmd call %INIT%\SetFreP.cmd
