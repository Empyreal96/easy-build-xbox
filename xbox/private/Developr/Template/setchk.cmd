@if NOT DEFINED _echo echo off

REM ****************************
REM *** DON'T EDIT THIS FILE ***
REM ****************************

REM *************************************************************************
REM *
REM * SetChk.cmd
REM *
REM * Set up a checked Razzle window.
REM *
REM *************************************************************************

REM
REM Build into objd instead of obj.
REM
set BUILD_ALT_DIR=d

REM
REM Build with full debug info.
REM
set NTDEBUG=ntsd
set NTDEBUGTYPE=both

REM
REM Set optimization settings.
REM
set MSC_OPTIMIZATION=
if /i "%1" == "NOOPT" set MSC_OPTIMIZATION=/Odi
if /i "%1" == "OPT" set MSC_OPTIMIZATION=

REM
REM Set default binplace subdirectory.
REM
if DEFINED _NTWIN64 (
	set _BINPLACE_SUBDIR=chk.64
) else (
	set _BINPLACE_SUBDIR=chk
)

REM
REM Set the binplace directory for this Razzle window.
REM
set _BINPLACE_DIR=%_BINPLACE_ROOT%\%_BINPLACE_SUBDIR%
set %_BINPLACE_VAR%=%_BINPLACE_DIR%

REM
REM Execute private commands for checked Razzle windows only.
REM
if EXIST %INIT%\SetChkP.cmd call %INIT%\SetChkP.cmd
