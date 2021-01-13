@echo off
cls
rem Here we set some variables that are not set by Easy-build.cmd/razzle during load, so we just load them
set _BUILDVER=4400
if /i "%COMPLEX%" == "" set COMPLEX=1 
set NODEVKIT=
if /i "%FOCUS%" == "" set FOCUS=1
if /i "%_BINPLACE_SUBDIR%" == "" call setfre.cmd
if /i "%1" == "" call setfre.cmd
if /i "%1" == "free" call setfre.cmd
if /i "%1" == "chk" call setchk.cmd
:eb-xbox-mainmenu
cls
set ebdrive=%_NTDrive%
set ebntroot=%_NTBINDIR%
set ebxbbins=%_BINPLACE_DIR%
set ebxbtype=%_BINPLACE_SUBDIR%
cd /d %ebntroot%
color 2F
echo --------------------------------------------------------------------------------------------
echo  Empyreal's Easy-Build for XBOX ORIGINAL (Very limited features currently, WIP)
echo --------------------------------------------------------------------------------------------
echo  Build User: %_NTUSER%	Build Machine: %COMPUTERNAME%
echo  Build Root: %ebntroot% 	Razzle Tool Path: %ebntroot%\public\tools
echo  Postbuild Dir: %ebxbbins%
echo --------------------------------------------------------------------------------------------
echo - Release Type: %ebxbtype%  - NT Tree: XBOX %BUILD_PRODUCT% %BUILD_PRODUCT_VER% - Xbox Ver: %_BUILDVER%
echo --------------------------------------------------------------------------------------------
echo  Here you can start the build for the XBOX source (with Team Complex's source patch). 
echo (Very limited features currently, WIP.. Suggestions are needed)
echo.
echo --------------------------------------------------------------------------------------------
echo  options) Modify Some Build Options.
echo --------------------------------------------------------------------------------------------
echo  1) Clean Build (Full err path, delete object files)
echo  2) 'Dirty' Build (Full err path, no checks)
echo  3) Build Specific Directory Only
echo  b/w) Open Build Error or Warning Logs
echo --------------------------------------------------------------------------------------------
echo  4) Binplace Kernel files       # 8) Build XDK (Needs help)
echo  5) Build XDK Samples CD        # 9) 'Retail' Recovery ISO
echo  6) Build HWT Recovery ISO      # 10) Attempt Bios Build (ADV.) 
echo  7) Place 'HVS Launcher'        # )
echo  r) Drop to Razzle Prompt       #
echo.
echo ____________________________________________________________________________________________
set /p NTMMENU=Select:
echo ____________________________________________________________________________________________
if /i "%NTMMENU%"=="1" goto cleanbuild
if /i "%NTMMENU%"=="2" goto DirtyBuild
REM Opens the most recent builds error logs in Notepad
if /i "%NTMMENU%"=="b" notepad %ebntroot%\private\build.err & goto eb-xbox-mainmenu
if /i "%NTMMENU%"=="w" notepad %ebntroot%\private\build.wrn & goto eb-xbox-mainmenu
if /i "%NTMMENU%"=="3" goto SpecificBLD
if /i "%NTMMENU%"=="4" goto postbuild-placeholder
if /i "%NTMMENU%"=="p" notepad %_NTPOSTBLD%\build_logs\postbuild.err & goto eb-xbox-mainmenu
if /i "%NTMMENU%"=="w" notepad %_NTPOSTBLD%\build_logs\postbuild.wrn & goto eb-xbox-mainmenu
if /i "%NTMMENU%"=="5" goto XDKSampleCD
if /i "%NTMMENU%"=="6" goto XBRecovery
if /i "%NTMMENU%"=="7" goto HVSLaunchtest
if /i "%NTMMENU%"=="8" goto SetupSDK
if /i "%NTMMENU%"=="9" goto RecoveryImage
if /i "%NTMMENU%"=="10" goto BuildBiosImage
if /i "%NTMMENU%"=="r" exit /b
if /i "%NTMMENU%"=="var" set && pause && goto eb-xbox-mainmenu
if /i "%NTMMENU%"=="options" goto BuildOptions
goto eb-xbox-mainmenu


:BuildInfo
echo still working this out..
pause
goto eb-xbox-mainmenu
:cleanbuild
cls
cd /d %ebntroot%\private
build -bcDeFZP
REM Rebuild private\ntos to ensure bios ROM files get built (mainly xpreldr.bin)
cd /d %ebntroot%\private\ntos
build -bcDeFZP
pause
goto eb-xbox-mainmenu
:DirtyBuild
cls
cd /d %ebntroot%\private
build -bDeFZP
REM Rebuild private\ntos to ensure bios ROM files get built (mainly xpreldr.bin)
cd /d %ebntroot%\private\ntos
build -bDeFZP
pause
goto eb-xbox-mainmenu

:HVSLaunchtest
CLS
cd /d %ebntroot%
echo.
echo This creates the appropriate directory structure for
echo the HVS Launcher and all of its tests.  This will allow these
echo programs to be compiled in to an Xbox disc image, or simply
echo copied over to an Xbox to be executed from the XDK Launcher.
echo.
echo Location: %ebxbbins%\xboxtest
echo.
pause
cmd /c %ebntroot%\private\test\hvs\createdirs.cmd | tee %_NT386TREE%\HVTestBinplace.log&& pause && goto eb-xbox-mainmenu

:RecoveryImage
cls
cd /d %ebntroot%
echo xupdrec (Originally updrec.cmd)
echo.
echo This will run a modified script to setup a 'Recovery' ISO
echo This may not be perfect, some files that were on a server share
echo have been directed to the built ones. dvdkey1d.bin didn't exist
echo but dvdkey1.bin did get build, so we use this for now
echo NOTE: It's likely this wont work, as some files may be different
echo from files originally copied (dvdkey1d.bin, dashboard.xbe from srv share
echo XBOXROM_QT.bin doesn't build either so it's been commented out.
echo.
echo Location: %ebxbbins%\Recovery.iso
echo.
pause
cmd /c xupdrec | tee %_NT386TREE%\xupdrec.log&& pause && goto eb-xbox-mainmenu


:SetupSDK
cls
echo XSDKBuild (Originally SDKBuild.cmd)
echo This will start a modified script to build the XSDK.
echo (A work in progress, need help with getting this working)
echo.
echo YOU NEED INSTALLSHIELD PROFESSIONAL 6.2 installed to:
echo "%programfiles%\InstallShield\InstallShield Professional 6.2\"
echo.
echo Currently if fails at finding "Language Independant Intel 32 Files"
echo when building the Setup.
echo A folder will be created at "%_NTDRIVE%\SDKScratch\"
pause
cmd /c %_NTDrive%%_NTROOT%\private\SDK\setup\xsdkbuild.bat | tee %_NT386TREE%\SDKBuild.log&& pause && goto eb-xbox-mainmenu

:BuildBiosImage
cls
echo This will try to use 'rombld' to build the Xbox BIOS image.
echo.
echo THIS WILL MOST LIKELY NOT BE BOOTABLE, I HAVE INCLUDED THIS JUST
echo AS A TEST AND EXAMPLE, TO EDIT WHAT SETTINGS ARE USED HERE, EDIT
echo 'EASYBUILD.CMD', GOTO LINE 135 TO CHANGE OPTIONS THERE.
echo.
echo This will be targeted as an XDK Xbox bios, retail 'XM3' Bioses fails
echo to build due to incorrect 'preloader' size currently.
echo.
echo NOTE: If xpreldr.bin is missing, clean build %_NTROOT%\private\ntos
echo.
echo From what I know built Bios roms from source aren't bootable, 
echo if this is bootable please let me know!
echo. 
pause
if not exist "%_NT386TREE%\boot\xboxbldr.bin" set ebromerror=xboxbldr.bin && goto RombldError
if not exist "%_NT386TREE%\boot\xpreldr.bin" set ebromerror=xpreldr.bin && goto RombldError
if not exist "%_NT386TREE%\xboxkrnl.exe" set ebromerror=xboxkrnl.exe && goto RombldError
if not exist "%_NT386TREE%\boot\inittbl_ret.bin" set ebromerror=inittbl_ret.bin && goto RombldError
if not exist "%_NT386TREE%\boot\romdec32.bin" set ebromerror=romdec32.bin && goto RombldError
if exist "%_NT386TREE%\xboxbios_xdk.bin" rename %_NT386TREE%\xboxbios_xdk.bin xboxbios_xdk_old.bin

rombld.exe /OUT:%_NT386TREE%\xboxbios_xdk.bin /BLDR:%_NT386TREE%\boot\xboxbldr.bin /PRELDR:%_NT386TREE%\boot\xpreldr.bin /KERNEL:%_NT386TREE%\xboxkrnl.exe /INITTBL:%_NT386TREE%\boot\inittbl_ret.bin /SYS:XDK /ROMDEC:%_NT386TREE%\boot\romdec32.bin | tee %_NT386TREE%\rombld.log
echo.
if NOT exist "%_NT386TREE%\xboxbios_xdk.bin" echo Failed!
if exist "%_NT386TREE%\xboxbios_xdk.bin" echo File created at "%_NT386TREE%\xboxbios_xdk.bin"
pause
goto eb-xbox-mainmenu

:RombldError
echo.
echo %ebromerror% is missing.. Rebuild NTOS
pause
goto eb-xbox-mainmenu


:SpecificBLD
cls
echo ----------------------------------------------------------------------
echo This section we can clean build certain components of the source
echo So if you want to rebuild the Kernel you would type:
echo.
echo F:\xbox\private\ntos
echo ----------------------------------------------------------------------
echo.
echo Type full path to folder or type back to return:
echo.
set /p userinput=:
if "%userinput%"=="back" goto eb-xbox-mainmenu
cd /d %userinput%
echo BUILD: %CD% STARTED
echo.
build -bcDeFZP
pause
goto eb-xbox-mainmenu

:postbuild-placeholder
cls
echo This is a 'Postbuild' script I made from 'copybins.cmd' and 'copytest.cmd' to
echo place files in %_NT386TREE% instead of an Xbox Dev Kit.. THIS IS NOT AN 'OFFICIAL' SCRIPT
echo This script needs your love! Know how to binplace something? Let me know!
echo.
echo xcopybins
pause
cmd /c xcopybins.cmd | tee %_NT386TREE%\xcopybins.log&& goto eb-xbox-mainmenu

:XDKSampleCD
cls 
echo.
echo Here the XDK Sample CD will be copied and built for Xboxes
echo These are only Xbox Direct Media Samples of:
echo Graphics, Sound and other stuff
echo.
echo The .iso will be placed in %_BINPLACE_ROOT%\XDKSamples%_BUILDVER%.iso
echo.
pause
cmd /c xmakesamples.cmd | tee %_NT386TREE%\xdksamples.log&& goto eb-xbox-mainmenu

:XBRecovery
cls
echo.
echo This will initiate building the Recovery iso.
echo This is still WIP and will need modifying to complete successfully
echo for now its ran to build what it can, users can look into the script:
echo "xbox\public\tools\hwtrec.cmd" 
echo If they wish to try apply a fix
echo.
pause
start hwtrec -all | tee %_NT386TREE%\hwtrec.log
echo Done you can find the output in %_NT386TREE%\rec_hwtest.iso
pause
goto eb-xbox-mainmenu

:BuildOptions

REM Over time I will add more features, first I need to find what to change and how
REM Suggestions and improvements greatly welcomed here.
REM
REM
if "%COMPLEX%" == "1" set ebcomplex=On
if "%COMPLEX%" == "" set ebcomplex=Off
if "%FOCUS%" == "1" set ebfocus=On
if "%FOCUS%" == "" set ebfocus=Off
if "%NODEVKIT%" == "1" set ebdevkit=Retail
if "%NODEVKIT%" == "" set ebdevkit=DevKit
cls
echo ----------------------------------------------------------------------
echo This section we can use to modify various parts of the build process
echo for example changing Release type, Build type etc
echo To switch, type the option after the Value:
echo ----------------------------------------------------------------------
echo.
echo (Current:%ebdevkit%) Release: devkit - retail
echo (Current:%ebxbtype%) Build Type: fre - chk
echo (Current:%ebcomplex%) Complex Patches: con - coff
echo (Current:%ebfocus%) Focus Video Tuner: fon - foff
echo.
echo I will slowly add more here over time
echo ----------------------------------------------------------------------
echo back) Return
echo.
set /p bldopt=:
if /i "%bldopt%"=="devkit" set NODEVKIT=""
if /i "%bldopt%"=="retail" set NODEVKIT="1"
if /i "%bldopt%"=="fre" SETFRE && goto BuildOptions
if /i "%bldopt%"=="chk" SETCHK && goto BuildOptions
if /i "%bldopt%"=="con" CPXON && goto BuildOptions
if /i "%bldopt%"=="coff CPXOFF && goto BuildOptions
if /i "%bldopt%"=="fon" FOCUSON && goto BuildOptions
if /i "%bldopt%"=="foff" FOCUSOFF && goto BuildOptions
if /i "%bldopt%"=="showopt" showopt && goto BuildOptions
if /i "%bldopt%"=="back" goto eb-xbox-mainmenu
goto BuildOptions

REM
