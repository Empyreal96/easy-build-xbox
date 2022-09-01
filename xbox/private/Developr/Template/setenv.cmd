@if NOT DEFINED _echo echo off

REM ****************************
REM *** DON'T EDIT THIS FILE ***
REM ****************************

REM *************************************************************************
REM *
REM * SetEnv.cmd
REM *
REM * This file is executed whenever you start a Razzle Screen Group or
REM * execute the NTUSER command from within the Razzle Screen Group.  It
REM * sets up any private environment variables, personal screen modes
REM * and colors, etc.
REM *
REM *************************************************************************

REM
REM Set the prompt to something useful.
REM   $+ = show + when doing pushdir
REM   Note the trailing space.
REM
set PROMPT=[$+$p] 

REM
REM Default the DIR command to sort and to stop on each screen.
REM
set DIRCMD=/Ogne/P

REM
REM Save the original path if it hasn't been saved before.  This allows the
REM path to be extended without adding multiple copies to it.
REM
if NOT DEFINED Path_Original set Path_Original=%PATH%

REM
REM Add the Init directory to the path so we can get to our Self directory.
REM
set Path=%Path_Original%;%Init%

REM
REM Support multiple processors if there are any.
REM
if "%NUMBER_OF_PROCESSORS%" GTR "1" set BUILD_MULTIPROCESSOR=1

REM
REM Set the default binplace root directory.
REM
set _BINPLACE_ROOT=%_NTDrive%%_NTRoot%\xboxbuilds

REM *************************************************************************
REM * X86-specific commands
REM *************************************************************************

if /i "%PROCESSOR_ARCHITECTURE%" == "ALPHA" goto do_alpha
REM
REM Set the type of target.
REM
if defined _NTWIN64 (
	set BUILD_DEFAULT_TARGETS=-ia64
) else (
	set BUILD_DEFAULT_TARGETS=-386
)

REM
REM Set the BINPLACE directory variable.
REM
set _BINPLACE_VAR=_NT386TREE
goto done_proc_specific

REM *************************************************************************
REM * Alpha-specific commands
REM *************************************************************************

:do_alpha
REM
REM Alpha optimization.
REM
set ACC_OPTIMIZATION=-O2
set ALPHA=1

REM
REM Set the type of target.
REM
if defined _NTWIN64 (
	set BUILD_DEFAULT_TARGETS=-axp64
) else (
	set BUILD_DEFAULT_TARGETS=-alpha
)

REM
REM Set the BINPLACE directory variable.
REM
set _BINPLACE_VAR=_NTALPHATREE

REM *************************************************************************
REM * Done with processor-specific commnds.
REM *************************************************************************

:done_proc_specific

REM
REM Support Win64.
REM
if defined _NTWIN64 call setwin64

REM
REM Load cluster private aliases.
REM This is a hack until we can generalize this process.
REM
if NOT EXIST %INIT%\ClusCue.pri goto skipclus
alias -p remote.exe -f %INIT%\ClusCue.pri
alias -f %INIT%\ClusCue.pri
:skipclus

REM
REM Load further private aliases.
REM
if NOT EXIST %INIT%\CueP.pri goto skipit
alias -p remote.exe -f %INIT%\CueP.pri
alias -f %INIT%\CueP.pri
:skipit

REM
REM Invoke private environment procedure.
REM
if EXIST %INIT%\setenvp.cmd call %INIT%\setenvp.cmd
