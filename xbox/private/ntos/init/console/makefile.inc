obj\$(TARGET_DIRECTORY)\$(NTTEST).def: ..\ntoskrnl.src
    copy ..\ntoskrnl.src obj\$(TARGET_DIRECTORY)\$(NTTEST).pp
    $(TARGET_CPP) -nologo -EP $(TARGET_DEFINES) $(TARGET_DBG_DEFINES) $(WIN32_DEFINE) $(C_DEFINES) obj\$(TARGET_DIRECTORY)\$(NTTEST).pp > obj\$(TARGET_DIRECTORY)\$(NTTEST).def
    -del obj\$(TARGET_DIRECTORY)\$(NTTEST).pp

$(BASEDIR)\public\sdk\lib\$(TARGET_DIRECTORY)\xboxkrnl.lib : $(O)\xboxkrnl.lib
    copy $** $@

$(O)\$(NTTEST).lib $(O)\$(NTTEST).exp: obj\$(TARGET_DIRECTORY)\$(NTTEST).def $(O)\bldnum.obj \
        $(KERNEL_LIBS)
    -lib @<<
$(LINK_LIB_IGNORE_FLAG)
-def:obj\$(TARGET_DIRECTORY)\$(NTTEST).def
-debugtype:cv
-out:$(@R).lib
-machine:$(MACHINE_TYPE)
$(KERNEL_LIBS)
<<NOKEEP
    -lib -out:$(@R).lib @<<
$(@R).lib
$(O)\bldnum.obj
<<NOKEEP

..\init.c: $(BASEDIR)\private\inc\xboxverp.h

{}bldnum.c{$O\}bldnum.obj:
    @type <<$(ECHO_RSP)
$(ECHO_MSG)
<<NOKEEP
    @$(C_COMPILER_NAME) @<<$(CL_RSP) -Fo"$(MAKEDIR)\$O/" $(USE_FC) $(USECXX_FLAG) $<
$(EX_C_COMPILER_FLAGS)
<<NOKEEP

$(XDK_LIB_PATH)\xboxkrnl.lib: $(SDK_LIB_PATH)\xboxkrnl.lib
    copy $** $@
    
!IF DEFINED(RETAILXM3)
ROMBLDSYS=xm3
!ELSE
ROMBLDSYS=xdk
!ENDIF

obj\i386\xboxrom_dvt4.bin: obj\i386\xboxkrnl.exe $(TARGETPATH2)\i386\inittbl_dvt4.bin $(TARGETPATH2)\i386\xboxbldr.bin $(TARGETPATH2)\i386\romdec32.bin
    copy $(_NTDRIVE)\xbox\private\ntos\init\console\obj\i386\xboxkrnl.exe $(IDW_DIR)\biospack\boot\xboxkrnl.img /Y /V
	biospack -t multi -i $(IDW_DIR)\biospack\boot\ -o $(_NTDRIVE)\xbox\private\ntos\init\console\obj\i386\xboxrom_dvt4.bin
!IF !DEFINED(NODEVKIT)
    biospack -t multi -i $(IDW_DIR)\biospack\boot\ -o $(_NTDRIVE)\xbox\private\ntos\init\console\obj\i386\xboxrom_dvt4_ext.bin
!ENDIF

!IFDEF _NT386TREE
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) $@
!IF !DEFINED(NODEVKIT)
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) obj\i386\xboxrom_dvt4_ext.bin
!ENDIF
!ENDIF


obj\i386\xboxrom_dvt6.bin: obj\i386\xboxkrnl.exe $(TARGETPATH2)\i386\inittbl_dvt6.bin $(TARGETPATH2)\i386\xboxbldr.bin $(TARGETPATH2)\i386\romdec32.bin
    copy $(_NTDRIVE)\xbox\private\ntos\init\console\obj\i386\xboxkrnl.exe $(IDW_DIR)\biospack\boot\xboxkrnl.img /Y /V
	biospack -t multi -i $(IDW_DIR)\biospack\boot\ -o $(_NTDRIVE)\xbox\private\ntos\init\console\obj\i386\xboxrom_dvt6.bin
!IF !DEFINED(NODEVKIT)
    biospack -t multi -i $(IDW_DIR)\biospack\boot\ -o $(_NTDRIVE)\xbox\private\ntos\init\console\obj\i386\xboxrom_dvt6_ext.bin
!ENDIF

!IFDEF _NT386TREE
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) $@
!IF !DEFINED(NODEVKIT)
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) obj\i386\xboxrom_dvt6_ext.bin
!ENDIF
!ENDIF


obj\i386\xboxrom_qt.bin: obj\i386\xboxkrnl.exe $(TARGETPATH2)\i386\inittbl_qt.bin $(TARGETPATH2)\i386\xboxbldr.bin $(TARGETPATH2)\i386\romdec32.bin
    copy $(_NTDRIVE)\xbox\private\ntos\init\console\obj\i386\xboxkrnl.exe $(IDW_DIR)\biospack\boot\xboxkrnl.img /Y /V
	biospack -t multi -i $(IDW_DIR)\biospack\boot\ -o $(_NTDRIVE)\xbox\private\ntos\init\console\obj\i386\xboxrom_qt.bin
!IF !DEFINED(NODEVKIT)
    biospack -t multi -i $(IDW_DIR)\biospack\boot\ -o $(_NTDRIVE)\xbox\private\ntos\init\console\obj\i386\xboxrom_qt_ext.bin
!ENDIF

!IFDEF _NT386TREE
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) $@
!IF !DEFINED(NODEVKIT)
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) obj\i386\xboxrom_qt_ext.bin
!ENDIF
!ENDIF


!IFDEF _NT386TREE
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) $@
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) obj\i386\xboxkrnl.map
!ENDIF
