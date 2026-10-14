#include "uefi/guids.h"
#include "uefi/status_codes.h"
#include <uefi/uefi.h>

BOOLEAN isGuidEqual(EFI_GUID a, EFI_GUID b) {
    if(a.Data1 != b.Data1) return false;
    if(a.Data2 != b.Data2) return false;
    if(a.Data3 != b.Data3) return false;
    for(int i = 0; i < 8; i++) {
        if(a.Data4[i] != b.Data4[i]) {
            return false;
        }
    }

    return true;
}

EFI_STATUS EFIAPI efi_main(EFI_HANDLE Image, EFI_SYSTEM_TABLE *st) {
    (void)Image;

    EFI_CONFIGURATION_TABLE smbios_table = {EFI_NULL_GUID, NULL};
    for(int i = 0; i < st->NumberOfTableEntries; i++) {
        if(isGuidEqual(st->ConfigurationTable[i].VendorGuid, SMBIOS3_TABLE_GUID)) {
            smbios_table = st->ConfigurationTable[i];
            break;
        }
    }

    if(smbios_table.VendorTable == NULL) {
        st->ConOut->OutputString(st->ConOut, u"No SMBIOS table found.\r\n");
        return EFI_NOT_FOUND;
    }
    
    st->ConOut->OutputString(st->ConOut, u"EFI Application: DeviceInfo\n\r");
    st->BootServices->Stall(3 * 1000000);

    st->ConOut->ClearScreen(st->ConOut);
    st->ConOut->OutputString(st->ConOut, u"Device Info:\n\n\r");
    
    st->ConOut->OutputString(st->ConOut, u"Firmware Vendor: ");
    st->ConOut->OutputString(st->ConOut, st->FirmwareVendor);
    

    st->ConOut->OutputString(st->ConOut, u"\n\n\rPress any key to exit");

    UINTN index;
    st->BootServices->WaitForEvent(1, &st->ConIn->WaitForKey, &index);

    return EFI_SUCCESS;
}