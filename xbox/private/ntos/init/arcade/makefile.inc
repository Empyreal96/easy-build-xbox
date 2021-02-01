BOOTXFILES=..\..\obj\i386

obj\$(TARGET_DIRECTORY)\$(NTTEST).def: ..\ntoskrnl.src
    copy ..\ntoskrnl.src obj\$(TARGET_DIRECTORY)\$(NTTEST).pp
    $(TARGET_CPP) -nologo -EP $(TARGET_DEFINES) $(TARGET_DBG_DEFINES) $(WIN32_DEFINE) $(C_DEFINES) obj\$(TARGET_DIRECTORY)\$(NTTEST).pp > obj\$(TARGET_DIRECTORY)\$(NTTEST).def
    -del obj\$(TARGET_DIRECTORY)\$(NTTEST).pp

$(O)\$(NTTEST).exp: obj\$(TARGET_DIRECTORY)\$(NTTEST).def \
        $(KERNEL_LIBS)
    -lib @<<
$(LINK_LIB_IGNORE_FLAG)
-def:obj\$(TARGET_DIRECTORY)\$(NTTEST).def
-debugtype:cv
-out:$(@R).lib
-machine:$(MACHINE_TYPE)
$(KERNEL_LIBS)
<<NOKEEP

..\init.c: $(BASEDIR)\private\inc\xboxverp.h

!IF DEFINED(RETAILXM3)
ROMBLDSYS=xm3
!ELSE
ROMBLDSYS=xdk
!ENDIF

obj\i386\arcdrom_dvt6.bin: obj\i386\arcdkrnl.exe $(BOOTXFILES)\inittbl_dvt6.bin $(BOOTXFILES)\xboxbldr.bin $(BOOTXFILES)\romdec32.bin
    copy $(_NTDRIVE)\xbox\private\ntos\init\arcade\obj\i386\xboxkrnl.exe $(IDW_DIR)\biospack\boot\xboxkrnl.img /Y /V
	biospack -t multi -i $(IDW_DIR)\biospack\boot\ -o $(_NTDRIVE)\xbox\private\ntos\init\arcade\obj\i386\arcdrom_dvt6.bin
!IF !DEFINED(NODEVKIT)
    biospack -t multi -i $(IDW_DIR)\biospack\boot\ -o $(_NTDRIVE)\xbox\private\ntos\init\arcade\obj\i386\arcdrom_dvt6_ext.bin
!ENDIF

!IFDEF _NT386TREE
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) $@
!IF !DEFINED(NODEVKIT)
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) obj\i386\arcdrom_dvt6_ext.bin
!ENDIF
!ENDIF

!IFDEF _NT386TREE
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) $@
    binplace -R $(_NTTREE) $(BINPLACE_DBGFLAGS_NT) $(BINPLACE_FLAGS) obj\i386\arcdkrnl.map
!ENDIF
