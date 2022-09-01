; CueP.Pri
;
; Alias definitions.
;
; If this file has been renamed as CueP.pri, it may be modified at will.
; UpdateSelf.cmd will not overwrite it.
;

D       Dir $*
DI      Dir $*
DD      Dir /ad $*
DA      Dir a:$*
DB      Dir b:$*
DGen    Dir /ogen $*
Dod     Dir /o-dne $*

...     cd ..\..\$*
....    cd ..\..\..\$*
Up      cd ..\$*
Home    %SystemDrive% $T cd \$*
Root    cd \$*

Co      Copy /v $*
Del     Del $* /p

T       Type $*
Ty      Type $* $B more
Typ     Type $*

NRO     attrib -r $*
RO      attrib +r $*
A       attrib $*

bc      Build /c $*

SS      "%VStudioDir%\Vss\Win32\ss.exe" $*
