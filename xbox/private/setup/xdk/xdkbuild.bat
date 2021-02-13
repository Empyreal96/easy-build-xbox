rem @echo off
REM The build machine is special
if not exist "%_NT386TREE%\XDKSetup\%_BuildVer%" mkdir "%_NT386TREE%\XDKSetup\%_BuildVer%"
if exist "%_NT386TREE%\XDKSetup\%_BuildVer%\XBLD_LOG" del "%_NT386TREE%\XDKSetup\%_BuildVer%\XBLD_LOG"
if not exist "%_NT386TREE%\doc\samples\" mkdir "%_NT386TREE%\doc\samples\"
If /i "%ComputerName%"=="XBuilds" goto BuildMachine

REM Normally this batch file (when run anywhere but the build machine) uses the latest bits from the
REM \\xbuilds\release\usa.  If you want to override that you must set _BuildVer in the environment.
REM You also must map the P: drive to the location of your files, which must be laid out like those on the
REM public share.
If Not "%_BuildVer%"=="" Goto AssumePDefined
rem for /f %%n in (\\xbuilds\release\usa\latest.txt) do set _BuildVer=%%n
rem set Pdrive=%_NT386TREE%\
set Pdrive=%_NT386TREE%
set _SETUP_FILE_PATH=%_NT386TREE%\dump
set _SETUP_TARGET_PATH=%_NT386TREE%\XDKSetup\%_BuildVer%
goto StartBuild

:AssumePDefined
set Pdrive=%_NT386TREE%
set _SETUP_FILE_PATH=%_NT386TREE%\dump
set _SETUP_TARGET_PATH=%_NT386TREE%\XDKSetup\%_BuildVer%

goto StartBuild

:BuildMachine
If Not "%_BuildVer%"=="" Goto SkipUsage
Echo _BuildVer needs to be defined
Goto END

:SkipUsage
set Pdrive=%_NT386TREE%
set _SETUP_FILE_PATH=%_NT386TREE%\dump
set _SETUP_TARGET_PATH=%_NT386TREE%\XDKSetup\%_BuildVer%

:StartBuild
set XBLD_LOG=%_NT386TREE%\XDKSetup\%_BuildVer%\XBLD_LOG
Echo XDK Setup Build Started at %Date% %Time%
if not exist "%_NTBINDIR%\private\ntos\init\console\obj\i386\xboxkrnl.exe" echo Build the Kernel first! && pause && exit /b
if not exist "%_NT386TREE%\DevKit\xbdm.dll" echo Build xbdm.dll first! && pause && exit /b
if not exist "%_NTBINDIR%\private\external\sdk\4400\symbols" mkdir "%_NTBINDIR%\private\external\sdk\4400\symbols"
copy %_NTBINDIR%\private\ntos\init\console\obj\i386\xboxkrnl.exe %_NTBINDIR%\private\external\sdk\4400\symbols
copy %_NTBINDIR%\private\ntos\init\console\obj\i386\xboxkrnl.pdb %_NTBINDIR%\private\external\sdk\4400\symbols
copy %_NT386TREE%\DevKit\xbdm.dll %_NTBINDIR%\private\external\sdk\4400\symbols
copy %_NT386TREE%\DevKit\xbdm.pdb %_NTBINDIR%\private\external\sdk\4400\symbols
if not exist "%_NT386TREE%\XDKSetup\%_BuildVer%\XBSESetup.exe" goto XBSEBuild
copy %_NTBINDIR%\private\external\sdk\doc\samples\* %_NT386TREE%\doc\samples\
copy %_NTBINDIR%\private\external\sdk\doc\XboxSDK.chi %_NT386TREE%\doc\
copy %_NTBINDIR%\private\external\sdk\doc\XboxSDK.chm %_NT386TREE%\doc\
copy %_NTBINDIR%\private\external\sdk\doc\XboxSDK.kwd %_NT386TREE%\doc\
copy %_NTBINDIR%\private\xdktools\dvdutil\dumpfst\obj\i386\dumpfst.exe %_NTBINDIR%\private\external\sdk\amc\release\aug01-final\
if not "%XBLD_LOG%"=="" Echo XDK Setup Build Started at %Date% %Time% >> %XBLD_LOG%
Pushd %_NTBINDIR%\private\setup\xdk
if not "%XBLD_LOG%"=="" (
    %_NT386TREE%\dump\xpacker.exe %1 %2 %3 %4 xdk.ini >> %XBLD_LOG%
    ) else (
    %_NT386TREE%\dump\xpacker.exe %1 %2 %3 %4 xdk.ini
)
PopD
Echo XDK Setup Build Completed at %Date% %Time%
if not "%XBLD_LOG%"=="" Echo XDK Setup Build Completed at %Date% %Time% >> %XBLD_LOG%
Echo.
Echo.

set Pdrive=
set _SETUP_FILE_PATH=
set _SETUP_TARGET_PATH=
 goto END

:XBSEBuild
echo XBSE Setup Missing, Building now.
echo.
Echo XBSE Setup Build Started at %Date% %Time%
Echo XBSE Setup Build Started at %Date% %Time% >> %XBLD_LOG%
Pushd %_NTBINDIR%\private\setup\xbse
%_NT386TREE%\dump\xpacker.exe %1 %2 %3 %4 xbse.ini >> %XBLD_LOG%
PopD
Echo XBSE Setup Build Completed at %Date% %Time%
Echo XBSE Setup Build Completed at %Date% %Time% >> %XBLD_LOG%
Echo.
Echo.
goto StartBuild


:END
