@if NOT DEFINED _echo echo off

REM ****************************
REM *** DON'T EDIT THIS FILE ***
REM ****************************

REM *************************************************************************
REM *
REM * ShowOpt.cmd
REM *
REM * This file dumps the options you've got selected for the build.
REM *
REM *************************************************************************

if DEFINED COMPLEX (
	echo Building with COMPLEX hacks.
) else (
	echo Building without COMPLEX hacks.
)

if DEFINED FOCUS (
	echo Building for FOCUS tuner.
) else (
	echo Building for CONEXANT tuner.
)

if DEFINED NODEVKIT (
	echo Building for RETAIL.
) else (
	echo Building for DEVKIT.
)
