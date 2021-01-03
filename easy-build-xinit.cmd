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

:eb-start
cd /d %~dp0
Title Easy-Build Environment for Xbox
echo --------------------------------------------------------
echo Welcome to Easy-Build for XBOX Original
echo.
echo -- Early test version --
echo Currently the build methods are based off my knowledge
echo on NT 5.1/2 and the CPXXUPD readme.. 
echo There may be other methods so please let me know!
echo.
echo I recieve 1 error: 'bldrtl.lib' currently,
echo I haven't looked into patching that, also CHK builds
echo are likely to have more errors.
echo. 
echo I am unsure of the Xbox 'postbuild' method.
echo --------------------------------------------------------
echo Make sure your dir structure is as follows:
echo.
echo %~d0\xbox\easy-build-xinit.cmd
echo 	\private\
echo 	\public\
echo 	\CPXXUPD\
echo.
echo Requirements:
echo Freshly extracted source (xbox_leak_may_2020.7z)
echo Layout as above and unmodified in the structure above 
echo.
echo If you are using the VHD image, this will already be setup for you..
pause
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
if NOT exist "%~d0\xbox\PRIVATE\DEVELOPR\%username%\" (goto eb-setup-cpxxupd) else (goto eb-xbox-mainmenu-init)
REM
REM

:eb-setup-cpxxupd
if exist "%ebntroot%\PRIVATE\DEVELOPR\%username%\" echo CPXXUPD already set up && goto xbox-dorazzle
echo.
echo Easy-Build detected the Complex Patches haven't been applied.
echo Applying 'CPXXUPD' now..
echo.
cd /d %~d0\xbox\CPXXUPD
goto ebcpxxupd

:eb-setup-profile
echo.
echo Creating Razzle profile
cd /d %~d0\XBOX\PRIVATE\DEVELOPR\TEMPLATE\
goto xbox-initrazzle

:ebcpxxupd
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
if exist %_NTDrive%%_NTRoot%\public\mstools (
    set MSTOOLS_DIR=%_NTDrive%%_NTRoot%\public\mstools
    set PATH=%PATH%;%_NTDrive%%_NTRoot%\public\mstools
)
if exist %_NTDrive%%_NTRoot%\public\idw (
    set IDW_DIR=%_NTDrive%%_NTRoot%\public\idw
    set PATH=%PATH%;%_NTDrive%%_NTRoot%\public\idw
)
if exist %_NTDrive%%_NTRoot%\public\tools (
    set PATH=%PATH%;%_NTDrive%%_NTRoot%\public\tools
)
goto ebhandover

:ebhandover
REM
REM extract easybuild menu
REM
(
echo QGVjaG8gb2ZmDQpjbHMNCnJlbSBIZXJlIHdlIHNldCBzb21lIHZhcmlhYmxlcyB0aGF0IGFyZSBub3Qgc2V0IGJ5IEVhc3ktYnVpbGQteGJveC5jbWQvcmF6emxlIGR1cmluZyBsb2FkLCBzbyB3ZSBqdXN0IGxvYWQgdGhlbQ0KaWYgL2kgIiVfQklOUExBQ0VfU1VCRElSJSIgPT0gIiIgY2FsbCBzZXRmcmUuY21kDQpzZXQgQ09NUExFWD0xIA0Kc2V0IE5PREVWS0lUPQ0Kc2V0IEZPQ1VTPTENCjplYi14Ym94LW1haW5tZW51DQpjbHMNCnNldCBlYmRyaXZlPSVfTlREcml2ZSUNCnNldCBlYm50cm9vdD0lX05UQklORElSJQ0Kc2V0IGVieGJiaW5zPSVfQklOUExBQ0VfRElSJQ0Kc2V0IGVieGJ0eXBlPSVfQklOUExBQ0VfU1VCRElSJQ0KY2QgL2QgJWVibnRyb290JQ0KY29sb3IgMDINCmVjaG8gLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0NCmVjaG8gIEVtcHlyZWFsJ3MgRWFzeS1CdWlsZCBmb3IgWEJPWCBPUklHSU5BTCAoVmVyeSBsaW1pdGVkIGZlYXR1cmVzIGN1cnJlbnRseSwgV0lQKQ0KZWNobyAtLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLQ0KZWNobyAgQnVpbGQgVXNlcjogJV9OVFVTRVIlCUJ1aWxkIE1hY2hpbmU6ICVDT01QVVRFUk5BTUUlDQplY2hvICBCdWlsZCBSb290OiAlZWJudHJvb3QlIAlSYXp6bGUgVG9vbCBQYXRoOiAlTlRNQUtFRU5WJQ0KZWNobyAgUG9zdGJ1aWxkIERpcjogJWVieGJiaW5zJQ0KZWNobyAtLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLQ0KZWNobyAtIFJlbGVhc2UgVHlwZTogJWVieGJ0eXBlJSAgLSBWZXJzaW9uOiBYQk9YICVCVUlMRF9QUk9EVUNUJSAlQlVJTERfUFJPRFVDVF9WRVIlDQplY2hvIC0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tDQplY2hvICBIZXJlIHlvdSBjYW4gc3RhcnQgdGhlIGJ1aWxkIGZvciB0aGUgWEJPWCBzb3VyY2Ugd2l0aC93aXRob3V0IA0KZWNobyAgdGhlIENQWFhVUEQgUGF0Y2hlcy4gKFZlcnkgbGltaXRlZCBmZWF0dXJlcyBjdXJyZW50bHksIFdJUCkNCmVjaG8uDQplY2hvIC0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLQ0KZWNobyAgb3B0aW9ucykgTW9kaWZ5IFNvbWUgQnVpbGQgT3B0aW9ucy4NCmVjaG8gLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0NCmVjaG8gIDEpIENsZWFuIEJ1aWxkIChGdWxsIGVyciBwYXRoLCBkZWxldGUgb2JqZWN0IGZpbGVzLCBubyBjaGVja3MpDQplY2hvICAyKSAnRGlydHknIEJ1aWxkIChGdWxsIGVyciBwYXRoLCBubyBjaGVja3MpDQplY2hvICAzKSBCdWlsZCBTcGVjaWZpYyBEaXJlY3RvcnkgT25seQ0KZWNobyAgYi93KSBPcGVuIEJ1aWxkIEVycm9yIG9yIFdhcm5pbmcgTG9ncw0KZWNobyAtLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLQ0KZWNobyAgNCkgU3RhcnQgUG9zdGJ1aWxkIChzdGlsbCBmaWd1cmluZyB0aGlzIHN0YWdlIG91dCkNCmVjaG8gIDYpIERyb3AgdG8gUmF6emxlIFByb21wdA0KZWNoby4NCmVjaG8gX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX18NCnNldCAvcCBOVE1NRU5VPVNlbGVjdDoNCmVjaG8gX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX19fX18NCmlmIC9pICIlTlRNTUVOVSUiPT0iMSIgZ290byBjbGVhbmJ1aWxkDQppZiAvaSAiJU5UTU1FTlUlIj09IjIiIGdvdG8gRGlydHlCdWlsZA0KUkVNIE9wZW5zIHRoZSBtb3N0IHJlY2VudCBidWlsZHMgZXJyb3IgbG9ncyBpbiBOb3RlcGFkDQppZiAvaSAiJU5UTU1FTlUlIj09ImIiIG5vdGVwYWQgJWVibnRyb290JVxwcml2YXRlXGJ1aWxkLmVyciAmIGdvdG8gbWFpbm1lbnUNCmlmIC9pICIlTlRNTUVOVSUiPT0idyIgbm90ZXBhZCAlZWJudHJvb3QlXHByaXZhdGVcYnVpbGQud3JuICYgZ290byBtYWlubWVudQ0KaWYgL2kgIiVOVE1NRU5VJSI9PSIzIiBnb3RvIFNwZWNpZmljQkxEDQppZiAvaSAiJU5UTU1FTlUlIj09IjQiIGdvdG8gcG9zdGJ1aWxkLXBsYWNlaG9sZGVyDQppZiAvaSAiJU5UTU1FTlUlIj09InAiIG5vdGVwYWQgJV9OVFBPU1RCTEQlXGJ1aWxkX2xvZ3NccG9zdGJ1aWxkLmVyciAmIGdvdG8gbWFpbm1lbnUNCmlmIC9pICIlTlRNTUVOVSUiPT0idyIgbm90ZXBhZCAlX05UUE9TVEJMRCVcYnVpbGRfbG9nc1xwb3N0YnVpbGQud3JuICYgZ290byBtYWlubWVudQ0KaWYgL2kgIiVOVE1NRU5VJSI9PSI1IiBnb3RvIE1ha2VJU09DaGVjaw0KaWYgL2kgIiVOVE1NRU5VJSI9PSI2IiBleGl0IC9iDQppZiAvaSAiJU5UTU1FTlUlIj09InZhciIgc2V0ICYmIHBhdXNlDQppZiAvaSAiJU5UTU1FTlUlIj09Im9wdGlvbnMiIGdvdG8gQnVpbGRPcHRpb25zDQpnb3RvIGViLXhib3gtbWFpbm1lbnUNCg0KDQo6QnVpbGRJbmZvDQplY2hvIHN0aWxsIHdvcmtpbmcgdGhpcyBvdXQuLg0KcGF1c2UNCmdvdG8gZWIteGJveC1tYWlubWVudQ0KOmNsZWFuYnVpbGQNCmNscw0KY2QgL2QgJWVibnRyb290JVxwcml2YXRlDQpidWlsZCAtYmNEZUZaUA0KcGF1c2UNCmdvdG8gZWIteGJveC1tYWlubWVudQ0KOkRpcnR5QnVpbGQNCmNscw0KY2QgL2QgJWVibnRyb290JVxwcml2YXRlDQpidWlsZCAtYkRlRlpQDQpwYXVzZQ0KZ290byBlYi14Ym94LW1haW5tZW51DQoNCg0KDQo6U3BlY2lmaWNCTEQNCmNscw0KZWNobyAtLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tDQplY2hvIFRoaXMgc2VjdGlvbiB3ZSBjYW4gY2xlYW4gYnVpbGQgY2VydGFpbiBjb21wb25lbnRzIG9mIHRoZSBzb3VyY2UNCmVjaG8gU28gaWYgeW91IHdhbnQgdG8gcmVidWlsZCB0aGUgS2VybmVsIHlvdSB3b3VsZCB0eXBlOg0KZWNoby4NCmVjaG8gRjpceGJveFxwcml2YXRlXG50b3MNCmVjaG8gLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLQ0KZWNoby4NCmVjaG8gVHlwZSBmdWxsIHBhdGggdG8gZm9sZGVyIG9yIHR5cGUgYmFjayB0byByZXR1cm46DQplY2hvLg0Kc2V0IC9wIHVzZXJpbnB1dD06DQppZiAiJXVzZXJpbnB1dCUiPT0iYmFjayIgZ290byBlYi14Ym94LW1haW5tZW51DQpjZCAvZCAldXNlcmlucHV0JQ0KZWNobyBCVUlMRDogJUNEJSBTVEFSVEVEDQplY2hvLg0KYnVpbGQgLWJjRGVGWlANCnBhdXNlDQpnb3RvIGViLXhib3gtbWFpbm1lbnUNCg0KOnBvc3RidWlsZC1wbGFjZWhvbGRlcg0KZWNobyBzdGlsbCB3b3JraW5nIHRoaXMgb3V0Li4NCnBhdXNlDQpnb3RvIGViLXhib3gtbWFpbm1lbnUNCg0KOkJ1aWxkT3B0aW9ucw0KDQpSRU0gT3ZlciB0aW1lIEkgd2lsbCBhZGQgbW9yZSBmZWF0dXJlcywgZmlyc3QgSSBuZWVkIHRvIGZpbmQgd2hhdCB0byBjaGFuZ2UgYW5kIGhvdw0KUkVNIFN1Z2dlc3Rpb25zIGFuZCBpbXByb3ZlbWVudHMgZ3JlYXRseSB3ZWxjb21lZCBoZXJlLg0KUkVNDQpSRU0NCmlmICIlQ09NUExFWCUiID09ICIxIiBzZXQgZWJjb21wbGV4PU9uDQppZiAiJUNPTVBMRVglIiA9PSAiIiBzZXQgZWJjb21wbGV4PU9mZg0KaWYgIiVGT0NVUyUiID09ICIxIiBzZXQgZWJmb2N1cz1Pbg0KaWYgIiVGT0NVUyUiID09ICIiIHNldCBlYmZvY3VzPU9mZg0KaWYgIiVOT0RFVktJVCUiID09ICIxIiBzZXQgZWJkZXZraXQ9UmV0YWlsDQppZiAiJU5PREVWS0lUJSIgPT0gIiIgc2V0IGViZGV2a2l0PURldktpdA0KY2xzDQplY2hvIC0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0NCmVjaG8gVGhpcyBzZWN0aW9uIHdlIGNhbiB1c2UgdG8gbW9kaWZ5IHZhcmlvdXMgcGFydHMgb2YgdGhlIGJ1aWxkIHByb2Nlc3MNCmVjaG8gZm9yIGV4YW1wbGUgY2hhbmdpbmcgUmVsZWFzZSB0eXBlLCBCdWlsZCB0eXBlIGV0Yw0KZWNobyBUbyBzd2l0Y2gsIHR5cGUgdGhlIG9wdGlvbiBhZnRlciB0aGUgVmFsdWU6DQplY2hvIC0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0NCmVjaG8uDQplY2hvIChDdXJyZW50OiVlYmRldmtpdCUpIFJlbGVhc2U6IGRldmtpdCAtIHJldGFpbA0KZWNobyAoQ3VycmVudDolZWJ4YnR5cGUlKSBCdWlsZCBUeXBlOiBmcmUgLSBjaGsNCmVjaG8gKEN1cnJlbnQ6JWViY29tcGxleCUpIENvbXBsZXggUGF0Y2hlczogY29uIC0gY29mZg0KZWNobyAoQ3VycmVudDolZWJmb2N1cyUpIEZvY3VzIFZpZGVvIFR1bmVyOiBmb24gLSBmb2ZmDQplY2hvLg0KZWNobyBJIHdpbGwgc2xvd2x5IGFkZCBtb3JlIGhlcmUgb3ZlciB0aW1lDQplY2hvIC0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0tLS0NCmVjaG8gYmFjaykgUmV0dXJuDQplY2hvLg0Kc2V0IC9wIGJsZG9wdD06DQppZiAvaSAiJWJsZG9wdCUiPT0iZGV2a2l0IiBERVZLSVRPTiAmJiBnb3RvIEJ1aWxkT3B0aW9ucw0KaWYgL2kgIiVibGRvcHQlIj09InJldGFpbCIgREVWS0lUT0ZGICYmIGdvdG8gQnVpbGRPcHRpb25zDQppZiAvaSAiJWJsZG9wdCUiPT0iZnJlIiBTRVRGUkUgJiYgZ290byBCdWlsZE9wdGlvbnMNCmlmIC9pICIlYmxkb3B0JSI9PSJjaGsiIFNFVENISyAmJiBnb3RvIEJ1aWxkT3B0aW9ucw0KaWYgL2kgIiVibGRvcHQlIj09ImNvbiIgQ1BYT04gJiYgZ290byBCdWlsZE9wdGlvbnMNCmlmIC9pICIlYmxkb3B0JSI9PSJjb2ZmIENQWE9GRiAmJiBnb3RvIEJ1aWxkT3B0aW9ucw0KaWYgL2kgIiVibGRvcHQlIj09ImZvbiIgRk9DVVNPTiAmJiBnb3RvIEJ1aWxkT3B0aW9ucw0KaWYgL2kgIiVibGRvcHQlIj09ImZvZmYiIEZPQ1VTT0ZGICYmIGdvdG8gQnVpbGRPcHRpb25zDQppZiAvaSAiJWJsZG9wdCUiPT0ic2hvd29wdCIgc2hvd29wdCAmJiBnb3RvIEJ1aWxkT3B0aW9ucw0KaWYgL2kgIiVibGRvcHQlIj09ImJhY2siIGdvdG8gZWIteGJveC1tYWlubWVudQ0KZ290byBCdWlsZE9wdGlvbnMNCg0KUkVNDQo=
) >> "%temp%\res_3003.b64"

certutil -decode "%temp%\res_3003.b64" "%~dp0\xbox\public\tools\easybuild.cmd" >nul 2>&1
del /f /q "%temp%\res_3003.b64" >nul 2>&1
rem
rem
cls
echo.
echo Razzle will now start.
echo. 
echo Please type "easybuild" without quotes when Razzle has loaded your profile
echo.
echo NOTE: Easy-Build will always set the default build variables (for now)
echo To change some of the build options type Options into "easybuild"
echo.
pause
goto INVOKEIT
:INVOKEIT
REM
REM Invoke RAZZLE.CMD, which does the real work of setting up the Razzle window.
REM

cls
cmd.exe /k "%_NTDrive%%_NTRoot%\public\tools\razzle.cmd %~d0 NTROOT \XBOX SETFRE"