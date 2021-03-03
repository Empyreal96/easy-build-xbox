@echo off
cd /d %~d0
REM
REM UAC elevation
REM I found this method to elevate the script to admin here: https://stackoverflow.com/a/12264592
REM I chose it because it is what my Windows 10 Toolbox uses, and works well!
REM
:init
 setlocal DisableDelayedExpansion
 set cmdInvoke=1
 set winSysFolder=System32
 set "batchPath=%~0"
 for %%k in (%0) do set batchName=%%~nk
 set "vbsGetPrivileges=%temp%\OEgetPriv_%batchName%.vbs"
 setlocal EnableDelayedExpansion

:checkPrivileges
  NET FILE 1>NUL 2>NUL
  if '%errorlevel%' == '0' ( goto gotPrivileges ) else ( goto getPrivileges )

:getPrivileges
  if '%1'=='ELEV' (echo ELEV & shift /1 & goto gotPrivileges)
  ECHO.
  ECHO.
  ECHO Invoking UAC for Privilege Escalation
  ECHO.

  ECHO Set UAC = CreateObject^("Shell.Application"^) > "%vbsGetPrivileges%"
  ECHO args = "ELEV " >> "%vbsGetPrivileges%"
  ECHO For Each strArg in WScript.Arguments >> "%vbsGetPrivileges%"
  ECHO args = args ^& strArg ^& " "  >> "%vbsGetPrivileges%"
  ECHO Next >> "%vbsGetPrivileges%"

  if '%cmdInvoke%'=='1' goto InvokeCmd 

  ECHO UAC.ShellExecute "!batchPath!", args, "", "runas", 1 >> "%vbsGetPrivileges%"
  goto ExecElevation

:InvokeCmd
  ECHO args = "/c """ + "!batchPath!" + """ " + args >> "%vbsGetPrivileges%"
  ECHO UAC.ShellExecute "%SystemRoot%\%winSysFolder%\cmd.exe", args, "", "runas", 1 >> "%vbsGetPrivileges%"

:ExecElevation
 "%SystemRoot%\%winSysFolder%\WScript.exe" "%vbsGetPrivileges%" %*
 exit /B

:gotPrivileges
 setlocal & cd /d %~dp0
 if '%1'=='ELEV' (del "%vbsGetPrivileges%" 1>nul 2>nul  &  shift /1)

:ebinit
if NOT exist "%~d0\xbox\public\tools\easybuild.conf" goto eb-start
if exist "%~d0\xbox\public\tools\easybuild.conf" goto eb-load-config
rem if NOT exist "%~dp0\easy-build_1strun.complete" goto eb-start
rem if exist "%~dp0\easy-build_1strun.complete" goto eb-xbox-checks

:eb-load-config
set "_EBConf=%~dp0\xbox\public\tools\easybuild.conf"

set "_FIRST_RUN="
for /F "skip=1 delims=" %%i in (%_EBConf%) do if not defined _FIRST_RUN set "_FIRST_RUN=%%i"
echo First Run = %_FIRST_RUN%
echo.

set "_CPXXUPD_DONE="
for /F "skip=1 delims=" %%i in (%_EBConf%) do if not defined _CPXXUPD_DONE set "_CPXXUPD_DONE=%%i"
echo Complex = %_CPXXUPD_DONE%
echo.

goto eb-xbox-checks

:eb-start
cd /d %~dp0
Title Easy-Build Environment for Xbox
echo --------------------------------------------------------
echo Welcome to Easy-Build Environment for XBOX Original
echo.
echo -- In constant testing --
echo This script sets up the COMPLEX patches and creates
echo your Razzle 'Profile'. Then will load allow you to load 
echo Easy-Build. Run this script to load each time.
echo.
echo This tool was created off the '4chan repack' Xbox Tree..
echo It is the same as 'xbox trunk.rar' with 'CPXXUPD.RAR'
echo If you want to use with a different tree, check the Wiki
echo On the Github page on how.
echo. 
echo --------------------------------------------------------
echo Make sure your dir structure is as follows:
echo.
echo %~d0
echo \easy-build-xinit.cmd
echo \xbox                \
echo 	                  \private\
echo 	                  \public\
echo 	                  \CPXXUPD\
echo.
echo Requirements:
echo - Freshly extracted source (xbox_leak_may_2020.7z/xbox trunk.rar + CPXXUPD.rar))
echo - Layout as above and unmodified in the structure above 
echo - Free space
echo - Patience
echo.
echo If you are using the VHD image, this will already be setup for you..
pause
rem echo First run complete! delete this file to show 'First Run' screen >> %~dp0\easy-build_1strun.complete
rem test settings
echo [FIRST_RUN] > %~d0\xbox\public\tools\easybuild.conf
echo 1 >> %~d0\xbox\public\tools\easybuild.conf
set _FIRST_RUN=1

goto eb-xbox-checks

:eb-xbox-checks
REM
REM
REM SIMPLE IF STATEMENTS TO CHECK IF THE SOURCE AND CPXXUPD FOLDERS ARE SET-UP
REM
REM
REM
echo Now running some checks..
echo.
if NOT exist "%~d0\xbox\private" echo Xbox Source not setup correctly&& pause&& exit
if NOT exist "%~d0\xbox\public" echo Xbox Source not setup correctly&& pause&& exit
if NOT exist "%~d0\xbox\CPXXUPD" echo Complex Updates not found in \xbox\CPXXUPD&& pause&& exit
if exist "%~d0\xbox\private" goto eb-xbox-checks2

:eb-xbox-checks2
if exist "%~d0\xbox\CPXXUPD" goto eb-xbox-init-check


:eb-xbox-init-check
REM
REM
REM ADD CHECK FOR ALREADY PATCHED SOURCE OR IF TO BE PATCHED
if exist "%~d0\easy-build_1strun.complete" goto remoldconfig
if NOT exist "%~d0\xbox\PRIVATE\DEVELOPR\%username%\" (goto eb-setup-cpxxupd) else (goto eb-xbox-mainmenu-init)
REM
REM

:remoldconfig
REM
REM Remove old config files if exist and switch to easybuild.conf
REM
if exist "%~d0\easy-build_1strun.complete" del "%~d0\easy-build_1strun.complete"
if exist "%~d0\xbox\public\tools\cpxxupd_done.txt" del "%~d0\xbox\public\tools\cpxxupd_done.txt"
echo [FIRST_RUN] > %~d0\xbox\public\tools\easybuild.conf
echo 1 >> %~d0\xbox\public\tools\easybuild.conf
set _FIRST_RUN=1
echo [CPXXUPD_DONE] >> %~dp0\xbox\public\tools\easybuild.conf
echo 1 >> %~dp0\xbox\public\tools\easybuild.conf
echo [BVT_ADDRESS] >> %~dp0\xbox\public\tools\easybuild.conf
echo \\tsclient\D  >> %~dp0\xbox\public\tools\easybuild.conf
echo [BVT_KERNEL_ONLY] >> %~dp0\xbox\public\tools\easybuild.conf
echo 1  >> %~dp0\xbox\public\tools\easybuild.conf
echo [XBOX_DEBUG_IP] >> %~dp0\xbox\public\tools\easybuild.conf
echo 192.168.0.5  >> %~dp0\xbox\public\tools\easybuild.conf
echo [XBOX_TITLE_IP] >> %~dp0\xbox\public\tools\easybuild.conf
echo 192.168.0.3 >> %~dp0\xbox\public\tools\easybuild.conf
goto eb-xbox-mainmenu-init

:eb-setup-cpxxupd
if exist "%ebntroot%\PRIVATE\DEVELOPR\%username%\" echo CPXXUPD already set up && goto xbox-dorazzle
echo.
echo Easy-Build detected the Complex Patches haven't been applied.
echo Applying 'CPXXUPD' now..
echo.
cd /d %~d0\xbox\CPXXUPD
goto eb-cpxxupd

:eb-setup-profile
echo.
echo Creating Razzle profile
cd /d %~d0\XBOX\PRIVATE\DEVELOPR\TEMPLATE\
goto xbox-initrazzle

:eb-cpxxupd
if /i "%_CPXXUPD_DONE%" == "1" goto eb-xbox-mainmenu-init
rem if exist "%~dp0\xbox\public\tools\cpxxupd_done.txt" goto eb-xbox-mainmenu-init
REM Contents of 'xbox\CPXXUPD\CPXXUPD.cmd' adapted for Easy-Build
cd /d %~d0\xbox\CPXXUPD
setlocal

set _location_=%~d0\xbox

REM CleanUp
del /f /q "%_location_%\private\p_build.cmd"
del /f /q "%_location_%\private\windbg.lnk"
del /s /f /q "%_location_%\copy *"
del /s /f /q "%_location_%\shortcut *"
rmdir /s /q "%_location_%\private\developr\template\old"

REM Update
xcopy /f /s /y private\*.* %_location_%\private\
xcopy /f /s /y public\*.* %_location_%\public\

if exist "%_location_%\public\lib-mar02\d3d8i.lib" (
    copy "%_location_%\public\lib-mar02\d3d8i.lib" "%_location_%\public\lib"
)

rem Test config
echo [CPXXUPD_DONE] >> %~dp0\xbox\public\tools\easybuild.conf
echo 1 >> %~dp0\xbox\public\tools\easybuild.conf
REM Finish setup of .conf with 'default' settings.
echo [BVT_ADDRESS] >> %~dp0\xbox\public\tools\easybuild.conf
echo \\tsclient\D  >> %~dp0\xbox\public\tools\easybuild.conf
echo [BVT_KERNEL_ONLY] >> %~dp0\xbox\public\tools\easybuild.conf
echo 1  >> %~dp0\xbox\public\tools\easybuild.conf
echo [XBOX_DEBUG_IP] >> %~dp0\xbox\public\tools\easybuild.conf
echo 192.168.0.5  >> %~dp0\xbox\public\tools\easybuild.conf
echo [XBOX_TITLE_IP] >> %~dp0\xbox\public\tools\easybuild.conf
echo 192.168.0.3 >> %~dp0\xbox\public\tools\easybuild.conf
endlocal
goto eb-setup-profile


REM Contents of 'xbox\private\developr\template\initrazzle.cmd' adapted for Easy-Build
:xbox-initrazzle
 
setlocal
set _drive_=%~d0
set _ntroot_=xbox

set _drive_=ntroot
if /i "%_drive%" == "ntroot" set _ntroot_=xbox& shift& shift

xcopy /f %~d0\xbox\private\developr\template %~d0\xbox\private\developr\%username%\
cd /d %~d0\xbox\private\developr\%username%
move cuep-sample.pri cuep.pri
move setenvp-sample.cmd setenvp.cmd
move setchkp-sample.cmd setchkp.cmd
move setfrep-sample.cmd setfrep.cmd
move tools-sample.ini tools.ini
set _ntdrive=%_drive_%
set _ntbindir=%_drive_%\%_ntroot_%

endlocal
echo Loading Razzle
goto eb-xbox-mainmenu-init

:eb-xbox-mainmenu-init
cd /d %~d0\XBOX\PRIVATE\DEVELOPR\%username%\
goto xbox-dorazzle

REM Contents of 'xbox\private\developr\%username%\dorazzle.cmd' adapted for Easy-Build
rem fixed the invalid paths to 'idw' and 'mstools'

:xbox-dorazzle

REM
REM Set _NTDrive
REM
set _NTDrive=%~d0
REM
REM Set _NTRoot
REM
set _NTRoot=\xbox
REM
REM Support Win64
REM
if /i "%1" == "WIN64" (
	set _NTWIN64=1
	shift
)
REM
REM Add MSTOOLS, IDW and PUBLIC\TOOLS to the path if not in system directory.
REM
if exist "%_NTDrive%%_NTRoot%\public\mstools" (
    set "MSTOOLS_DIR=%_NTDrive%%_NTRoot%\public\mstools"
    set "PATH=%PATH%;%_NTDrive%%_NTRoot%\public\mstools"
)
if exist "%_NTDrive%%_NTRoot%\public\idw" (
    set "IDW_DIR=%_NTDrive%%_NTRoot%\public\idw"
    set "PATH=%PATH%;%_NTDrive%%_NTRoot%\public\idw"
)
if exist "%_NTDrive%%_NTRoot%\public\tools" (
    set "PATH=%PATH%;%_NTDrive%%_NTRoot%\public\tools"
)
REM This adds the Barnabus Repack's BIOSpack to the PATH variable so it can be called easy
if exist "%_NTDrive%%_NTRoot%\public\idw\biospack" set "PATH=%PATH%;%_NTDrive%%_NTRoot%\public\idw\biospack"

goto ebhandover

:ebhandover

rem
rem
cls
echo.
echo Razzle will now start.
echo. 
echo Please type "easybuild" without quotes or type:
echo.
echo - easybuild free        //For FREE Devkit build
echo - easybuild free xm3    //For FREE Retail build
echo - easybuild chk         //For CHK DevKit Build
echo - easybuild chk xm3     //For CHK Retail Build
echo.
echo NOTE: You need to build FREE before CHK
echo.
echo NOTE: Easy-Build will always set the default build to:
echo FREE DEVKIT if no options specified
echo.
pause



goto INVOKEIT

:INVOKEIT
REM
REM Invoke RAZZLE.CMD, which does the real work of setting up the Razzle window.
REM


if /i "%PROCESSOR_ARCHITECTURE%" == "AMD64" set PROCESSOR_ARCHITECTURE=x86
cls
cmd.exe /k "%_NTDrive%%_NTRoot%\public\tools\razzle.cmd %~d0 NTROOT \XBOX SETFRE"