@echo off
set _BVTMonVer=v0.11
Title Easy-Build XBOX Build Verification Testing Monitor - %_BVTMonVer%
if exist "%~dp0\xboxbins\NEEDED_BY_BVTMONITOR" cd /d "%~dp0\xboxbins\NEEDED_BY_BVTMONITOR" && del *.* /f /q
color 2F
:BVTWelcome
echo --------------------------------------------------------------------------------------------
echo  Easy-Build XBOX Build Verification Testing Monitor
echo --------------------------------------------------------------------------------------------
echo.
echo  The sole purpose of this script is to monitor the Shared folder between
echo  the Build VM and the Host.
echo  When You finish the desired build in the VM, the required files will be placed
echo  to the Shared directory, This script will then set up the BVT environment with XEMU.
echo.
echo  REQUIREMENTS, FOLLOW CAREFULLY:
echo  - The Build VM Configured with a shared drive/folder   *Will be referred as $(BVT_DRIVE)*
echo  - This script MUST be in the ROOT of the Shared area   '$(BVT_DRIVE)\BVTMonitor.cmd'
echo  - Place your desired copy of XEMU into                 '$(BVT_DRIVE)\BVT1_XEMU\xemu.exe'
echo  - An XBOX Formatted .qcow2 image in                    '$(BVT_DRIVE)\BVT1_XEMU\xbox_hdd.qcow2'
echo  - Make sure you have your MCPX ROM in                  '$(BVT_DRIVE)\Bldr_Files\mcpx.bin'
echo  - Enough Free Space for copying build files to         '$(BVT_DRIVE)\xboxbins\'
echo.
echo  THINGS TO REMEMBER:
echo  - 'xboxbins' will act as the Server Share, BVT Prep and some Postbuild Scripts.
echo  - You will be required to know the Network Share Path e.g \\tsclient\D .
echo.
echo  Please set up the Shared Directory/Drive for the BVT now.
echo.
echo  Type 'Continue' when it is set up, or close now.
echo.
set /p UserInput=:
echo --------------------------------------------------------------------------------------------
if /i "%UserInput%"=="Continue" goto CheckBVTFiles
if /i "%UserInput%"=="" goto BVTWelcome


:CheckBVTFiles
REM
REM Here we check to see if the needed files are where they need to be, if they are missing, say whats missing
REM
echo.
if not exist "%~dp0\BVT1_XEMU" echo "%~dp0\BVT1_XEMU" Cannot be found! && pause && goto BVTWelcome
if not exist "%~dp0\Bldr_Files" echo "%~dp0\Bldr_Files" Cannot be found! && pause && goto BVTWelcome
if not exist "%~dp0\xboxbins" echo "%~dp0\xboxbins" Cannot be found! && pause && goto BVTWelcome
if exist "%~dp0\BVT1_XEMU\xqemu.exe" echo XQEMU is not supported at this time, please use XEMU && pause && goto BVTWelcome
if not exist "%~dp0\BVT1_XEMU\xemu.exe" echo "%~dp0\BVT1_XEMU\xemu.exe" Cannot be found! && pause && goto BVTWelcome
if not exist "%~dp0\BVT1_XEMU\xbox_hdd.qcow2" echo "%~dp0\BVT1_XEMU\xbox_hdd.qcow2" Cannot be found! && pause && goto BVTWelcome
if not exist "%~dp0\Bldr_Files\mcpx.bin" echo "%~dp0\Bldr_Files\mcpx.bin" Cannot be found! && pause && goto BVTWelcome
echo Everything seems to be in order!
if not exist "%~dp0\xboxbins\NEEDED_BY_BVTMONITOR" mkdir %~dp0\xboxbins\NEEDED_BY_BVTMONITOR
echo.
cls
goto BVTPreface

:BVTPreface
echo --------------------------------------------------------------------------------------------
echo  Easy-Build XBOX Build Verification Testing Monitor - BVT Prep
echo --------------------------------------------------------------------------------------------
echo.
echo Now BVTMonitor will set up it's env variables, and then wait for specific events
echo throughout the VM's Buildthrough.. When needed this will modify your XEMU Config
echo to reflect the Binplaced files.
echo.
echo Degugging at this stage is not implimented, due to bugs in XEMU awating fixes
echo You should already know how to set up the Xbox VHD with a Dashboard.
echo.
echo Please Wait.
timeout /t 5
echo.
goto FileChecker_PATHInit

:FileChecker_PATHInit
REM
REM Here we set up env variables to make it easier than typing loads out
REM
echo.
echo Setting up Environment Variables..
set _BVTDrive=%~dp0
set _VirtualBVTDir=%_BVTDrive%\BVT1_XEMU\
set _VirtualBVTExec=%_VirtualBVTDir%xemu.exe
set _BVTHDD=%_VirtualBVTDir%xbox_hdd.qcow2
set _XBins=%_BVTDrive%\xboxbins
set _BVTBldrFiles=%_BVTDrive%\Bldr_Files
set _BVTMCPX=%_BVTBldrFiles%\mcpx.bin
set _BVTMonSanityChecks=%_XBins%\NEEDED_BY_BVTMONITOR
if not exist %_XBins%\Release\ mkdir %_XBins%\Release\
set _XBOXBINPLACED=%_XBins%\Release\
set _XBOXKERNEL=%_XBins%\Release\xboxkrnl.exe
set _XBOXBIOS=%_XBins%\Release\xboxbios.bin
set _BVTToolPath=%_BVTDrive%\Tools

goto BVTConnectionTest

REM
REM How we get this (BVTMonitor.cmd) to talk to the VM's Easy-Build script is not pretty, For each 'step' we create a .txt file
REM on the network share to tell each script what stage they are at.. a bunch of looping IF statements will happen for each 'step' 
REM until either script creates the file needed to go onto the next loop
REM
:BVTConnectionTest
echo.
echo Waiting for Connection.
if exist "%_BVTMonSanityChecks%\BVTConnected.txt" echo echo Build Machine Found && goto WaitForBuild
goto BVTConnectionTest

:WaitForBuild
echo.
echo Waiting for Build Start.
if exist "%_BVTMonSanityChecks%\BuildStarted.txt" echo Build has started && goto WaitForPostbuild
goto WaitForBuild

:WaitForPostbuild
echo.
echo Waiting for files to be Binplaced.
if exist "%_BVTMonSanityChecks%\FinalBuildPrep.txt" echo Binplacing Started && goto FileChecker_Stage1
goto WaitForPostbuild

:FileChecker_Stage1
echo Looking for XBOX Kernel
if exist "%_XBOXBINPLACED%eeprom.bin" goto FileChecker_Stage-GotEEPROM
if exist "%_XBOXKERNEL%" goto FileChecker_Stage2
goto FileChecker_Stage1

:FileChecker_Stage-GotEEPROM
echo EEPROM Found.
if /i "%_EEPROM%" == "" set _EEPROM=%_XBOXBINPLACED%eeprom.bin
if exist "%_XBOXKERNEL%" goto FileChecker_Stage2
goto FileChecker_Stage-GotEEPROM

:FileChecker_Stage2
echo Kernel Check Successful! >> "%_BVTMonSanityChecks%\KernelFound.txt"
if exist "%_BVTMonSanityChecks%\Build.log" goto FileChecker_Final
if exist "%_BVTMonSanityChecks%\Buildd.log" goto FileChecker_Final
goto FileChecker_Stage2

:FileChecker_Final
echo BIOS Found.
if exist "%_XBOXBIOS%" echo Bios Found and Copied >> "%_BVTMonSanityChecks%\BiosFound.txt" && goto PrepBVT
goto FileChecker_Final

:PrepBVT
cls
echo We will need to edit "%APPDATA%\xemu\xemu\xemu.ini" to apply the BVT Config
echo The old file will be backed up to "xemu.ini.bak" 
echo.
echo Please Wait..
echo.
if exist "%_BVTBldrFiles%\xboxbios.bin" del "%_BVTBldrFiles%\xboxbios.bin"
copy "%_XBOXBIOS%" "%_BVTBldrFiles%\xboxbios.bin"
cd /d %_VirtualBVTDir%
REM I struggled to find easy and working methods here, so I am using a messy 'find the string, set it as a variable, set new path as variable, run both through replace.vbs
REM                              xboxbios.bin
if exist "%APPDATA%\xemu\xemu\xemu.ini" copy %APPDATA%\xemu\xemu\xemu.ini %APPDATA%\xemu\xemu\xemu.ini.bak
if NOT Exist "%_BVTDrive%\xemu.ini.bak" copy "%APPDATA%\xemu\xemu\xemu.ini.bak" "%_BVTDrive%\xemu.ini.bak"
for /f "tokens=*" %%i in ('findstr flash_path* "%APPDATA%\xemu\xemu\xemu.ini"') do if "%%i"=="%%i" set "_XEMUOLDFlashPath=%%i"
echo Old entry: %_XEMUOLDFlashPath%
set "_XEMUNEWFlashPath=flash_path = %_BVTBldrFiles%\xboxbios.bin"
rem Pause
echo New Entry: %_XEMUNEWFlashPath%
%_BVTToolPath%\replace.vbs %APPDATA%\xemu\xemu\xemu.ini "%_XEMUOLDFlashPath%" "%_XEMUNEWFlashPath%" /I
REM                              MCPX.bin
for /f "tokens=*" %%i in ('findstr bootrom_path* "%APPDATA%\xemu\xemu\xemu.ini"') do if "%%i"=="%%i" set "_XEMUOLDBootromPath=%%i"
echo  Old entry: %_XEMUOLDBootromPath%
set "_XEMUNEWBootromPath=bootrom_path = %_BVTMCPX%"
rem Pause
echo  New entry: %_XEMUNEWBootromPath%
%_BVTToolPath%\replace.vbs %APPDATA%\xemu\xemu\xemu.ini "%_XEMUOLDBootromPath%" "%_XEMUNEWBootromPath%" /I
REM                             xbox_hdd.qcow2
for /f "tokens=*" %%i in ('findstr hdd_path* "%APPDATA%\xemu\xemu\xemu.ini"') do if "%%i"=="%%i" set "_XEMUOLDHDDPath=%%i"
echo  Old entry: %_XEMUOLDHDDPath%
set "_XEMUNEWHDDPath=hdd_path = %_BVTHDD%"
rem Pause
echo New entry: %_XEMUNEWHDDPath%
%_BVTToolPath%\replace.vbs %APPDATA%\xemu\xemu\xemu.ini "%_XEMUOLDHDDPath%" "%_XEMUNEWHDDPath%" /I

if exist "%_EEPROM%" goto StartBVTEEPROMTest
rem notepad %APPDATA%\xemu\xemu\xemu.ini

goto StartBVTTest



:StartBVTTest
REM Start XEMU with the above applied changes, if an EEPROM file is found, we will use this.
if /i "%_EEPROM%" == "%_XBOXBINPLACED%eeprom.bin" goto StartBVTWithEEPROM
echo Virtual BVT Started >> %_BVTMonSanityChecks%\VirtualBVTStarted.txt
cmd.exe /c %_VirtualBVTExec% 
echo.
echo When you are finished with the BVT Session, Press Enter to clean up session
pause
echo BVT Session has finished >> %_BVTMonSanityChecks%\BVTTestFinished.txt
goto WaitForVM



:WaitForVM
if exist "%_BVTMonSanityChecks%\VMGoneHome.txt" goto BVTCleanupSession
goto WaitForVM

:StartBVTWithEEPROM
REM                             eeprom.bin
for /f "tokens=*" %%i in ('findstr eeprom_path* "%APPDATA%\xemu\xemu\xemu.ini"') do if "%%i"=="%%i" set "_XEMUOLDEEPROMPath=%%i"
echo Old entry: %_XEMUOLDEEPROMPath%
set "_XEMUNEWEEPROMPath=eeprom_path = %_EEPROM%"
Pause
echo New entry: %_XEMUNEWEEPROMPath%
%_BVTToolPath%\replace.vbs %APPDATA%\xemu\xemu\xemu.ini "%_XEMUOLDEEPROMPath%" "%_XEMUNEWEEPROMPath%" /I

echo Virtual BVT Started EEPROM >> %_BVTMonSanityChecks%\VirtualBVTStarted.txt
cmd.exe /c %_VirtualBVTExec%
echo.
echo When you are finished with the BVT Session, 
echo Press Enter to clean up session and copy testing files to %_XBins%\Tested
echo.
pause
echo BVT Session has finished >> %_BVTMonSanityChecks%\BVTTestFinished.txt
goto WaitForVM

:BVTCleanupSession
REM This is required because of the leftover .txt files
if exist "%_BVTMonSanityChecks%\BVTTestFinished.txt" cd /d "%_BVTMonSanityChecks%" && del *.* /f /q
if not exist "%_XBins%\Tested" mkdir %_XBins%\Tested 
cd /d "%_XBins%\Tested" && del *.* /f /q
copy "%_XBOXBIOS%" "%_XBins%\Tested\"
copy "%_XBins%\Release\Symbols\xboxkrnl.*" "%_XBins%\Tested\Symbols"
if exist "%_XBins%\Tested\xboxbios.bin" del "%_XBOXBIOS%"
copy "%_XBOXKERNEL%" "%_XBins%\Tested\"
if exist "%_XBins%\Tested\xboxkrnl.exe" del "%_XBOXKERNEL%"
rem if exist "%_EEPROM%" copy "%_EEPROM%" "%_XBins%\Tested\"
rem if exist "%_XBins%\Tested\eeprom.bin" del "%_EEPROM%"
if exist "%_BVTBldrFiles%\xboxbios.bin" del %_BVTBldrFiles%\xboxbios.bin
if exist "%_BVTMonSanityChecks%\Build.log" copy %_BVTMonSanityChecks%\Build.log %_XBins%\Tested
if exist "%_BVTMonSanityChecks%\Build.err" copy %_BVTMonSanityChecks%\Build.err %_XBins%\Tested
if exist "%_BVTMonSanityChecks%\Build.wrn" copy %_BVTMonSanityChecks%\Build.wrn %_XBins%\Tested
if exist "%_BVTMonSanityChecks%\Buildd.log" copy %_BVTMonSanityChecks%\Buildd.log %_XBins%\Tested
if exist "%_BVTMonSanityChecks%\Buildd.err" copy %_BVTMonSanityChecks%\Buildd.err %_XBins%\Tested
if exist "%_BVTMonSanityChecks%\Buildd.wrn" copy %_BVTMonSanityChecks%\Buildd.wrn %_XBins%\Tested
set _BVTDrive=
set _VirtualBVTDir=
set _VirtualBVTExec=
set _BVTHDD=
set _XBins=
set _BVTBldrFiles=
set _BVTMCPX=
set _BVTMonSanityChecks=
set _XBOXBINPLACED=
set _XBOXKERNEL=
set _XBOXBIOS=
echo.
echo Finished.
goto End
:End
echo Press Enter to exit.
pause









