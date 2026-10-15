#include <uefi/uefi.h>

#pragma pack(push, 1)
typedef struct {
    CHAR8   Anchor[5];         // "_SM3_"
    UINT8   Checksum;
    UINT8   Length;            // entry point length (0x18)
    UINT8   Major;
    UINT8   Minor;
    UINT8   Docrev;
    UINT8   EpRevision;
    UINT8   Reserved;
    UINT32  TableMaxSize;      // max size of the structure table
    UINT64  TableAddress;      // physical address of the structure table
} SMBIOS3_ENTRY_POINT;

typedef struct {
    UINT8   Type;
    UINT8   Length;            // length of the formatted area (not the strings)
    UINT16  Handle;
} SMBIOS_HEADER;
#pragma pack(pop)

static EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *Out;

/* ---------- Basic helpers (replacing libc) ---------- */

BOOLEAN isGuidEqual(EFI_GUID a, EFI_GUID b) {
    if(a.Data1 != b.Data1) return FALSE;
    if(a.Data2 != b.Data2) return FALSE;
    if(a.Data3 != b.Data3) return FALSE;
    for(UINTN i = 0; i < 8; i++) {
        if(a.Data4[i] != b.Data4[i]) return FALSE;
    }
    return TRUE;
}

static UINTN AsciiLen(const CHAR8 *s) {
    UINTN n = 0;
    while(s[n]) n++;
    return n;
}

static BOOLEAN BytesEqual(const CHAR8 *a, const CHAR8 *b, UINTN n) {
    for(UINTN i = 0; i < n; i++) {
        if(a[i] != b[i]) return FALSE;
    }
    return TRUE;
}

/* ---------- Output helpers ---------- */

static void PrintW(const CHAR16 *s) {
    Out->OutputString(Out, (CHAR16 *)s);
}

/* Print a CHAR8 (ASCII) string by widening it to CHAR16 in chunks. */
static void PrintA(const CHAR8 *s) {
    CHAR16 buf[65];
    UINTN  n = 0;
    while(*s) {
        buf[n++] = (CHAR16)(UINT8)*s++;
        if(n == 64) {
            buf[n] = 0;
            Out->OutputString(Out, buf);
            n = 0;
        }
    }
    buf[n] = 0;
    Out->OutputString(Out, buf);
}

/* Print an unsigned number in decimal.
 * Process: repeatedly take (value % 10) as the lowest digit, then value = value / 10. */
static void PrintDec(UINT64 value) {
    CHAR16 buf[21];            // UINT64 max has 20 digits + NUL
    UINTN  pos = 20;
    buf[pos] = 0;
    if(value == 0) buf[--pos] = u'0';
    while(value > 0) {
        buf[--pos] = (CHAR16)(u'0' + (value % 10));
        value /= 10;
    }
    Out->OutputString(Out, &buf[pos]);
}

/* Print a number as a fixed-width, zero-padded hex value (no "0x" prefix).
 * Process: for each nibble from the most significant, digit = (value >> (4 * position)) & 0xF. */
static void PrintHex(UINT64 value, UINTN digits) {
    CHAR16 buf[17];
    for(UINTN i = 0; i < digits; i++) {
        UINT8 nibble = (UINT8)((value >> (4 * (digits - 1 - i))) & 0xF);
        buf[i] = (CHAR16)(nibble < 10 ? u'0' + nibble : u'A' + (nibble - 10));
    }
    buf[digits] = 0;
    Out->OutputString(Out, buf);
}

static void PrintLabelA(const CHAR16 *label, const CHAR8 *value) {
    PrintW(label);
    PrintA(value);
    PrintW(u"\r\n");
}

static void PrintLabelDec(const CHAR16 *label, UINT64 value, const CHAR16 *suffix) {
    PrintW(label);
    PrintDec(value);
    PrintW(suffix);
    PrintW(u"\r\n");
}

/* SMBIOS stores the first three UUID fields little-endian, so bytes are swapped per field. */
static void PrintUuid(const UINT8 *d) {
    PrintHex(d[3], 2); PrintHex(d[2], 2); PrintHex(d[1], 2); PrintHex(d[0], 2);
    PrintW(u"-");
    PrintHex(d[5], 2); PrintHex(d[4], 2);
    PrintW(u"-");
    PrintHex(d[7], 2); PrintHex(d[6], 2);
    PrintW(u"-");
    PrintHex(d[8], 2); PrintHex(d[9], 2);
    PrintW(u"-");
    for(UINTN i = 10; i < 16; i++) PrintHex(d[i], 2);
}

/* ---------- SMBIOS helpers ---------- */

/* Returns the Nth (1-based) string that follows the formatted area. */
static const CHAR8 *SmbiosString(const SMBIOS_HEADER *h, UINT8 index) {
    if(index == 0) return "N/A";

    const CHAR8 *s = (const CHAR8 *)h + h->Length;
    while(--index > 0 && *s) {
        s += AsciiLen(s) + 1;          // skip to the next string
    }
    return (*s) ? s : "N/A";
}

/* Returns a pointer to the next structure (skips formatted area + string set). */
static const UINT8 *SmbiosNext(const SMBIOS_HEADER *h, const UINT8 *end) {
    const UINT8 *p = (const UINT8 *)h + h->Length;
    // The string set ends with two consecutive NUL bytes
    while(p + 1 < end && !(p[0] == 0 && p[1] == 0)) p++;
    return p + 2;
}

static void ParseSmbios(const SMBIOS3_ENTRY_POINT *ep) {
    const UINT8 *p   = (const UINT8 *)(UINTN)ep->TableAddress;
    const UINT8 *end = p + ep->TableMaxSize;

    UINT64 totalMemMb = 0;
    UINTN  dimmCount  = 0;

    PrintW(u"SMBIOS version: ");
    PrintDec(ep->Major);  PrintW(u".");
    PrintDec(ep->Minor);  PrintW(u".");
    PrintDec(ep->Docrev); PrintW(u"\r\n\r\n");

    while(p + sizeof(SMBIOS_HEADER) <= end) {
        const SMBIOS_HEADER *h = (const SMBIOS_HEADER *)p;
        if(h->Type == 127) break;                  // End-of-table marker

        switch(h->Type) {
        case 0: // BIOS Information
            PrintLabelA(u"BIOS Vendor:     ", SmbiosString(h, p[4]));
            PrintLabelA(u"BIOS Version:    ", SmbiosString(h, p[5]));
            PrintLabelA(u"BIOS Date:       ", SmbiosString(h, p[8]));
            PrintW(u"\r\n");
            break;

        case 1: // System Information
            PrintLabelA(u"Manufacturer:    ", SmbiosString(h, p[4]));
            PrintLabelA(u"Product Name:    ", SmbiosString(h, p[5]));
            PrintLabelA(u"Version:         ", SmbiosString(h, p[6]));
            PrintLabelA(u"Serial Number:   ", SmbiosString(h, p[7]));
            if(h->Length >= 0x19) {
                PrintW(u"UUID:            ");
                PrintUuid(p + 8);
                PrintW(u"\r\n");
            }
            PrintW(u"\r\n");
            break;

        case 2: // Baseboard Information
            PrintLabelA(u"Board Maker:     ", SmbiosString(h, p[4]));
            PrintLabelA(u"Board Product:   ", SmbiosString(h, p[5]));
            PrintLabelA(u"Board Version:   ", SmbiosString(h, p[6]));
            PrintW(u"\r\n");
            break;

        case 4: { // Processor Information
            UINT16 maxSpeed = *(const UINT16 *)(p + 0x14);
            PrintLabelA(u"CPU:             ", SmbiosString(h, p[0x10]));
            PrintLabelA(u"CPU Maker:       ", SmbiosString(h, p[7]));
            PrintLabelDec(u"CPU Max Speed:   ", maxSpeed, u" MHz");
            if(h->Length >= 0x28) {
                PrintLabelDec(u"CPU Cores:       ", p[0x23], u"");
                PrintLabelDec(u"CPU Threads:     ", p[0x25], u"");
            }
            PrintW(u"\r\n");
            break;
        }

        case 17: { // Memory Device
            UINT16 size = *(const UINT16 *)(p + 0x0C);
            UINT64 mb;
            if(size == 0 || size == 0xFFFF) {
                mb = 0;                                    // empty / unknown
            } else if(size == 0x7FFF && h->Length >= 0x20) {
                mb = *(const UINT32 *)(p + 0x1C);          // extended size, already in MB
            } else if(size & 0x8000) {
                mb = (size & 0x7FFF) / 1024;               // bit 15 set: value is in KB, MB = KB / 1024
            } else {
                mb = size;                                 // value is in MB
            }
            if(mb > 0) {
                totalMemMb += mb;
                dimmCount++;
            }
            break;
        }
        }

        p = SmbiosNext(h, end);
    }

    PrintW(u"Memory:          ");
    PrintDec(totalMemMb);
    PrintW(u" MB in ");
    PrintDec(dimmCount);
    PrintW(u" module(s)\r\n");
}

/* ---------- Entry point ---------- */

EFI_STATUS EFIAPI efi_main(EFI_HANDLE Image, EFI_SYSTEM_TABLE *st) {
    (void)Image;
    Out = st->ConOut;

    EFI_CONFIGURATION_TABLE smbiosTable = {EFI_NULL_GUID, NULL};
    for(UINTN i = 0; i < st->NumberOfTableEntries; i++) {
        if(isGuidEqual(st->ConfigurationTable[i].VendorGuid, SMBIOS3_TABLE_GUID)) {
            smbiosTable = st->ConfigurationTable[i];
            break;
        }
    }

    if(smbiosTable.VendorTable == NULL) {
        PrintW(u"No SMBIOS table found.\r\n");
        return EFI_NOT_FOUND;
    }

    const SMBIOS3_ENTRY_POINT *ep = (const SMBIOS3_ENTRY_POINT *)smbiosTable.VendorTable;

    // Validate anchor "_SM3_"
    if(!BytesEqual(ep->Anchor, "_SM3_", 5)) {
        PrintW(u"Invalid SMBIOS3 anchor.\r\n");
        return EFI_COMPROMISED_DATA;
    }

    // Validate checksum: all bytes of the entry point must sum to 0 (mod 256)
    UINT8 sum = 0;
    for(UINT8 i = 0; i < ep->Length; i++) sum += ((const UINT8 *)ep)[i];
    if(sum != 0) {
        PrintW(u"SMBIOS3 checksum mismatch.\r\n");
        return EFI_CRC_ERROR;
    }

    PrintW(u"EFI Application: DeviceInfo\n\r");
    st->BootServices->Stall(3 * 1000000);

    st->ConOut->ClearScreen(st->ConOut);
    PrintW(u"Device Info:\n\n\r");

    PrintW(u"Firmware Vendor: ");
    PrintW(st->FirmwareVendor);
    PrintW(u"\r\n");

    ParseSmbios(ep);

    PrintW(u"\n\rPress any key to exit");

    UINTN index;
    st->BootServices->WaitForEvent(1, &st->ConIn->WaitForKey, &index);

    return EFI_SUCCESS;
}