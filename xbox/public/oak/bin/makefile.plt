#
# If not defined, specify where to get incs and libs.
#

!IFNDEF _NTROOT
_NTROOT=\xbox
!ENDIF

!IFNDEF BASEDIR
BASEDIR=$(_NTDRIVE)$(_NTROOT)
!ENDIF

COPYRIGHT_STRING = Copyright (c) 1990-1999  Microsoft Corporation

#
# If not defined, define the build message banner.
#

!IFNDEF BUILDMSG
BUILDMSG=
!ENDIF

!if ("$(NTDEBUG)" == "") || ("$(NTDEBUG)" == "ntsdnodbg")
FREEBUILD=1
!else
FREEBUILD=0
!endif


# Allow alternate object directories.

!ifndef BUILD_ALT_DIR
BUILD_ALT_DIR=
! ifdef CHECKED_ALT_DIR
! if !$(FREEBUILD)
BUILD_ALT_DIR=d
!  endif
! endif
!endif

_OBJ_DIR = obj
# _OBJ_DIR = obj$(BUILD_ALT_DIR)


#
# Determine which target is being built (i386, axp64 or Alpha) and define
# the appropriate target variables.
#

!IFNDEF 386
386=0
!ENDIF

!IFNDEF AXP64
AXP64=0
!ENDIF

!IFNDEF ALPHA
ALPHA=0
!ENDIF

!IFNDEF PPC
PPC=0
!ENDIF

!IFNDEF MPPC
MPPC=0
!ENDIF

!IFNDEF IA64
IA64=0
!ENDIF

# Disable for now.
MIPS=0

#
# Default to building for the i386 target, if no target is specified.
#

!IF !$(386)
!   IF !$(AXP64)
!       IF !$(ALPHA)
!           IF !$(PPC)
!               IF !$(MPPC)
!                   IF !$(IA64)
!                       IFDEF NTAXP64DEFAULT
AXP64=1
!                           IFNDEF TARGETCPU
TARGETCPU=AXP64
!                           ENDIF
!                       ELSE
!                           IFDEF NTALPHADEFAULT
ALPHA=1
!                               IFNDEF TARGETCPU
TARGETCPU=ALPHA
!                               ENDIF
!                           ELSE
!                               IFDEF NTPPCDEFAULT
PPC=1
!                                   IFNDEF TARGETCPU
TARGETCPU=PPC
!                                   ENDIF
!                               ELSE
!                                   IFDEF NTMPPCDEFAULT
MPPC=1
!                                       IFNDEF TARGETCPU
TARGETCPU=MPPC
!                                       ENDIF
!                                   ELSE
!                                       IFDEF NTIA64DEFAULT
IA64=1
!                                           IFNDEF TARGETCPU
TARGETCPU=IA64
!                                           ENDIF
!                                       ELSE
386=1
!                                           IFNDEF TARGETCPU
TARGETCPU=I386
!                                           ENDIF
!                                       ENDIF
!                                   ENDIF
!                               ENDIF
!                           ENDIF
!                       ENDIF
!                   ENDIF
!               ENDIF
!           ENDIF
!       ENDIF
!   ENDIF
!ENDIF

#
# Define the target platform specific information.
#

!if $(386)

ASM_SUFFIX=asm
ASM_INCLUDE_SUFFIX=inc

TARGET_BRACES=

!if "$(SUBSTITUTE_386_CC)" != ""
TARGET_CPP=$(SUBSTITUTE_386_CC)
!else
TARGET_CPP=$(VC_PATH)cl
!endif

TARGET_DEFINES=-Di386 -D_X86_
TARGET_DIRECTORY=i386
TLB_SWITCHES=/tlb
TARGET_NTTREE=$(_NT386TREE)

MIDL_CPP=$(TARGET_CPP)
MIDL_FLAGS=$(TARGET_DEFINES) -D_WCHAR_T_DEFINED

!if "$(BUILD_ALT_DIR)" != ""
! if defined(_NT386TREE_ALT) && defined (_NT386TREE)
_NT386TREE = $(_NT386TREE_ALT)
! endif
! if defined(_NT386TREE_NS_ALT) && defined (_NT386TREE_NS)
_NT386TREE_NS = $(_NT386TREE_NS_ALT)
! endif
!endif

!IF DEFINED (CHICAGO_PRODUCT)
! IF DEFINED (_CHICAGO386TREE)
_NTTREE=$(_CHICAGO386TREE)
_NTTREE_NO_SPLIT=$(_CHICAGO386TREE_NS)
! elseif defined(_NT386TREE)
_NTTREE=$(_NT386TREE)
_NTTREE_NO_SPLIT=$(_NT386TREE_NS)
! ENDIF
!ELSEIF DEFINED (_NT386TREE)
_NTTREE=$(_NT386TREE)
_NTTREE_NO_SPLIT=$(_NT386TREE_NS)
!ENDIF

VCCOM_SUPPORTED=1
COFF_SUPPORTED=1
WIN64=0
PLATFORM_MFC_VER=0x0421
MACHINE_TYPE=ix86

!elseif $(AXP64)

ASM_SUFFIX=s
ASM_INCLUDE_SUFFIX=h

TARGET_BRACES=-B
TARGET_CPP=$(VC_PATH)cl
TARGET_DEFINES=-D_WIN64 -D_AXP64_ -D_M_AXP64 -DALPHA -D_ALPHA_
TARGET_DIRECTORY=axp64
TLB_SWITCHES=/tlb
# default to Alpha for now
!ifndef HOST_TARGETCPU
HOST_TARGETCPU=alpha
!endif
TARGET_NTTREE=$(_NTAXP64TREE)

MIDL_CPP=$(TARGET_CPP)
MIDL_FLAGS=$(TARGET_DEFINES) -D_WCHAR_T_DEFINED

!if "$(BUILD_ALT_DIR)" != ""
! if defined(_NTAXP64TREE_ALT) && defined (_NTAXP64TREE)
_NTAXP64TREE = $(_NTAXP64TREE_ALT)
! endif
! if defined(_NTAXP64TREE_NS_ALT) && defined (_NTAXP64TREE_NS)
_NTAXP64TREE_NS = $(_NTAXP64TREE_NS_ALT)
! endif
!endif

!IFDEF _NTAXP64TREE
_NTTREE=$(_NTAXP64TREE)
_NTTREE_NO_SPLIT=$(_NTAXP64TREE_NS)
!ENDIF

VCCOM_SUPPORTED=1
COFF_SUPPORTED=0
MACHINE_TYPE=alpha64
WIN64=1
PLATFORM_MFC_VER=0x0600

!elseif $(ALPHA)

ASM_SUFFIX=s
ASM_INCLUDE_SUFFIX=h

TARGET_BRACES=-B
TARGET_CPP=$(VC_PATH)cl
TARGET_DEFINES=-DALPHA -D_ALPHA_
TARGET_DIRECTORY=alpha
TLB_SWITCHES=/tlb
TARGET_NTTREE=$(_NTALPHATREE)

MIDL_CPP=$(TARGET_CPP)
MIDL_FLAGS=$(TARGET_DEFINES) -D_WCHAR_T_DEFINED

!if "$(BUILD_ALT_DIR)" != ""
! if defined(_NTALPHATREE_ALT) && defined (_NTALPHATREE)
_NTALPHATREE = $(_NTALPHATREE_ALT)
! endif
! if defined(_NTALPHATREE_NS_ALT) && defined (_NTALPHATREE_NS)
_NTALPHATREE_NS = $(_NTALPHATREE_NS_ALT)
! endif
!endif

!IFDEF _NTALPHATREE
_NTTREE=$(_NTALPHATREE)
_NTTREE_NO_SPLIT=$(_NTALPHATREE_NS)
!ENDIF

VCCOM_SUPPORTED=1
COFF_SUPPORTED=1
WIN64=0
PLATFORM_MFC_VER=0x0421
MACHINE_TYPE=alpha

!elseif $(MPPC)

ASM_SUFFIX=s
ASM_INCLUDE_SUFFIX=h

TARGET_BRACES=-B
TARGET_CPP= $(VC_PATH)cl
TARGET_DEFINES=-DMPPC -D_MPPC_
TARGET_DIRECTORY=mppc
TLB_SWITCHES=/tlb
TARGET_NTTREE=$(_NTMPPCTREE)
WIN64=0
PLATFORM_MFC_VER=0x0421
MACHINE_TYPE=mppc

MIDL_CPP=$(TARGET_CPP)
MIDL_FLAGS=$(TARGET_DEFINES) -D_WCHAR_T_DEFINED

!elseif $(IA64)

ASM_SUFFIX=s
ASM_INCLUDE_SUFFIX=h

TARGET_BRACES=-B
TARGET_CPP=$(VC_PATH)cl
TARGET_DEFINES=-DIA64 -D_IA64_
TARGET_DIRECTORY=ia64
TLB_SWITCHES=/tlb
# default to X86 for now
!ifndef HOST_TARGETCPU
HOST_TARGETCPU=i386
!endif
TARGET_NTTREE=$(_NTIA64TREE)

MIDL_CPP=$(TARGET_CPP)
MIDL_FLAGS=$(TARGET_DEFINES) -D_WCHAR_T_DEFINED

!if "$(BUILD_ALT_DIR)" != ""
! if defined(_NTIA64TREE_ALT) && defined (_NTIA64TREE)
_NTIA64TREE = $(_NTIA64TREE_ALT)
! endif
! if defined(_NTIA64TREE_NS_ALT) && defined (_NTIA64TREE_NS)
_NTIA64TREE_NS = $(_NTIA64TREE_NS_ALT)
! endif
!endif

!IFDEF _NTIA64TREE
_NTTREE=$(_NTIA64TREE)
_NTTREE_NO_SPLIT=$(_NTIA64TREE_NS)
!ENDIF

WIN64=1
PLATFORM_MFC_VER=0x0600
COFF_SUPPORTED=1
MACHINE_TYPE=ia64

!else
!error Must define the target as 386, axp64, alpha, ppc, mppc or ia64.
!endif

#
#  These flags don't depend on i386 etc. however have to be in this file.
#
#  MIDL_OPTIMIZATION is the optimization flag set for the current NT.
#  MIDL_OPTIMIZATION_NO_ROBUST is the current optimization without robust.
#
!IFNDEF MIDL_OPTIMIZATION
MIDL_OPTIMIZATION=-Oicf -robust -error all
!ENDIF
MIDL_OPTIMIZATION_NO_ROBUST=-Oicf -error all
MIDL_OPTIMIZATION_NT4=-Oicf -error all
MIDL_OPTIMIZATION_NT5=-Oicf -robust -error all

#
# If not defined, simply set to default
#

!IFNDEF HOST_TARGETCPU
HOST_TARGETCPU=$(TARGET_DIRECTORY)
!ENDIF

! if $(WIN64)
MIDL_ALWAYS_GENERATE_STUBS=1
! else
MIDL_ALWAYS_GENERATE_STUBS=0
! endif
