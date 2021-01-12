@echo on
REM setlocal

if "" == "%_NT386TREE%" goto usage
if "-?" == "%1" goto usage
if "/?" == "%1" goto usage

if "-e" == "%1" goto external
if "-m" == "%1" goto manufacturing
if not exist "%_NT386TREE%\Recovery" mkdir %_NT386TREE%\Recovery
set __RETAILTREE=%_NT386TREE%
set __DEST=%_NT386TREE%\Recovery
rem if "" == "%1" set __RETAILTREE=
rem if "" == "%2" 
set __DEST=%_NTBINDIR%\private\ntos\recovery\cdfiles
set __XBETREE=%_NT386TREE%
set __X386TREE=%_NT386TREE%
set __XBINDIR=%_NTBINDIR%
set __IMAGE=recovery.iso
if "%STRESSCD%"=="1" set __IMAGE=rec_stress.iso
if "%NODEVKIT%" == "1" set __IMAGE=rec_retail.iso
goto cleanupfiles

:external
REM set __RETAILTREE=%2
REM set __XBETREE=%3
REM set __X386TREE=%4
REM set __XBINDIR=%5
REM set __DEST=%6

rem if "" == "%2" 
set __RETAILTREE=%_NT386TREE%
rem if "" == "%3" 
set __XBETREE=%_NT386TREE%
rem if "" == "%4" 
set __X386TREE=%_NT386TREE%
rem if "" == "%5" 
set __XBINDIR=%_NTBINDIR%
rem if "" == "%6" 
set __DEST=%_NTBINDIR%\private\ntos\recovery\cdf_ext
set __IMAGE=rec_ext.iso
if "%STRESSCD%"=="1" set __IMAGE=rec_ext_stress.iso
goto cleanupfiles

:manufacturing
rem set __RETAILTREE=%2
rem set __XBETREE=%3
rem set __X386TREE=%4
rem set __XBINDIR=%5
rem set __DEST=%6
rem 
rem if "" == "%2" 
set __RETAILTREE=%_NT386TREE%
rem if "" == "%3" 
set __XBETREE=%_NT386TREE%
rem if "" == "%4" 
set __X386TREE=%_NT386TREE%
rem if "" == "%5" 
set __XBINDIR=%_NTBINDIR%
rem if "" == "%6" 
set __DEST=%_NTBINDIR%\private\ntos\recovery\cdf_man
set __IMAGE=rec_manu.iso
if "%STRESSCD%"=="1" set __IMAGE=rec_ext_stress.iso
goto cleanupfiles

:cleanupfiles
@echo Cleaning up %__DEST%...
@echo.

mkdir %__DEST% 2>nul
delnode /q %__DEST%\*.*

@echo Copying files to %__DEST%...
@echo.

rd /s /q %__DEST%\RETAIL
mkdir %__DEST%\RETAIL 2>nul

copy /y %__XBETREE%\dump\recovery.xbe %__DEST%\default.xbe
rem copy /y %__RETAILTREE%\dump\recovery.xbe %__DEST%\default.xbe
copy /y %__X386TREE%\devkit\xbdm.dll %__DEST%\xbdm.dll

if not "-e" == "%1" if not "-m" == "%1" if not exist %_NT386tree%\boot\XBOXROM_DVT6.BIN goto failure
if not "-e" == "%1" if not "-m" == "%1" if not exist %_NT386tree%\boot\XBOXROM_DVT4.BIN goto failure
rem if not "-e" == "%1" if not "-m" == "%1" if not exist %__X386TREE%\boot\XBOXROM_QT.BIN goto failure
if not "-e" == "%1" if not "-m" == "%1" copy /y %_NT386tree%\boot\XBOXROM_DVT6.BIN %__DEST%\XBOXROM.BIN
if not "-e" == "%1" if not "-m" == "%1" copy /y %_NT386tree%\boot\XBOXROM_DVT4.BIN %__DEST%\XBOXROM_DVT4.BIN
rem if not "-e" == "%1" if not "-m" == "%1" copy /y %__X386TREE%\boot\XBOXROM_QT.BIN %__DEST%\XBOXROM_QT.BIN

if "-m" == "%1" if not exist %__X386TREE%\boot\XBOXROM_DVT6.BIN goto failure
if "-m" == "%1" if not exist %__X386TREE%\boot\XBOXROM_DVT4.BIN goto failure
rem if "-m" == "%1" if not exist %__X386TREE%\boot\XBOXROM_QT.BIN goto failure
if "-m" == "%1" copy /y %__X386TREE%\boot\XBOXROM_DVT6.BIN %__DEST%\XBOXROM.BIN
if "-m" == "%1" copy /y %__X386TREE%\boot\XBOXROM_DVT4.BIN %__DEST%\XBOXROM_DVT4.BIN
rem if "-m" == "%1" copy /y %__X386TREE%\boot\XBOXROM_QT.BIN %__DEST%\XBOXROM_QT.BIN

if "-e" == "%1" not exist %_NT386tree%\boot\XBOXROM_DVT6_EXT.BIN goto failure
if "-e" == "%1" not exist %_NT386tree%\boot\XBOXROM_DVT4_EXT.BIN goto failure
if "-e" == "%1" copy /y %_NT386tree%\boot\XBOXROM_DVT6_EXT.BIN %__DEST%\XBOXROM.BIN
if "-e" == "%1" copy /y %_NT386tree%\boot\XBOXROM_DVT4_EXT.BIN %__DEST%\XBOXROM_DVT4.BIN

if not "-m" == "%1" if not "%__RETAILTREE%" == "" if not exist %_NT386tree%\boot\XBOXROM_DVT6.BIN goto failure
if not "-m" == "%1" if not "%__RETAILTREE%" == "" if not exist %_NT386tree%\boot\XBOXROM_DVT4.BIN goto failure
rem if not "-e" == "%1" if not "-m" == "%1" if not "%__RETAILTREE%" == "" if not exist %__RETAILTREE%\boot\XBOXROM_QT.BIN goto failure
if not "-m" == "%1" if not "%__RETAILTREE%" == "" copy /y %_NT386tree%\boot\XBOXROM_DVT6.BIN %__DEST%\RETAIL\XBOXROM.BIN
if not "-m" == "%1" if not "%__RETAILTREE%" == "" copy /y %_NT386tree%\boot\XBOXROM_DVT4.BIN %__DEST%\RETAIL\XBOXROM_DVT4.BIN
rem if not "-e" == "%1" if not "-m" == "%1" if not "%__RETAILTREE%" == "" copy /y %__RETAILTREE%\boot\XBOXROM_QT.BIN %__DEST%\RETAIL\XBOXROM_QT.BIN

if "-m" == "%1" if not "%__RETAILTREE%" == "" if not exist %__RETAILTREE%\boot\XBOXROM_DVT6.BIN goto failure
if "-m" == "%1" if not "%__RETAILTREE%" == "" if not exist %__RETAILTREE%\boot\XBOXROM_DVT4.BIN goto failure
rem if "-m" == "%1" if not "%__RETAILTREE%" == "" if not exist %__RETAILTREE%\boot\XBOXROM_QT.BIN goto failure
if "-m" == "%1" if not "%__RETAILTREE%" == "" copy /y %__RETAILTREE%\boot\XBOXROM_DVT6.BIN %__DEST%\RETAIL\XBOXROM.BIN
if "-m" == "%1" if not "%__RETAILTREE%" == "" copy /y %__RETAILTREE%\boot\XBOXROM_DVT4.BIN %__DEST%\RETAIL\XBOXROM_DVT4.BIN
rem if "-m" == "%1" if not "%__RETAILTREE%" == "" copy /y %__RETAILTREE%\boot\XBOXROM_QT.BIN %__DEST%\RETAIL\XBOXROM_QT.BIN

mkdir %__DEST%\RECMEDIA 2>nul
xcopy /y /e %__XBINDIR%\private\ntos\recovery\recmedia %__DEST%\RECMEDIA

rd /s /q %__DEST%\XDASH
mkdir %__DEST%\XDASH 2>nul

if "%NODEVKIT%" == "1" goto dashboard

mkdir %__DEST%\devkit 2>nul
mkdir %__DEST%\devkit\dxt 2>nul

if not "-e" == "%1" copy /y %__X386TREE%\devkit\cydrive.exe %__DEST%\devkit\dxt\cydrive.dxt
copy /y %__XBINDIR%\private\external\sdk\vtune\current\*.* %__DEST%\devkit\dxt

if "%STRESSCD%"=="1" goto stresscd

copy /y %__XBETREE%\dump\xshell.xbe %__DEST%\XDASH
xcopy /y /e %__XBINDIR%\private\test\ui\xshell\tdata %__DEST%\XDASH
copy /y %__XBINDIR%\private\test\ui\xshell\dashboard.xbx %__DEST%\dashboard.xbx
if "%RECOVERYNOSAMPLES%" == "1" goto aftersamples

rd /s /q %__DEST%\DEVKIT\SAMPLES 2>nul
mkdir %__DEST%\DEVKIT\SAMPLES 2>nul

mkdir %__DEST%\DEVKIT\SAMPLES\DolphinClassic 2>nul
mkdir %__DEST%\DEVKIT\SAMPLES\DolphinClassic\Media 2>nul
copy /y %__XBETREE%\dump\DolphinClassic.xbe %__DEST%\DEVKIT\SAMPLES\DolphinClassic
xcopy /y /e %__XBINDIR%\private\atg\samples\graphics\dolphinclassic\media %__DEST%\DEVKIT\SAMPLES\DolphinClassic\Media

mkdir %__DEST%\DEVKIT\SAMPLES\PlayField 2>nul
mkdir %__DEST%\DEVKIT\SAMPLES\PlayField\Media 2>nul
copy /y %__XBETREE%\dump\PlayField.xbe %__DEST%\DEVKIT\SAMPLES\PlayField
xcopy /y /e %__XBINDIR%\private\atg\samples\graphics\playfield\media %__DEST%\DEVKIT\SAMPLES\PlayField\Media

mkdir %__DEST%\DEVKIT\SAMPLES\Gamepad 2>nul
mkdir %__DEST%\DEVKIT\SAMPLES\Gamepad\Media 2>nul
copy /y %__XBETREE%\dump\Gamepad.xbe %__DEST%\DEVKIT\SAMPLES\Gamepad
xcopy /y /e %__XBINDIR%\private\atg\samples\input\gamepad\media %__DEST%\DEVKIT\SAMPLES\Gamepad\Media

rd /s /q %__DEST%\DEVKIT\TOOLS 2>nul
mkdir %__DEST%\DEVKIT\TOOLS 2>nul

mkdir %__DEST%\DEVKIT\TOOLS\AudioConsole 2>nul
mkdir %__DEST%\DEVKIT\TOOLS\AudioConsole\Media 2>nul
copy /y %__XBETREE%\dump\audconsole.xbe %__DEST%\DEVKIT\TOOLS\AudioConsole
xcopy /y /e %__XBINDIR%\private\windows\directx\dsound\tools\dmconsole\media %__DEST%\DEVKIT\TOOLS\AudioConsole\Media

:dashboard
mkdir %__DEST%\TDATA 2>nul
mkdir %__DEST%\TDATA\fffe0000 2>nul

if not "-m" == "%1" if not exist %_NT386tree%\dump\xboxdash.xbe goto failure
if not "-m" == "%1" if not "%__RETAILTREE%" == "" if not exist %_NT386tree%\dump\xboxdash.xbe goto failure
if not "-m" == "%1" copy /y %_NT386tree%\dump\xboxdash.xbe %__DEST%\XDASH
if not "-m" == "%1" if not "%__RETAILTREE%" == "" copy /y %_NT386tree%\dump\xboxdash.xbe %__DEST%\RETAIL
if not exist "%__DEST%\XDASH\" mkdir %__DEST%\XDASH\
if not "-m" == "%1" xcopy /y /e %_NT386tree%\xdash %__DEST%\XDASH\

if "-m" == "%1" if not exist %__XBETREE%\dump\xboxdash.xbe goto failure
if "-m" == "%1" if not "%__RETAILTREE%" == "" if not exist %__RETAILTREE%\dump\xboxdash.xbe goto failure
if "-m" == "%1" copy /y %__XBETREE%\dump\xboxdash.xbe %__DEST%\XDASH
if "-m" == "%1" if not "%__RETAILTREE%" == "" copy /y %__RETAILTREE%\dump\xboxdash.xbe %__DEST%\RETAIL
if "-m" == "%1" xcopy /y %__XBINDIR%\private\ui\dash\*.xip %__DEST%\XDASH
if "-m" == "%1" xcopy /y %__XBINDIR%\private\ui\dash\*.xtf %__DEST%\XDASH
if "-m" == "%1" xcopy /y %__XBINDIR%\private\ui\dash\SoundFXADPCM %__DEST%\XDASH
if "-m" == "%1" xcopy /y %__XBINDIR%\private\ui\dash\Audio\AmbientAudioADPCM %__DEST%\XDASH\Audio\AmbientAudio\
if "-m" == "%1" xcopy /y %__XBINDIR%\private\ui\dash\Audio\TransitionAudioADPCM %__DEST%\XDASH\Audio\TransitionAudio\
if "-m" == "%1" xcopy /y %__XBINDIR%\private\ui\dash\Audio\MusicAudioADPCM %__DEST%\XDASH\Audio\MusicAudio\
if "-m" == "%1" xcopy /y %__XBINDIR%\private\ui\dash\Audio\MainAudioADPCM %__DEST%\XDASH\Audio\MainAudio\
if "-m" == "%1" xcopy /y %__XBINDIR%\private\ui\dash\Audio\MemoryAudioADPCM %__DEST%\XDASH\Audio\MemoryAudio\
if "-m" == "%1" xcopy /y %__XBINDIR%\private\ui\dash\Audio\SettingsAudioADPCM %__DEST%\XDASH\Audio\SettingsAudio\

if "%NODEVKIT%" == "1" goto image

mkdir %__DEST%\TDATA\fffe0000\music 2>nul
xcopy /y /e %__XBINDIR%\private\test\ui\sndtrk\content %__DEST%\TDATA\fffe0000\music

:aftersamples
if not "-e" == "%1" if not exist %_NT386TREE%\dump\dvdkey1.bin goto failure
if not "-e" == "%1" copy /y %_NT386TREE%\dump\dvdkey1.bin %__DEST%\XDASH\dvdkeyd.bin
if not "-e" == "%1" xcopy /i /y %__XBINDIR%\private\online\test\serverip\internal\xonline.ini %__DEST%\DEVKIT\
if "-e" == "%1" xcopy /i /y %__XBINDIR%\private\online\test\serverip\external\xonline.ini %__DEST%\DEVKIT\
goto image

:stresscd
copy /y %__XBETREE%\dump\harness.xbe %__DEST%\XDASH\xboxdash.xbe
xcopy /i /y %_NTBINDIR%\private\test\buildlab\ini\stress\testini.ini  %__DEST%\tdata\a7049955\
xcopy /i /y /e %_NTBINDIR%\private\test\multimedia\dmusic\dmtest1\media\*.* %__DEST%\tdata\a7049955\media\
xcopy /i /y /e %_NTBINDIR%\private\test\multimedia\dsound\media\*.* %__DEST%\tdata\a7049955\media\

:image
@echo off

gdfimage %__DEST% %__X386TREE%\%__IMAGE%
goto end

:usage
echo "usage: updrec [-e [XBETREE]] [-m] destpath (_NT386TREE must be defined; -e (external version); -m (manufacturing version))"
goto end

:failure
echo "Failed to create recovery CD because one or more files are missing"
goto end

:end
endlocal
