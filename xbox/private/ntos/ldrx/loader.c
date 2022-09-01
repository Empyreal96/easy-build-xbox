/*++

Copyright (c) 1989-2002  Microsoft Corporation

Module Name:

    loader.c

Abstract:

    This module implements the Xbox executable image loader.

--*/

#include "ldrx.h"

//
// Verify that the definitions in the xbeimage.h match those found in xconfig.h.
//
C_ASSERT(XBEIMAGE_GAME_REGION_NA == XC_GAME_REGION_NA);
C_ASSERT(XBEIMAGE_GAME_REGION_JAPAN == XC_GAME_REGION_JAPAN);
C_ASSERT(XBEIMAGE_GAME_REGION_RESTOFWORLD == XC_GAME_REGION_RESTOFWORLD);
C_ASSERT(XBEIMAGE_GAME_REGION_MANUFACTURING == XC_GAME_REGION_MANUFACTURING);

//
// Contains the public key data to verify the encrypted header digest of an Xbox
// executable image.
//
#ifdef INTERIMPUBKEY

DECLSPEC_RDATA UCHAR XePublicKeyData[XC_PUBLIC_KEYDATA_SIZE] = {
    0x52,0x53,0x41,0x31,0x08,0x01,0x00,0x00,
    0x00,0x08,0x00,0x00,0xFF,0x00,0x00,0x00,
    0x01,0x00,0x01,0x00,0xE5,0xEC,0x86,0x9B,
    0x50,0x89,0x3B,0xB3,0xEB,0x41,0x30,0x8A,
    0x13,0x6E,0xAD,0xE9,0x7D,0xCA,0x9B,0xB4,
    0xF1,0x89,0x58,0xCE,0xAE,0xE1,0x9A,0x66,
    0xE9,0x1C,0x3C,0x04,0x39,0x2E,0xF7,0xA1,
    0x13,0xE8,0xFB,0x66,0x1C,0x59,0xC1,0xCE,
    0x18,0x7C,0x71,0xB8,0x5D,0x25,0xBB,0x5F,
    0x21,0x30,0x59,0xF7,0xDC,0x3B,0x24,0x13,
    0x42,0x69,0x30,0xF6,0x88,0x3D,0x99,0x89,
    0xCC,0xEB,0xFB,0x16,0x7C,0x78,0xEB,0x05,
    0x6B,0x41,0x8F,0xE0,0xB3,0x55,0x5F,0xEB,
    0x74,0xCA,0xB4,0x31,0x04,0xA2,0xB2,0x5A,
    0xF0,0x03,0x8E,0xDC,0x27,0x1A,0x61,0xF8,
    0x90,0x45,0xFE,0x20,0xD4,0x29,0x1C,0x58,
    0xAF,0xE9,0xFD,0x77,0x37,0x90,0xEC,0xBD,
    0x19,0xD0,0x8F,0x06,0x56,0x3C,0x95,0xC8,
    0xA0,0xA9,0x5A,0x07,0x62,0x4E,0xE8,0x35,
    0xCB,0xCF,0xA5,0xAE,0x27,0x94,0xE5,0x1C,
    0x4F,0xA6,0x18,0x5E,0xD3,0x5C,0xE9,0x06,
    0x24,0x93,0x57,0x38,0x91,0xC7,0x98,0x17,
    0x81,0x3B,0xD3,0x39,0xC7,0xD6,0xEC,0x6D,
    0xFC,0xB1,0xE1,0x52,0xAB,0x82,0x90,0x40,
    0x7B,0xC9,0x1D,0xD4,0x45,0x87,0xAC,0x94,
    0xD6,0x9D,0xB1,0x40,0xB3,0x19,0xE5,0xAD,
    0x18,0xF2,0xB8,0x9C,0xB0,0x54,0xE5,0x07,
    0x4C,0x74,0x4C,0xBD,0x9F,0xCC,0xDF,0xEC,
    0x7C,0xFF,0xAD,0x19,0x2A,0x44,0x03,0xD4,
    0xAA,0xA2,0x79,0x23,0xF9,0x1A,0xDD,0x46,
    0x05,0xF1,0x11,0xB0,0x89,0xC0,0xEB,0xBC,
    0xD7,0x3D,0x94,0xFC,0x04,0x9E,0x67,0x01,
    0x4C,0x10,0x00,0x70,0x2A,0x3F,0x5A,0xD0,
    0x68,0xCF,0xC7,0x59,0xA9,0x01,0x7B,0x07,
    0x34,0x26,0x54,0xA6,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00
};

#elif RETAILXM3

DECLSPEC_RDATA UCHAR XePublicKeyData[XC_PUBLIC_KEYDATA_SIZE] = {
    0x52,0x53,0x41,0x31,0x08,0x01,0x00,0x00,
    0x00,0x08,0x00,0x00,0xFF,0x00,0x00,0x00,
    0x01,0x00,0x01,0x00,0xD3,0xD7,0x4E,0xE5,
    0x66,0x3D,0xD7,0xE6,0xC2,0xD4,0xA3,0xA1,
    0xF2,0x17,0x36,0xD4,0x2E,0x52,0xF6,0xD2,
    0x02,0x10,0xF5,0x64,0x9C,0x34,0x7B,0xFF,
    0xEF,0x7F,0xC2,0xEE,0xBD,0x05,0x8B,0xDE,
    0x79,0xB4,0x77,0x8E,0x5B,0x8C,0x14,0x99,
    0xE3,0xAE,0xC6,0x73,0x72,0x73,0xB5,0xFB,
    0x01,0x5B,0x58,0x46,0x6D,0xFC,0x8A,0xD6,
    0x95,0xDA,0xED,0x1B,0x2E,0x2F,0xA2,0x29,
    0xE1,0x3F,0xF1,0xB9,0x5B,0x64,0x51,0x2E,
    0xA2,0xC0,0xF7,0xBA,0xB3,0x3E,0x8A,0x75,
    0xFF,0x06,0x92,0x5C,0x07,0x26,0x75,0x79,
    0x10,0x5D,0x47,0xBE,0xD1,0x6A,0x52,0x90,
    0x0B,0xAE,0x6A,0x0B,0x33,0x44,0x93,0x5E,
    0xF9,0x9D,0xFB,0x15,0xD9,0xA4,0x1C,0xCF,
    0x6F,0xE4,0x71,0x94,0xBE,0x13,0x00,0xA8,
    0x52,0xCA,0x07,0xBD,0x27,0x98,0x01,0xA1,
    0x9E,0x4F,0xA3,0xED,0x9F,0xA0,0xAA,0x73,
    0xC4,0x71,0xF3,0xE9,0x4E,0x72,0x42,0x9C,
    0xF0,0x39,0xCE,0xBE,0x03,0x76,0xFA,0x2B,
    0x89,0x14,0x9A,0x81,0x16,0xC1,0x80,0x8C,
    0x3E,0x6B,0xAA,0x05,0xEC,0x67,0x5A,0xCF,
    0xA5,0x70,0xBD,0x60,0x0C,0xE8,0x37,0x9D,
    0xEB,0xF4,0x52,0xEA,0x4E,0x60,0x9F,0xE4,
    0x69,0xCF,0x52,0xDB,0x68,0xF5,0x11,0xCB,
    0x57,0x8F,0x9D,0xA1,0x38,0x0A,0x0C,0x47,
    0x1B,0xB4,0x6C,0x5A,0x53,0x6E,0x26,0x98,
    0xF1,0x88,0xAE,0x7C,0x96,0xBC,0xF6,0xBF,
    0xB0,0x47,0x9A,0x8D,0xE4,0xB3,0xE2,0x98,
    0x85,0x61,0xB1,0xCA,0x5F,0xF7,0x98,0x51,
    0x2D,0x83,0x81,0x76,0x0C,0x88,0xBA,0xD4,
    0xC2,0xD5,0x3C,0x14,0xC7,0x72,0xDA,0x7E,
    0xBD,0x1B,0x4B,0xA4,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00
};

#else

DECLSPEC_RDATA UCHAR XePublicKeyData[XC_PUBLIC_KEYDATA_SIZE] = {
    0x52,0x53,0x41,0x31,0x08,0x01,0x00,0x00,
    0x00,0x08,0x00,0x00,0xFF,0x00,0x00,0x00,
    0x01,0x00,0x01,0x00,0x9B,0x83,0xD4,0xD5,
    0xDE,0x16,0x25,0x8E,0xE5,0x15,0xF2,0x18,
    0x9D,0x19,0x1C,0xF8,0xFE,0x91,0xA5,0x83,
    0xAE,0xA5,0xA8,0x95,0x3F,0x01,0xB2,0xC9,
    0x34,0xFB,0xC7,0x51,0x2D,0xAC,0xFF,0x38,
    0xE6,0xB6,0x7B,0x08,0x4A,0xDF,0x98,0xA3,
    0xFD,0x31,0x81,0xBF,0xAA,0xD1,0x62,0x58,
    0xC0,0x6C,0x8F,0x8E,0xCD,0x96,0xCE,0x6D,
    0x03,0x44,0x59,0x93,0xCE,0xEA,0x8D,0xF4,
    0xD4,0x6F,0x6F,0x34,0x5D,0x50,0xF1,0xAE,
    0x99,0x7F,0x1D,0x92,0x15,0xF3,0x6B,0xDB,
    0xF9,0x95,0x8B,0x3F,0x54,0xAD,0x37,0xB5,
    0x4F,0x0A,0x58,0x7B,0x48,0xA2,0x9F,0x9E,
    0xA3,0x16,0xC8,0xBD,0x37,0xDA,0x9A,0x37,
    0xE6,0x3F,0x10,0x1B,0xA8,0x4F,0xA3,0x14,
    0xFA,0xBE,0x12,0xFB,0xD7,0x19,0x4C,0xED,
    0xAD,0xA2,0x95,0x8F,0x39,0x8C,0xC4,0x69,
    0x0F,0x7D,0xB8,0x84,0x0A,0x99,0x5C,0x53,
    0x2F,0xDE,0xF2,0x1B,0xC5,0x1D,0x4C,0x43,
    0x3C,0x97,0xA7,0xBA,0x8F,0xC3,0x22,0x67,
    0x39,0xC2,0x62,0x74,0x3A,0x0C,0xB5,0x57,
    0x01,0x3A,0x67,0xC6,0xDE,0x0C,0x0B,0xF6,
    0x08,0x01,0x64,0xDB,0xBD,0x81,0xE4,0xDC,
    0x09,0x2E,0xD0,0xF1,0xD0,0xD6,0x1E,0xBA,
    0x38,0x36,0xF4,0x4A,0xDD,0xCA,0x39,0xEB,
    0x76,0xCF,0x95,0xDC,0x48,0x4C,0xF2,0x43,
    0x8C,0xD9,0x44,0x26,0x7A,0x9E,0xEB,0x99,
    0xA3,0xD8,0xFB,0x30,0xA8,0x14,0x42,0x82,
    0x8D,0xB4,0x31,0xB3,0x1A,0xD5,0x2B,0xF6,
    0x32,0xBC,0x62,0xC0,0xFE,0x81,0x20,0x49,
    0xE7,0xF7,0x58,0x2F,0x2D,0xA6,0x1B,0x41,
    0x62,0xC7,0xE0,0x32,0x02,0x5D,0x82,0xEC,
    0xA3,0xE4,0x6C,0x9B,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00
};

#endif

//
// Stores the name of the image file.
//
OBJECT_STRING XeImageFileName;

//
// Guards multiple threads attempting to load or unload image sections.
//
INITIALIZED_CRITICAL_SECTION(XepLoaderLock);

//
// Name of the default Xbox executable to load from the CD-ROM device.
//
const OCHAR XepDefaultImagePath[] = OTEXT("\\Device\\CdRom0\\default.xbe");

//
// Name of the default dashboard Xbox executable to load from the disk device.
//
const OCHAR XepDashboardImagePath[] = OTEXT("\\Device\\Harddisk0\\Partition2\\xboxdash.xbe");

//
// Name of the DOS device symbolic link for the CD-ROM device.
//
INITIALIZED_OBJECT_STRING_RDATA(XepCdRomDosDevice, "\\??\\D:");

#ifdef DEVKIT
//
// Static loader data table entry for the Xbox executable image.
//
LDR_DATA_TABLE_ENTRY XepDataTableEntry;

//
// Indicates whether or not section load and unload debugger notifications
// should be sent.
//
BOOLEAN XepSendSectionDebugNotifications;

//
// Name of the dashboard direction file used to specify an alternate dashboard.
//
INITIALIZED_OBJECT_STRING_RDATA(XepDashboardRedirectionPath, "\\Device\\Harddisk0\\Partition1\\dashboard.xbx");
#endif

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, XeLoadImageAfterTrayEjectBoot)
#endif

//
// Stores the title identifier for the media located in the CD-ROM device.  The
// loader only allows a single title identifier to be launched from a single
// media.
//
DECLSPEC_STICKY ULONG XeCdRomMediaTitleID;

NTSTATUS
XepOpenImageFile(
    OUT PHANDLE ImageFileHandle
    )
/*++

Routine Description:

    This routine opens the image file with the appropriate options.

Arguments:

    ImageFileHandle - Specifies the buffer to receive the handle to the image
        file.

Return Value:

    Status of operation.

--*/
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;

    InitializeObjectAttributes(&ObjectAttributes, &XeImageFileName,
        OBJ_CASE_INSENSITIVE, NULL, NULL);

    return NtOpenFile(ImageFileHandle, GENERIC_READ, &ObjectAttributes,
        &IoStatusBlock, FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_NONALERT |
        FILE_NON_DIRECTORY_FILE);
}

NTSTATUS
XepReadImageFile(
    IN HANDLE ImageFileHandle,
    IN PVOID Buffer,
    IN ULONG Length,
    IN ULONG ByteOffset
    )
/*++

Routine Description:

    This routine reads the requested number of bytes from the file to the
    supplied buffer from the starting file byte offset.

Arguments:

    ImageFileHandle - Specifies the handle to the image file.

    Buffer - Specifies the address of the buffer to receive the image file data.

    Length - Specifies the number of bytes to transfer to the buffer.

    ByteOffset - Specifies the byte offset within the file to begin the read
        operation.

Return Value:

    Status of operation.

--*/
{
    NTSTATUS status;
    LARGE_INTEGER ByteOffsetLarge;
    IO_STATUS_BLOCK IoStatusBlock;

    ByteOffsetLarge.QuadPart = ByteOffset;

    status = NtReadFile(ImageFileHandle, NULL, NULL, NULL, &IoStatusBlock,
        Buffer, Length, &ByteOffsetLarge);

    if (NT_SUCCESS(status)) {

        if (IoStatusBlock.Information != Length) {
            status = STATUS_FILE_CORRUPT_ERROR;
        }
    }

    return status;
}

NTSTATUS
XepCommitSectionPages(
    IN PXBEIMAGE_SECTION Section
    )
/*++

Routine Description:

    This routine commits the pages assigned to the supplied section.

Arguments:

    Section - Specifies the section to commit pages for.

Return Value:

    Status of operation.

--*/
{
    NTSTATUS status;
    PVOID BaseAddress;
    SIZE_T RegionSize;

    //
    // Commit the pages for the section.  Note that NtAllocateVirtualMemory is
    // serving double duty if the pages are already committed: the pages may be
    // marked as PAGE_EXECUTE_READ, but we need to change them to
    // PAGE_EXECUTE_READWRITE in order to transfer data to the page.
    //

    BaseAddress = (PVOID)Section->VirtualAddress;
    RegionSize = Section->VirtualSize;

    status = NtAllocateVirtualMemory(&BaseAddress, 0, &RegionSize,
        MEM_COMMIT, PAGE_EXECUTE_READWRITE);

    if (!NT_SUCCESS(status)) {
        LdrxDbgPrint(("LDRX: failed to commit memory for section %s (size=%08x).\n",
            Section->SectionName, Section->VirtualSize));
        return status;
    }

    //
    // Increment the shared page reference counts.
    //

    (*Section->HeadSharedPageReferenceCount)++;
    (*Section->TailSharedPageReferenceCount)++;

    return STATUS_SUCCESS;
}

VOID
XepMarkSectionPagesReadOnly(
    IN PXBEIMAGE_SECTION Section
    )
/*++

Routine Description:

    This routine changes the protection of a section's page to be read-only.

Arguments:

    Section - Specifies the section to change the protection for.

Return Value:

    None.

--*/
{
    NTSTATUS status;
    PVOID BaseAddress;
    PVOID EndingAddress;
    SIZE_T RegionSize;
    ULONG OldProtect;

    ASSERT(Section->SectionReferenceCount > 0);
    ASSERT(LdrxIsFlagClear(Section->SectionFlags, XBEIMAGE_SECTION_WRITEABLE));

    //
    // Compute the pointers to the first and last byte, exclusive, of the
    // section.
    //

    BaseAddress = (PVOID)Section->VirtualAddress;
    EndingAddress = (PVOID)(Section->VirtualAddress + Section->VirtualSize);

    //
    // If the tail page of the section can't be marked as read-only, then align
    // the pointer down to the nearest page boundary so that we don't change the
    // protection of this page.
    //

    if (LdrxIsFlagClear(Section->SectionFlags, XBEIMAGE_SECTION_TAIL_PAGE_READONLY)) {
        EndingAddress = PAGE_ALIGN(EndingAddress);
    }

    //
    // If the head page of the section can't be marked as read-only, then align
    // the pointer up to the next page boundary so that we don't change the
    // protection of this page.
    //

    if (LdrxIsFlagClear(Section->SectionFlags, XBEIMAGE_SECTION_HEAD_PAGE_READONLY)) {
        BaseAddress = PAGE_ALIGN((PUCHAR)BaseAddress + PAGE_SIZE);
    }

    //
    // If the ending address is still beyond the base address, then there's at
    // least one page that can be marked as read-only.  Note that
    // NtProtectVirtualMemory can handle the base address not being page aligned
    // and the region size not being an integral number of pages.
    //

    if (EndingAddress > BaseAddress) {

        RegionSize = (ULONG)EndingAddress - (ULONG)BaseAddress;

        status = NtProtectVirtualMemory(&BaseAddress, &RegionSize,
            PAGE_EXECUTE_READ, &OldProtect);

        ASSERT(NT_SUCCESS(status));
    }
}

VOID
XepDecommitSectionPages(
    IN PXBEIMAGE_SECTION Section
    )
/*++

Routine Description:

    This routine decommits the pages assigned to the supplied section.

Arguments:

    Section - Specifies the section to decommit pages for.

Return Value:

    None.

--*/
{
    PVOID BaseAddress;
    PVOID EndingAddress;
    SIZE_T RegionSize;

    ASSERT(*Section->HeadSharedPageReferenceCount > 0);
    ASSERT(*Section->TailSharedPageReferenceCount > 0);

    //
    // Compute the pointers to the first and last byte, exclusive, of the
    // section.
    //

    BaseAddress = (PVOID)Section->VirtualAddress;
    EndingAddress = (PVOID)(Section->VirtualAddress + Section->VirtualSize);

    //
    // Decrement the shared page reference counts.
    //

    (*Section->HeadSharedPageReferenceCount)--;
    (*Section->TailSharedPageReferenceCount)--;

    //
    // If the tail page still has references, then align the pointer down to the
    // nearest page boundary so that we don't free the page.
    //

    if ((*Section->TailSharedPageReferenceCount) != 0) {
        EndingAddress = PAGE_ALIGN(EndingAddress);
    }

    //
    // If the head page still has references, then align the pointer up to the
    // next page boundary so that we don't free the page.
    //

    if ((*Section->HeadSharedPageReferenceCount) != 0) {
        BaseAddress = PAGE_ALIGN((PUCHAR)BaseAddress + PAGE_SIZE);
    }

    //
    // If the ending address is still beyond the base address, then there's at
    // least one page that can be decommitted.  Note that NtFreeVirtualMemory
    // can handle the base address not being page aligned and the region size
    // not being an integral number of pages.
    //

    if (EndingAddress > BaseAddress) {

        RegionSize = (ULONG)EndingAddress - (ULONG)BaseAddress;

        NtFreeVirtualMemory(&BaseAddress, &RegionSize, MEM_DECOMMIT);
    }
}

NTSTATUS
XepLoadSection(
    IN HANDLE ImageFileHandle,
    IN PXBEIMAGE_SECTION Section,
    IN BOOLEAN Preloading
    )
/*++

Routine Description:

    This routine loads the supplied section into memory.

Arguments:

    ImageFileHandle - Specifies the handle of the file that the section can be
        loaded from.

    Section - Specifies the section to be loaded.

    Preloading - Specifies TRUE if the section is being loaded from inside
        XeLoadImage, else FALSE.

Return Value:

    Status of operation.

--*/
{
    NTSTATUS status;
    UCHAR SectionDigest[XC_DIGEST_LEN];

    ASSERT(Section->SectionReferenceCount == 0);
    ASSERT(Section->SizeOfRawData <= Section->VirtualSize);

    //
    // Commit the pages assigned to the section.  Note that if this fails, then
    // we'll always allow the error to propagate back to the caller.
    //

    status = XepCommitSectionPages(Section);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //
    // Read the raw data from the image file.
    //

    status = XepReadImageFile(ImageFileHandle, (PVOID)Section->VirtualAddress,
        Section->SizeOfRawData, Section->PointerToRawData);

    if (!NT_SUCCESS(status)) {
        goto CleanupAndExit;
    }

    //
    // Zero the rest of the section.
    //

    RtlZeroMemory((PUCHAR)Section->VirtualAddress + Section->SizeOfRawData,
        Section->VirtualSize - Section->SizeOfRawData);

    //
    // Verify that the digest of the data that was read from the file matches
    // the digest stored in the section header.
    //

    XCCalcDigest((PUCHAR)Section->VirtualAddress, Section->SizeOfRawData,
        SectionDigest);

    if (memcmp(Section->SectionDigest, SectionDigest, XC_DIGEST_LEN) != 0) {
        LdrxDbgPrint(("LDRX: failed to verify section digest.\n"));
        status = STATUS_TRUST_FAILURE;
        goto CleanupAndExit;
    }

    //
    // The section is now loaded.
    //

    Section->SectionReferenceCount = 1;

    status = STATUS_SUCCESS;

#ifdef DEVKIT
    //
    // Notify the debugger that the section has been loaded.
    //

    if (XepSendSectionDebugNotifications) {
        DebugService(BREAKPOINT_LOAD_XESECTION, Section, NULL);
    }
#endif

CleanupAndExit:
    if (!NT_SUCCESS(status)) {

        //
        // Decommit the pages assigned to the section.
        //

        XepDecommitSectionPages(Section);

        //
        // If a section fails to load due to a device I/O error or the section
        // digest not matching and we're not inside XeLoadImage, then halt the
        // system.  If the current title is the dashboard, then the disk is
        // probably corrupt, so we'll display the universal error message.  We
        // don't simply return an error to the caller, because we can't trust
        // that the title will properly handle the error and the title will
        // likely crash anyway.
        //

        if (!Preloading) {

            LdrxDbgPrint(("LDRX: failed to read section %08x (status=%08x).\n",
                Section, status));

            if (XeImageHeader()->Certificate->TitleID == LDR_DASHBOARD_TITLE_ID) {
                HalReturnToFirmware(HalFatalErrorRebootRoutine);
            } else {
                HalHaltSystem();
            }
        }
    }


    return status;
}

NTSTATUS
XeLoadSection(
    IN PXBEIMAGE_SECTION Section
    )
/*++

Routine Description:

    This routine loads the supplied section into memory or increments the
    reference count for the section if the section is already loaded.

Arguments:

    Section - Specifies the section to be loaded.

Return Value:

    Status of operation.

--*/
{
    NTSTATUS status;
    HANDLE ImageFileHandle;

    ImageFileHandle = NULL;

    RtlEnterCriticalSectionAndRegion(&XepLoaderLock);

    //
    // Check if the section is already loaded and if so, just increment the
    // section's reference count and bail out.
    //

    if (Section->SectionReferenceCount > 0) {
        Section->SectionReferenceCount++;
        status = STATUS_SUCCESS;
        goto CleanupAndExit;
    }

    //
    // Open the image file.
    //

    status = XepOpenImageFile(&ImageFileHandle);

    if (!NT_SUCCESS(status)) {
        goto CleanupAndExit;
    }

    //
    // Delegate the actual section loading to a common worker routine.
    //

    status = XepLoadSection(ImageFileHandle, Section, FALSE);

    if (NT_SUCCESS(status)) {

        //
        // Change the protection of the section to PAGE_EXECUTE_READ if the
        // section is not a writeable section.
        //

        if (LdrxIsFlagClear(Section->SectionFlags, XBEIMAGE_SECTION_WRITEABLE)) {
            XepMarkSectionPagesReadOnly(Section);
        }
    }

CleanupAndExit:
    //
    // Close the image file handle.
    //

    if (ImageFileHandle != NULL) {
        NtClose(ImageFileHandle);
    }

    RtlLeaveCriticalSectionAndRegion(&XepLoaderLock);

    return status;
}

NTSTATUS
XeUnloadSection(
    IN PXBEIMAGE_SECTION Section
    )
/*++

Routine Description:

    This routine decrements the reference count for the supplied section and if
    the count reaches zero, unloads the section from memory.

Arguments:

    Section - Specifies the section to be unloaded.

Return Value:

    Status of operation.

--*/
{
    NTSTATUS status;

    RtlEnterCriticalSectionAndRegion(&XepLoaderLock);

    //
    // Check if the section has been loaded.  If not, return an error.
    //

    if (Section->SectionReferenceCount == 0) {
        status = STATUS_INVALID_PARAMETER;
        goto CleanupAndExit;
    }

    //
    // Decrement the section's reference count and check if there are still
    // references to the section.  If so, bail out.
    //

    Section->SectionReferenceCount--;

    if (Section->SectionReferenceCount > 0) {
        status = STATUS_SUCCESS;
        goto CleanupAndExit;
    }

#ifdef DEVKIT
    //
    // Notify the debugger that the section is unloading.
    //

    if (XepSendSectionDebugNotifications) {
        DebugService(BREAKPOINT_UNLOAD_XESECTION, Section, NULL);
    }
#endif

    //
    // Decommit the pages that are used by the section.
    //

    XepDecommitSectionPages(Section);

    status = STATUS_SUCCESS;

CleanupAndExit:
    RtlLeaveCriticalSectionAndRegion(&XepLoaderLock);

    return status;
}

NTSTATUS
XepVerifyImageHeaderEncryptedDigest(
    VOID
    )
/*++

Routine Description:

    This routine verifies that the encrypted header digest stored in the image
    header matches the digest of the data in the rest of the image headers.

Arguments:

    None.

Return Value:

    Status of operation.

--*/
{
    PUCHAR PublicKeyData;
    PUCHAR Workspace;
    ULONG HeaderDigestLength;
    UCHAR HeaderDigest[XC_DIGEST_LEN];
    BOOLEAN Verified;

    PublicKeyData = XePublicKeyData;

    //
    // Allocate a workspace to do the digest verification.
    //

    Workspace = (PUCHAR)ExAllocatePoolWithTag(XCCalcKeyLen(PublicKeyData) * 2,
        'sWeX');

    if (Workspace == NULL) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    //
    // The encrypted header digest starts at the field after the encrypted
    // header field in the main image header.
    //

    HeaderDigestLength = XeImageHeader()->SizeOfHeaders -
        FIELD_OFFSET(XBEIMAGE_HEADER, BaseAddress);

    //
    // Calculate the SHA1 digest for the headers.
    //

    XCCalcDigest((PUCHAR)&XeImageHeader()->BaseAddress, HeaderDigestLength,
        HeaderDigest);

    //
    // Verify that the SHA1 digest matches the encrypted header digest.
    //

    Verified = XCVerifyDigest(XeImageHeader()->EncryptedDigest, PublicKeyData,
        Workspace, HeaderDigest);

    ExFreePool(Workspace);

    if (Verified) {
        return STATUS_SUCCESS;
    } else {
        return STATUS_TRUST_FAILURE;
    }
}

NTSTATUS
XepComputeImageMediaTypes(
    IN HANDLE ImageFileHandle,
    OUT PULONG ReturnedImageMediaTypes
    )
/*++

Routine Description:

    This routine computes the type of image media that the supplied file handle
    references.

Arguments:

    ImageFileHandle - Specifies the handle of the image file.

    ReturnedImageMediaTypes - Specifies the buffer to receive the type of media
        that the supplied file handle references.

Return Value:

    Status of operation.

--*/
{
    NTSTATUS status;
    IO_STATUS_BLOCK IoStatusBlock;
    FILE_FS_DEVICE_INFORMATION DeviceInformation;
#if !defined(ARCADE) || defined(DEVKIT)
    DVD_READ_STRUCTURE ReadStructure;
    UCHAR ReadStructureOutput[ALIGN_UP(sizeof(DVD_DESCRIPTOR_HEADER) +
        sizeof(DVD_LAYER_DESCRIPTOR), ULONG)];
    PDVD_DESCRIPTOR_HEADER DescriptorHeader;
    PDVD_LAYER_DESCRIPTOR LayerDescriptor;
    ULONG ImageMediaTypes;
#endif

    //
    // Query the file's volume to find out what type of device we have.
    //

    status = NtQueryVolumeInformationFile(ImageFileHandle, &IoStatusBlock,
        &DeviceInformation, sizeof(FILE_FS_DEVICE_INFORMATION),
        FileFsDeviceInformation);

    if (!NT_SUCCESS(status)) {
        LdrxDbgPrint(("LDRX: failed to query device information (status=%08x).\n",
            status));
        return status;
    }

#ifdef ARCADE
    //
    // Check if this is a handle to a file loaded from the media board.
    //

    if (DeviceInformation.DeviceType == FILE_DEVICE_MEDIA_BOARD) {
        *ReturnedImageMediaTypes = XBEIMAGE_MEDIA_TYPE_MEDIA_BOARD;
        return STATUS_SUCCESS;
    }
#endif

#if !defined(ARCADE) || defined(DEVKIT)
    //
    // If this isn't a handle to a file from the CD-ROM device, then it must be
    // a file from the hard disk device.
    //

    if (DeviceInformation.DeviceType != FILE_DEVICE_CD_ROM) {
        ASSERT(DeviceInformation.DeviceType == FILE_DEVICE_DISK);
        *ReturnedImageMediaTypes = XBEIMAGE_MEDIA_TYPE_HARD_DISK;
        return STATUS_SUCCESS;
    }

    //
    // If the device has already passed DVD-X2 authentication, then we only
    // return this media type flag.
    //

    if (IdexCdRomDVDX2Authenticated) {
        *ReturnedImageMediaTypes = XBEIMAGE_MEDIA_TYPE_DVD_X2;
        return STATUS_SUCCESS;
    }

    //
    // Read the DVD physical descriptor structure.  If the device reports back
    // that this is an unrecognized media, then assume that we have a CD media.
    //

    RtlZeroMemory(&ReadStructure, sizeof(DVD_READ_STRUCTURE));

    ReadStructure.Format = DvdPhysicalDescriptor;

    RtlZeroMemory(ReadStructureOutput, sizeof(ReadStructureOutput));

    status = NtDeviceIoControlFile(ImageFileHandle, NULL, NULL, NULL,
        &IoStatusBlock, IOCTL_DVD_READ_STRUCTURE, &ReadStructure,
        sizeof(DVD_READ_STRUCTURE), &ReadStructureOutput,
        sizeof(ReadStructureOutput));

    if (status == STATUS_UNRECOGNIZED_MEDIA) {
        *ReturnedImageMediaTypes = XBEIMAGE_MEDIA_TYPE_DVD_CD |
            XBEIMAGE_MEDIA_TYPE_CD;
        return STATUS_SUCCESS;
    }

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //
    // Decode the DVD physical descriptor structure.
    //

    DescriptorHeader = (PDVD_DESCRIPTOR_HEADER)ReadStructureOutput;
    LayerDescriptor = (PDVD_LAYER_DESCRIPTOR)DescriptorHeader->Data;

    if (LayerDescriptor->LayerType == 0x01) {

        if (LayerDescriptor->NumberOfLayers == 0x00) {
            ImageMediaTypes = XBEIMAGE_MEDIA_TYPE_DVD_CD |
                XBEIMAGE_MEDIA_TYPE_DVD_5_RO;
        } else {
            ImageMediaTypes = XBEIMAGE_MEDIA_TYPE_DVD_CD |
                XBEIMAGE_MEDIA_TYPE_DVD_9_RO;
        }

    } else {

        if (LayerDescriptor->NumberOfLayers == 0x00) {
            ImageMediaTypes = XBEIMAGE_MEDIA_TYPE_DVD_CD |
                XBEIMAGE_MEDIA_TYPE_DVD_5_RW;
        } else {
            ImageMediaTypes = XBEIMAGE_MEDIA_TYPE_DVD_CD |
                XBEIMAGE_MEDIA_TYPE_DVD_9_RW;
        }
    }

    *ReturnedImageMediaTypes = ImageMediaTypes;
    return STATUS_SUCCESS;
#else
    //
    // The only media type that is supported is the media board.  Don't allow
    // executables to be loaded from any other type of media.
    //

    return STATUS_UNSUCCESSFUL;
#endif
}

NTSTATUS
XepResolveImageImports(
    IN PVOID ExportBaseAddress,
    IN PIMAGE_THUNK_DATA ImageThunkData
    )
/*++

Routine Description:

    This routine resolves ordinal imports from the supplied export executable.

Arguments:

    ExportBaseAddress - Specifies the base address of the image that is
        providing the exports.

    ImageThunkData - Specifies the pointer to the array of ordinal import thunks
        that are to be replaced by pointers to functions in the export image.

Return Value:

    Status of operation.

--*/
{
    PIMAGE_EXPORT_DIRECTORY ExportDirectory;
    ULONG ExportDirectorySize;
    PULONG AddressOfFunctions;
    ULONG OrdinalNumber;

    //
    // Lookup the export directory from the export executable.
    //

    ExportDirectory =
        (PIMAGE_EXPORT_DIRECTORY)RtlImageDirectoryEntryToData(ExportBaseAddress,
        TRUE, IMAGE_DIRECTORY_ENTRY_EXPORT, &ExportDirectorySize);

    if (ExportDirectory == NULL) {
        LdrxDbgPrint(("LDRX: cannot import from image %p.\n", ExportBaseAddress));
        return STATUS_ORDINAL_NOT_FOUND;
    }

    AddressOfFunctions = (PULONG)((ULONG)ExportBaseAddress +
        (ULONG)ExportDirectory->AddressOfFunctions);

    //
    // Loop over the image thunks and resolve the ordinals against the export
    // directory.
    //

    while (ImageThunkData->u1.Ordinal != 0) {

        ASSERT(IMAGE_SNAP_BY_ORDINAL(ImageThunkData->u1.Ordinal));

        OrdinalNumber = IMAGE_ORDINAL(ImageThunkData->u1.Ordinal) -
            ExportDirectory->Base;

        if ((OrdinalNumber >= ExportDirectory->NumberOfFunctions) ||
            (AddressOfFunctions[OrdinalNumber] == 0)) {
            LdrxDbgPrint(("LDRX: cannot import ordinal %d from image %p.\n",
                IMAGE_ORDINAL(ImageThunkData->u1.Ordinal), ExportBaseAddress));
            return STATUS_ORDINAL_NOT_FOUND;
        }

        ImageThunkData->u1.Function = (ULONG)ExportBaseAddress +
            AddressOfFunctions[OrdinalNumber];

        ImageThunkData++;
    }

    return STATUS_SUCCESS;
}

#ifdef DEVKIT

NTSTATUS
XepResolveNonKernelImageImports(
    VOID
    )
/*++

Routine Description:

    This routine resolves imports for modules other than the kernel.

Arguments:

    None.

Return Value:

    Status of operation.

--*/
{
    NTSTATUS status;
    PXBEIMAGE_IMPORT_DESCRIPTOR ImportDescriptor;
    UNICODE_STRING ImageName;
    PLIST_ENTRY NextListEntry;
    PLDR_DATA_TABLE_ENTRY DataTableEntry;

    ASSERT(XeImageHeader()->ImportDirectory != NULL);

    //
    // Loop over each import descriptor in the import directory.  Stop when we
    // find an import descriptor with a NULL thunk data.
    //

    ImportDescriptor = XeImageHeader()->ImportDirectory;

    while (ImportDescriptor->ImageThunkData != NULL) {

        RtlInitUnicodeString(&ImageName, ImportDescriptor->ImageName);

        //
        // Search the loaded module list for the image name.
        //

        NextListEntry = KdLoadedModuleList.Flink;

        while (NextListEntry != &KdLoadedModuleList) {

            DataTableEntry = CONTAINING_RECORD(NextListEntry, LDR_DATA_TABLE_ENTRY,
                InLoadOrderLinks);

            if (RtlEqualUnicodeString(&ImageName, &DataTableEntry->BaseDllName,
                TRUE)) {

                //
                // Resolve the imports from this module.
                //

                status = XepResolveImageImports(DataTableEntry->DllBase,
                    ImportDescriptor->ImageThunkData);

                if (!NT_SUCCESS(status)) {
                    return status;
                }

                break;
            }

            NextListEntry = DataTableEntry->InLoadOrderLinks.Flink;
        }

        if (NextListEntry == &KdLoadedModuleList) {
            LdrxDbgPrint(("LDRX: cannot import from module %wZ.\n", &ImageName));
            return STATUS_DLL_NOT_FOUND;
        }

        ImportDescriptor++;
    }

    return STATUS_SUCCESS;
}

VOID
XepNotifyDebuggerOfImageLoad(
    VOID
    )
/*++

Routine Description:

    This routine notifies the debugger that an Xbox executable image has been
    loaded and sends the section load notifications for the preload sections.

Arguments:

    ImageFileName - Specifies the name of the Xbox executable image to load.

Return Value:

    Status of operation.

--*/
{
    PVOID BaseAddress;
    KD_SYMBOLS_INFO SymbolInfo;
    STRING SymbolFileName;
    PXBEIMAGE_SECTION Section;
    PXBEIMAGE_SECTION EndingSection;

    //
    // The Xbox executable has NtBaseOfDll set to where the portable executable
    // (PE) headers would have been located given the base address of the first
    // section of this executable.  There may or may not actually be PE headers
    // at this address.  However, we don't allow the base address to be located
    // below the Xbox executable header.
    //

    BaseAddress = XeImageHeader()->NtBaseOfDll;

    if (BaseAddress < (PVOID)XeImageHeader()) {
        BaseAddress = XeImageHeader();
    }

    //
    // Initialize the loader data table for the Xbox executable and attach it to
    // the loaded module list.
    //

    RtlInitUnicodeString(&XepDataTableEntry.FullDllName,
        XeImageHeader()->DebugUnicodeFileName);

    XepDataTableEntry.DllBase = BaseAddress;
    XepDataTableEntry.SizeOfImage = XeImageHeader()->NtSizeOfImage;
    XepDataTableEntry.CheckSum = XeImageHeader()->NtCheckSum;
    XepDataTableEntry.Flags = LDRP_ENTRY_XE_IMAGE;
    XepDataTableEntry.LoadedImports = (PVOID)MAXULONG_PTR;
    XepDataTableEntry.BaseDllName = XepDataTableEntry.FullDllName;

    ExInterlockedInsertTailList(&KdLoadedModuleList,
        &XepDataTableEntry.InLoadOrderLinks);

    //
    // Notify the debugger that the module has been loaded.
    //

    SymbolInfo.BaseOfDll = BaseAddress;
    SymbolInfo.ProcessId = (ULONG_PTR)-1;
    SymbolInfo.SizeOfImage = XeImageHeader()->NtSizeOfImage;
    SymbolInfo.CheckSum = XeImageHeader()->NtCheckSum;

    RtlInitAnsiString(&SymbolFileName, XeImageHeader()->DebugFileName);

    DebugLoadImageSymbols(&SymbolFileName, &SymbolInfo);

    //
    // Notify the debugger that all of the preload sections have been loaded.
    //

    Section = XeImageHeader()->SectionHeaders;
    EndingSection = Section + XeImageHeader()->NumberOfSections;

    while (Section < EndingSection) {

        if (LdrxIsFlagSet(Section->SectionFlags, XBEIMAGE_SECTION_PRELOAD)) {
            DebugService(BREAKPOINT_LOAD_XESECTION, Section, NULL);
        }

        Section++;
    }

    //
    // Allow debugger notifications to be sent from the normal section load and
    // unload code paths.
    //

    XepSendSectionDebugNotifications = TRUE;
}

#endif

VOID
XepExtractCertificateKey(
    IN XBEIMAGE_CERTIFICATE_KEY InputKey,
    OUT PUCHAR OutputKey
    )
/*++

Routine Description:

    This routine extracts the raw data from the supplied input key and stores
    the result in the supplied output key.

Arguments:

    InputKey - Specifies the raw data to use as input for generating the output
        key.

    OutputKey - Specifies the location to receive the output key.

Return Value:

    None.

--*/
{
    UCHAR Digest[XC_DIGEST_LEN];

    //
    // Generate the output key from the input key and the certificate key stored
    // in the ROM.
    //

    XcHMAC(XboxCERTKey, XBOX_KEY_LENGTH, InputKey,
        XBEIMAGE_CERTIFICATE_KEY_LENGTH, NULL, 0, Digest);

    RtlCopyMemory(OutputKey, Digest, XBOX_KEY_LENGTH);
}

VOID
XepExtractCertificateKeys(
    VOID
    )
/*++

Routine Description:

    This routine extracts the raw keys stored in the Xbox executable and stores
    them in global data.

Arguments:

    None.

Return Value:

    None.

--*/
{
    PXBEIMAGE_CERTIFICATE Certificate;
    ULONG Index;

    Certificate = XeImageHeader()->Certificate;

    //
    // Extract the LAN key.
    //

    XepExtractCertificateKey(Certificate->LANKey, XboxLANKey);

    //
    // Extract the signature key.
    //

    XepExtractCertificateKey(Certificate->SignatureKey, XboxSignatureKey);

    //
    // Extract the alternate signature keys.
    //

    for (Index = 0; Index < XBEIMAGE_ALTERNATE_TITLE_ID_COUNT; Index++) {
        XepExtractCertificateKey(Certificate->AlternateSignatureKeys[Index],
            XboxAlternateSignatureKeys[Index]);
    }
}

NTSTATUS
XeLoadImage(
    IN PCOSTR ImageFileName,
    IN BOOLEAN LoadingDashboard,
    IN ULONG SettingsError
    )
/*++

Routine Description:

    This routine loads and verifies an Xbox executable image.

Arguments:

    ImageFileName - Specifies the name of the Xbox executable image to load.

    LoadingDashboard - Specifies TRUE if we're attempting to load the dashboard.

    SettingsError - If non-zero, specifies what settings are invalid and should
        be fixed via the dashboard.  Used as a flag to indicate whether or not
        the executable image needs to be signed to run from the manufacturing
        region.

Return Value:

    Status of operation.

--*/
{
    NTSTATUS status;
    HANDLE ImageFileHandle;
    PXBEIMAGE_HEADER ImageHeader;
    BOOLEAN ReleaseBaseAddress;
    SIZE_T ImageFileNameLength;
    IO_STATUS_BLOCK IoStatusBlock;
    LARGE_INTEGER ByteOffset;
    PVOID BaseAddress;
    SIZE_T RegionSize;
    ULONG HeaderBytesRemaining;
    PVOID EndOfHeaders;
    PULONG ConfoundingKey;
    PXBEIMAGE_CERTIFICATE Certificate;
    ULONG TitleGameRegion;
    ULONG ImageMediaTypes;
    PXBEIMAGE_SECTION Section;
    PXBEIMAGE_SECTION EndingSection;
#ifdef DEVKIT
    PXBEIMAGE_LIBRARY_VERSION LibraryVersion;
    PXBEIMAGE_LIBRARY_VERSION EndingLibraryVersion;
#endif
    BOOLEAN ChangeResetOnTrayOpen;
    ULONG ResetOnTrayOpen;

    ImageFileHandle = NULL;
    ImageHeader = NULL;
    ReleaseBaseAddress = FALSE;

    //
    // Duplicate the image file name.
    //

    ImageFileNameLength = ocslen(ImageFileName) * sizeof(OCHAR);

    ASSERT(ImageFileNameLength <= USHRT_MAX);

    XeImageFileName.Buffer = (POCHAR)ExAllocatePoolWithTag(ImageFileNameLength,
        'nFeX');

    if (XeImageFileName.Buffer == NULL) {
        status = STATUS_INSUFFICIENT_RESOURCES;
        goto CleanupAndExit;
    }

    RtlCopyMemory(XeImageFileName.Buffer, ImageFileName, ImageFileNameLength);
    XeImageFileName.Length = (USHORT)ImageFileNameLength;
    XeImageFileName.MaximumLength = (USHORT)ImageFileNameLength;

#ifdef DEVKIT
    //
    // On DEVKIT systems, we assume that the system is never so messed up that
    // system settings cannot be restored in the dash.  So if we've been called
    // to find something to run instead of the dash when system settings are
    // bad, we won't even bother to do any work.
    //

    if (SettingsError != 0) {
        status = STATUS_OBJECT_PATH_NOT_FOUND;
        goto CleanupAndExit;
    }
#endif

    //
    // Open the image file.
    //

    status = XepOpenImageFile(&ImageFileHandle);

    if (!NT_SUCCESS(status)) {
        goto CleanupAndExit;
    }

    //
    // Allocate a buffer to read the first page of the image header.
    //

    ImageHeader = (PXBEIMAGE_HEADER)ExAllocatePoolWithTag(PAGE_SIZE, 'hIeX');

    if (ImageHeader == NULL) {
        status = STATUS_INSUFFICIENT_RESOURCES;
        goto CleanupAndExit;
    }

    //
    // Read the first page of the image header.
    //

    ByteOffset.QuadPart = 0;

    status = NtReadFile(ImageFileHandle, NULL, NULL, NULL, &IoStatusBlock,
        ImageHeader, PAGE_SIZE, &ByteOffset);

    if (!NT_SUCCESS(status)) {
        goto CleanupAndExit;
    }

    //
    // Verify that we read at least an entire image header.
    //

    if (IoStatusBlock.Information < XBEIMAGE_HEADER_BASE_SIZEOF) {
        LdrxDbgPrint(("LDRX: image too small.\n"));
        status = STATUS_INVALID_IMAGE_FORMAT;
        goto CleanupAndExit;
    }

    //
    // Validate that the signature is correct, that the size of the headers is
    // at least large enough to contain the image header, that the size of the
    // image is greater than the size of the headers, and that the image is
    // based at the expected address.
    //

    if ((ImageHeader->Signature != XBEIMAGE_SIGNATURE) ||
        (ImageHeader->SizeOfHeaders <= XBEIMAGE_HEADER_BASE_SIZEOF) ||
        (ImageHeader->SizeOfHeaders > ImageHeader->SizeOfImage) ||
        (ImageHeader->BaseAddress != (PVOID)XBEIMAGE_STANDARD_BASE_ADDRESS)) {
        LdrxDbgPrint(("LDRX: invalid image header.\n"));
        status = STATUS_INVALID_IMAGE_FORMAT;
        goto CleanupAndExit;
    }

    //
    // Reserve the address space used to map the XBE image.
    //

    BaseAddress = (PVOID)XBEIMAGE_STANDARD_BASE_ADDRESS;
    RegionSize = ImageHeader->SizeOfImage;

    status = NtAllocateVirtualMemory(&BaseAddress, 0, &RegionSize, MEM_RESERVE,
        PAGE_READWRITE);

    if (!NT_SUCCESS(status)) {
        goto CleanupAndExit;
    }

    ReleaseBaseAddress = TRUE;

    //
    // Commit the address space used to map the XBE headers.
    //

    BaseAddress = (PVOID)XBEIMAGE_STANDARD_BASE_ADDRESS;
    RegionSize = ImageHeader->SizeOfHeaders;

    status = NtAllocateVirtualMemory(&BaseAddress, 0, &RegionSize, MEM_COMMIT,
        PAGE_READWRITE);

    if (!NT_SUCCESS(status)) {
        goto CleanupAndExit;
    }

    //
    // Copy the first page of the image header to its actual base address and
    // free the temporary buffer.
    //

    RtlCopyMemory((PVOID)XBEIMAGE_STANDARD_BASE_ADDRESS, ImageHeader, PAGE_SIZE);

    ExFreePool(ImageHeader);
    ImageHeader = NULL;

    //
    // Read in the rest of the image headers if the headers are larger than a
    // single sector.
    //

    if (XeImageHeader()->SizeOfHeaders > PAGE_SIZE) {

        HeaderBytesRemaining = XeImageHeader()->SizeOfHeaders - PAGE_SIZE;

        status = XepReadImageFile(ImageFileHandle,
            (PVOID)(XBEIMAGE_STANDARD_BASE_ADDRESS + PAGE_SIZE),
            HeaderBytesRemaining, PAGE_SIZE);

        if (!NT_SUCCESS(status)) {
            goto CleanupAndExit;
        }
    }

    //
    // Validate that all of the important header data is present and part of the
    // signed headers.
    //

    EndOfHeaders = (PUCHAR)XeImageHeader() + XeImageHeader()->SizeOfHeaders;

    if ((PVOID)XeImageHeader()->Certificate < (PVOID)XeImageHeader() ||
        (PVOID)((ULONG_PTR)XeImageHeader()->Certificate + XBEIMAGE_CERTIFICATE_BASE_SIZEOF) > EndOfHeaders ||
        (PVOID)XeImageHeader()->SectionHeaders < (PVOID)XeImageHeader() ||
        (PVOID)(XeImageHeader()->SectionHeaders + XeImageHeader()->NumberOfSections) >
        EndOfHeaders) {
        status = STATUS_INVALID_IMAGE_FORMAT;
        goto CleanupAndExit;
    }

    //
    // Verify that the encrypted digest stored in the image header is valid.
    //

    status = XepVerifyImageHeaderEncryptedDigest();

    if (!NT_SUCCESS(status)) {
        LdrxDbgPrint(("LDRX: failed to verify header digest.\n"));
        goto CleanupAndExit;
    }

    //
    // Cache the pointer to the certificate and the title game region in locals.
    //

    Certificate = XeImageHeader()->Certificate;
    TitleGameRegion = Certificate->GameRegion;

#ifndef DEVKIT
#ifndef ARCADE
    //
    // Verify that we have a valid locked hard drive unless the title is
    // willing to run without one, or we're in the manufacturing region.
    //

    if (!IdexDiskSecurityUnlocked &&
        LdrxIsFlagClear(XboxGameRegion, XC_GAME_REGION_MANUFACTURING) &&
        LdrxIsFlagClear(Certificate->AllowedMediaTypes,
        XBEIMAGE_MEDIA_TYPE_NONSECURE_HARD_DISK)) {
        IdexDiskFatalError(FATAL_ERROR_HDD_NOT_LOCKED);
    }
#endif

    //
    // Verify that the image isn't attempting to bind to any imports that aren't
    // available in a non-DEVKIT version of the kernel.
    //

    if (XeImageHeader()->ImportDirectory != NULL) {
        LdrxDbgPrint(("LDRX: cannot import from modules other than kernel.\n"));
        status = STATUS_INVALID_IMAGE_FORMAT;
        goto CleanupAndExit;
    }

    //
    // If we have invalid settings, then make sure this image is signed to run
    // in the manufacturing region before proceeding.
    //

    if ((SettingsError != 0) &&
        LdrxIsFlagClear(TitleGameRegion, XBEIMAGE_GAME_REGION_MANUFACTURING)) {
        status = STATUS_OBJECT_PATH_NOT_FOUND;
        goto CleanupAndExit;
    }
#endif

    //
    // Unconfound portions of the header data.
    //

    ConfoundingKey = (PULONG)&XePublicKeyData[128];

    *((PULONG)&XeImageHeader()->AddressOfEntryPoint) ^=
        ConfoundingKey[0] ^ ConfoundingKey[4];
    *((PULONG)&XeImageHeader()->XboxKernelThunkData) ^=
        ConfoundingKey[1] ^ ConfoundingKey[2];

    //
    // Check if the game region is supported by the console.  If the console is
    // configured to run manufacturing content, then only allow manufacturing
    // content to run.
    //

    if (LdrxIsFlagSet(XboxGameRegion, XC_GAME_REGION_MANUFACTURING)) {
        TitleGameRegion &= XBEIMAGE_GAME_REGION_MANUFACTURING;
    }

    if ((TitleGameRegion & XboxGameRegion) == 0) {
        LdrxDbgPrint(("LDRX: game region mismatch; cannot run image.\n"));
        status = STATUS_IMAGE_GAME_REGION_VIOLATION;
        goto CleanupAndExit;
    }

    //
    // Check if the image is allowed to run from this type of media.
    //

    status = XepComputeImageMediaTypes(ImageFileHandle, &ImageMediaTypes);

    if (!NT_SUCCESS(status)) {
        LdrxDbgPrint(("LDRX: failed to compute media types.\n"));
        goto CleanupAndExit;
    }

    if ((Certificate->AllowedMediaTypes & ImageMediaTypes) == 0) {
        LdrxDbgPrint(("LDRX: media types mismatch; cannot run image.\n"));
        status = STATUS_IMAGE_MEDIA_TYPE_VIOLATION;
        goto CleanupAndExit;
    }

    //
    // For images loaded from the CD-ROM device, verify that this executable's
    // title identifier matches the title identifier of previously loaded
    // media.
    //
    // If this is the first title that has run from this media, then remember
    // it's title identifier.
    //

    if ((ImageMediaTypes & (XBEIMAGE_MEDIA_TYPE_DVD_X2 |
        XBEIMAGE_MEDIA_TYPE_DVD_CD)) != 0) {

        if ((XeCdRomMediaTitleID != 0) &&
            (XeCdRomMediaTitleID != Certificate->TitleID)) {
            LdrxDbgPrint(("LDRX: title ID mismatch; cannot run image.\n"));
            status = STATUS_IMAGE_MEDIA_TYPE_VIOLATION;
            goto CleanupAndExit;
        }

        XeCdRomMediaTitleID = Certificate->TitleID;
    }

    //
    // Pass the Microsoft logo embedded in the Xbox executable to the animation
    // code so that it can be displayed as long as possible.
    //

    if (!KeHasQuickBooted) {
        AniSetLogo(XeImageHeader()->MicrosoftLogo, XeImageHeader()->SizeOfMicrosoftLogo);
    }

    //
    // Bring in all of the preload sections.
    //

    Section = XeImageHeader()->SectionHeaders;
    EndingSection = Section + XeImageHeader()->NumberOfSections;

    while (Section < EndingSection) {

        if (LdrxIsFlagSet(Section->SectionFlags, XBEIMAGE_SECTION_PRELOAD)) {

            status = XepLoadSection(ImageFileHandle, Section, TRUE);

            if (!NT_SUCCESS(status)) {
                goto CleanupAndExit;
            }
        }

        Section++;
    }

    //
    // Now that we've loaded a candidate Xbox executable, disable the option of
    // hitting the tray eject button without causing a reboot to occur.  If a
    // tray eject has occurred before this routine is called and we're not
    // loading the dashboard, then pretend like the file wasn't found and fail
    // this image load.
    //
    // Note that if we're loading the dashboard and we're not already in
    // non-secure mode, then this call will switch us to non-secure mode.  This
    // is done to close a window where tray ejects are causing a reboot, but
    // the HAL doesn't yet think that we're going to be running trusted code.
    //

    if (HalEnableTrayEjectRequiresReboot(LoadingDashboard)) {

        ASSERT(!KeHasQuickBooted);
        ASSERT(LdrxIsFlagSet(XboxBootFlags, XBOX_BOOTFLAG_NONSECUREMODE));

        if (!LoadingDashboard) {
            status = STATUS_OBJECT_PATH_NOT_FOUND;
            goto CleanupAndExit;
        }
    }

    //
    // Resolve the image imports from the kernel.
    //

    if (XeImageHeader()->XboxKernelThunkData != NULL) {

        status = XepResolveImageImports(PsNtosImageBase,
            XeImageHeader()->XboxKernelThunkData);

        if (!NT_SUCCESS(status)) {
            goto CleanupAndExit;
        }
    }

#ifdef DEVKIT
    //
    // Resolve the image imports from outside the kernel.
    //

    if (XeImageHeader()->ImportDirectory != NULL) {

        status = XepResolveNonKernelImageImports();

        if (!NT_SUCCESS(status)) {
            goto CleanupAndExit;
        }
    }

    //
    // Notify the debugger that the image has been loaded.
    //
    // Note that this has to be done before marking the section pages as
    // read-only.  The debug monitor may change the attributes of the sections
    // to not be read-only in order to support components like VTune.
    //

    XepNotifyDebuggerOfImageLoad();
#endif

    //
    // Change the protection of all of the preload non-writeable sections to
    // PAGE_EXECUTE_READ.  This is done after resolving the image imports
    // because the image imports are typically merged into the read-only .text
    // section.
    //

    Section = XeImageHeader()->SectionHeaders;
    EndingSection = Section + XeImageHeader()->NumberOfSections;

    while (Section < EndingSection) {

        if (LdrxIsFlagSet(Section->SectionFlags, XBEIMAGE_SECTION_PRELOAD) &&
            LdrxIsFlagClear(Section->SectionFlags, XBEIMAGE_SECTION_WRITEABLE)) {
            XepMarkSectionPagesReadOnly(Section);
        }

        Section++;
    }

#ifdef DEVKIT
    //
    // Print out the path to the Xbox executable that has been loaded.
    //

    DbgPrint("Xbox image loaded: %Z\n", &XeImageFileName);

    //
    // Print out the library versions that this Xbox executable linked against.
    //

    LibraryVersion = XeImageHeader()->LibraryVersions;
    EndingLibraryVersion = LibraryVersion + XeImageHeader()->NumberOfLibraryVersions;

    while (LibraryVersion < EndingLibraryVersion) {
        DbgPrint("This XBE was linked with %8.8s.LIB version %d.%02d.%d.%02d%s\n",
            LibraryVersion->LibraryName, LibraryVersion->MajorVersion,
            LibraryVersion->MinorVersion, LibraryVersion->BuildVersion,
            LibraryVersion->QFEVersion, LibraryVersion->DebugBuild ? " (Debug)" : "");
        LibraryVersion++;
    }

#ifndef ARCADE
    //
    // Check if the title wants extra memory in the development kit to be
    // automatically used for memory manager allocations or not.
    //

    if (LdrxIsFlagClear(XeImageHeader()->InitFlags, XINIT_LIMIT_DEVKIT_MEMORY)) {
        MmReleaseDeveloperKitMemory();
    }
#endif
#endif

    //
    // Extract the raw certificate keys.
    //

    XepExtractCertificateKeys();

    //
    // If we're not loading a title that's enabled to run from the manufacturing
    // content, then zero out the EEPROM key.  The console may not be configured
    // to allow manufacturing content, but we still allow some titles to access
    // the EEPROM key as long as they're marked as being able to run from the
    // manufacturing region.
    //

    if (LdrxIsFlagClear(TitleGameRegion, XBEIMAGE_GAME_REGION_MANUFACTURING)) {
        RtlZeroMemory(XboxEEPROMKey, XBOX_KEY_LENGTH);
    }

    //
    // If we're loading the dashboard, booting a kernel from the CD-ROM (such as
    // recovery), or if we're in the manufacturing region, then switch the SMC
    // to non-secure mode, where tray open events do not force a reboot.  In all
    // other cases, switch the SMC to secure mode.
    //
    // Note that the SMC only allows us to set non-secure mode once and this
    // must be the first override request to the SMC after a power cycle.  Once
    // the SMC is configured to run as secure mode, it cannot be switched back,
    // so this command may have no effect on the actual state of the SMC.
    //

    ResetOnTrayOpen = SMC_RESET_ON_TRAY_OPEN_SECURE_MODE;
    ChangeResetOnTrayOpen = FALSE;

    if (LoadingDashboard ||
        LdrxIsFlagSet(Certificate->AllowedMediaTypes, XBEIMAGE_MEDIA_TYPE_NONSECURE_MODE) ||
        LdrxIsFlagSet(XboxBootFlags, XBOX_BOOTFLAG_CDBOOT) ||
        LdrxIsFlagSet(XboxGameRegion, XC_GAME_REGION_MANUFACTURING)) {

        if (!KeHasQuickBooted &&
            LdrxIsFlagClear(XboxBootFlags, XBOX_BOOTFLAG_NONSECUREMODE)) {
            XboxBootFlags |= XBOX_BOOTFLAG_NONSECUREMODE;
            ResetOnTrayOpen = SMC_RESET_ON_TRAY_OPEN_NONSECURE_MODE;
            ChangeResetOnTrayOpen = TRUE;
        }

    } else {

        if (!KeHasQuickBooted ||
            LdrxIsFlagSet(XboxBootFlags, XBOX_BOOTFLAG_NONSECUREMODE)) {
            XboxBootFlags &= ~XBOX_BOOTFLAG_NONSECUREMODE;
            ChangeResetOnTrayOpen = TRUE;
        }
    }

    if (ChangeResetOnTrayOpen) {

        do {
            status = HalWriteSMBusByte(SMC_SLAVE_ADDRESS,
                SMC_COMMAND_OVERRIDE_RESET_ON_TRAY_OPEN, ResetOnTrayOpen);
        } while (!NT_SUCCESS(status));
    }

#ifdef ARCADE
    //
    // Force all Xbox executables to run with the "no setup hard disk" flag for
    // the arcade build.  This turns off the XAPI startup code that attempts to
    // create title and user directories as well as mounting a utility drive.
    //

    XeImageHeader()->InitFlags |= XINIT_NO_SETUP_HARD_DISK;
#endif

    //
    // The image is successfully loaded.
    //

    status = STATUS_SUCCESS;

CleanupAndExit:
    //
    // Free the temporary buffer used to hold the first sector of the image
    // header.
    //

    if (ImageHeader != NULL) {
        ExFreePool(ImageHeader);
    }

    //
    // Close the image file handle.
    //

    if (ImageFileHandle != NULL) {
        NtClose(ImageFileHandle);
    }

    if (!NT_SUCCESS(status)) {

        //
        // Release the virtual address range used to map the XBE image.
        //

        if (ReleaseBaseAddress) {

            BaseAddress = (PVOID)XBEIMAGE_STANDARD_BASE_ADDRESS;
            RegionSize = 0;

            NtFreeVirtualMemory(&BaseAddress, &RegionSize, MEM_RELEASE);
        }

        //
        // Delete the image file name string.  Note that if
        // XeImageFileName.Buffer is NULL, then this call does nothing.
        //

        RtlFreeObjectString(&XeImageFileName);
    }

    return status;
}

#ifdef DEVKIT

NTSTATUS
XeLoadAlternateDashboardImage(
    VOID
    )
/*++

Routine Description:

    This routine attempts to load the dashboard as specified in the dashboard
    redirection file.

Arguments:

    None.

Return Value:

    Status of operation.

--*/
{
    NTSTATUS status;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;
    HANDLE RedirectionFileHandle;
    OCHAR RedirectionBuffer[MAX_LAUNCH_PATH + 1];
    POSTR Delimiter;

    //
    // Attempt to open the dashboard redirection file.
    //

    InitializeObjectAttributes(&ObjectAttributes, &XepDashboardRedirectionPath,
        OBJ_CASE_INSENSITIVE, NULL, NULL);

    status = NtOpenFile(&RedirectionFileHandle, GENERIC_READ, &ObjectAttributes,
        &IoStatusBlock, FILE_SHARE_READ, FILE_SYNCHRONOUS_IO_NONALERT |
        FILE_NON_DIRECTORY_FILE);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //
    // Read in the dashboard redirection file and close the handle to the file.
    //

    status = NtReadFile(RedirectionFileHandle, NULL, NULL, NULL, &IoStatusBlock,
        RedirectionBuffer, MAX_LAUNCH_PATH * sizeof(OCHAR), NULL);

    NtClose(RedirectionFileHandle);

    if (!NT_SUCCESS(status)) {
        return status;
    } else if (IoStatusBlock.Information != (MAX_LAUNCH_PATH * sizeof(OCHAR))) {
        return STATUS_FILE_CORRUPT_ERROR;
    }

    //
    // Search for the title path delimiter and replace it with a backslash.  The
    // first portion of the string used to specify the object manager path for
    // the D: symbolic link, but the dashboard and XDK launcher don't require
    // that drive letter to be set up, so we no longer do that.
    //

    RedirectionBuffer[MAX_LAUNCH_PATH] = OTEXT('\0');

    Delimiter = ocschr(RedirectionBuffer, TITLE_PATH_DELIMITER);

    if (Delimiter != NULL) {
        *Delimiter = OTEXT('\\');
    }

    //
    // Attempt to load the alternate dashboard image.
    //

    status = XeLoadImage(RedirectionBuffer, TRUE, 0);

    return status;
}

#endif

VOID
XeLoadDashboardImage(
    VOID
    )
/*++

Routine Description:

    This routine attempts to load the dashboard from the shell partition.  This
    routine will not return if the dashboard cannot be loaded.

Arguments:

    None.

Return Value:

    None.

--*/
{
    NTSTATUS status;
    ULONG FatalErrorCode;
    PLD_LAUNCH_DASHBOARD LaunchDashboard;

#ifdef DEVKIT
    //
    // Tell the debug monitor that we're going to be running the dashboard.
    //

    if (DmGetCurrentDmi() != NULL) {
        DmGetCurrentDmi()->Flags |= DMIFLAG_RUNSHELL;
    }

    //
    // Attempt to load the dashboard as specified in the dashboard redirection
    // file and return if successful.
    //

    status = XeLoadAlternateDashboardImage();

    if (NT_SUCCESS(status)) {
        return;
    }
#endif

    //
    // Attempt to load the standard dashboard image and return if successful.
    //

    status = XeLoadImage(XepDashboardImagePath, TRUE, 0);

    if (!NT_SUCCESS(status)) {

        LdrxDbgPrint(("INIT: Failed to launch an XBE (status=%08x).\n", status));
        LdrxDbgBreakPoint();

        //
        // Display the universal error message.
        //

        if (!KeHasQuickBooted) {

            HalWriteSMCLEDStates(SMC_LED_STATES_GREEN_STATE0 | SMC_LED_STATES_RED_STATE1 |
                SMC_LED_STATES_RED_STATE2 | SMC_LED_STATES_RED_STATE3);

            FatalErrorCode = FATAL_ERROR_XBE_DASH_GENERIC;

            //
            // If the launch data is available and there is a dashboard error,
            // change the fatal error code to indicate the dashboard error.
            // Otherwise, indicate if DVD-X2 authentication has passed.
            //

            if (LaunchDataPage != NULL &&
                LaunchDataPage->Header.dwLaunchDataType == LDT_LAUNCH_DASHBOARD) {

                LaunchDashboard = (PLD_LAUNCH_DASHBOARD)&(LaunchDataPage->LaunchData[0]);

                FatalErrorCode += LaunchDashboard->dwReason;

            } else {

#ifndef ARCADE
                if (IdexCdRomDVDX2Authenticated) {
                    FatalErrorCode = FATAL_ERROR_XBE_DASH_X2_PASS;
                }
#endif
            }

            ExDisplayFatalError(FatalErrorCode);

        } else {
            HalReturnToFirmware(HalFatalErrorRebootRoutine);
        }
    }
}

VOID
XeLoadDashboardImageWithReason(
    IN ULONG Reason,
    IN ULONG Parameter1
    )
/*++

Routine Description:

    This routine attempts to load the dashboard from the shell partition.  The
    dashboard is given a launch data page with the supplied reason and parameter
    codes.  This routine will not return if the dashboard cannot be loaded.

Arguments:

    Reason - Specifies the reason that the dashboard is being loaded.

    Parameter1 - Specifies more information about the reason that the dashboard
        is being loaded.

Return Value:

    None.

--*/
{
    PLD_LAUNCH_DASHBOARD LaunchDashboard;

    //
    // If a launch data page is not already allocated, then allocate a new page.
    //

    if (LaunchDataPage == NULL) {
        LaunchDataPage = MmAllocateContiguousMemory(PAGE_SIZE);
    }

    //
    // If a launch data page is available, then zero it out and fill it in with
    // supplied error codes.  If there's not enough memory to obtain a launch
    // data page, then we'll still try to launch the dashboard, but we'll likely
    // fail and end up displaying the universal error message.
    //

    if (LaunchDataPage != NULL) {

        MmPersistContiguousMemory(LaunchDataPage, PAGE_SIZE, TRUE);

        RtlZeroMemory(LaunchDataPage, PAGE_SIZE);

        LaunchDataPage->Header.dwLaunchDataType = LDT_LAUNCH_DASHBOARD;

        LaunchDashboard = (PLD_LAUNCH_DASHBOARD)&(LaunchDataPage->LaunchData[0]);
        LaunchDashboard->dwReason = Reason;
        LaunchDashboard->dwParameter1 = Parameter1;
    }

    //
    // Attempt to load the dashboard.  This will not return if the dashboard
    // cannot be loaded.
    //

    XeLoadDashboardImage();
}

VOID
XeLoadTitleImage(
    IN ULONG SettingsError
    )
/*++

Routine Description:

    This routine attempts to load a title as by either using the launch data
    page or the default Xbox executable path.  If the title load fails, then the
    dashboard is loaded instead.  This routine will not return if no image can
    be loaded.

Arguments:

    SettingsError - If non-zero, specifies what settings are invalid and should
        be fixed via the dashboard.  However, instead of immediately loading the
        dashboard, a check is made to see if a title can be loaded from the
        default Xbox executable path.  If a title can be loaded and the title
        is enabled to run from the manufacturing region, then that title is
        allowed to run.

Return Value:

    None.

--*/
{
    NTSTATUS status;
    OCHAR CapturedLaunchPath[MAX_LAUNCH_PATH + 1];
    PLAUNCH_DATA_PAGE SavedLaunchDataPage;
    POSTR Delimiter;
    OBJECT_STRING LinkTarget;
    BOOLEAN CreatedSymbolicLink;
    ULONG TrayState;
    ULONG TrayStateChangeCount1;
    ULONG TrayStateChangeCount2;
    ULONG DashboardError;

    //
    // Check to see if there is launch data has already been read by the kernel
    // during a previous reboot, and if so, free it now and ignore the data.
    //

    if ((SettingsError == 0) && (LaunchDataPage != NULL) &&
        LdrxIsFlagSet(LaunchDataPage->Header.dwFlags, LDF_HAS_BEEN_READ)) {
        SavedLaunchDataPage = LaunchDataPage;
        LaunchDataPage = NULL;
        MmFreeContiguousMemory(SavedLaunchDataPage);
    }

    //
    // Check if there's a settings error.  If so, then force a load from the
    // default Xbox executable path and ignore the launch data page (this should
    // only occur from a cold boot, so the launch data page is probably NULL).
    //
    // Check if there's a launch data page available.  If so, this page will
    // specify what Xbox executable image should be loaded.
    //

    if ((SettingsError == 0) && (LaunchDataPage != NULL)) {

        //
        // Immediately mark the launch data as having been read by the kernel.
        //

        LaunchDataPage->Header.dwFlags |= LDF_HAS_BEEN_READ;

        //
        // Copy out the launch path and ensure that the buffer is
        // null-terminated.
        //

        RtlCopyMemory(CapturedLaunchPath, LaunchDataPage->Header.szLaunchPath,
            MAX_LAUNCH_PATH * sizeof(OCHAR));
        CapturedLaunchPath[MAX_LAUNCH_PATH] = OTEXT('\0');

        //
        // Free the launch data page now if it only specified a launch path.
        //

        if (LaunchDataPage->Header.dwLaunchDataType == LDT_NONE) {
            SavedLaunchDataPage = LaunchDataPage;
            LaunchDataPage = NULL;
            MmFreeContiguousMemory(SavedLaunchDataPage);
        }

        //
        // If the launch path is empty, then this is an alias for loading the
        // dashboard.
        //

        if (CapturedLaunchPath[0] == OTEXT('\0')) {
            XeLoadDashboardImage();
            return;
        }

        //
        // Check for a path delimiter in the launch path.  If present, then the
        // first part of the string is the object manager path to use for the D:
        // symbolic link.  The second half of the string is a path relative to
        // the first half of the string.
        //
        // If no delimiter is present, then this is an absolute object manager
        // path and no D: symbolic link is created.
        //

        CreatedSymbolicLink = FALSE;
        Delimiter = ocschr(CapturedLaunchPath, TITLE_PATH_DELIMITER);

        if (Delimiter != NULL) {

            *Delimiter = OTEXT('\\');

            LinkTarget.Buffer = CapturedLaunchPath;
            LinkTarget.Length = (USHORT)(Delimiter - CapturedLaunchPath);
            LinkTarget.MaximumLength = LinkTarget.Length;

            status = IoCreateSymbolicLink(&XepCdRomDosDevice, &LinkTarget);

            if (NT_SUCCESS(status)) {
                CreatedSymbolicLink = TRUE;
            }
        }

        //
        // Attempt to load the specified image and return if successful.
        //

        status = XeLoadImage(CapturedLaunchPath, FALSE, 0);

        if (NT_SUCCESS(status)) {
            return;
        }

        //
        // Delete the symbolic link created above.
        //

        if (CreatedSymbolicLink) {
            IoDeleteSymbolicLink(&XepCdRomDosDevice);
        }

    } else {

        //
        // Read the initial tray state change count so that we can check below
        // if anything has changed.
        //

        HalReadSMCTrayState(&TrayState, &TrayStateChangeCount1);

        //
        // Attempt to load the default image and return if successful.  Note
        // that this can return an error if there is a settings error and the
        // default image isn't signed to run from the manufacturing region.
        //

        status = XeLoadImage(XepDefaultImagePath, FALSE, SettingsError);

        if (NT_SUCCESS(status)) {
            return;
        }

        //
        // If there's a settings error and the default title couldn't be found
        // or wasn't signed to run from the manufacturing region, then launch
        // the dashboard to handle the settings error.
        //

        if (SettingsError != 0) {
            XeLoadDashboardImageWithReason(XLD_LAUNCH_DASHBOARD_SETTINGS,
                SettingsError);
            return;
        }

        //
        // We failed to load the default image; check if it's due to the tray
        // state changing while we were attempting to load that image.  If so,
        // then normalize the error so that we don't disable an invalid XBE
        // message just because the user ejected the tray while we were reading
        // from the file.  Also, if the tray appears to be empty, then don't
        // send device errors to the dashboard.
        //

        HalReadSMCTrayState(&TrayState, &TrayStateChangeCount2);

        if (TrayState != SMC_TRAY_STATE_MEDIA_DETECT) {
            status = STATUS_NO_MEDIA_IN_DEVICE;
        } else if (TrayStateChangeCount1 != TrayStateChangeCount2) {
            status = STATUS_DEVICE_NOT_READY;
        }

        //
        // If this is a non-critical error, such as the drive being empty or
        // containing media without a title, then simply launch the dashboard
        // with no error code.
        //

        switch (status) {

            case STATUS_OBJECT_NAME_NOT_FOUND:
            case STATUS_OBJECT_PATH_NOT_FOUND:
            case STATUS_NO_MEDIA_IN_DEVICE:
            case STATUS_INVALID_DEVICE_REQUEST:
            case STATUS_UNRECOGNIZED_VOLUME:
            case STATUS_DEVICE_NOT_READY:
            case STATUS_NONEXISTENT_SECTOR:
                XeLoadDashboardImage();
                return;
        }
    }

    //
    // We failed to load a title.  Convert the error code to a dashboard error
    // code and load the dashboard to handle the error.
    //

    ASSERT(!NT_SUCCESS(status));

    LdrxDbgPrint(("LDRX: failed to load title image (status=%08x).\n", status));

    switch (status) {

        case STATUS_IMAGE_GAME_REGION_VIOLATION:
            DashboardError = XLD_ERROR_XBE_REGION;
            break;

        case STATUS_IMAGE_MEDIA_TYPE_VIOLATION:
            DashboardError = XLD_ERROR_XBE_MEDIA_TYPE;
            break;

        default:
            DashboardError = XLD_ERROR_INVALID_XBE;
            break;
    }

    XeLoadDashboardImageWithReason(XLD_LAUNCH_DASHBOARD_ERROR, DashboardError);
}

VOID
XeLoadImageAfterTrayEjectBoot(
    VOID
    )
/*++

Routine Description:

    This routine is called when the console was either powered on by hitting the
    tray eject button or if the tray eject button is hit shortly after hitting
    the power on button.

Arguments:

    None.

Return Value:

    None.

--*/
{
    NTSTATUS status;
    ULONG TrayState;
    LARGE_INTEGER Interval;

    //
    // Block until the boot animation has completed and is ready to display the
    // Microsoft logo.
    //
    // Note that the time that it typically takes for the tray to eject, for
    // media to be put in the drive, the tray closed, and for the media to be
    // detected by the drive is approximately the length required to display the
    // boot animation, so we don't bother blocking on a media detected event.
    //

    AniBlockOnAnimation();

    for (;;) {

        //
        // Read the tray state from the SMC.
        //
        // Note that we can't use HalReadSMCTrayState here because that routine
        // can't tell us if the tray is in process of being closed.
        //

        status = HalReadSMBusByte(SMC_SLAVE_ADDRESS, SMC_COMMAND_TRAY_STATE,
            &TrayState);

        if (!NT_SUCCESS(status)) {
            TrayState = SMC_TRAY_STATE_OPEN;
            break;
        }

        //
        // Check if the tray is in the process of closing or detecting media.
        // If we're in either of these states, then keep looping until we're out
        // of these states.
        //

        TrayState &= SMC_TRAY_STATE_STATE_MASK;

        if ((TrayState != SMC_TRAY_STATE_CLOSING) &&
            (TrayState != SMC_TRAY_STATE_CLOSED)) {
            break;
        }

        //
        // Delay for 250 milliseconds.
        //

        Interval.QuadPart = -250 * 10000;
        KeDelayExecutionThread(KernelMode, FALSE, &Interval);
    }

    //
    // If media was detected, then attempt to load a title image, otherwise load
    // the dashboard image.  Note that we might need to dismount the CD-ROM
    // device, especially for DEVKIT systems, if a file system was mounted
    // before the XBOX_BOOTFLAG_TRAYEJECT flag was checked by the calling
    // routine.
    //

    if (TrayState == SMC_TRAY_STATE_MEDIA_DETECT) {
        IoDismountVolume(IdexCdRomDeviceObject);
        XeLoadTitleImage(0);
    } else {
        XeLoadDashboardImage();
    }
}
