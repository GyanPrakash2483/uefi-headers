#pragma once

#include "types.h"
#include "status_codes.h"

// Forward declarations

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// Data structure that precedes all of the standard EFI table types.
typedef struct {
    UINT64          Signature;
    UINT32          Revision;
    UINT32          HeaderSize;
    UINT32          CRC32;
    UINT32          Reserved;
} EFI_TABLE_HEADER;

// Resets the input device hardware
typedef EFI_STATUS (EFIAPI *EFI_INPUT_RESET) (
    IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL       *This,
    IN BOOLEAN                              ExtendedVerification
);

// A pointer to a buffer that is filled in with the keystroke information for the key that was pressed
typedef struct {
    UINT16      ScanCode;
    CHAR16      UnicodeChar;
} EFI_INPUT_KEY;

// Reads the next keystroke from the input device
typedef EFI_STATUS (EFIAPI *EFI_INPUT_READ_KEY) (
    IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL       *This,
    OUT EFI_INPUT_KEY                       *Key
);

// This protocol is used to obtain input from the ConsoleIn device
struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    EFI_INPUT_RESET         Reset;
    EFI_INPUT_READ_KEY      ReadKeyStroke;
    EFI_EVENT               WaitForKey;
};

// Resets the text output device hardware
typedef EFI_STATUS (EFIAPI *EFI_TEXT_RESET) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *This,
    IN BOOLEAN                              ExtendedVerification
);

// Writes a string to the output device
typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *This,
    IN CHAR16                               *String
);

// Verifies that all characters in a string can be output to the target device
typedef EFI_STATUS (EFIAPI *EFI_TEXT_TEST_STRING) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *This,
    IN CHAR16                               *String
);

// Returns information for an available text mode that the output device(s) supports
typedef EFI_STATUS (EFIAPI *EFI_TEXT_QUERY_MODE) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *This,
    IN UINTN                                ModeNumber,
    OUT UINTN                               *Columns,
    OUT UINTN                               *Rows
);

// Sets the output device(s) to a specified mode
typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_MODE) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *This,
    IN UINTN                                ModeNumber
);

// Sets the background and foreground colors for the OutputString()
typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_ATTRIBUTE) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *This,
    IN UINTN                                Attribute
);

// Clears the output device(s) display to the currently selected background color
typedef EFI_STATUS (EFIAPI *EFI_TEXT_CLEAR_SCREEN) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *This
);

// Sets the current coordinates of the cursor position
typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_CURSOR_POSITION) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *This,
    IN UINTN                                Column,
    IN UINTN                                Row
);

// Makes the cursor visible or invisible
typedef EFI_STATUS (EFIAPI *EFI_TEXT_ENABLE_CURSOR) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL      *This,
    IN BOOLEAN                              Visible
);

/*
The following data values in the SIMPLE_TEXT_OUTPUT_MODE
interface are read-only and are changed by using the
appropriate interface functions
*/
typedef struct {
    INT32                   MaxMode;
    //current settings
    INT32                   Mode;
    INT32                   Attribute;
    INT32                   CursorColumn;
    INT32                   CursorRow;
    BOOLEAN                 CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

// This protocol is used to control text-based output devices
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET                          Reset;
    EFI_TEXT_STRING                         OutputString;
    EFI_TEXT_TEST_STRING                    TestString;
    EFI_TEXT_QUERY_MODE                     QueryMode;
    EFI_TEXT_SET_MODE                       SetMode;
    EFI_TEXT_SET_ATTRIBUTE                  SetAttribute;
    EFI_TEXT_CLEAR_SCREEN                   ClearScreen;
    EFI_TEXT_SET_CURSOR_POSITION            SetCursorPosition;
    EFI_TEXT_ENABLE_CURSOR                  EnableCursor;
    SIMPLE_TEXT_OUTPUT_MODE                 *Mode;
};

// A pointer to storage to receive a snapshot of the current time
typedef struct {
    UINT16          Year;
    UINT8           Month;
    UINT8           Day;
    UINT8           Hour;
    UINT8           Minute;
    UINT8           Second;
    UINT8           Pad1;
    UINT32          NanoSecond;
    UINT16          TimeZone;
    UINT8           Daylight;
    UINT8           Pad2;
} EFI_TIME;

// This provides the capabilities of the real time clock device as exposed through the EFI
typedef struct {
    UINT32          Resolution;
    UINT32          Accuracy;
    BOOLEAN         SetsToZero;
} EFI_TIME_CAPABILITIES;

/*
Returns the current time and date information, and
the time-keeping capabilities of the hardware platform
*/
typedef EFI_STATUS (EFIAPI *EFI_GET_TIME) (
    OUT EFI_TIME                    *Time,
    OUT EFI_TIME_CAPABILITIES       *Capabilities OPTIONAL
);

// Sets the current local time and date information
typedef EFI_STATUS (EFIAPI *EFI_SET_TIME) (
    IN EFI_TIME         *Time
);

// Returns the current wakeup alarm clock setting
typedef EFI_STATUS (EFIAPI *EFI_GET_WAKEUP_TIME) (
    OUT BOOLEAN         *Enabled,
    OUT BOOLEAN         *Pending,
    OUT EFI_TIME        *Time 
);

// Sets the system wakeup alarm clock time
typedef EFI_STATUS (EFIAPI *EFI_SET_WAKEUP_TIME) (
    IN BOOLEAN          *Enabled,
    IN EFI_TIME         *Time OPTIONAL
);

// Memory Descriptor
typedef struct {
    UINT32                      Type;
    EFI_PHYSICAL_ADDRESS        PhysicalStart;
    EFI_VIRTUAL_ADDRESS         VirtualStart;
    UINT64                      NumberOfPages;
    UINT64                      Attribute;
} EFI_MEMORY_DESCRIPTOR;

// Changes the runtime addressing mode of EFI firmware from physical to virtual
typedef EFI_STATUS (EFIAPI *EFI_SET_VIRTUAL_ADDRESS_MAP) (
    IN UINTN                    MemoryMapSize,
    IN UINTN                    DescriptorSize,
    IN UINT32                   DescriptorVersion,
    IN EFI_MEMORY_DESCRIPTOR    *VirtualMap
);

// Determines the new virtual address that is to be used on subsequent memory accesses
typedef EFI_STATUS (EFIAPI *EFI_CONVERT_POINTER) (
    IN UINTN            DebugDisposition,
    IN VOID             **Address
);

// Returns the value of a variable
typedef EFI_STATUS (EFIAPI *EFI_GET_VARIABLE) (
    IN CHAR16           *VariableName,
    IN EFI_GUID         *VendorGuid,
    OUT UINT32          *Attributes OPTIONAL,
    IN OUT UINTN        *DataSize,
    OUT VOID            *Data OPTIONAL
);

// Enumerates the current variable names
typedef EFI_STATUS (EFIAPI *EFI_GET_NEXT_VARIABLE_NAME) (
    IN OUT UINTN        *VariableNameSize,
    IN OUT CHAR16       *VariableName,
    IN OUT EFI_GUID     *VendorGuid
);

/**
Sets the value of a variable. This service can be used to create a new
variable, modify the value of an existing variable, or to delete an existing variable.
*/
typedef EFI_STATUS (EFIAPI *EFI_SET_VARIABLE) (
    IN CHAR16           *VariableName,
    IN EFI_GUID         *VendorGuid,
    IN UINT32           Attributes,
    IN UINTN            DataSize,
    IN VOID             *Data
);

// Returns the next high 32 bits of the platform’s monotonic counter
typedef EFI_STATUS (EFIAPI *EFI_GET_NEXT_HIGH_MONO_COUNT) (
    OUT UINT32          *HighCount
);

// Reset Type
typedef enum {
    EfiResetCold,
    EfiResetWarm,
    EfiResetShutdown,
    EfiResetPlatformSpecific
} EFI_RESET_TYPE;

// Resets the entire platform
typedef VOID (EFIAPI *EFI_RESET_SYSTEM) (
    IN EFI_RESET_TYPE       ResetType,
    IN EFI_STATUS           ResetStatus,
    IN UINTN                DataSize,
    IN VOID                 *ResetData OPTIONAL
);

// Capsule Header
typedef struct {
    EFI_GUID            CapsuleGuid;
    UINT32              HeaderSize;
    UINT32              Flags;
    UINT32              CapsuleImageSize;
} EFI_CAPSULE_HEADER;

// Passes capsules to the firmware with both virtual and physical mapping
typedef EFI_STATUS (EFIAPI *EFI_UPDATE_CAPSULE) (
    IN EFI_CAPSULE_HEADER       **CapsuleHeaderArray,
    IN UINTN                    CapsuleCount,
    IN EFI_PHYSICAL_ADDRESS     ScatterGatherList OPTIONAL
);

// Returns if the capsule can be supported via UpdateCapsule()
typedef EFI_STATUS (EFIAPI *EFI_QUERY_CAPSULE_CAPABILITIES) (
    IN EFI_CAPSULE_HEADER       **CapsuleHeaderArray,
    IN UINTN                    CapsuleCount,
    OUT UINT64                  *MaximumCapsuleSize,
    OUT EFI_RESET_TYPE          *ResetType
);


typedef EFI_STATUS (EFIAPI *EFI_QUERY_VARIABLE_INFO) (
    IN UINT32           Attributes,
    OUT UINT64          *MaximumVariableStorageSize,
    OUT UINT64          *RemainingVariableStorageSize,
    OUT UINT64          *MaximumVariableSize
);

// Contains a table header and pointers to all of the runtime services
typedef struct {
    EFI_TABLE_HEADER                    Hdr;
    
    // Time Services
    EFI_GET_TIME                        GetTime;
    EFI_SET_TIME                        SetTime;
    EFI_GET_WAKEUP_TIME                 GetWakeupTime;
    EFI_SET_WAKEUP_TIME                 SetWakeupTime;

    // Virtual Memory Services
    EFI_SET_VIRTUAL_ADDRESS_MAP         SetVirtualAddressMap;
    EFI_CONVERT_POINTER                 ConvertPointer;

    // Variable Services
    EFI_GET_VARIABLE                    GetVariable;
    EFI_GET_NEXT_VARIABLE_NAME          GetNextVariableName;
    EFI_SET_VARIABLE                    SetVariable;

    // Miscellaneous Services
    EFI_GET_NEXT_HIGH_MONO_COUNT        GetNextHighMonotonicCount;
    EFI_RESET_SYSTEM                    ResetSystem;

    // UEFI 2.0 Capsule Services
    EFI_UPDATE_CAPSULE                  UpdateCapsule;
    EFI_QUERY_CAPSULE_CAPABILITIES      QueryCapsuleCapabilities;

    // Miscellaneous UEFI 2.0 Services
    EFI_QUERY_VARIABLE_INFO             QueryVariableInfo;
} EFI_RUNTIME_SERVICES;

// Raises a task’s priority level and returns it's previous level
typedef EFI_TPL (EFIAPI *EFI_RAISE_TPL) (
    IN EFI_TPL          NewTpl
);

// Restores a task’s priority level to its previous value
typedef VOID (EFIAPI *EFI_RESTORE_TPL) (
    IN EFI_TPL          OldTpl
);

// Page allocation types
typedef enum {
    AllocateAnyPages,
    AllocateMaxAddress,
    AllocateAddress,
    MaxAllocateType
} EFI_ALLOCATE_TYPE;

// Memory types
typedef enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiUnacceptedMemoryType,
    EfiMaxMemoryType
} EFI_MEMORY_TYPE;

// Allocates memory pages from the system
typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_PAGES) (
    IN EFI_ALLOCATE_TYPE            Type,
    IN EFI_MEMORY_TYPE              MemoryType,
    IN UINTN                        Pages,
    IN OUT EFI_PHYSICAL_ADDRESS     *Memory
);


// Frees memory pages
typedef EFI_STATUS (EFIAPI *EFI_FREE_PAGES) (
    IN EFI_PHYSICAL_ADDRESS     Memory,
    IN UINTN                    Pages
);

// Returns the current memory map
typedef EFI_STATUS (EFIAPI *EFI_GET_MEMORY_MAP) (
    IN OUT UINTN                *MemoryMapSize,
    OUT EFI_MEMORY_DESCRIPTOR   *MemoryMap,
    OUT UINTN                   *MapKey,
    OUT UINTN                   *DescriptorSize,
    OUT UINT32                  *DescriptorVersion
);

// Allocates pool memory
typedef EFI_STATUS (EFIAPI *EFI_ALLOCATE_POOL) (
    IN EFI_MEMORY_TYPE          PoolType,
    IN UINTN                    Size,
    OUT VOID                    **Buffer
);

// Returns pool memory to the system
typedef EFI_STATUS (EFIAPI *EFI_FREE_POOL) (
    IN VOID                     *Buffer
);

// Notify event
typedef VOID (EFIAPI *EFI_EVENT_NOTIFY) (
    IN EFI_EVENT            Event,
    IN VOID                 *Context
);

// Creates an event
typedef EFI_STATUS (EFIAPI *EFI_CREATE_EVENT) (
    IN UINT32               Type,
    IN EFI_TPL              NotifyTpl,
    IN EFI_EVENT_NOTIFY     NotifyFunction OPTIONAL,
    IN VOID                 *NotifyContext OPTIONAL,
    OUT EFI_EVENT           *Event
);

// Timer Delay type
typedef enum {
    TimerCancel,
    TimerPeriodic,
    TimerRelative
} EFI_TIMER_DELAY;

// Sets the type of timer and the trigger time for a timer event
typedef EFI_STATUS (EFIAPI *EFI_SET_TIMER) (
    IN EFI_EVENT            Event,
    IN EFI_TIMER_DELAY      Type,
    IN UINT64               TriggerTime
);

// Stops execution until an event is signaled
typedef EFI_STATUS (EFIAPI *EFI_WAIT_FOR_EVENT) (
    IN UINTN                NumberOfEvents,
    IN EFI_EVENT            *Event,
    OUT UINTN               *Index
);

// Signals an event
typedef EFI_STATUS (EFIAPI *EFI_SIGNAL_EVENT) (
    IN EFI_EVENT        Event
);

// Closes an event
typedef EFI_STATUS (EFIAPI *EFI_CLOSE_EVENT) (
    IN EFI_EVENT        Event
);

// Checks whether an event is in the signaled state
typedef EFI_STATUS (EFIAPI *EFI_CHECK_EVENT) (
    IN EFI_EVENT        Event
);

/*
Installs a protocol interface on a device handle. If the handle does not exist, it is created and added to the list of handles
in the system. InstallMultipleProtocolInterfaces() performs more error checking than InstallProtocolInterface(), so it
is recommended that InstallMultipleProtocolInterfaces() be used in place of InstallProtocolInterface()
*/
typedef EFI_STATUS (EFIAPI *EFI_INSTALL_PROTOCOL_INTERFACE) (
    IN OUT EFI_HANDLE               *Handle,
    IN EFI_GUID                     *Protocol,
    IN EFI_INTERFACE_TYPE           InterfaceType,
    IN VOID                         *Interface
);

// Reinstalls a protocol interface on a device handle
typedef EFI_STATUS (EFIAPI *EFI_REINSTALL_PROTOCOL_INTERFACE) (
    IN EFI_HANDLE               Handle,
    IN EFI_GUID                 *Protocol,
    IN VOID                     *OldInterface,
    IN VOID                     *NewInterface
);

/*
Removes a protocol interface from a device handle.
It is recommended that UninstallMultipleProtocolInterfaces() be
used in place of UninstallProtocolInterface()
*/
typedef EFI_STATUS (EFIAPI *EFI_UNINSTALL_PROTOCOL_INTERFACE) (
    IN EFI_HANDLE               Handle,
    IN EFI_GUID                 *Protocol,
    IN VOID                     *Interface
);

// Queries a handle to determine if it supports a specified protocol
typedef EFI_STATUS (EFIAPI *EFI_HANDLE_PROTOCOL) (
    IN EFI_HANDLE               Handle,
    IN EFI_GUID                 *Protocol,
    OUT VOID                    **Interface
);

// Creates an event that is to be signaled whenever an interface is installed for a specified protocol
typedef EFI_STATUS (EFIAPI *EFI_REGISTER_PROTOCOL_NOTIFY) (
    IN EFI_GUID                 *Protocol,
    IN EFI_EVENT                Event,
    OUT VOID                    **Registration
);

// Search Type
typedef enum {
    AllHandles,
    ByRegisterNotify,
    ByProtocol
} EFI_LOCATE_SEARCH_TYPE;

// Returns an array of handles that support a specified protocol
typedef EFI_STATUS (EFIAPI *EFI_LOCATE_HANDLE) (
    IN EFI_LOCATE_SEARCH_TYPE   SearchType,
    IN EFI_GUID                 *Protocol OPTIONAL,
    IN VOID                     *SearchKey OPTIONAL,
    IN OUT UINTN                *BufferSize,
    OUT EFI_HANDLE              *Buffer
);

/*
Can be used on any device handle to obtain generic path/location information concerning the physical device or logical
device. If the handle does not logically map to a physical device, the handle may not necessarily support the device
path protocol. The device path describes the location of the device the handle is for. The size of the Device Path can
be determined from the structures that make up the Device Path.
*/
typedef struct {
    UINT8           Type;
    UINT8           SubType;
    UINT8           Length[2];
} EFI_DEVICE_PATH_PROTOCOL;

// Locates the handle to a device on the device path that supports the specified protocol
typedef EFI_STATUS (EFIAPI *EFI_LOCATE_DEVICE_PATH) (
    IN EFI_GUID                         *Protocol,
    IN OUT EFI_DEVICE_PATH_PROTOCOL     **DevicePath,
    OUT EFI_HANDLE                      *Device
);

// Adds, updates, or removes a configuration table entry from the EFI System Table
typedef EFI_STATUS (EFIAPI *EFI_INSTALL_CONFIGURATION_TABLE) (
    IN EFI_GUID             *Guid,
    IN VOID                 *Table
);

// Unloads an image from memory
typedef EFI_STATUS (EFIAPI *EFI_IMAGE_UNLOAD) (
    IN EFI_HANDLE           ImageHandle
);

// Transfers control to a loaded image’s entry point
typedef EFI_STATUS (EFIAPI *EFI_IMAGE_START) (
    IN EFI_HANDLE           ImageHandle,
    OUT UINTN               *ExitDataSize,
    OUT CHAR16              **ExitData OPTIONAL
);

// Loads an EFI image into memory
typedef EFI_STATUS (EFIAPI *EFI_IMAGE_LOAD) (
    IN BOOLEAN                          BootPolicy,
    IN EFI_HANDLE                       ParentImageHandle,
    IN EFI_DEVICE_PATH_PROTOCOL         *DevicePath OPTIONAL,
    IN VOID                             *SourceBuffer OPTIONAL,
    IN UINTN                            SourceSize,
    OUT EFI_HANDLE                      *ImageHandle
);

// Terminates a loaded EFI image and returns control to boot services
typedef EFI_STATUS (EFIAPI *EFI_EXIT) (
    IN EFI_HANDLE           ImageHandle,
    IN EFI_STATUS           ExitStatus,
    IN UINTN                ExitDataSize,
    IN CHAR16               *ExitData OPTIONAL
);

// Terminates all boot services
typedef EFI_STATUS (EFIAPI *EFI_EXIT_BOOT_SERVICES) (
    IN EFI_HANDLE           ImageHandle,
    IN UINTN                MapKey
);

// Returns a monotonically increasing count for the platform
typedef EFI_STATUS (EFIAPI *EFI_GET_NEXT_MONOTONIC_COUNT) (
    OUT UINT64      *Count
);

// Induces a fine-grained stall
typedef EFI_STATUS (EFIAPI *EFI_STALL ) (
    IN UINTN        Microseconds
);

// Sets the system’s watchdog timer
typedef EFI_STATUS (EFIAPI *EFI_SET_WATCHDOG_TIMER) (
    IN UINTN        Timeout,
    IN UINT64       *WatchdogCode,
    IN UINTN        DataSize,
    IN CHAR16       *WatchdogData OPTIONAL
);

// Connects one or more drivers to a controller
typedef EFI_STATUS (EFIAPI *EFI_CONNECT_CONTROLLER) (
    IN EFI_HANDLE                   ControllerHandle,
    IN EFI_HANDLE                   *DriverImageHandle OPTIONAL,
    IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL,
    IN BOOLEAN                      Recursive
);

// Disconnects one or more drivers from a controller
typedef EFI_STATUS (EFIAPI *EFI_DISCONNECT_CONTROLLER) (
    IN EFI_HANDLE                   ControllerHandle,
    IN EFI_HANDLE                   DriverImageHandle OPTIONAL,
    IN EFI_HANDLE                   ChildHandle OPTIONAL
);

// Queries a handle to determine if it supports a specified protocol
typedef EFI_STATUS (EFIAPI *EFI_OPEN_PROTOCOL) (
    IN EFI_HANDLE               Handle,
    IN EFI_GUID                 *Protocol,
    OUT VOID                    **Interface OPTIONAL,
    IN EFI_HANDLE               AgentHandle,
    IN EFI_HANDLE               ControllerHandle,
    IN UINT32                   Attributes
);

// Closes a protocol on a handle that was opened using EFI_BOOT_SERVICES.OpenProtocol() 
typedef EFI_STATUS (EFIAPI *EFI_CLOSE_PROTOCOL) (
    IN EFI_HANDLE           Handle,
    IN EFI_GUID             *Protocol,
    IN EFI_HANDLE           AgentHandle,
    IN EFI_HANDLE           ControllerHandle
);

// Open protocol information entry
typedef struct {
    EFI_HANDLE          AgentHandle;
    EFI_HANDLE          ControllerHandle;
    UINT32              Attributes;
    UINT32              OpenCount;
} EFI_OPEN_PROTOCOL_INFORMATION_ENTRY;

// Retrieves the list of agents that currently have a protocol interface opened
typedef EFI_STATUS (EFIAPI *EFI_OPEN_PROTOCOL_INFORMATION) (
    IN EFI_HANDLE                               Handle,
    IN EFI_GUID                                 *Protocol,
    IN EFI_OPEN_PROTOCOL_INFORMATION_ENTRY      **EntryBuffer,
    OUT UINTN                                   *EntryCount
);

// Retrieves the list of protocol interface GUIDs that are installed on a handle in a buffer allocated from pool
typedef EFI_STATUS (EFIAPI *EFI_PROTOCOLS_PER_HANDLE) (
    IN EFI_HANDLE           Handle,
    OUT EFI_GUID            ***ProtocolBuffer,
    OUT UINTN               *ProtocolBufferCount
);

// Returns an array of handles that support the requested protocol in a buffer allocated from pool
typedef EFI_STATUS (EFIAPI *EFI_LOCATE_HANDLE_BUFFER) (
    IN EFI_LOCATE_SEARCH_TYPE       SearchType,
    IN EFI_GUID                     *Protocol OPTIONAL,
    IN VOID                         *SearchKey OPTIONAL,
    IN UINTN                        *NoHandles,
    OUT EFI_HANDLE                  **Buffer
);

// Returns the first protocol instance that matches the given protocol
typedef EFI_STATUS (EFIAPI *EFI_LOCATE_PROTOCOL) (
    IN EFI_GUID             *Protocol,
    IN VOID                 *Registration OPTIONAL,
    OUT VOID                **Interface
);

// Installs one or more protocol interfaces into the boot services environment
typedef EFI_STATUS (EFIAPI *EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES) (
    IN OUT EFI_HANDLE       *Handle,
    ...
);

// Removes one or more protocol interfaces into the boot services environment
typedef EFI_STATUS (EFIAPI *EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES) (
    IN EFI_HANDLE       Handle,
    ...
);

// Computes and returns a 32-bit CRC for a data buffer
typedef EFI_STATUS (EFIAPI *EFI_CALCULATE_CRC32) (
    IN VOID             *Data,
    IN UINTN            DataSize,
    OUT UINT32          *Crc32
);

// Copies the contents of one buffer to another buffer
typedef VOID (EFIAPI *EFI_COPY_MEM) (
    IN VOID             *Destination,
    IN VOID             *Source,
    IN UINTN            Length
);

// Fills a buffer with a specified value
typedef VOID (EFIAPI *EFI_SET_MEM) (
    IN VOID             *Buffer,
    IN UINTN            Size,
    IN UINT8            Value
);

// Creates an event in a group
typedef EFI_STATUS (EFIAPI *EFI_CREATE_EVENT_EX) (
    IN UINT32               Type,
    IN EFI_TPL              NotifyTpl,
    IN EFI_EVENT_NOTIFY     NotifyFunction OPTIONAL,
    IN CONST VOID           *NotifyContext OPTIONAL,
    IN CONST EFI_GUID       *EventGroup OPTIONAL,
    OUT EFI_EVENT           *Event
);

// Contains a table header and pointers to all of the boot services
typedef struct {
    EFI_TABLE_HEADER Hdr;

    // Task Priority Services
    EFI_RAISE_TPL                       RaiseTPL;
    EFI_RESTORE_TPL                     RestoreTPL;

    // Memory Services
    EFI_ALLOCATE_PAGES                  AllocatePages;
    EFI_FREE_PAGES                      FreePages;
    EFI_GET_MEMORY_MAP                  GetMemoryMap;
    EFI_ALLOCATE_POOL                   AllocatePool;
    EFI_FREE_POOL                       FreePool;

    // Event & Timer Services
    EFI_CREATE_EVENT                    CreateEvent;
    EFI_SET_TIMER                       SetTimer;
    EFI_WAIT_FOR_EVENT                  WaitForEvent;
    EFI_SIGNAL_EVENT                    SingnalEvent;
    EFI_CLOSE_EVENT                     CloseEvent;
    EFI_CHECK_EVENT                     CheckEvent;

    // Protocol Handler Services
    EFI_INSTALL_PROTOCOL_INTERFACE      InstallProtocolInterface;
    EFI_REINSTALL_PROTOCOL_INTERFACE    ReinstallProtocolInterface;
    EFI_UNINSTALL_PROTOCOL_INTERFACE    UninstallProtocolInterface;
    EFI_HANDLE_PROTOCOL                 HandleProtocol;
    VOID                                *Reserved;
    EFI_REGISTER_PROTOCOL_NOTIFY        RegisterProtocolNotify;
    EFI_LOCATE_HANDLE                   LocateHandle;
    EFI_LOCATE_DEVICE_PATH              LocateDevicePath;
    EFI_INSTALL_CONFIGURATION_TABLE     InstallConfigurationTable;

    // Image Services
    EFI_IMAGE_LOAD                    LoadImage;
    EFI_IMAGE_START                     StartImage;
    EFI_EXIT                            Exit;
    EFI_IMAGE_UNLOAD                    UnloadImage;
    EFI_EXIT_BOOT_SERVICES              ExitBootServices;

    // Misclaneous Services
    EFI_GET_NEXT_MONOTONIC_COUNT        GetNextMonotonicCount;
    EFI_STALL                           Stall;
    EFI_SET_WATCHDOG_TIMER              SetWatchdogTimer;

    // DriverSupport Services
    EFI_CONNECT_CONTROLLER              ConnectController;
    EFI_DISCONNECT_CONTROLLER           DisconnectController;

    // Open and Close Protocol Services
    EFI_OPEN_PROTOCOL                   OpenProtocol;
    EFI_CLOSE_PROTOCOL                  CloseProtocol;
    EFI_OPEN_PROTOCOL_INFORMATION       OpenProtocolInformation;

    // Library Services
    EFI_PROTOCOLS_PER_HANDLE                    ProtocolsPerHandle;
    EFI_LOCATE_HANDLE_BUFFER                    LocateHandleBuffer;
    EFI_LOCATE_PROTOCOL                         LocateProtocol;
    EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES    InstallMultipleProtocolInterfaces;
    EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES  UninstallMultipleProtocolInterfaces;

    // 32-bit CRC Services
    EFI_CALCULATE_CRC32                 CalculateCrc32;

    // Miscellaneous Services
    EFI_COPY_MEM            CopyMen;
    EFI_SET_MEM             SetMem;
    EFI_CREATE_EVENT_EX     CreateEventEx;
} EFI_BOOT_SERVICES;

// Contains pointers to the runtime and boot services tables.
typedef struct {
    EFI_TABLE_HEADER                    Hdr;
    CHAR16                              *FirmwareVendor;
    UINT32                              FirmwareRevision;
    EFI_HANDLE                          ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL      *ConIn;
    EFI_HANDLE                          ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL     *ConOut;
    EFI_HANDLE                          StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL     *StdErr;
    EFI_RUNTIME_SERVICES                *RuntimeServices;
    EFI_BOOT_SERVICES                   *BootServices;
    UINTN                               NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE             *ConfigurationTable;
} EFI_SYSTEM_TABLE;

// This is the main entry point for a UEFI Image
typedef EFI_STATUS (EFIAPI *EFI_IMAGE_ENTRY_POINT) (
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
);