@if NOT DEFINED _echo echo off

REM *************************************************************************
REM *
REM * SetEnvP.cmd
REM *
REM * This file is executed whenever you started a Razzle Screen Group or
REM * execute the NTUSER command from within the Razzle Screen Group.  It
REM * sets up any private environment variables, personal screen modes
REM * and colors, etc.
REM *
REM * Make changes here instead of in SetEnv.cmd so that more global
REM * changes can be picked up without affecting private settings.
REM *
REM * If this file has been renamed as SetEnvP.cmd, it may be modified at will.
REM * UpdateSelf.cmd will not overwrite it.
REM *
REM *************************************************************************

REM
REM Reset prompt to default.
REM
rem set PROMPT=$p$g

REM
REM Restore DIR command to default behavior.
REM
rem set DIRCMD=

REM
REM Display build times by default.
REM
set BUILD_DEFAULT=%BUILD_DEFAULT% -P

REM
REM Set default build type for SUCKNT.
REM By default, SUCKNT gets files from Fre builds.
REM
rem set SUCKNT_BuildType=Chk

REM
REM Set default NT flavor for SUCKNT.
REM By default, SUCKNT gets files from the Srv build.
REM
rem set SUCKNT_Flavor=Ent

REM
REM Set replace policy for SUCKNT.
REM By default, SUCKNT replaces files.
REM
rem set SUCKNT_NoReplace=1

REM
REM Set default root directory for symbols for SUCKNT.
REM By default, SUCKNT copies symbol files to %WinDir%
REM
rem set SUCKNT_Symbols=%_NTBinDir%

REM
REM Set default root directory for tools for SUCKNT.
REM By default, SUCKNT looks in %SystemRoot% for
REM mstools and idw.
REM
rem set SUCKNT_ToolsRoot=%_NTBinDir%

REM
REM Set the Visual Studio environment variables to be used
REM with aliases for navigating to Visual Studio directories.
REM
REM VC5 DEFINITIONS
rem set VStudioDir=%SystemDrive%\Program Files\DevStudio
rem set VSCommonDir=SharedIDE
rem set VSMSDevDir=%VSCommonDir%
rem set VCDir=VC
REM VC6 DEFINITIONS
rem set VStudioDir=%SystemDrive%\Program Files\Microsoft Visual Studio
rem set VSCommonDir=Common
rem set VSMSDevDir=%VSCommonDir%\MSDev98
rem set VCDir=VC98
REM ANY VERSION
set VSTemplateDir=%VStudioDir%\%VSMSDevDir%\Template
