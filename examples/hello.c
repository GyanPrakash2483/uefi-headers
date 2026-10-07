#include "../include/uefi/uefi.h"


EFI_STATUS EFIAPI efi_main(EFI_HANDLE Image, EFI_SYSTEM_TABLE *st) {
    st->ConOut->OutputString(st->ConOut, u"Hello, World!\n\r");

    st->BootServices->Stall(5 * 1000000);

    return EFI_SUCCESS;
}