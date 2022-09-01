/*++

Copyright (c) 1989-2001  Microsoft Corporation

Module Name:

    rombld.cpp

Abstract:


--*/


#include <string.h>
#include "rombld.h"
#include "util.h"
#include <bldr.h>
#include <xcrypt.h>
#include <sha.h>
#include <conio.h>

#include "enckey.h"

void RomHash(PBYTE pbStart, PBYTE pbEnd, PBYTE pbHash);

//
// Main object for tracking global state
//
CRomBuilder g_ib;

//
// Property names
//
#define PROP_HELP            TEXT("Help")
#define PROP_CONFIG          TEXT("Config")
#define PROP_OUTPUT          TEXT("Output")
#define PROP_OUTPUTEXT       TEXT("OutputExt")
#define PROP_BOOTLDR         TEXT("Bldr")
#define PROP_KERNEL          TEXT("Kernel")
#define PROP_INITTBL         TEXT("InitTbl")
#define PROP_ROMDEC          TEXT("RomDec")
#define PROP_ENCROM          TEXT("EncRomDec")
#define PROP_PRELDR          TEXT("PreLdr")
#define PROP_KEYPATH         TEXT("KeyPath")
#define PROP_SYS             TEXT("Sys")

//
// Command table. NOTE: Must be sorted by lpCmdLine
//
CMDTABLE CmdTable[] =
{
    {"?",            PROP_HELP,         CMDTABLE::VAR_NONE,       NULL},
    {"BLDR:",        PROP_BOOTLDR,      CMDTABLE::VAR_NONE,       NULL},
    {"CONFIG:",      PROP_CONFIG,       CMDTABLE::VAR_NONE,       NULL},
    {"ENCROM:",      PROP_ENCROM,       CMDTABLE::VAR_NONE,       NULL},
    {"HACKINITTBL:", "",                CMDTABLE::VAR_INT,        &g_ib.m_HackInitTable},
    {"INITTBL:",     PROP_INITTBL,      CMDTABLE::VAR_NONE,       NULL},
    {"KERNEL:",      PROP_KERNEL,       CMDTABLE::VAR_NONE,       NULL},
    {"KEYPATH:",     PROP_KEYPATH,      CMDTABLE::VAR_NONE,       NULL},
    {"OUT:",         PROP_OUTPUT,       CMDTABLE::VAR_NONE,       NULL},
    {"OUTEXT:",      PROP_OUTPUTEXT,    CMDTABLE::VAR_NONE,       NULL},
    {"PRELDR:",      PROP_PRELDR,       CMDTABLE::VAR_NONE,       NULL},
    {"ROMDEC:",      PROP_ROMDEC,       CMDTABLE::VAR_NONE,       NULL},
    {"SIZEK:",       "",                CMDTABLE::VAR_INT,        &g_ib.m_nRomSize},
    {"SYS:",         PROP_SYS,          CMDTABLE::VAR_NONE,       NULL},
    {"V:",           "",                CMDTABLE::VAR_INT,        &g_ib.m_nTraceLevel}
};


#define ROM_VERSION_OFFSET          30 // In DWORDs (0x78 is the absolute offset)
#define ROM_VERSION_BYTE_OFFSET     0x78
#define ROM_VERSION_KDDELAY_FLAG    0x80000000


//
// Interim public key
//
UCHAR g_PublicKeyData[XC_PUBLIC_KEYDATA_SIZE] = {
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



//
// CRomBuilder class
//

void
CRomBuilder::PrintUsage()
{
        fprintf(stderr,
        "Usage: rombld [switches] [cfgfile]\n\n"
        "  /OUT:outfile\t\tOutput ROM image file (Required)\n"
        "  /OUTEXT:outfile2\tAlso output external ROM image file\n"
        "  /BLDR:file\t\t16-bit boot loader file (Required)\n"
        "  /PRELDR:file\t\tboot preloader file (Required)\n"
        "  /KERNEL:file\t\tNtoskrnl file (Required)\n"
        "  /INITTBL:file\t\tInittbl file (Required)\n"
        "  /SYS:{XDK|XM3|XM3P}\tType of system (Required)\n"
        "  /ROMDEC:file\t\tRomdec file (Required)\n"
        "  /ENCROM:file\t\tEncrypted romdec file (Required for XM3P)\n"
        "  /KEYPATH:dir\t\tPath to keys\n"
        "  /HACKINITTBL:{0|1}\tAllow manually editing init table\n"
        "  /SIZEK:size\t\tTotal ROM size (default=256)\n"
        "  /CONFIG:cfgfile\tConfiguration file (read switches from file)\n"
        "  /V:{0|1|2|3}\t\tVerbose level (0=quiet, default=1)\n"
        "  /?\t\t\tDisplay this message\n"
        "\n");
}



void
CRomBuilder::Initialize(
    int argc,
    char** argv
    )
{
    printf("Microsoft (R) ROMBLD - MICROSOFT CONFIDENTIAL\n"
        "Copyright (C) Microsoft Corporation 2000-2001. All rights reserved.\n\n");

    //
    // Read switches from command line
    //
    for (int i = 1 ; i < argc ; i++)
    {
        if (*argv[i] == '-' || *argv[i] == '/')
        {
           ParseSwitch(argv[i]);
        }
        else
        {
            //
            // not a switch. See if we already have an exe file. If we do, this is an error
            //
            if (g_ib.m_prop.Get(PROP_CONFIG))
            {
                    g_ib.PrintUsage();
                    g_ib.Exit(-1, "Only one config file can be specified at a time");
            }
            g_ib.m_prop.Set(PROP_CONFIG, argv[i]);

        }
    }

    //
    // If a config file is specified, read switches from it
    //
    if (g_ib.m_prop.Get(PROP_CONFIG))
    {
        ReadSwitchesFromFile(g_ib.m_prop.Get(PROP_CONFIG));
    }

}


void
CRomBuilder::ParseSwitch(
    LPCTSTR pszArg
    )
{
    LPCTSTR lpKey;
    CMDTABLE* lpCommand;


    // Search for the command in the command table, bail if we don't find it
    lpKey = pszArg + 1;
    lpCommand = (CMDTABLE*)bsearch((char*)&lpKey, (char*)&CmdTable[0],
        ARRAYSIZE(CmdTable), sizeof (CMDTABLE),
        CMDTABLE::Compare);
    if (!lpCommand)
    {
        g_ib.PrintUsage();
        ERROR_OUT("Invalid switch '%s'", lpKey);
        g_ib.Exit(-1, NULL);
    }

    // Note that the CRT handles quoted (")
    // command line arguments and puts them in argv correctly and strips the quotes
    lpKey = pszArg + strlen(lpCommand->lpCmdLine) + 1;

    //
    // If a property name is specified, add the command and value as a property.
    //
    if (lpCommand->lpPropName[0] != '\0')
    {
        g_ib.m_prop.Set(lpCommand->lpPropName, lpKey);
    }

    // 
    // If there's a variable, set it
    //
    if (lpCommand->lpVar && lpCommand->vartype != CMDTABLE::VAR_NONE)
    {
        switch (lpCommand->vartype)
        {
            case CMDTABLE::VAR_STRING:
                *(char**)lpCommand->lpVar = (char*)lpKey;
                break;

            case CMDTABLE::VAR_INT:
                *(int*)lpCommand->lpVar = atoi(lpKey);
                break;

            case CMDTABLE::VAR_STRINGLIST:
                {
                    CStrNode* pStrNode;

                    pStrNode = new CStrNode;
                    pStrNode->SetValue(lpKey);

                    // 
                    // Add it to the list
                    //
                    pStrNode->Link((CListHead*)(lpCommand->lpVar), NULL);
                }
                break;
        } 
    }
}



void
CRomBuilder::ReadSwitchesFromFile(
    LPCSTR pszFN
    )
{
    FILE* f;
    char szLine[255];
    int ret;

    f = fopen(pszFN, "r");
    if (f != NULL)
    {
        while (!feof(f))
                {
            ret = fscanf(f, "%s", szLine);
            if (ret == 0 || ret == EOF)
                break;

                    if (szLine[0] == '-' || szLine[0] == '/')
                    {
                ParseSwitch(szLine);
                    }
            else
            {
                g_ib.Exit(-1, "Invalid entry in config file");
            }
        }
        fclose(f);
    }
    else
    {
        g_ib.Exit(-1, "Could not open config file");
    }

}



void
CRomBuilder::Exit(
    int nReturnCode,
    LPCSTR lpszFatalError
    )
{
    //
    // Spew the final messages if any
    //

    TRACE_OUT(TRACE_ALWAYS, "");

    for (CNode* pNode = m_Warnings.GetHead(); pNode != NULL; pNode = pNode->Next())
    {
        CStrNode* pStr = (CStrNode*)pNode;

        WARNING_OUT(pStr->GetValue());
    }

    if (lpszFatalError)
    {
        ERROR_OUT(lpszFatalError);
    }
        if (m_pszExitMsg)
    {
        ERROR_OUT(m_pszExitMsg);

        // 
        // If exit msg was specified, force return code to be -1
        //
        nReturnCode = -1;
    }


    if (nReturnCode == -1)
    {
        //
        // We had a fatal error, delete the out files because it is invalid
        //
        if (m_prop.Get(PROP_OUTPUT) != NULL)
        {
            DeleteFile(m_prop.Get(PROP_OUTPUT));
        }
        if (m_prop.Get(PROP_OUTPUTEXT) != NULL)
        {
            DeleteFile(m_prop.Get(PROP_OUTPUTEXT));
        }
    }


    exit(nReturnCode);
}


void
CRomBuilder::SetExitMsg(
    HRESULT hr,
    LPCSTR pszExitMsg
    )
{
    LPSTR pszMsg;
    char szMsg[512];
    const char szNULL[] = "NULL";
    int l;

    if (pszExitMsg == NULL)
    {
        pszExitMsg = szNULL;
    }

    if (hr == E_UNEXPECTED)
    {
        sprintf(szMsg, "%s (Unexpected error occurred)", pszExitMsg);
    }
    else
    {
        if (FormatMessage(
                FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL,
                hr,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPSTR)&pszMsg,
                0,
                NULL) != 0)
        {
            sprintf(szMsg, "%s (%s", pszExitMsg, pszMsg);

            LocalFree(pszMsg);

            // strip the end of line characters
            l = strlen(szMsg);
            if (l >= 2 && szMsg[l - 2] == '\r')
            {
                        szMsg[l - 2] = '\0';
            }

            strcat(szMsg, ")");
        }
        else
        {
            sprintf(szMsg, "%s", pszExitMsg);
        }
    }

    SetExitMsg(szMsg);
}


void
CRomBuilder::SetExitMsg(
    LPCSTR pszMsg
    )
{
    m_pszExitMsg = _strdup(pszMsg);
}



void
CRomBuilder::TraceOut(
    int nLevel,
    LPCSTR lpszFormat,
    va_list vararg
    )
{
    char szOutput[4096];

    if (nLevel > g_ib.m_nTraceLevel)
        return;

    vsprintf((char*)szOutput, (char*)lpszFormat, vararg);

    printf(TEXT("%s\n"), szOutput);
}


void
CRomBuilder::ErrorOut(
    BOOL bErr,
        LPCSTR lpszFormat,
        va_list vararg
        )
{
        char szOutput[4096];

        vsprintf((char*)szOutput, (char*)lpszFormat, vararg);

    if (bErr)
    {
        fprintf(stderr, "ROMBLD : Error : %s\n", szOutput);
    }
    else
    {
        fprintf(stderr, "ROMBLD : Warning : %s\n", szOutput);
    }
}


void
CRomBuilder::AddWarning(
    LPCSTR pszFormat, ...
    )
{
    CStrNode* pStrNode;
        char szBuf[512];
        va_list ArgList;

        va_start(ArgList, pszFormat);
        wvsprintf(szBuf, pszFormat, ArgList);
        va_end(ArgList);

    pStrNode = new CStrNode;
    pStrNode->SetValue(szBuf);

    // add it to the list
    pStrNode->Link(&m_Warnings, NULL);

}


BOOL
CRomBuilder::LocateDataSection(
    CMemFile &PEFile,
    LPVOID *ppvDataSectionStart,
    DWORD *pdwDataSectionSize
    )
{
    LPVOID ImageBase;
    PIMAGE_NT_HEADERS NtHeader;
    ULONG SectionIndex;
    PIMAGE_SECTION_HEADER SectionHeader;
    LPBYTE pbDataSectionStart;
    DWORD dwDataSectionSize;
    DWORD dwUninitializedDataSectionSize;
    PXDATA_SECTION_HEADER DataSectionHeader;

    ImageBase = PEFile.Ptr();
    NtHeader = PEFile.NtHeaders();

    for (SectionIndex = 0; SectionIndex < NtHeader->FileHeader.NumberOfSections;
        SectionIndex++) {

        SectionHeader = IMAGE_FIRST_SECTION(NtHeader) + SectionIndex;

        if (((*(PULONG)SectionHeader->Name) == 'tad.') &&
            (SectionHeader->SizeOfRawData >= SectionHeader->Misc.VirtualSize)) {

            pbDataSectionStart = (LPBYTE)ImageBase + SectionHeader->PointerToRawData;
            dwDataSectionSize = SectionHeader->Misc.VirtualSize;

            //
            // The linker ends up merging .bss at the end of the .data section.
            // The ROM doesn't need to hold this zero space, so figure out the
            // last non-zero byte in the section.
            //
            // If we end up taking more bytes than were actually in the original
            // .bss, then great, the .data section had some initialized zero
            // bytes that we were able to strip out.
            //

            while (dwDataSectionSize > 0) {
                if (pbDataSectionStart[dwDataSectionSize - 1] != 0) {
                    break;
                }
                dwDataSectionSize--;
            }

            //
            // Round up to a DWORD boundary.
            //

            dwDataSectionSize = (dwDataSectionSize + sizeof(DWORD) - 1) & (~(sizeof(DWORD) - 1));

            dwUninitializedDataSectionSize = SectionHeader->Misc.VirtualSize - dwDataSectionSize;
            dwUninitializedDataSectionSize = (dwUninitializedDataSectionSize + sizeof(DWORD) - 1) & (~(sizeof(DWORD) - 1));

            //
            // We've got 10 reserved WORDs in the old DOS header that we'll take
            // over to pass the relative offset to the .data section, its size,
            // and the size of the .data section.
            //

            DataSectionHeader = (PXDATA_SECTION_HEADER)&((PIMAGE_DOS_HEADER)ImageBase)->e_res2;
            DataSectionHeader->SizeOfInitializedData = dwDataSectionSize;
            DataSectionHeader->SizeOfUninitializedData = dwUninitializedDataSectionSize;
            DataSectionHeader->PointerToRawData = 0 - ROM_DEC_SIZE - BLDR_BLOCK_SIZE - dwDataSectionSize;
            DataSectionHeader->VirtualAddress = NtHeader->OptionalHeader.ImageBase + SectionHeader->VirtualAddress;

            *ppvDataSectionStart = pbDataSectionStart;
            *pdwDataSectionSize = dwDataSectionSize;

            return TRUE;
        }
    }

    return FALSE;
}


PVOID
CRomBuilder::LocateExportOrdinal(
    CMemFile& PEFile,
    ULONG Ordinal
    )
{
    PULONG Addr;
    ULONG i;
    ULONG DirectoryAddress;
    PIMAGE_SECTION_HEADER NtSection = 0;
    PVOID Func = NULL;
    PIMAGE_EXPORT_DIRECTORY ExportDirectory = NULL;
    USHORT DirectoryEntry = IMAGE_DIRECTORY_ENTRY_EXPORT;
    ULONG ExportSize = 0;
    PVOID DllBase = PEFile.Ptr();
    PIMAGE_NT_HEADERS NtHeaders = PEFile.NtHeaders();

    DirectoryAddress = NtHeaders->OptionalHeader.DataDirectory[DirectoryEntry].VirtualAddress;

    if (DirectoryAddress != NULL) {
    
        ExportSize = NtHeaders->OptionalHeader.DataDirectory[DirectoryEntry].Size;
        
        NtSection = (PIMAGE_SECTION_HEADER)((ULONG)NtHeaders + sizeof(ULONG) + sizeof(IMAGE_FILE_HEADER) +
            NtHeaders->FileHeader.SizeOfOptionalHeader);
    
        for (i = 0; i < (ULONG)NtHeaders->FileHeader.NumberOfSections; i++) {
            if (DirectoryAddress >= NtSection->VirtualAddress &&
                DirectoryAddress <= NtSection->VirtualAddress + NtSection->SizeOfRawData) {
                
                ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)
                    ((ULONG)DllBase + (DirectoryAddress - NtSection->VirtualAddress) + NtSection->PointerToRawData);
                break;
            }
            ++NtSection;
        }
    }


    if (ExportDirectory) {

        Addr = (PULONG)((PCHAR)DllBase + (ULONG)ExportDirectory->AddressOfFunctions - 
            NtSection->VirtualAddress + NtSection->PointerToRawData);
        
        Ordinal -= ExportDirectory->Base;

        if (Ordinal < ExportDirectory->NumberOfFunctions) {
            Func = (PVOID)((ULONG_PTR)DllBase + Addr[Ordinal]);
        }
    }

    return Func;
}





static void GetPasswd(LPCSTR szPrompt, LPSTR sz, int cchMax)
{
    char ch;
    int ich = 0;

    _cputs(szPrompt);
    for(;;) {
        ch = (char)_getch();
        switch(ch) {
        case 8:
            if(ich)
                --ich;
            break;
        case 10:
        case 13:
            sz[ich] = 0;
            _putch('\r');
            _putch('\n');
            return;
        default:
            if(ich < cchMax)
                sz[ich++] = ch;
            break;
        }
    }
}

void
CRomBuilder::BuildImage(
    BOOL External
    )
{

    char szCompKernelFile[MAX_PATH];
    CMemFile LdrFile;
    CMemFile PreldrFile;
    CMemFile UncompressedKernelFile;
    LPCSTR szOutputName;
    LPVOID pvDataSectionStart;
    DWORD dwDataSectionSize;
    CMemFile KernelFile;
    CMemFile RomDec;
    CMemFile EncRomDec;
    CMemFile InitTbl;
    PUSHORT pwKernelVer;
    PBYTE pbRomdec = NULL;
    XBOOT_PARAM BootParam;
    HANDLE hWriteFile = INVALID_HANDLE_VALUE;
    HANDLE hWriteMap = NULL;
    PBYTE pbOutput = NULL;
    PBYTE pbOutputBldr;
    PBYTE pbOutputData;
    PBYTE pbOutputKernel;
    PBYTE pbOutputPadding;
    DWORD dwBytes;
    HRESULT hr = S_OK;
    DWORD dwHashTemp;
    A_SHA_CTX SHAHash;
    CTimer Timer;
    DWORD dwMainEntry;
    DWORD dwMediaEntry;
    DWORD DesiredPreldrSize;
    DWORD dwMaxBldrSize;
    PBYTE pKeyDataInBldr;
    PBYTE pKeyToEncryptBldr;
    DWORD dwKeyEntry;
    PDWORD rgdwHashDataSizes;
    BYTE rgbTemp[8192];
    BYTE rgbKeyToEncryptKernel[16];
    BYTE rgbKeyForRandomData[16];
    BYTE rgbBldrNonce[16];
    FILETIME ft;
    int i;
    BOOL fSuccess = FALSE;
    size_t enc_size;
    char scratch[128];
    memset(scratch, 0, 128);

    szCompKernelFile[0] = '\0';
    memset(&BootParam, 0, sizeof(BootParam));
    BootParam.Signature = BLDR_SIGNATURE;

    if (External) 
    {
        TRACE_OUT(TRACE_VERBOSE, "Output external \t%s", g_ib.m_prop.Get(PROP_OUTPUTEXT));
    }
    else
    {
        TRACE_OUT(TRACE_VERBOSE, "Output filename \t%s", g_ib.m_prop.Get(PROP_OUTPUT));
    }
    TRACE_OUT(TRACE_VERBOSE, "System          \t%s", g_ib.m_prop.Get(PROP_SYS));

    //
    // Create the output file
    //
    if (External) 
    {
        szOutputName = g_ib.m_prop.Get(PROP_OUTPUTEXT);
        hWriteFile = CreateFile(szOutputName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hWriteFile == INVALID_HANDLE_VALUE) {
            g_ib.SetExitMsg(HRESULT_FROM_WIN32(GetLastError()), "Failed to create external output file");
            goto Cleanup;
        }
    }
    else
    {
        szOutputName = g_ib.m_prop.Get(PROP_OUTPUT);
        hWriteFile = CreateFile(szOutputName, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hWriteFile == INVALID_HANDLE_VALUE) {
            g_ib.SetExitMsg(HRESULT_FROM_WIN32(GetLastError()), "Failed to create output file");
            goto Cleanup;
        }
    }

    //
    // Fill the output file with 1 MB of zeroes
    //
    memset(rgbTemp, 0, sizeof rgbTemp);
    for(i = 0; i < 128; ++i) {
        if(!WriteFile(hWriteFile, rgbTemp, sizeof rgbTemp, &dwBytes, NULL) ||
            dwBytes != sizeof rgbTemp)
        {
WriteError:
            g_ib.SetExitMsg(HRESULT_FROM_WIN32(GetLastError()), "Failed to create output file");
            goto Cleanup;
        }
    }

    //
    // Map in the output file
    //
    hWriteMap = CreateFileMapping(hWriteFile, NULL, PAGE_READWRITE, 0,
        0x100000, NULL);
    if(!hWriteMap)
        goto WriteError;
    pbOutput = (PBYTE)MapViewOfFile(hWriteMap, FILE_MAP_WRITE, 0, 0, 0x100000);
    if(!pbOutput)
        goto WriteError;

    //
    // Get temporary file name for compressed kernel file
    //
    if (FAILED(hr = TempFileName(szCompKernelFile)))
    {
        g_ib.SetExitMsg(hr, "Failed to get temp file name");
        goto Cleanup;
    }

    //
    // Read the uncompressed kernel
    //
    if (FAILED(hr = UncompressedKernelFile.Open(g_ib.m_prop.Get(PROP_KERNEL), TRUE)))
    {
        g_ib.SetExitMsg(hr, "Failed to open uncompressed kernel file");
        goto Cleanup;
    }

    //
    // We hash the current system time plus the kernel contents to use as a
    // seed to generate random encryption keys
    //

    A_SHAInit(&SHAHash);
    GetSystemTimeAsFileTime(&ft);
    A_SHAUpdate(&SHAHash, (PUCHAR)&ft, sizeof ft);
    A_SHAUpdate(&SHAHash, (PUCHAR)UncompressedKernelFile.Ptr(),
        UncompressedKernelFile.Size());
    A_SHAFinal(&SHAHash, rgbTemp);

    //
    // Find the pointer to the start of the kernel's .data section and its size.
    //
    if (!LocateDataSection(UncompressedKernelFile, &pvDataSectionStart, &dwDataSectionSize))
    {
        g_ib.SetExitMsg("Could not locate the kernel's data section");
        goto Cleanup;
    }

    //
    // Figure out where we're going to put things
    //
    pbOutputBldr = pbOutput + g_ib.m_nRomSize - ROM_DEC_SIZE - BLDR_BLOCK_SIZE;
    pbOutputData = pbOutputBldr - dwDataSectionSize;

    if(pbOutputData < pbOutput) {
        g_ib.SetExitMsg("Kernel data section exceeds available size");
        goto Cleanup;
    }

    //
    // Copy the kernel's data into the output file
    //
    memcpy(pbOutputData, pvDataSectionStart, dwDataSectionSize);
    TRACE_OUT(TRACE_VERBOSE, "Wrote Kernel DATA @ 0x%08X (%u bytes)", pbOutputData-pbOutput, dwDataSectionSize);

    //
    // Replace the public key
    //
    if (m_UseSpecifiedKeys) {
        
        if (m_szPubKeyFN[0] != '\0') {
            if (!ReadKeyFromEncFile(g_ib.m_szPubKeyFN, g_ib.m_szPassword, g_PublicKeyData, XC_PUBLIC_KEYDATA_SIZE, FALSE))             {
                g_ib.Exit(-1, "Bad public key file");
            }
            TRACE_OUT(TRACE_VERBOSE, "Public Key Injected\t%s", g_ib.m_szPubKeyFN);
        } else {
            TRACE_OUT(TRACE_VERBOSE, "Public Key Injected\t(Interim)");
        }

        //
        // Find XePublicKeyData with ordinal
        //
        PBYTE pKeyInBinary = (PBYTE)LocateExportOrdinal(UncompressedKernelFile, 355);

        if (pKeyInBinary == NULL) {
            g_ib.Exit(-1, "XePublicKeyData(355) export not found in kernel binary to inject public key");
        }
        
        //
        // Verify that we found the right place by checking for RSA1 signature
        //
        if (*(DWORD*)pKeyInBinary != 0x31415352) {
            g_ib.Exit(-1, "XePublicKeyData(355) in kernel binary does not point to a valid key place holder");
        }

        //
        // Inject the public key into the binary
        //
        memcpy(pKeyInBinary, g_PublicKeyData, XC_PUBLIC_KEYDATA_SIZE);
    }


    //
    // Compress the kernel
    //
    Timer.Start();
    if (!CompressFile(UncompressedKernelFile, szCompKernelFile))
    {
        g_ib.SetExitMsg("Could not compress the kernel");
        goto Cleanup;
    }
    Timer.Stop();
    TRACE_OUT(TRACE_VERBOSE, "Uncompressed kernel \t%d bytes", UncompressedKernelFile.Size());
    TRACE_OUT(TRACE_VERBOSE, "Time to compress \t%0.4f ms", Timer.getTime());

    //
    // Read the compressed kernel
    //
    if (FAILED(hr = KernelFile.Open(szCompKernelFile)))
    {
        g_ib.SetExitMsg(hr, "Failed to open compressed kernel file");
        goto Cleanup;
    }

    //
    // Copy the compressed kernel into the output file
    //
    pbOutputKernel = pbOutputData - KernelFile.Size();
    if(pbOutputKernel < pbOutput) {
        g_ib.SetExitMsg("Compressed kernel exceeds available size");
        goto Cleanup;
    }
    memcpy(pbOutputKernel, KernelFile.Ptr(), KernelFile.Size());
    TRACE_OUT(TRACE_VERBOSE, "Compressed Kernel @ 0x%08X (%u bytes)", pbOutputKernel-pbOutput, KernelFile.Size());

    //
    // Read the boot loader
    //
    if (m_nBuildFor == ROMBLD_XM3)
    {
        DesiredPreldrSize = 0;
        dwMaxBldrSize = BLDR_BLOCK_SIZE - DesiredPreldrSize;
        pKeyToEncryptBldr = g_XM3KeyToEncryptBldr;
    }
    else
    {
        DesiredPreldrSize = PRELDR_BLOCK_SIZE;
        dwMaxBldrSize = BLDR_BLOCK_SIZE - DesiredPreldrSize;
        pKeyToEncryptBldr = g_DevKeyToEncryptBldr; // TODO: XDK & XM3P
    }
    if (FAILED(hr = LdrFile.Open(g_ib.m_prop.Get(PROP_BOOTLDR))))
    {
        g_ib.SetExitMsg(hr, "Failed to open boot loader file");
        goto Cleanup;
    }

    if (LdrFile.Size() > BLDR_BLOCK_SIZE - DesiredPreldrSize)
    {
        ERROR_OUT("Boot loader block is too big (%d).  Maxsize=%d", 
            LdrFile.Size(), dwMaxBldrSize);
        goto Cleanup;
    }

    //
    // Copy the boot loader into the output
    //
    memcpy(pbOutputBldr, LdrFile.Ptr(), LdrFile.Size());
    TRACE_OUT(TRACE_VERBOSE, "Boot Loader @ 0x%08X (%u bytes)", pbOutputBldr-pbOutput, LdrFile.Size());

    //
    // A pointer to the start of keys in boot loader is stored a DWORD found two DWORDs 
    // before the entry point.  Entry point is the first DWORD in the binary.
    //

    dwKeyEntry = (*(DWORD*)pbOutputBldr) - BLDR_BOOT_ORIGIN - 8;
    dwKeyEntry = *(DWORD*)((BYTE*)pbOutputBldr + dwKeyEntry);
    dwKeyEntry -= BLDR_RELOCATED_ORIGIN; 
    pKeyDataInBldr = (BYTE*)pbOutputBldr + dwKeyEntry;

    //
    // Inject keys in the boot loader image and retrieve encryption keys
    //
    //UpdateKeys(rgbTemp, rgbKeyForRandomData, rgbKeyToEncryptKernel,
    //    rgbBldrNonce, pKeyDataInBldr);
    memcpy(rgbKeyToEncryptKernel, g_KeyToEncryptKernel, 16);

    if (m_nBuildFor != ROMBLD_XM3)
    {
        //
        // Move the entry points around
        //
        dwMainEntry = *(PDWORD)pbOutputBldr;
        dwMainEntry -= BLDR_BOOT_ORIGIN;
        dwMediaEntry = *(PDWORD)(pbOutputBldr + dwMainEntry - 4);
        *(PDWORD)pbOutputBldr = (BLDR_BOOT_ORIGIN + BLDR_BLOCK_SIZE - DesiredPreldrSize);
        *(PDWORD)(pbOutputBldr + BLDR_BLOCK_SIZE - DesiredPreldrSize - 4) = dwMediaEntry;
        *(PDWORD)(pbOutputBldr + BLDR_BLOCK_SIZE - DesiredPreldrSize - 8) = dwMainEntry;
    }

    //
    // Encrypt the kernel in place
    //
    Timer.Start();
    XCSymmetricEncDec(pbOutputKernel, KernelFile.Size(), rgbKeyToEncryptKernel, 16); 
    Timer.Stop();
    TRACE_OUT(TRACE_VERBOSE, "Time to encrypt kernel\t\t%0.4f ms", Timer.getTime());

    //
    // Read the inittbl
    //
    if (FAILED(hr = InitTbl.Open(g_ib.m_prop.Get(PROP_INITTBL))))
    {
        g_ib.SetExitMsg(hr, "Failed to open InitTbl file");
        goto Cleanup;
    }

    TRACE_OUT(TRACE_VERBOSE, "Init table  \t\t%d bytes", InitTbl.Size());
    TRACE_OUT(TRACE_VERBOSE, "Available space \t\t%d bytes", pbOutputKernel-pbOutput);

    if(pbOutput + InitTbl.Size() > pbOutputKernel) {
        g_ib.SetExitMsg("Init table exceeds available size");
        goto Cleanup;
    }

    //
    // Copy it into the output
    //
    memcpy(pbOutput, InitTbl.Ptr(), InitTbl.Size());

    //
    // Set the kernel build number
    //
    pwKernelVer = (PUSHORT)LocateExportOrdinal(UncompressedKernelFile, 324);
    if(pwKernelVer) {
        ((PUSHORT)pbOutput)[ROM_VERSION_OFFSET * 2 + 1] =
            pwKernelVer[2];
    }

    //
    // For external images modify the init table to add the KD delay flag
    //
    if (External) 
    {
        ((PDWORD)pbOutput)[ROM_VERSION_OFFSET] |= ROM_VERSION_KDDELAY_FLAG;
    }

    //
    // Save the sizes into the boot param
    //
    BootParam.CompressedKernelSize = KernelFile.Size();
    BootParam.UncompressedKernelDataSize = dwDataSectionSize;
    BootParam.InitTableSize = InitTbl.Size();

    if(m_nBuildFor == ROMBLD_XM3) {

        //
        // Calculate a SHA1 digest with the following components
        //     1. Size and contents of compressed and encrypted kernel 
        //     2. Size and contents of uncompressed data
        //     3. Size and contents of init table
        //
        Timer.Start();
        A_SHAInit(&SHAHash);

        A_SHAUpdate(&SHAHash, rgbKeyToEncryptKernel, 16);
        for(i = 0; i < 16; i++)
            sprintf(scratch+strlen(scratch), "%02X ", rgbKeyToEncryptKernel[i]);
        TRACE_OUT(TRACE_VERBOSE, "HASHED KERNEL KEY: %s", scratch);
        memset(scratch, 0, sizeof(scratch));
    
        dwHashTemp = KernelFile.Size();
        A_SHAUpdate(&SHAHash, (PBYTE)&dwHashTemp, sizeof(DWORD));
        TRACE_OUT(TRACE_VERBOSE, "HASHED COMPRESSED KERNEL SIZE: %u", KernelFile.Size());

        A_SHAUpdate(&SHAHash, (PUCHAR)pbOutputKernel, dwHashTemp);
        for(i = 0; i < 16; i++)
            sprintf(scratch+strlen(scratch), "%02X ", pbOutputKernel[i]);
        sprintf(scratch+strlen(scratch), "...");
        TRACE_OUT(TRACE_VERBOSE, "HASHED COMPRESSED KERNEL: %s", scratch);
        memset(scratch, 0, sizeof(scratch));
    
        dwHashTemp = dwDataSectionSize;
        A_SHAUpdate(&SHAHash, (PBYTE)&dwHashTemp, sizeof(DWORD));
        TRACE_OUT(TRACE_VERBOSE, "HASHED UNCOMPRESSED KERNEL DATA SIZE: %u", dwHashTemp);

        A_SHAUpdate(&SHAHash, (PUCHAR)pbOutputData, dwHashTemp);
        for(i = 0; i < 16; i++)
            sprintf(scratch+strlen(scratch), "%02X ", pbOutputData[i]);
        sprintf(scratch+strlen(scratch), "...");
        TRACE_OUT(TRACE_VERBOSE, "HASHED UNCOMPRESSED KERNEL DATA: %s", scratch);
        memset(scratch, 0, sizeof(scratch));

        //
        // When /HACKINITTABLE:1 is specified, we don't digest the
        // the init table.  This is indicated by InitTableSize set to 0
        //
        if (m_HackInitTable == 0) 
        {
            TRACE_OUT(TRACE_VERBOSE, "Manual editing\t\tNot Allowed");

            dwHashTemp = InitTbl.Size();
            A_SHAUpdate(&SHAHash, (PBYTE)&dwHashTemp, sizeof(DWORD));
            TRACE_OUT(TRACE_VERBOSE, "HASHED X-CODE TABLE SIZE: %u", dwHashTemp);

            A_SHAUpdate(&SHAHash, (PUCHAR)pbOutput, dwHashTemp);
            for(i = 0; i < 16; i++)
                sprintf(scratch+strlen(scratch), "%02X ", pbOutput[i]);
            sprintf(scratch+strlen(scratch), "...");
            TRACE_OUT(TRACE_VERBOSE, "HASHED X-CODE TABLE DATA: %s", scratch);
            memset(scratch, 0, sizeof(scratch));
        }
        else
        {
            BootParam.InitTableSize = 0;
            TRACE_OUT(TRACE_VERBOSE, "Manual editing\tAllowed");
        }

        //
        // Save the digest in the boot param
        //
        A_SHAFinal(&SHAHash, &(BootParam.MainRomDigest[0]));

        for(i = 0; i < 20; i++)
            sprintf(scratch+strlen(scratch), "%02X ", BootParam.MainRomDigest[i]);
        sprintf(scratch+strlen(scratch), "...");
        TRACE_OUT(TRACE_VERBOSE, "SHA FINALIZATION: %s", scratch);
        memset(scratch, 0, sizeof(scratch));

        A_SHAInit(&SHAHash);
        A_SHAUpdate(&SHAHash, rgbKeyToEncryptKernel, 16);
        A_SHAUpdate(&SHAHash, &(BootParam.MainRomDigest[0]), XC_DIGEST_LEN);
        A_SHAFinal(&SHAHash, &(BootParam.MainRomDigest[0]));

        for(i = 0; i < 20; i++)
            sprintf(scratch+strlen(scratch), "%02X ", BootParam.MainRomDigest[i]);
        sprintf(scratch+strlen(scratch), "...");
        TRACE_OUT(TRACE_VERBOSE, "FINAL SHA %s", scratch);
        memset(scratch, 0, sizeof(scratch));
    
        Timer.Stop();
        TRACE_OUT(TRACE_VERBOSE, "Digest calculation\t%0.4f ms", Timer.getTime());
    }

    //
    // And save away the boot params
    //
    memcpy(pbOutputBldr + BLDR_BLOCK_SIZE - sizeof BootParam, &BootParam, sizeof(BootParam));
    TRACE_OUT(TRACE_VERBOSE, "Boot Params @ 0x%08X (%u bytes)", (pbOutputBldr + BLDR_BLOCK_SIZE - sizeof BootParam) - pbOutputBldr, sizeof(BootParam));

    //
    // Encrypt the boot loader using the key that will be used by romdec32
    //
    if(m_nBuildFor == ROMBLD_XM3P) {

        ERROR_OUT("Building XM3P (MCPX 1.1) is NOT fully functional yet.");

        //
        // Read the encrypted romdec
        //
        if (FAILED(hr = EncRomDec.Open(g_ib.m_prop.Get(PROP_ENCROM))))
        {
            g_ib.SetExitMsg(hr, "Failed to open EncRomDec file");
            goto Cleanup;
        }
        if(EncRomDec.Size() != ROM_DEC_SIZE + 28) {
            ERROR_OUT("Encrypted romdec size is incorrect (%d).  Should be %d", 
                EncRomDec.Size(), ROM_DEC_SIZE + 28);
            goto Cleanup;
        }
        pbRomdec = (PBYTE)malloc(ROM_DEC_SIZE);

        //
        // Decrypt it
        //
        GetPasswd("Enter romdec password:", (LPSTR)rgbTemp, sizeof rgbTemp);
        A_SHAInit(&SHAHash);
        A_SHAUpdate(&SHAHash, rgbTemp, strlen((LPSTR)rgbTemp));
        A_SHAFinal(&SHAHash, rgbTemp);
        memcpy(rgbTemp + 20, EncRomDec.Ptr(), EncRomDec.Size());
        XCSymmetricEncDec(rgbTemp + 20, 8, rgbTemp, 20);
        XCSymmetricEncDec(rgbTemp + 28, EncRomDec.Size() - 8, rgbTemp + 20, 8);
        memcpy(pbRomdec, rgbTemp + 48, ROM_DEC_SIZE);
        A_SHAInit(&SHAHash);
        A_SHAUpdate(&SHAHash, pbRomdec, ROM_DEC_SIZE);
        A_SHAFinal(&SHAHash, rgbTemp);
        if(memcmp(rgbTemp, rgbTemp + 28, 20)) {
            ERROR_OUT("Romdec password is incorrect");
            goto Cleanup;
        }
        memset(rgbTemp, 0, 1024);

        //
        // The actual bldr encryption key is a cross of a nonce and the SB
        // key
        //
        A_SHAInit(&SHAHash);
        A_SHAUpdate(&SHAHash, pbRomdec + ROM_DEC_SIZE - ROMDEC_N, 16);
        A_SHAUpdate(&SHAHash, rgbBldrNonce, 16);
        for(i = 0; i < 16; ++i)
            rgbTemp[i] = (BYTE)((pbRomdec + ROM_DEC_SIZE - ROMDEC_N)[i] ^
                0x5C);
        A_SHAUpdate(&SHAHash, rgbTemp, 16);
        A_SHAFinal(&SHAHash, rgbTemp);

        XCSymmetricEncDec(pbOutputBldr, BLDR_BLOCK_SIZE, rgbTemp,
            XC_DIGEST_LEN);
        memcpy(pbOutputBldr + BLDR_BLOCK_SIZE - 16, rgbBldrNonce, 16);
        memset(rgbTemp, 0, 16);

        //
        // Take out the first 16 bytes of the bldr (we know they're unused
        // to minimze the chance of the "resolved key" RC4 attack
        //
        memset(pbOutputBldr, 0, 16);
    } else
        XCSymmetricEncDec(pbOutputBldr, BLDR_BLOCK_SIZE, pKeyToEncryptBldr, 16);

    //
    // Read the preloader
    //
    if(m_nBuildFor == ROMBLD_XM3P)
    {
        if(FAILED(hr = PreldrFile.Open(g_ib.m_prop.Get(PROP_PRELDR))))
        {
            g_ib.SetExitMsg(hr, "Failed to open preloader file");
            goto Cleanup;
        }

        if ((PreldrFile.Size() + 0x180) > DesiredPreldrSize)
        {
            ERROR_OUT("Preloader block is too big (%d).  Maxsize=%d",
                PreldrFile.Size(), DesiredPreldrSize - 0x180);
            goto Cleanup;
        }

        TRACE_OUT(TRACE_VERBOSE, "Preloader\t\t%d bytes", PreldrFile.Size());

        //
        // Insert the preloader
        //
        memset(pbOutputBldr + BLDR_BLOCK_SIZE - DesiredPreldrSize, 0,
            DesiredPreldrSize - 0x180);
        memcpy(pbOutputBldr + BLDR_BLOCK_SIZE - DesiredPreldrSize, PreldrFile.Ptr(),
            PreldrFile.Size());

    }

    //
    // Read the romdec
    //
    if (FAILED(hr = RomDec.Open(g_ib.m_prop.Get(PROP_ROMDEC))))
    {
        if(!pbRomdec) {
            g_ib.SetExitMsg(hr, "Failed to open RomDec file");
            goto Cleanup;
        }
    } else if(RomDec.Size() != ROM_DEC_SIZE) {
        ERROR_OUT("Romdec block is incorrect (%d).  Should be %d", 
            RomDec.Size(), ROM_DEC_SIZE);
        goto Cleanup;
    } else {
        if(pbRomdec) {
            memset(pbRomdec, 0, ROM_DEC_SIZE);
            free(pbRomdec);
        }
        pbRomdec = (PBYTE)RomDec.Ptr();
    }

    memcpy(pbOutputBldr + BLDR_BLOCK_SIZE, pbRomdec, ROM_DEC_SIZE);
    if(pbRomdec != RomDec.Ptr()) {
        memset(pbRomdec, 0, ROM_DEC_SIZE);
        free(pbRomdec);
    }
    pbRomdec = NULL;

    if(m_nBuildFor == ROMBLD_XDK) {
        //
        // Find the preloader's public key and encrypt it
        //
        pKeyDataInBldr = (PBYTE)((PULONG)(pbOutputBldr + BLDR_BLOCK_SIZE -
            DesiredPreldrSize))[3];
        pKeyDataInBldr = pKeyDataInBldr - (PUCHAR)(0UL - ROM_DEC_SIZE -
            BLDR_BLOCK_SIZE) + pbOutputBldr;
        XCSymmetricEncDec(pKeyDataInBldr, XC_PUBLIC_KEYDATA_SIZE,
            pbOutputBldr + BLDR_BLOCK_SIZE + ROM_DEC_SIZE - ROMDEC_N, 12);
    }

    //
    // Generate a random sequence to fill in the padding.
    //
    pbOutputPadding = pbOutput + InitTbl.Size();
    XCSymmetricEncDec(pbOutputPadding, pbOutputKernel - pbOutputPadding,
        rgbKeyForRandomData, sizeof rgbKeyForRandomData);

    //
    // Mark this ROM's sizes
    //
    rgdwHashDataSizes = (PDWORD)(pbOutput + g_ib.m_nRomSize - ROM_DEC_SIZE - 0x80);
    rgdwHashDataSizes[0] = 0UL - g_ib.m_nRomSize;
    rgdwHashDataSizes[1] = BootParam.InitTableSize;
    rgdwHashDataSizes[2] = pbOutputKernel - pbOutput + rgdwHashDataSizes[0];

    //
    // Last step is to PK sign the whole thing.  First comes the top 128
    // bytes, which includes the sizes, then the init table, then the rest
    // of the ROM up to the signature
    //
    A_SHAInit(&SHAHash);

    TRACE_OUT(TRACE_VERBOSE, "Hashing 0x%X bytes at 0x%08X", 0x80, (pbOutput + g_ib.m_nRomSize - ROM_DEC_SIZE - 0x80) - pbOutput);
    A_SHAUpdate(&SHAHash, pbOutput + g_ib.m_nRomSize - ROM_DEC_SIZE - 0x80, 0x80);

    TRACE_OUT(TRACE_VERBOSE, "Hashing 0x%X bytes at 0x%08X (X-Code Table)", BootParam.InitTableSize, 0);
    A_SHAUpdate(&SHAHash, pbOutput, BootParam.InitTableSize);

    enc_size = (pbOutput + g_ib.m_nRomSize - ROM_DEC_SIZE - 0x180) - pbOutputKernel;
    TRACE_OUT(TRACE_VERBOSE, "Hashing 0x%X bytes at 0x%08X (Encrpyted Kernel)", enc_size, pbOutputKernel - pbOutput);
    A_SHAUpdate(&SHAHash, pbOutputKernel, enc_size);

    A_SHAFinal(&SHAHash, rgbTemp);

    TRACE_OUT(TRACE_VERBOSE, "Writing digest @ 0x%08X", (pbOutput + g_ib.m_nRomSize - ROM_DEC_SIZE - 0x180) - pbOutput);
    XCSignDigest(rgbTemp, g_KeyToSignROM, pbOutput + g_ib.m_nRomSize - ROM_DEC_SIZE - 0x180);

    //
    // Replicate the data up to 1 MB size
    //

    if(g_ib.m_nRomSize < 0x80000)
        memcpy(pbOutput + 0x40000, pbOutput, g_ib.m_nRomSize);
    if(g_ib.m_nRomSize < 0x100000)
        memcpy(pbOutput + 0x80000, pbOutput, 0x80000);

    fSuccess = TRUE;

Cleanup:
    if(pbRomdec) {
        memset(pbRomdec, 0, ROM_DEC_SIZE);
        free(pbRomdec);
    }
    if(pbOutput)
        fSuccess = UnmapViewOfFile(pbOutput) && fSuccess;
    if(hWriteMap)
        CloseHandle(hWriteMap);
    if (hWriteFile != INVALID_HANDLE_VALUE)
    {
        CloseHandle(hWriteFile);
    }
    if(!fSuccess)
        DeleteFile(szOutputName);

    if (szCompKernelFile[0] != '\0')
    {
        KernelFile.Close();
        DeleteFile(szCompKernelFile);
    }
}


void 
CRomBuilder::UpdateKeys(
    PBYTE rgbRandomHash,
    PBYTE rgbRandomKey,
    PBYTE rgbKernelKey,
    PBYTE rgbBldrEncKey,
    PBYTE rgbBldrKeys
    )
{
    BYTE rgbKeys[48];
    BYTE EEPROMKey[16];
    BYTE CERTKey[16];

    /* Use the random hash as an rc4 key to encrypt a stream of zeroes -- this
     * will be our random set of key bytes */
    memset(rgbKeys, 0, sizeof rgbKeys);
    XCSymmetricEncDec(rgbKeys, sizeof rgbKeys, rgbRandomHash, XC_DIGEST_LEN);

    /* We'll use the first 16 bytes as a random number key */
    memcpy(rgbRandomKey, rgbKeys, 16);

    /* The next 16 bytes will be the key to encrypt the kernel */
    memcpy(rgbKernelKey, rgbKeys + 16, 16);

    /* The kernel key is also the third of the bldr's keys */
    memcpy(rgbBldrKeys + 32, rgbKernelKey, 16);

    /* The last 16 bytes are a nonce to encrypt the bldr */
    memcpy(rgbBldrEncKey, rgbKeys + 32, 16);

    //
    // Read the EEPROM key
    //
    if (m_szEEPROMKeyFN[0] != '\0') 
    {
        if (!ReadKeyFromEncFile(m_szEEPROMKeyFN, m_szPassword, EEPROMKey, 16, TRUE))
        {
            g_ib.Exit(-1, "Bad EEPROM key file");
        }
        
        memcpy(rgbBldrKeys, EEPROMKey, 16);
    }
    
    //
    // Read the CERT key
    //
    if (m_szCERTKeyFN[0] != '\0') 
    {
        if (!ReadKeyFromEncFile(m_szCERTKeyFN, m_szPassword, CERTKey, 16, TRUE))
        {
            g_ib.Exit(-1, "Bad CERT key file");
        }

        memcpy(rgbBldrKeys + 16, CERTKey, 16);
    }
}


extern "C"
int
_cdecl
main(
    int argc,
    char** argv
    )
{

    g_ib.Initialize(argc, argv);

    if (g_ib.m_prop.Get(PROP_HELP))
    {
        g_ib.PrintUsage();
    }
    else
    {
        //
        // Build image
        //

        if (g_ib.m_prop.Get(PROP_OUTPUT) == NULL ||
            g_ib.m_prop.Get(PROP_BOOTLDR) == NULL ||
            g_ib.m_prop.Get(PROP_INITTBL) == NULL ||
            g_ib.m_prop.Get(PROP_SYS) == NULL ||
            g_ib.m_prop.Get(PROP_KERNEL) == NULL

            )
        {
usage:
            g_ib.PrintUsage();
            g_ib.Exit(-1, "Required switches were not supplied");
        }

        if (_stricmp(g_ib.m_prop.Get(PROP_SYS), "XDK") == 0)
            g_ib.m_nBuildFor = ROMBLD_XDK;
        else if(_stricmp(g_ib.m_prop.Get(PROP_SYS), "XM3") == 0)
            g_ib.m_nBuildFor = ROMBLD_XM3;
        else if(_stricmp(g_ib.m_prop.Get(PROP_SYS), "XM3P") == 0)
            g_ib.m_nBuildFor = ROMBLD_XM3P;
        else {
            g_ib.PrintUsage();
            g_ib.Exit(-1, "/SYS has an invalid value");
        }

        /* Make sure we have an appropriate romdec */
        if(g_ib.m_prop.Get(g_ib.m_nBuildFor == ROMBLD_XM3P ? PROP_ENCROM :
                PROP_ROMDEC) == NULL)
            goto usage;

        /* Make sure XM3P (MCPX 1.1) builds specify a pre-bootloader */
        if(g_ib.m_nBuildFor == ROMBLD_XM3P && g_ib.m_prop.Get(PROP_PRELDR) == NULL)
            goto usage;

        /* Only support 256k, 512k, 1MB ROMs */
        switch(g_ib.m_nRomSize) {
            case 256:
            case 512:
            case 1024:
                // convert to byte count
                g_ib.m_nRomSize *= 1024;
                break;
            default:
                g_ib.Exit(-1, "ROM size must be 256, 512, or 1024");
                break;
        }

        if (g_ib.m_prop.Get(PROP_KEYPATH) != NULL) 
        {
            g_ib.m_UseSpecifiedKeys = TRUE;

            printf("Using following encrypted key files:\n");
            
            g_ib.m_szRomEncKeyFN[0] = '\0';
            g_ib.m_szEEPROMKeyFN[0] = '\0';
            g_ib.m_szCERTKeyFN[0] = '\0';

            //
            // Build file names and verify key files
            //
            BuildFilename(g_ib.m_szRomEncKeyFN, g_ib.m_prop.Get(PROP_KEYPATH), "bldrenc.key");
            printf("   %s => ", g_ib.m_szRomEncKeyFN);
            if (!IsEncKeyFile(g_ib.m_szRomEncKeyFN))
            {
                printf("*** Not found or invalid\n");
                g_ib.m_szRomEncKeyFN[0] = '\0';
            }
            else
            {
                printf("Found\n");
            }


            BuildFilename(g_ib.m_szEEPROMKeyFN, g_ib.m_prop.Get(PROP_KEYPATH), "eeprom.key");
            printf("   %s => ", g_ib.m_szEEPROMKeyFN);
            if (!IsEncKeyFile(g_ib.m_szEEPROMKeyFN))
            {
                printf("*** Not found or invalid\n");
                g_ib.m_szEEPROMKeyFN[0] = '\0';
            }
            else
            {
                printf("Found\n");
            }

            BuildFilename(g_ib.m_szCERTKeyFN, g_ib.m_prop.Get(PROP_KEYPATH), "cert.key");
            printf("   %s => ", g_ib.m_szCERTKeyFN);
            if (!IsEncKeyFile(g_ib.m_szCERTKeyFN))
            {
                printf("*** Not found or invalid\n");
                g_ib.m_szCERTKeyFN[0] = '\0';
            }
            else
            {
                printf("Found\n");
            }

            BuildFilename(g_ib.m_szPubKeyFN, g_ib.m_prop.Get(PROP_KEYPATH), "pub.key");
            printf("   %s => ", g_ib.m_szPubKeyFN);
            if (!IsEncKeyFile(g_ib.m_szPubKeyFN))
            {
                printf("*** Not found or invalid--interim key will be used\n");
                g_ib.m_szPubKeyFN[0] = '\0';
            }
            else
            {
                printf("Found\n");
            }



            //
            // Read passwords
            //
            if (!ReadPassword("Enter password to decrypt: ", g_ib.m_szPassword))
            {
                g_ib.Exit(-1, "Password is required with /KEYPATH");
            }

            //
            // Read the key to encrypt the boot loader
            //
            if (g_ib.m_szRomEncKeyFN[0] != '\0') 
            {
                if (!ReadKeyFromEncFile(g_ib.m_szRomEncKeyFN, g_ib.m_szPassword, g_CLIKeyToEncryptBldr, 16, TRUE))
                {
                    g_ib.Exit(-1, "Bad bldrenc key file");
                }
                ERROR_OUT("TODO: Finish re-implementing CLI key selection.");
            }

        }


        g_ib.BuildImage(FALSE);
    
        if (g_ib.m_prop.Get(PROP_OUTPUTEXT) != NULL)
        {
            g_ib.BuildImage(TRUE);
        }
    }

    g_ib.Exit(0, NULL);

    return 0;
}
