#pragma once

typedef signed char INT8;               // 1-byte signed value
typedef signed short INT16;             // 2-byte signed value
typedef signed int INT32;               // 4-byte signed value
typedef signed long long INT64;         // 8-byte signed value

typedef unsigned char UINT8;            // 1-byte unsigned value
typedef unsigned short UINT16;          // 2-byte unsigned value
typedef unsigned int UINT32;            // 4-byte unsigned value
typedef unsigned long long UINT64;      // 8-byte unsigned value

typedef char CHAR8;                     // 1-byte character
typedef UINT16 CHAR16;                  // 2-byte character

typedef void VOID;                      // Undeclared type
typedef UINT8 BOOLEAN;                  // Logical Boolean. 1-byte value containing a 0 for FALSE or a 1 for TRUE.

#define TRUE 1
#define FALSE 0
#define true 1
#define false 0

#define NULL ((VOID *)0)

typedef INT64 INTN;                     // Signed value of native width (8 bytes on supported 64-bit processor instructions)
typedef UINT64 UINTN;                   // Unsigned value of native width (8 bytes on supported 64-bit processor instructions)

// 128-bit buffer containing a unique identifier value. Unless otherwise specified, aligned on a 64-bit boundary.
typedef struct __attribute__((aligned(8))) {
    UINT32 Data1;
    UINT16 Data2;
    UINT16 Data3;
    UINT8 Data4[8];                     
} EFI_GUID;

typedef enum {
    EFI_NATIVE_INTERFACE
} EFI_INTERFACE_TYPE;

typedef UINTN EFI_STATUS;               // Status code (Type UINTN)

typedef VOID *EFI_HANDLE;               // A collection of related interfaces (Type VOID *)
typedef VOID *EFI_EVENT;                // Handle to an event structure (Type VOID *)

typedef UINT64 EFI_LBA;                 // Logical block address (Type UINT64)
typedef UINTN EFI_TPL;                  // Task priority level (Type UINTN)

// 32-byte buffer containing a network Media Access Control address
typedef struct {
    UINT8 Addr[32];
} EFI_MAC_ADDRESS;

// An IPv4 internet protocol address (4-byte buffer)
typedef struct {
    UINT8 Addr[4];
} EFI_IPv4_ADDRESS;

// An IPv6 internet protocol address (16-byte buffer)
typedef struct {
    UINT8 Addr[16];
} EFI_IPv6_ADDRESS;

// 16-byte buffer aligned on a 4-byte boundary. An IPv4 or IPv6 internet protocol address.
typedef union __attribute__((aligned(4))) {
    UINT32 Addr[4];
    EFI_IPv4_ADDRESS v4;
    EFI_IPv6_ADDRESS v6;
} EFI_IP_ADDRESS;

typedef UINT64 EFI_PHYSICAL_ADDRESS;
typedef UINT64 EFI_VIRTUAL_ADDRESS;

// Modifiers for Common UEFI Data Types
#define IN
#define OUT
#define OPTIONAL
#define CONST const
#define EFIAPI __attribute__((ms_abi))