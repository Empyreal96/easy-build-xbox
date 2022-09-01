;
; DON'T EDIT THIS FILE
;
; Cue.Pri
;
; Alias definitions.
;

SetFind	   Set $B FindStr /i $*
NetFind	   Net View $B FindStr /i $*
IncFind	   FindStr /r /i $* "%_NTBinDir%\public\sdk\inc"
WhereInc   where /r "%_NTBinDir%\public\sdk\inc" $*
WhereLib   where /r "%_NTBinDir%\public\sdk\lib" $*

MSDev      "%VStudioDir%\%VSMSDevDir%\bin\msdev.exe" $*
VSS        "%VStudioDir%\Vss\Win32\ssexp.exe" $*
SSExp      "%VStudioDir%\Vss\Win32\ssexp.exe" $*

DevStudio  cd /d %VStudioDir%\$*
DevStud    cd /d %VStudioDir%\$*
VStudio    cd /d %VStudioDir%\$*
VStud      cd /d %VStudioDir%\$*
VC         cd /d %VStudioDir%\%VCDir%\$*
VCBin      cd /d %VStudioDir%\%VCDir%\bin\$*
SharedIDE  cd /d %VStudioDir%\%VSCommonDir%\$*
VSCommon   cd /d %VStudioDir%\%VSCommonDir%\$*
VSTemplate cd /d %VSTemplateDir%\$*

NtBuilds   cd /d %_BINPLACE_ROOT%\$*
NtBld      cd /d %_BINPLACE_ROOT%\$*
NtBldChk   cd /d %_BINPLACE_ROOT%\chk\$*
NtBldFre   cd /d %_BINPLACE_ROOT%\fre\$*

; Complex Aliases

CpxOn      set COMPLEX=1
CpxOff     set COMPLEX
FocusOn    set FOCUS=1
FocusOff   set FOCUS=
DevKitOn   set NODEVKIT=
DevkitOff  set NODEVKIT=1
