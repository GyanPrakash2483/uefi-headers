param(
    [Parameter(Mandatory, Position = 0)] [string]$EfiFile,
    [string]$Ovmf = "C:\Program Files\qemu\share\edk2-x86_64-code.fd"
)

$ErrorActionPreference = "Stop"

if(-not (Test-Path $Ovmf)) { throw "OVMF firmware not found: $Ovmf" }

$esp = Join-Path $PSScriptRoot "build\esp"
$boot = Join-Path $esp "EFI\BOOT"

New-Item -ItemType Directory -Force $boot | Out-Null
Copy-Item $EfiFile (Join-Path $boot "BOOTX64.EFI") -Force

qemu-system-x86_64 `
    -machine pc,smbios-entry-point-type=64 `
    -m 256M `
    -drive "if=pflash,format=raw,readonly=on,file=$Ovmf" `
    -drive "format=raw,file=fat:rw:$esp" `
    -nic user,model=virtio-net-pci