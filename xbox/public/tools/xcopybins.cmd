@echo off
REM
REM Adapted from 'original' copybins.cmd 
REM Changes are to create folders and copy files to the NT build drive instead of pushing to
REM a connected Dev Kit Xbox.. 
REM
if "" == "%_NT386TREE%" goto usage
if not "%1" == "" SET _XBOXMACHINE=-x %1
if "%1" == "" SET _XBOXMACHINE=
cls
echo Ready to copy the Xbox's kernel and dashboard from %_NT386TREE%
pause
echo.
echo Backing up old files to "%_NT386TREE%\release_old"
echo This will remove any current "%_NT386TREE%\release_old" that exists
echo (Save the folder now if you want to, then continue)
pause
REM A very ugly way to Remove old postbuilds, backup the current and set fresh 'release' folder
cd /d %_NT386TREE%
if exist "%_NT386TREE%\release_old" rmdir /Q /S "%_NT386TREE%\release_old"
if exist "%_NT386TREE%\release" xbcp -f -q -y -r %_NT386TREE%\release\ %_NT386TREE%\release_old\
rmdir /Q /S %_NT386TREE%\release
if not exist "%_NT386TREE%\release\" mkdir %_NT386TREE%\release\
if not exist "%_NT386TREE%\release\c\" mkdir %_NT386TREE%\release\c\
if not exist "%_NT386TREE%\release\e\" mkdir %_NT386TREE%\release\e\
if not exist "%_NT386TREE%\release\e\dxt" mkdir %_NT386TREE%\release\e\dxt
if not exist "%_NT386TREE%\release\y\" mkdir %_NT386TREE%\release\y\
REM Adapted from copytest.cmd
if not exist "%_NT386TREE%\release\c\tdata\a7049955" mkdir %_NT386TREE%\release\c\tdata\a7049955
if not exist "%_NT386TREE%\release\c\tdata\103be6d2" mkdir %_NT386TREE%\release\c\tdata\103be6d2
if not exist "%_NT386TREE%\release\c\devkit" mkdir %_NT386TREE%\release\c\devkit
REM ------------

REM using the xbcp command in the source, we copy the required files that would be placed to an Xbox, from the %_NT386TREE% dir, into %_NT386TREE%\Release
rem
xbcp -f -q -y %_NT386TREE%\boot\xboxrom_dvt4.bin %_NT386TREE%\release\c\xboxrom.bin
if not "0" == "%errorlevel%" goto end
xbcp -f -q -y %_NT386TREE%\devkit\xbdm.dll %_NT386TREE%\release\c\
xbcp -f -q -y %_NT386TREE%\devkit\cydrive.exe %_NT386TREE%\release\e\dxt\cydrive.dxt

xbcp -f -q -y %_NT386TREE%\dump\xshell.xbe %_NT386TREE%\release\y\
xbcp -f -q -y -r %_NTBINDIR%\private\test\ui\xshell\tdata\ %_NT386TREE%\release\y\
xbcp -f -q -y %_NTBINDIR%\private\test\ui\xshell\dashboard.xbx %_NT386TREE%\release\c\
REM Adapted from copytest.cmd

xbcp -q -y %_XBOXMACHINE% %_NT386TREE%\dump\harness.xbe %_NT386TREE%\release\c\devkit
xbcp -q -y %_XBOXMACHINE% %_NT386TREE%\dump\hwtest.xbe %_NT386TREE%\release\c\devkit
xbcp -q -y %_XBOXMACHINE% %_NT386TREE%\xboxtest\testini.ini %_NT386TREE%\release\c\tdata\a7049955
REM 
echo.
echo Files have been copied to %_NT386TREE%\release in the layout of Xbox partitions
timeout /t 5
cls
echo Finished Opening %_NT386TREE%
pause 
explorer %_NT386TREE%
goto end

:usage
echo usage: copybins (_NT386TREE must be defined)

:end
exit /b