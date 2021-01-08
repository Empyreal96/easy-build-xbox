rem @Echo Off
Echo SDK Build Started at %Date% %Time%

REM If Not x%_BuildVer%x == xx Goto SkipUsage
REM This is needed to tell the Env that this machine can buikd the 'release' SDK
set ComputerName=XBuilds
goto SkipUsage
:Usage
REM Microsoft's purpose of the script
Echo On the XBuilds machine this batch file copies the relevant source trees to 
Echo D:\SDKScratch (deleting the destination directory first so that 
Echo if a file is deleted it will show up missing in the build) and then 
Echo builds the SDK. After building the SDK it creates an ISO-1660 image of the CD.
Echo .
Echo On other machines this batch file does not copy all of the files for the
Echo source tree. Normally the S: drive is mapped to the %_NTDrive%%_NTRoot%\Private directory
Echo This means that the files are mostly read-only so the resulting SDK build can't be
Echo shipped, but the build is faster
Echo .
Echo In addition the P: drive needs to be mapped to the relevant directory on \\XBuilds\Release
Echo or \\XBuilds\PvtRelease (e.g. \\XBuilds\Release\USA\3424_Apr01)

Goto :EOF

:SkipUsage

If /i NOT %ComputerName% == XBuilds Goto NotBuildMachine

Set _BuildMachine=1

:: Set up %_NTDrive%\SDKScratch

:: Remove SDKScratch trees so missing files in the source depot tree show up as missing in the SDK build

Rd /s /q %_NTDrive%\SDKScratch\Src\ATG
Rd /s /q %_NTDrive%\SDKScratch\Src\UE
Rd /s /q %_NTDrive%\SDKScratch\Src\External
Rd /s /q %_NTDrive%\SDKScratch\Src\SDK
Rd /s /q %_NTDrive%\SDKScratch\Src\Windows

Md %_NTDrive%\SDKScratch > nul 2>&1

:: S: is a copy of the relevant source trees (to remove the r/o bit)

XCopy /cdefiy %_NTDrive%%_NTRoot%\Private\UE\SDK %_NTDrive%\SDKScratch\Src\UE\SDK
XCopy /cdefiy %_NTDrive%%_NTRoot%\Private\UE\Pxdk %_NTDrive%\SDKScratch\Src\UE\Pxdk
XCopy /cdefiy %_NTDrive%%_NTRoot%\Private\SDK %_NTDrive%\SDKScratch\Src\SDK
XCopy /cdefiy %_NTDrive%%_NTRoot%\Private\External %_NTDrive%\SDKScratch\Src\External
XCopy /cdefiy %_NTDrive%%_NTRoot%\Private\ATG %_NTDrive%\SDKScratch\Src\ATG
XCopy /cdefiy %_NTDrive%%_NTRoot%\Private\Windows\DirectX\DXG\XGraphics %_NTDrive%\SDKScratch\Src\Windows\DirectX\DXG\XGraphics
XCopy /cdefiy %_NTDrive%%_NTRoot%\Private\Windows\DirectX\DXG\Tools %_NTDrive%\SDKScratch\Src\Windows\DirectX\DXG\Tools

REM Premoved these to prevent the 'phone home' to the Xbuilds source depot
rem Net Use S: /d
rem Net Use S: \\XBuilds\D$\SDKScratch\Src /p:y
set Sdrive=%_NTDRIVE%\SDKScratch\Src

:: P: is a copy of the build share (\\XBuilds\PvtRelease\USA\<BldNum>)

rem Net Use P: /d
rem Net Use P: \\XBuilds\PvtRelease\USA\%_BuildVer% /p:y
if not exist "%_NTDRIVE%\PvtRelease\" mkdir %_NTDRIVE%\PvtRelease\
if not exist "%_NTDRIVE%\PvtRelease\USA\" mkdir %_NTDRIVE%\PvtRelease\USA
if not exist "%_NTDRIVE%\PvtRelease\USA\%_BuildVer%" mkdir %_NTDRIVE%\PvtRelease\USA\%_BuildVer%
set Pdrive=%_NTDRIVE%\PvtRelease\USA\%_BuildVer%

Echo SDK scratch tree completed at %Date% %Time%

Set Builder=%ProgramFiles%\InstallShield\InstallShield Professional 6.2\Program\ISBuild.Exe
Set IncludeIFX=%ProgramFiles%\InstallShield\InstallShield Professional 6.2\Script\Ifx\Include
Set IncludeIsRt=%ProgramFiles%\InstallShield\InstallShield Professional 6.2\Script\IsRt\Include
Set LinkPaths=-LibPath"%ProgramFiles%\InstallShield\InstallShield Professional 6.2\Script\Ifx\Lib" -LibPath"%ProgramFiles%\InstallShield\InstallShield Professional 6.2\Script\IsRt\Lib"

Goto Build

:NotBuildMachine

Set _BuildMachine=

Set Builder=%ProgramFiles%\InstallShield\InstallShield Professional 6.2\Program\ISBuild.Exe
Set IncludeIFX=%ProgramFiles%\InstallShield\InstallShield Professional 6.2\Script\Ifx\Include
Set IncludeIsRt=%ProgramFiles%\InstallShield\InstallShield Professional 6.2\Script\IsRt\Include
Set LinkPaths=-LibPath"%ProgramFiles%\InstallShield\InstallShield Professional 6.2\Script\Ifx\Lib" -LibPath"%ProgramFiles%\InstallShield\InstallShield Professional 6.2\Script\IsRt\Lib"

:: If you're not on the build machine, it's up to you to properly set up the drive letters.
set Pdrive=P:
set Sdrive=S:

:: The SDK is built from SDKScratch so at least SDKScratch\Src\SDK needs to exist.

XCopy /cdefiy %_NTDrive%%_NTRoot%\Private\SDK %_NTDrive%\SDKScratch\Src\SDK

Echo SDK scratch tree completed at %Date% %Time%


:Build

:: Check to see if a few expected files are in place; if not error out.
REM Redirected these to existing files in our source tree
If Not Exist %_NTDrive%%_NTRoot%\Public\XDK\Inc\XTL.h                              Goto Usage
If Not Exist %_NTDrive%%_NTRoot%\Private\ATG\Samples\Graphics\DolphinClassic\DolphinClassic.Cpp Goto Usage
If Not Exist %_NTDrive%%_NTRoot%\Private\SDK\Update\Update.Ipr                                  Goto Usage
If Not Exist %_NTDrive%%_NTRoot%\Private\UE\SDK\RelNotes\ReadMe1st.Txt                          Goto Usage

:: ===============================================
:: Set up tools.
:: ===============================================

Set Compiler=%ProgramFiles%\Common Files\InstallShield\IScript\Compile.Exe

:: ===============================================
:: Set the environment for the compiler
:: ===============================================

Set Libraries="IsRt.Obl" "Ifx.Obl"
Set Definitions=
Set Switches=-w50 -e50 -v3 -g

:: The SDK needs to get the build number of the Xbox bits to compare.
:: IS can't deal with the (mininal) preprocessor stuff in XboxVerP, so strip it down to just #defines.

FindStr VER_PRODUCTBUILD "%_NTDrive%%_NTRoot%\Private\Inc\XboxVerP.h" | findstr "#define" > "%IncludeIsRT%\XboxVerP.h"

PushD %_NTDrive%\SDKScratch\Src\SDK\Update

:: Copy these here as single-file builds won't get this in the usual place (since it's packed in the cab)
REM Redirected this to existing file
XCopy /i  %_NTDrive%%_NTRoot%\Private\UE\SDK\RelNotes\ReadMe1st*.Txt "Setup Files\Uncompressed Files\Language Independent\OS Independent"

Set InstallProject=%_NTDrive%\SDKScratch\Src\SDK\Update\Update.Ipr
Set CurrentBuild=CD
Set IncludeScript=%_NTDrive%\SDKScratch\Src\SDK\Update\Script Files
Set RulFiles=%_NTDrive%\SDKScratch\Src\SDK\Update\Script Files\Setup.Rul

:: Remove old media directory so each build is truly a clean build

rd /s /q "Media\CD\Disk Images\Disk1"

:: ==================================================
:: Compile
:: ==================================================
"%Compiler%" "%RulFiles%" -I"%IncludeIFX%" -I"%IncludeIsRt%" -I"%IncludeScript%" %LinkPaths% %Libraries% %Definitions% %Switches%

If ErrorLevel 1 Goto InstallShieldCompileError

:: Prep for building CD. Nothing is compressed. CD Image is part of build

Ini -f ".\File Groups\Default.Fdf" "C Sample Source.Compress" = No
Ini -f ".\File Groups\Default.Fdf" "DX Docs (DX8).Compress" = No
Ini -f ".\File Groups\Default.Fdf" "Header Files.Compress" = No
Ini -f ".\File Groups\Default.Fdf" "Libraries.Compress" = No
Ini -f ".\File Groups\Default.Fdf" "VC Add-Ins.Compress" = No
Ini -f ".\File Groups\Default.Fdf" "Xbox Header Files.Compress" = No
Ini -f ".\File Groups\Default.Fdf" "Xbox Libraries.Compress" = No
Ini -f ".\File Groups\Default.Fdf" "SDK CD Image.Compress" = No
Ini -f ".\File Groups\Default.Fdf" "Misc.Compress" = No
Ini -f ".\File Groups\Default.Fdf" "Xbox C Compile Environment.Compress" = No

Ini -f ".\Component Definitions\Default.Cdf" "DX CD Image.IncludeInBuild" = Yes

:: ==================================================
:: Build
:: ==================================================
"%Builder%" -p"%InstallProject%" -m"%CurrentBuild%"
Goto BuildComplete

:InstallShieldCompileError
Echo ERROR: SDK Build did not complete due to compiler error in the script.
Goto :END

:BuildComplete
Echo SDK InstallSHIELD build Completed at %Date% %Time%

::Copy the SDK to the PvtRelease share
REM Adjusted where the output goes to to help consolodate files on one drive
if not exist "%_NTROOT%\pvtrelease" mkdir %_NTROOT%\pvtrelease
If x1x == x%_BuildMachine%x Xcopy /cdefiy "%_NTDrive%\SDKScratch\Src\SDK\Update\Media\CD\Disk Images\Disk1" %_NTROOT%\pvtrelease

:: Now to build the CD Image

:: Assume CDImage is on the path,

If x1x == x%_BuildMachine%x CDImage -lXbox052001 -n -d -c -x -ocis "%_NTDrive%\SDKScratch\Src\SDK\Update\Media\CD\Disk Images\Disk1" %_NT386TREE%\XboxSDK.Iso

:: Build the SDK Updater

Set InstallProject=%_NTDrive%\SDKScratch\Src\SDK\Update\Update.Ipr
Set CurrentBuild=XDK
Set IncludeScript=%_NTDrive%\SDKScratch\Src\SDK\Update\Script Files
Set RulFiles=%_NTDrive%\SDKScratch\Src\SDK\Update\Script Files\Setup.Rul


:: ==================================================
:: Compile
:: ==================================================
"%Compiler%" "%RulFiles%" -I"%IncludeIFX%" -I"%IncludeIsRt%" -I"%IncludeScript%" %LinkPaths% %Libraries% %Definitions% %Switches%

If ErrorLevel 1 Goto UpdateInstallShieldCompileError

:: Prep for building download. Everything is compressed. CD Image is not part of build

Ini -f ".\File Groups\Default.Fdf" "C Sample Source.Compress" = Yes
Ini -f ".\File Groups\Default.Fdf" "DX Docs (DX8).Compress" = Yes
Ini -f ".\File Groups\Default.Fdf" "Header Files.Compress" = Yes
Ini -f ".\File Groups\Default.Fdf" "Libraries.Compress" = Yes
Ini -f ".\File Groups\Default.Fdf" "VC Add-Ins.Compress" = Yes
Ini -f ".\File Groups\Default.Fdf" "Xbox Header Files.Compress" = Yes
Ini -f ".\File Groups\Default.Fdf" "Xbox Libraries.Compress" = Yes
Ini -f ".\File Groups\Default.Fdf" "SDK CD Image.Compress" = Yes
Ini -f ".\File Groups\Default.Fdf" "Misc.Compress" = Yes
Ini -f ".\File Groups\Default.Fdf" "Xbox C Compile Environment.Compress" = Yes

Ini -f ".\Component Definitions\Default.Cdf" "DX CD Image.IncludeInBuild" = No

:: ==================================================
:: Build
:: ==================================================
"%Builder%" -p"%InstallProject%" -m"%CurrentBuild%"
Goto UpdateBuildComplete

:UpdateInstallShieldCompileError
Echo ERROR: SDK Build did not complete due to compiler error in the script.
Goto :END

:UpdateBuildComplete
REM changed dir to xbox\xboxbins\fre
If x1x == x%_BuildMachine%x Copy "%_NTDrive%\SDKScratch\Src\SDK\Update\Media\XDK\Disk Images\Disk1\Update.Exe" "%_NT386TREE%\XboxSDKUpdate%_BuildVer%.Exe"

:END

PopD

Echo Echo SDK Build Completed at %Date% %Time%
