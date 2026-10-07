# uefi-headers

Clean, spec-based, documented C headers for writing UEFI applications, bootloaders, and drivers.

uefi-headers provides type definitions, protocol structures, GUIDs, and constants from the UEFI specification. It's header-only and has no dependencies. Add it to your include path and start writing.

## Usage

Add `include/` to your compiler's include path:

Example:
```c
#include <uefi/uefi.h>

EFI_STATUS EFIAPI efi_main(EFI_HANDLE image, EFI_SYSTEM_TABLE *st) {
    st->ConOut->OutputString(st->ConOut, L"Hello, UEFI!\r\n");
    return EFI_SUCCESS;
}
```

## Notes

- Names follow the UEFI specification (`EFI_STATUS`, `EFI_SYSTEM_TABLE`, etc.), so you can cross-reference the spec directly.
- Written from the UEFI specification, not derived from EDK2 or gnu-efi.

## TODO

- Split into multiple files for ease of use and simplicity.
- Adapt for architectures other than x86_64

## License

[License](LICENSE)