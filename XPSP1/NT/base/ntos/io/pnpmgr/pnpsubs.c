/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    pnpsubs.c

Abstract:

    This module contains the plug-and-play subroutines for the
    I/O system.


Author:

    Shie-Lin Tzong (shielint) 3-Jan-1995

Environment:

    Kernel mode

Revision History:


--*/

#include "pnpmgrp.h"
#pragma hdrstop

//
// Data structure for each entry in the device reference table.
//
typedef struct _DEVICE_REFERENCE {
    PDEVICE_OBJECT  DeviceObject;   // PDO
    PUNICODE_STRING DeviceInstance; // Pointer to instance path for the devnode for the PDO
} DEVICE_REFERENCE, *PDEVICE_REFERENCE;

#ifdef POOL_TAGGING
#undef ExAllocatePool
#define ExAllocatePool(a,b) ExAllocatePoolWithTag(a,b,'uspP')
#endif

#define PpLogEvent(s, r, st, d, ds)

//
// Regular data segment
//

#ifdef ALLOC_DATA_PRAGMA
#pragma  data_seg()
#endif

//
// Table to map InstancePath to DO.
//
RTL_GENERIC_TABLE PpDeviceReferenceTable;

//
// Lock to synchronize access to the table.
//
FAST_MUTEX PpDeviceReferenceTableLock;

//
// Table of BusType GUIDs
//
GUID *PpBusTypeGuidArray;

//
// Number of entries in the BusTypeGuid table.
//
ULONG PpBusTypeGuidCount;

//
// Maximum number of entries in the BusTypeGuid table.
//
ULONG PpBusTypeGuidCountMax;

//
// Lock used to synchronize access to the BusTypeGuid table.
//
FAST_MUTEX PpBusTypeGuidLock;

//
// Prototype of internal functions
//

VOID
IopDisableDevice(
    IN PDEVICE_NODE DeviceNode
    );

BOOLEAN
IopDeleteKeyRecursiveCallback(
    IN HANDLE KeyHandle,
    IN PUNICODE_STRING KeyName,
    IN OUT PVOID Context
    );

NTSTATUS
PipGenerateMadeupNodeName (
    IN  PUNICODE_STRING ServiceKeyName,
    OUT PUNICODE_STRING MadeupNodeName
    );

RTL_GENERIC_COMPARE_RESULTS
NTAPI
PiCompareInstancePath (
    PRTL_GENERIC_TABLE Table,
    PVOID FirstStruct,
    PVOID SecondStruct
    );

ULONG
PiFixupID(
    IN PWCHAR ID,
    IN ULONG MaxIDLength,
    IN BOOLEAN Multi,
    IN ULONG AllowedSeparators,
    IN PUNICODE_STRING LogString OPTIONAL
    );

#ifdef ALLOC_PRAGMA
#pragma alloc_text(INIT, PpInitializeDeviceReferenceTable)
#pragma alloc_text(INIT, PipRegMultiSzToUnicodeStrings)
#pragma alloc_text(INIT, PipFreeUnicodeStringList)
#pragma alloc_text(INIT, PpBusTypeGuidInitialize)

#pragma alloc_text(PAGE, PipApplyFunctionToServiceInstances)
#pragma alloc_text(PAGE, PipApplyFunctionToSubKeys)
#pragma alloc_text(PAGE, IopCleanupDeviceRegistryValues)
#pragma alloc_text(PAGE, IopCmResourcesToIoResources)
#pragma alloc_text(PAGE, PipConcatenateUnicodeStrings)
#pragma alloc_text(PAGE, PipCreateMadeupNode)
#pragma alloc_text(PAGE, PipGenerateMadeupNodeName)
#pragma alloc_text(PAGE, IopCreateRegistryKeyEx)
#pragma alloc_text(PAGE, IopDeleteKeyRecursive)
#pragma alloc_text(PAGE, IopDeleteKeyRecursiveCallback)
#pragma alloc_text(PAGE, IopDeleteLegacyKey)
#pragma alloc_text(PAGE, IopDetermineResourceListSize)
#pragma alloc_text(PAGE, IopSaveDeviceCapabilities)
#pragma alloc_text(PAGE, IopQueryAndSaveDeviceNodeCapabilities)
#pragma alloc_text(PAGE, IopDeviceObjectFromDeviceInstance)
#pragma alloc_text(PAGE, IopDeviceObjectToDeviceInstance)
#pragma alloc_text(PAGE, IopDisableDevice)
#pragma alloc_text(PAGE, IopDriverLoadingFailed)
#pragma alloc_text(PAGE, IopFilterResourceRequirementsList)
#pragma alloc_text(PAGE, IopGetDeviceInstanceCsConfigFlags)
#pragma alloc_text(PAGE, IopGetDeviceResourcesFromRegistry)
#pragma alloc_text(PAGE, PipGetServiceInstanceCsConfigFlags)
#pragma alloc_text(PAGE, IopIsAnyDeviceInstanceEnabled)
#pragma alloc_text(PAGE, IopIsDeviceInstanceEnabled)
#pragma alloc_text(PAGE, PipIsDuplicatedDevices)
#pragma alloc_text(PAGE, IopIsLegacyDriver)
#pragma alloc_text(PAGE, IopMergeCmResourceLists)
#pragma alloc_text(PAGE, IopMergeFilteredResourceRequirementsList)
#pragma alloc_text(PAGE, IopOpenCurrentHwProfileDeviceInstanceKey)
#pragma alloc_text(PAGE, IopOpenRegistryKeyEx)
#pragma alloc_text(PAGE, PipOpenServiceEnumKeys)
#pragma alloc_text(PAGE, IopPrepareDriverLoading)
#pragma alloc_text(PAGE, PipReadDeviceConfiguration)
#pragma alloc_text(PAGE, IopRestartDeviceNode)
#pragma alloc_text(PAGE, PipServiceInstanceToDeviceInstance)
#pragma alloc_text(PAGE, IopMapDeviceObjectToDeviceInstance)
#pragma alloc_text(PAGE, PiRegSzToString)
#pragma alloc_text(PAGE, PiCompareInstancePath)
#pragma alloc_text(PAGE, PiAllocateGenericTableEntry)
#pragma alloc_text(PAGE, PiFreeGenericTableEntry)
#pragma alloc_text(PAGE, PpSystemHiveLimitCallback)
#pragma alloc_text(PAGE, PpBusTypeGuidGetIndex)
#pragma alloc_text(PAGE, PpBusTypeGuidGet)
#pragma alloc_text(PAGE, PpQueryDeviceID)
#pragma alloc_text(PAGE, PpQueryID)
#pragma alloc_text(PAGE, PiFixupID)
#if DBG

#pragma alloc_text(PAGE, IopDebugPrint)

#endif
#endif

NTSTATUS
PipCreateMadeupNode(
    IN PUNICODE_STRING ServiceKeyName,
    OUT PHANDLE ReturnedHandle,
    OUT PUNICODE_STRING KeyName,
    OUT PULONG InstanceNumber,
    IN BOOLEAN ResourceOwned
    )

/*++

Routine Description:

    This routine creates a new instance node under System\Enum\Root\LEGACY_<ServiceKeyName>
    key and all the required default value entries.  Also a value entry under
    Service\<ServiceKeyName>\Enum is created to point to the newly created madeup
    entry.  A handle and the keyname of the new key are returned to caller.
    Caller must free the unicode string when he is done with it.

Parameters:

    ServiceKeyName - Supplies a pointer to the name of the subkey in the
        system service list (HKEY_LOCAL_MACHINE\CurrentControlSet\Services)
        that caused the driver to load. This is the RegistryPath parameter
        to the DriverEntry routine.

    ReturnedHandle - Supplies a variable to receive the handle of the
        newly created key.

    KeyName - Supplies a variable to receive the name of the newly created
        key.

    InstanceNumber - supplies a variable to receive the InstanceNumber value
        entry created under service\name\enum subkey.

    ResourceOwned - supplies a BOOLEAN variable to indicate if caller owns
        the registry resource shared.

        ADRIAO N.B. 08/25/2000 - All users of this function pass in TRUE...

Return Value:

    Status code that indicates whether or not the function was successful.

--*/

{
    PKEY_VALUE_FULL_INFORMATION keyValueInformation;
    UNICODE_STRING tmpKeyName, unicodeInstanceName, unicodeString;
    UNICODE_STRING rootKeyName, unicodeValueName, unicodeKeyName;
    HANDLE handle, enumRootHandle;
    ULONG instance;
    UCHAR unicodeBuffer[20];
    ULONG tmpValue, disposition = 0;
    NTSTATUS status;
    PWSTR p;
    BOOLEAN releaseResource = FALSE;
    BOOLEAN successful;

    if (!ResourceOwned) {
        PiLockPnpRegistry(FALSE);
        releaseResource = TRUE;
    }

    //
    // Open LocalMachine\System\CurrentControlSet\Enum\Root
    //

    status = IopOpenRegistryKeyEx( &enumRootHandle,
                                   NULL,
                                   &CmRegistryMachineSystemCurrentControlSetEnumRootName,
                                   KEY_ALL_ACCESS
                                   );

    if (!NT_SUCCESS(status)) {
        goto local_exit0;
    }

    //
    // Generate the LEGACY_<ServiceKeyName> device id name from the
    // ServiceKeyName.
    //
    status = PipGenerateMadeupNodeName( ServiceKeyName,
                                        &unicodeKeyName);
    if (!NT_SUCCESS(status)) {
        ZwClose(enumRootHandle);
        goto local_exit0;
    }

    //
    // Open, and create if not already exist,
    // System\Enum\Root\LEGACY_<ServiceKeyName>
    //

    status = IopCreateRegistryKeyEx( &handle,
                                     enumRootHandle,
                                     &unicodeKeyName,
                                     KEY_ALL_ACCESS,
                                     REG_OPTION_NON_VOLATILE,
                                     NULL
                                     );
    ZwClose(enumRootHandle);
    if (!NT_SUCCESS(status)) {
        RtlFreeUnicodeString(&unicodeKeyName);
        goto local_exit0;
    }

    instance = 1;

    PiWstrToUnicodeString(&unicodeValueName, REGSTR_VALUE_NEXT_INSTANCE);
    status = ZwSetValueKey(
                handle,
                &unicodeValueName,
                TITLE_INDEX_VALUE,
                REG_DWORD,
                &instance,
                sizeof(instance)
                );

    instance--;
    *InstanceNumber = instance;
    PiUlongToInstanceKeyUnicodeString(&unicodeInstanceName,
                                      unicodeBuffer + sizeof(WCHAR), // reserve first WCHAR space
                                      20 - sizeof(WCHAR),
                                      instance
                                      );
    status = IopCreateRegistryKeyEx( ReturnedHandle,
                                     handle,
                                     &unicodeInstanceName,
                                     KEY_ALL_ACCESS,
                                     REG_OPTION_NON_VOLATILE,
                                     &disposition
                                     );
    ZwClose(handle);
    if (!NT_SUCCESS(status)) {
        RtlFreeUnicodeString(&unicodeKeyName);
        goto local_exit0;
    }

    //
    // Prepare newly created registry key name for returning to caller
    //

    *(PWSTR)unicodeBuffer = OBJ_NAME_PATH_SEPARATOR;
    unicodeInstanceName.Buffer = (PWSTR)unicodeBuffer;
    unicodeInstanceName.Length += sizeof(WCHAR);
    unicodeInstanceName.MaximumLength += sizeof(WCHAR);
    PiWstrToUnicodeString(&rootKeyName, REGSTR_KEY_ROOTENUM);
    PiWstrToUnicodeString(&tmpKeyName, L"\\");
    successful = PipConcatenateUnicodeStrings(&unicodeString, &tmpKeyName, &unicodeKeyName);
    RtlFreeUnicodeString(&unicodeKeyName);
    if (!successful) {
        goto local_exit0;
    }
    successful = PipConcatenateUnicodeStrings(&tmpKeyName, &rootKeyName, &unicodeString);
    RtlFreeUnicodeString(&unicodeString);
    if (!successful) {
        goto local_exit0;
    }
    successful = PipConcatenateUnicodeStrings(KeyName, &tmpKeyName, &unicodeInstanceName);
    if (!successful) {
        RtlFreeUnicodeString(&tmpKeyName);
        goto local_exit0;
    }

    if (disposition == REG_CREATED_NEW_KEY) {

        //
        // Create all the default value entry for the newly created key.
        // Service = ServiceKeyName
        // FoundAtEnum = 1
        // Class = "LegacyDriver"
        // ClassGUID = GUID for legacy driver class
        // ConfigFlags = 0
        //
        // Create "Control" subkey with "NewlyCreated" value key
        //

        PiWstrToUnicodeString(&unicodeValueName, REGSTR_KEY_CONTROL);
        status = IopCreateRegistryKeyEx( &handle,
                                         *ReturnedHandle,
                                         &unicodeValueName,
                                         KEY_ALL_ACCESS,
                                         REG_OPTION_VOLATILE,
                                         NULL
                                         );
        if (NT_SUCCESS(status)) {
            PiWstrToUnicodeString(&unicodeValueName, REGSTR_VALUE_NEWLY_CREATED);
            tmpValue = 0;
            ZwSetValueKey(handle,
                          &unicodeValueName,
                          TITLE_INDEX_VALUE,
                          REG_DWORD,
                          &tmpValue,
                          sizeof(tmpValue)
                          );
            ZwClose(handle);
        }

        handle = *ReturnedHandle;

        PiWstrToUnicodeString(&unicodeValueName, REGSTR_VALUE_SERVICE);
        p = (PWSTR)ExAllocatePool(PagedPool,
                                  ServiceKeyName->Length + sizeof(UNICODE_NULL));
        if(p) {
            RtlCopyMemory(p, ServiceKeyName->Buffer, ServiceKeyName->Length);
            p[ServiceKeyName->Length / sizeof (WCHAR)] = UNICODE_NULL;
            ZwSetValueKey(
                        handle,
                        &unicodeValueName,
                        TITLE_INDEX_VALUE,
                        REG_SZ,
                        p,
                        ServiceKeyName->Length + sizeof(UNICODE_NULL)
                        );
            //
            // We'll keep the null-terminated service name buffer around for a while,
            // because we may need it later on for the DeviceDesc in case the service
            // has no DisplayName.
            //
            // ExFreePool(p);
        }

        PiWstrToUnicodeString(&unicodeValueName, REGSTR_VALUE_LEGACY);
        tmpValue = 1;
        ZwSetValueKey(
                    handle,
                    &unicodeValueName,
                    TITLE_INDEX_VALUE,
                    REG_DWORD,
                    &tmpValue,
                    sizeof(tmpValue)
                    );

        PiWstrToUnicodeString(&unicodeValueName, REGSTR_VALUE_CONFIG_FLAGS);
        tmpValue = 0;
        ZwSetValueKey(
                    handle,
                    &unicodeValueName,
                    TITLE_INDEX_VALUE,
                    REG_DWORD,
                    &tmpValue,
                    sizeof(tmpValue)
                    );

        PiWstrToUnicodeString(&unicodeValueName, REGSTR_VALUE_CLASS);
        ZwSetValueKey(
                    handle,
                    &unicodeValueName,
                    TITLE_INDEX_VALUE,
                    REG_SZ,
                    REGSTR_VALUE_LEGACY_DRIVER,
                    sizeof(REGSTR_VALUE_LEGACY_DRIVER)
                    );

        PiWstrToUnicodeString(&unicodeValueName, REGSTR_VALUE_CLASSGUID);
        ZwSetValueKey(
                    handle,
                    &unicodeValueName,
                    TITLE_INDEX_VALUE,
                    REG_SZ,
                    (PVOID)&REGSTR_VALUE_LEGACY_DRIVER_CLASS_GUID,
                    sizeof(REGSTR_VALUE_LEGACY_DRIVER_CLASS_GUID)
                    );


        //
        // Initialize DeviceDesc= value entry.  If the service key has a "DisplayName"
        // value entry, it is used as the DeviceDesc value.  Otherwise, the service key
        // name is used.
        //

        status = PipOpenServiceEnumKeys(ServiceKeyName,
                                        KEY_READ,
                                        &handle,
                                        NULL,
                                        FALSE
                                        );
        if (NT_SUCCESS(status)) {

            keyValueInformation = NULL;
            unicodeString.Length = 0;
            status = IopGetRegistryValue(handle,
                                         REGSTR_VALUE_DISPLAY_NAME,
                                         &keyValueInformation
                                        );
            if (NT_SUCCESS(status)) {
                if (keyValueInformation->Type == REG_SZ) {
                    if (keyValueInformation->DataLength > sizeof(UNICODE_NULL)) {
                        IopRegistryDataToUnicodeString(&unicodeString,
                                                       (PWSTR)KEY_VALUE_DATA(keyValueInformation),
                                                       keyValueInformation->DataLength
                                                       );
                    }
                }
            }
            if ((unicodeString.Length == 0) && p) {

                //
                // No DisplayName--use the service key name.
                //

                unicodeString.Length = ServiceKeyName->Length;
                unicodeString.MaximumLength = ServiceKeyName->Length + sizeof(UNICODE_NULL);
                unicodeString.Buffer = p;
            }

            if(unicodeString.Length) {
                PiWstrToUnicodeString(&unicodeValueName, REGSTR_VALUE_DEVICE_DESC);
                ZwSetValueKey(*ReturnedHandle,
                              &unicodeValueName,
                              TITLE_INDEX_VALUE,
                              REG_SZ,
                              unicodeString.Buffer,
                              unicodeString.Length + sizeof(UNICODE_NULL)
                              );
            }
            if (keyValueInformation) {
                ExFreePool(keyValueInformation);
            }
            ZwClose(handle);
        }

        if(p) {
            ExFreePool(p);
        }
    }

    //
    // Create new value entry under ServiceKeyName\Enum to reflect the newly
    // added made-up device instance node.
    //

    PiUnlockPnpRegistry();
    releaseResource = FALSE;

    status = PpDeviceRegistration( KeyName, TRUE, NULL );

    if (ResourceOwned) {
        PiLockPnpRegistry(FALSE);
    }
    RtlFreeUnicodeString(&tmpKeyName);
    if (!NT_SUCCESS( status )) {

        //
        // There is no registry key for the ServiceKeyName information.
        //

        ZwClose(*ReturnedHandle);
        RtlFreeUnicodeString(KeyName);
    }
local_exit0:
    if (releaseResource) {
        PiUnlockPnpRegistry();
    }
    return status;
}

NTSTATUS
PipGenerateMadeupNodeName (
    IN  PUNICODE_STRING ServiceKeyName,
    OUT PUNICODE_STRING MadeupNodeName
    )

/*++

Routine Description:

    This routine parses the ServiceKeyName string and replaces any space
    characters with an underscore character, and any invalid characters (not
    allowed in a "device instance") with their hexadecimal character
    representation.

    Invalid characters are:
        c <  0x20 (' ')
        c >  0x7F
        c == 0x2C (',')

    The resulting modified ServiceKeyName string is used to create a valid
    device id.  Paged pool space is allocated for the destination string.
    Caller must release the space once done with it.

Arguments:

    ServiceKeyName - Supplies a pointer to the name of the subkey in the
        system service list (HKEY_LOCAL_MACHINE\CurrentControlSet\Services)
        that caused the driver to load. This is the RegistryPath parameter
        to the DriverEntry routine.

    MadeupNodeName - Supplies a variable to receive the name of madeup device
        id.  If successful, the caller is responsible for freeing the allocated
        buffer.

Return Value:

    Status code that indicates whether or not the function was successful.

--*/
{
    PWCHAR BufferEnd, p, q;
    ULONG length;
    PWSTR buffer;

    //
    // We'll need at least as much room as the size of the unicode service key
    // name, plus the LEGACY_ prefix and terminating NULL char..
    //
    length = sizeof(REGSTR_KEY_MADEUP) + ServiceKeyName->Length;

    p = ServiceKeyName->Buffer;
    BufferEnd = (PWCHAR)((PUCHAR)p + ServiceKeyName->Length);
    while(p != BufferEnd) {
        if ((*p < L' ') || (*p > (WCHAR)0x7F) || (*p == L',')) {
            //
            // Each "invalid" character will be replaced with a '*' character
            // (size already accounted for in calculated length), plus one
            // character for each nibble of each byte in the invalid character.
            //
            length += 2*sizeof(WCHAR)*sizeof(WCHAR);
        }
        p++;
    }

    //
    // Allocate a buffer large enough to hold the converted
    // LEGACY_<ServiceKeyName> string.
    //
    buffer = (PWSTR)ExAllocatePool(PagedPool, length);
    if (!buffer) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    MadeupNodeName->Buffer = buffer;
    MadeupNodeName->Length = (USHORT)(length - sizeof(UNICODE_NULL));
    MadeupNodeName->MaximumLength = (USHORT)length;

    RtlCopyMemory(buffer, REGSTR_KEY_MADEUP, sizeof(REGSTR_KEY_MADEUP));

    q = buffer + (sizeof(REGSTR_KEY_MADEUP) - sizeof(UNICODE_NULL))/sizeof(WCHAR);

    p = ServiceKeyName->Buffer;
    BufferEnd = (PWCHAR)((PUCHAR)p + ServiceKeyName->Length);
    while(p != BufferEnd) {
        if (*p == L' ') {
            //
            // replace ' ' with '_'
            //
            *q = L'_';
            q++;

        } else if ((*p < L' ')  || (*p > (WCHAR)0x7F) || (*p == L',')) {
            //
            // replace invalid characters with '*' plus a character string
            // representation of the hexadecimal digits.
            //
            int i, nibble;

            *q = L'*';
            q++;

            for (i = 1; i <= 2*sizeof(WCHAR); i++) {
                nibble = ((USHORT)((*p) >> (0x10 - 4*i)) & 0xF);
                *q = nibble > 9 ? (nibble - 10 + L'A') : (nibble + L'0');
                q++;
            }

        } else {
            //
            // copy the existing character.
            //
            *q = *p;
            q++;
        }
        p++;
    }

    *q = UNICODE_NULL;

    //
    // Upcase the resulting device id.
    //

    RtlUpcaseUnicodeString(MadeupNodeName, MadeupNodeName, FALSE);

    //
    // Sanity check to make sure that the device id we generated is valid.  At
    // this point, there should be absolutely no reason that it wouldn't be.
    //

    if (!PiFixupID(MadeupNodeName->Buffer, MAX_DEVICE_ID_LEN, FALSE, 0, NULL)) {
        ASSERT(0);
        RtlFreeUnicodeString(MadeupNodeName);
        return STATUS_INVALID_PARAMETER;
    }

    return STATUS_SUCCESS;
}

BOOLEAN
PipConcatenateUnicodeStrings (
    OUT PUNICODE_STRING Destination,
    IN  PUNICODE_STRING String1,
    IN  PUNICODE_STRING String2  OPTIONAL
    )

/*++

Routine Description:

    This routine returns a buffer containing the concatenation of the
    two specified strings.  Since String2 is optional, this function may
    also be used to make a copy of a unicode string.  Paged pool space
    is allocated for the destination string.  Caller must release the
    space once done with it.

Parameters:

    Destination - Supplies a variable to receive the concatenated
        UNICODE_STRING.

    String1 - Supplies a pointer to the frist UNICODE_STRING.

    String2 - Supplies an optional pointer to the second UNICODE_STRING.

Return Value:

    Status code that indicates whether or not the function was successful.

--*/

{
    ULONG length;
    PWSTR buffer;

    length = String1->Length + sizeof(UNICODE_NULL);
    if (ARGUMENT_PRESENT(String2)) {
        length += String2->Length;
    }
    buffer = (PWSTR)ExAllocatePool(PagedPool, length);
    if (!buffer) {
        return FALSE;
    }
    Destination->Buffer = buffer;
    Destination->Length = (USHORT)length - sizeof(UNICODE_NULL);
    Destination->MaximumLength = (USHORT)length;
    RtlCopyMemory (buffer, String1->Buffer, String1->Length);
    if(ARGUMENT_PRESENT(String2)) {
        RtlCopyMemory((PUCHAR)buffer + String1->Length,
                      String2->Buffer,
                      String2->Length
                     );
    }
    buffer[length / sizeof(WCHAR) - 1] = UNICODE_NULL;
    return TRUE;
}

NTSTATUS
IopPrepareDriverLoading (
    IN PUNICODE_STRING KeyName,
    IN HANDLE KeyHandle,
    IN PVOID ImageBase,
    IN BOOLEAN IsFilter
    )

/*++

Routine Description:

    This routine first checks if the driver is loadable.  If its a
    PnP driver, it will always be loaded (we trust it to do the right
    things.)  If it is a legacy driver, we need to check if its device
    has been disabled.  Once we decide to load the driver, the Enum
    subkey of the service node will be checked for duplicates, if any.

Parameters:

    KeyName - Supplies a pointer to the driver's service key unicode string

    KeyHandle - Supplies a handle to the driver service node in the registry
        that describes the driver to be loaded.

Return Value:

    The function value is the final status of the load operation.

--*/

{
    NTSTATUS status;
    PKEY_VALUE_FULL_INFORMATION keyValueInformation;
    ULONG tmp, count;
    HANDLE serviceEnumHandle = NULL, sysEnumXxxHandle, controlHandle;
    UNICODE_STRING unicodeKeyName, unicodeValueName;
    BOOLEAN IsPlugPlayDriver;
    PIMAGE_NT_HEADERS header;
    GUID blockedDriverGuid;

    header = RtlImageNtHeader(ImageBase);
    status = STATUS_SUCCESS;
    IsPlugPlayDriver = (header &&
                        (header->OptionalHeader.DllCharacteristics & IMAGE_DLLCHARACTERISTICS_WDM_DRIVER))? TRUE : FALSE;

    if (!IopIsAnyDeviceInstanceEnabled(KeyName, KeyHandle, (BOOLEAN)(IsPlugPlayDriver ? FALSE : TRUE))) {

        if (!IsPlugPlayDriver) {

            PiLockPnpRegistry(FALSE);

            //
            // First open registry ServiceKeyName\Enum branch
            //

            PiWstrToUnicodeString(&unicodeKeyName, REGSTR_KEY_ENUM);
            status = IopCreateRegistryKeyEx( &serviceEnumHandle,
                                             KeyHandle,
                                             &unicodeKeyName,
                                             KEY_ALL_ACCESS,
                                             REG_OPTION_VOLATILE,
                                             NULL
                                             );
            if (NT_SUCCESS(status)) {

                //
                // Find out how many device instances listed in the ServiceName's
                // Enum key.
                //

                count = 0;
                status = IopGetRegistryValue ( serviceEnumHandle,
                                               REGSTR_VALUE_COUNT,
                                               &keyValueInformation);
                if (NT_SUCCESS(status)) {

                    if (    keyValueInformation->Type == REG_DWORD &&
                            keyValueInformation->DataLength >= sizeof(ULONG)) {

                        count = *(PULONG)KEY_VALUE_DATA(keyValueInformation);

                    }

                    ExFreePool(keyValueInformation);

                }
                if (    NT_SUCCESS(status) ||
                        status == STATUS_OBJECT_PATH_NOT_FOUND ||
                        status == STATUS_OBJECT_NAME_NOT_FOUND) {

                    if (count) {

                        status = STATUS_PLUGPLAY_NO_DEVICE;

                    } else {

                        //
                        // If there is no Enum key or instance under Enum for the
                        // legacy driver we will create a madeup node for it.
                        //

                        status = PipCreateMadeupNode(   KeyName,
                                                        &sysEnumXxxHandle,
                                                        &unicodeKeyName,
                                                        &tmp,
                                                        TRUE);
                        if (NT_SUCCESS(status)) {

                            RtlFreeUnicodeString(&unicodeKeyName);

                            //
                            // Create and set Control\ActiveService value
                            //

                            PiWstrToUnicodeString(&unicodeValueName, REGSTR_KEY_CONTROL);
                            status = IopCreateRegistryKeyEx( &controlHandle,
                                                             sysEnumXxxHandle,
                                                             &unicodeValueName,
                                                             KEY_ALL_ACCESS,
                                                             REG_OPTION_VOLATILE,
                                                             NULL
                                                             );
                            if (NT_SUCCESS(status)) {

                                PiWstrToUnicodeString(&unicodeValueName, REGSTR_VAL_ACTIVESERVICE);
                                ZwSetValueKey(  controlHandle,
                                                &unicodeValueName,
                                                TITLE_INDEX_VALUE,
                                                REG_SZ,
                                                KeyName->Buffer,
                                                KeyName->Length + sizeof(UNICODE_NULL));
                                ZwClose(controlHandle);

                            }
                            count++;
                            //
                            // Don't forget to update the "Count=" and "NextInstance=" value entries
                            //

                            PiWstrToUnicodeString(&unicodeValueName, REGSTR_VALUE_COUNT);
                            ZwSetValueKey(  serviceEnumHandle,
                                            &unicodeValueName,
                                            TITLE_INDEX_VALUE,
                                            REG_DWORD,
                                            &count,
                                            sizeof(count));

                            PiWstrToUnicodeString(&unicodeValueName, REGSTR_VALUE_NEXT_INSTANCE);
                            ZwSetValueKey(  serviceEnumHandle,
                                            &unicodeValueName,
                                            TITLE_INDEX_VALUE,
                                            REG_DWORD,
                                            &count,
                                            sizeof(count));

                            ZwClose(sysEnumXxxHandle);
                            status = STATUS_SUCCESS;
                        }
                    }
                }

                ZwClose(serviceEnumHandle);
            }

            PiUnlockPnpRegistry();
        }
    }
    if (NT_SUCCESS(status)) {

        RtlZeroMemory(&blockedDriverGuid, sizeof(GUID));

        status = PpCheckInDriverDatabase(
            KeyName,
            KeyHandle,
            ImageBase,
            header->OptionalHeader.SizeOfImage,
            IsFilter,
            &blockedDriverGuid);

        if (status == STATUS_DRIVER_BLOCKED ||
            status == STATUS_DRIVER_BLOCKED_CRITICAL) {
            //
            // Notify the user-mode Plug and Play manager that a driver was just
            // blocked.
            //
            PpSetBlockedDriverEvent(&blockedDriverGuid);
        }
    }

    return status;
}

NTSTATUS
PipServiceInstanceToDeviceInstance (
    IN  HANDLE ServiceKeyHandle OPTIONAL,
    IN  PUNICODE_STRING ServiceKeyName OPTIONAL,
    IN  ULONG ServiceInstanceOrdinal,
    OUT PUNICODE_STRING DeviceInstanceRegistryPath OPTIONAL,
    OUT PHANDLE DeviceInstanceHandle OPTIONAL,
    IN  ACCESS_MASK DesiredAccess
    )

/*++

Routine Description:

    This routine reads the service node enum entry to find the desired device instance
    under the System\Enum tree.  It then optionally returns the registry path of the
    specified device instance (relative to HKLM\System\Enum) and an open handle
    to that registry key.

    It is the caller's responsibility to close the handle returned if
    DeviceInstanceHandle is supplied, and also to free the (PagedPool) memory
    allocated for the unicode string buffer of DeviceInstanceRegistryPath, if
    supplied.

Parameters:

    ServiceKeyHandle - Optionally, supplies a handle to the driver service node in the
        registry that controls this device instance.  If this argument is not specified,
        then ServiceKeyName is used to specify the service entry.

    ServiceKeyName - Optionally supplies the name of the service entry that controls
        the device instance. This must be specified if ServiceKeyHandle isn't given.

    ServiceInstanceOrdinal - Supplies the instance value under the service entry's
        volatile Enum subkey that references the desired device instance.

    DeviceInstanceRegistryPath - Optionally, supplies a pointer to a unicode string
        that will be initialized with the registry path (relative to HKLM\System\Enum)
        to the device instance key.

    DeviceInstanceHandle - Optionally, supplies a pointer to a variable that will
        receive a handle to the opened device instance registry key.

    DesiredAccess - If DeviceInstanceHandle is specified (i.e., the device instance
        key is to be opened), then this variable specifies the access that is needed
        to this key.

Return Value:

    NT status code indicating whether the function was successful.

--*/

{
    WCHAR unicodeBuffer[20];
    UNICODE_STRING unicodeKeyName;
    NTSTATUS status;
    HANDLE handle;
    PKEY_VALUE_FULL_INFORMATION keyValueInformation;

    //
    // Open registry ServiceKeyName\Enum branch
    //
    if(ARGUMENT_PRESENT(ServiceKeyHandle)) {

        PiWstrToUnicodeString(&unicodeKeyName, REGSTR_KEY_ENUM);
        status = IopOpenRegistryKeyEx( &handle,
                                       ServiceKeyHandle,
                                       &unicodeKeyName,
                                       KEY_READ
                                       );
    } else {

        status = PipOpenServiceEnumKeys(ServiceKeyName,
                                        KEY_READ,
                                        NULL,
                                        &handle,
                                        FALSE
                                       );
    }

    if (!NT_SUCCESS( status )) {

        //
        // There is no registry key for the ServiceKeyName\Enum information.
        //

        return status;
    }

    //
    // Read a path to System\Enum hardware tree branch specified by the service
    // instance ordinal
    //

    swprintf(unicodeBuffer, REGSTR_VALUE_STANDARD_ULONG_FORMAT, ServiceInstanceOrdinal);
    status = IopGetRegistryValue ( handle,
                                   unicodeBuffer,
                                   &keyValueInformation
                                   );

    ZwClose(handle);
    if (!NT_SUCCESS( status )) {
        return status;
    } else {
        if(keyValueInformation->Type == REG_SZ) {
            IopRegistryDataToUnicodeString(&unicodeKeyName,
                                           (PWSTR)KEY_VALUE_DATA(keyValueInformation),
                                           keyValueInformation->DataLength
                                          );
            if(!unicodeKeyName.Length) {
                status = STATUS_OBJECT_PATH_NOT_FOUND;
            }
        } else {
            status = STATUS_INVALID_PLUGPLAY_DEVICE_PATH;
        }

        if(!NT_SUCCESS(status)) {
            goto PrepareForReturn;
        }
    }

    //
    // If the DeviceInstanceHandle argument was specified, open the device instance
    // key under HKLM\System\CurrentControlSet\Enum
    //

    if (ARGUMENT_PRESENT(DeviceInstanceHandle)) {

        status = IopOpenRegistryKeyEx( &handle,
                                       NULL,
                                       &CmRegistryMachineSystemCurrentControlSetEnumName,
                                       KEY_READ
                                       );

        if (NT_SUCCESS( status )) {

            status = IopOpenRegistryKeyEx( DeviceInstanceHandle,
                                           handle,
                                           &unicodeKeyName,
                                           DesiredAccess
                                           );
            ZwClose(handle);
        }

        if (!NT_SUCCESS( status )) {
            goto PrepareForReturn;
        }
    }

    //
    // If the DeviceInstanceRegistryPath argument was specified, then store a
    // copy of the device instance path in the supplied unicode string variable.
    //
    if (ARGUMENT_PRESENT(DeviceInstanceRegistryPath)) {

        if (!PipConcatenateUnicodeStrings(DeviceInstanceRegistryPath,
                                          &unicodeKeyName,
                                          NULL)) {

            if(ARGUMENT_PRESENT(DeviceInstanceHandle)) {
                ZwClose(*DeviceInstanceHandle);
            }
            status = STATUS_INSUFFICIENT_RESOURCES;
        }
    }

PrepareForReturn:

    ExFreePool(keyValueInformation);
    return status;
}

NTSTATUS
IopOpenRegistryKeyEx(
    OUT PHANDLE Handle,
    IN HANDLE BaseHandle OPTIONAL,
    IN PUNICODE_STRING KeyName,
    IN ACCESS_MASK DesiredAccess
    )

/*++

Routine Description:

    Opens a registry key using the name passed in based at the BaseHandle node.
    This name may specify a key that is actually a registry path.

Arguments:

    Handle - Pointer to the handle which will contain the registry key that
        was opened.

    BaseHandle - Optional handle to the base path from which the key must be opened.
        If KeyName specifies a registry path that must be created, then this parameter
        must be specified, and KeyName must be a relative path.

    KeyName - Name of the Key that must be opened/created (possibly a registry path)

    DesiredAccess - Specifies the desired access that the caller needs to
        the key.

Return Value:

   The function value is the final status of the operation.

--*/

{
    OBJECT_ATTRIBUTES objectAttributes;

    PAGED_CODE();

    InitializeObjectAttributes( &objectAttributes,
                                KeyName,
                                OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                                BaseHandle,
                                (PSECURITY_DESCRIPTOR) NULL
                                );
    //
    // Simply attempt to open the path, as specified.
    //
    return ZwOpenKey( Handle, DesiredAccess, &objectAttributes );
}

NTSTATUS
IopCreateRegistryKeyEx(
    OUT PHANDLE Handle,
    IN HANDLE BaseHandle OPTIONAL,
    IN PUNICODE_STRING KeyName,
    IN ACCESS_MASK DesiredAccess,
    IN ULONG CreateOptions,
    OUT PULONG Disposition OPTIONAL
    )

/*++

Routine Description:

    Opens or creates a registry key using the name
    passed in based at the BaseHandle node. This name may specify a key
    that is actually a registry path, in which case each intermediate subkey
    will be created (if Create is TRUE).

    NOTE: Creating a registry path (i.e., more than one of the keys in the path
    do not presently exist) requires that a BaseHandle be specified.

Arguments:

    Handle - Pointer to the handle which will contain the registry key that
        was opened.

    BaseHandle - Optional handle to the base path from which the key must be opened.
        If KeyName specifies a registry path that must be created, then this parameter
        must be specified, and KeyName must be a relative path.

    KeyName - Name of the Key that must be opened/created (possibly a registry path)

    DesiredAccess - Specifies the desired access that the caller needs to
        the key.

    CreateOptions - Options passed to ZwCreateKey.

    Disposition - If Create is TRUE, this optional pointer receives a ULONG indicating
        whether the key was newly created:

            REG_CREATED_NEW_KEY - A new Registry Key was created
            REG_OPENED_EXISTING_KEY - An existing Registry Key was opened

Return Value:

   The function value is the final status of the operation.

--*/

{
    OBJECT_ATTRIBUTES objectAttributes;
    ULONG disposition, baseHandleIndex = 0, keyHandleIndex = 1, closeBaseHandle;
    HANDLE handles[2];
    BOOLEAN continueParsing;
    PWCHAR pathEndPtr, pathCurPtr, pathBeginPtr;
    ULONG pathComponentLength;
    UNICODE_STRING unicodeString;
    NTSTATUS status;

    PAGED_CODE();

    InitializeObjectAttributes( &objectAttributes,
                                KeyName,
                                OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                                BaseHandle,
                                (PSECURITY_DESCRIPTOR) NULL
                                );
    //
    // Attempt to create the path as specified. We have to try it this
    // way first, because it allows us to create a key without a BaseHandle
    // (if only the last component of the registry path is not present).
    //
    status = ZwCreateKey(&(handles[keyHandleIndex]),
                         DesiredAccess,
                         &objectAttributes,
                         0,
                         (PUNICODE_STRING) NULL,
                         CreateOptions,
                         &disposition
                         );

    if (status == STATUS_OBJECT_NAME_NOT_FOUND && ARGUMENT_PRESENT(BaseHandle)) {
        //
        // If we get to here, then there must be more than one element of the
        // registry path that does not currently exist.  We will now parse the
        // specified path, extracting each component and doing a ZwCreateKey on it.
        //
        handles[baseHandleIndex] = NULL;
        handles[keyHandleIndex] = BaseHandle;
        closeBaseHandle = 0;
        continueParsing = TRUE;
        pathBeginPtr = KeyName->Buffer;
        pathEndPtr = (PWCHAR)((PCHAR)pathBeginPtr + KeyName->Length);
        status = STATUS_SUCCESS;

        while(continueParsing) {
            //
            // There's more to do, so close the previous base handle (if necessary),
            // and replace it with the current key handle.
            //
            if(closeBaseHandle > 1) {
                ZwClose(handles[baseHandleIndex]);
            }
            baseHandleIndex = keyHandleIndex;
            keyHandleIndex = (keyHandleIndex + 1) & 1;  // toggle between 0 and 1.
            handles[keyHandleIndex] = NULL;

            //
            // Extract next component out of the specified registry path.
            //
            for (pathCurPtr = pathBeginPtr;
                ((pathCurPtr < pathEndPtr) && (*pathCurPtr != OBJ_NAME_PATH_SEPARATOR));
                pathCurPtr++);

            pathComponentLength = (ULONG)((PCHAR)pathCurPtr - (PCHAR)pathBeginPtr);
            if (pathComponentLength != 0) {
                //
                // Then we have a non-empty path component (key name).  Attempt
                // to create this key.
                //
                unicodeString.Buffer = pathBeginPtr;
                unicodeString.Length = unicodeString.MaximumLength = (USHORT)pathComponentLength;

                InitializeObjectAttributes(&objectAttributes,
                                           &unicodeString,
                                           OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                                           handles[baseHandleIndex],
                                           (PSECURITY_DESCRIPTOR) NULL
                                          );
                status = ZwCreateKey(&(handles[keyHandleIndex]),
                                     DesiredAccess,
                                     &objectAttributes,
                                     0,
                                     (PUNICODE_STRING) NULL,
                                     CreateOptions,
                                     &disposition
                                    );
                if(NT_SUCCESS(status)) {
                    //
                    // Increment the closeBaseHandle value, which basically tells us whether
                    // the BaseHandle passed in has been 'shifted out' of our way, so that
                    // we should start closing our base handles when we're finished with them.
                    //
                    closeBaseHandle++;
                } else {
                    continueParsing = FALSE;
                    continue;
                }
            } else {
                //
                // Either a path separator ('\') was included at the beginning of
                // the path, or we hit 2 consecutive separators.
                //
                status = STATUS_INVALID_PARAMETER;
                continueParsing = FALSE;
                continue;
            }

            if((pathCurPtr == pathEndPtr) ||
               ((pathBeginPtr = pathCurPtr + 1) == pathEndPtr)) {
                //
                // Then we've reached the end of the path
                //
                continueParsing = FALSE;
            }
        }

        if(closeBaseHandle > 1) {
            ZwClose(handles[baseHandleIndex]);
        }
    }

    if(NT_SUCCESS(status)) {
        *Handle = handles[keyHandleIndex];

        if(ARGUMENT_PRESENT(Disposition)) {
            *Disposition = disposition;
        }
    }

    return status;
}

NTSTATUS
PipOpenServiceEnumKeys (
    IN PUNICODE_STRING ServiceKeyName,
    IN ACCESS_MASK DesiredAccess,
    OUT PHANDLE ServiceHandle OPTIONAL,
    OUT PHANDLE ServiceEnumHandle OPTIONAL,
    IN BOOLEAN CreateEnum
    )

/*++

Routine Description:

    This routine opens the HKEY_LOCAL_MACHINE\CurrentControlSet\Services\
    ServiceKeyName and its Enum subkey and returns handles for both key.
    It is caller's responsibility to close the returned handles.

Arguments:

    ServiceKeyName - Supplies a pointer to the name of the subkey in the
        system service list (HKEY_LOCAL_MACHINE\CurrentControlSet\Services)
        that caused the driver to load. This is the RegistryPath parameter
        to the DriverEntry routine.

    DesiredAccess - Specifies the desired access to the keys.

    ServiceHandle - Supplies a variable to receive a handle to ServiceKeyName.
        A NULL ServiceHandle indicates caller does not want need the handle to
        the ServiceKeyName.

    ServiceEnumHandle - Supplies a variable to receive a handle to ServiceKeyName\Enum.
        A NULL ServiceEnumHandle indicates caller does not need the handle to
        the ServiceKeyName\Enum.

    CreateEnum - Supplies a BOOLEAN variable to indicate should the Enum subkey be
        created if not present.

Return Value:

    status

--*/

{
    HANDLE handle, serviceHandle, enumHandle;
    UNICODE_STRING enumName;
    NTSTATUS status;

    //
    // Open System\CurrentControlSet\Services
    //

    status = IopOpenRegistryKeyEx( &handle,
                                   NULL,
                                   &CmRegistryMachineSystemCurrentControlSetServices,
                                   DesiredAccess
                                   );

    if (!NT_SUCCESS( status )) {
        return status;
    }

    //
    // Open the registry ServiceKeyName key.
    //

    status = IopOpenRegistryKeyEx( &serviceHandle,
                                   handle,
                                   ServiceKeyName,
                                   DesiredAccess
                                   );

    ZwClose(handle);
    if (!NT_SUCCESS( status )) {

        //
        // There is no registry key for the ServiceKeyName information.
        //

        return status;
    }

    if (ARGUMENT_PRESENT(ServiceEnumHandle) || CreateEnum) {

        //
        // Open registry ServiceKeyName\Enum branch if caller wants
        // the handle or wants to create it.
        //

        PiWstrToUnicodeString(&enumName, REGSTR_KEY_ENUM);

        if (CreateEnum) {
            status = IopCreateRegistryKeyEx( &enumHandle,
                                             serviceHandle,
                                             &enumName,
                                             DesiredAccess,
                                             REG_OPTION_VOLATILE,
                                             NULL
                                             );
        } else {
            status = IopOpenRegistryKeyEx( &enumHandle,
                                           serviceHandle,
                                           &enumName,
                                           DesiredAccess
                                           );

        }

        if (!NT_SUCCESS( status )) {

            //
            // There is no registry key for the ServiceKeyName\Enum information.
            //

            ZwClose(serviceHandle);
            return status;
        }
        if (ARGUMENT_PRESENT(ServiceEnumHandle)) {
            *ServiceEnumHandle = enumHandle;
        } else {
            ZwClose(enumHandle);
        }
    }

    //
    // if caller wants to have the ServiceKey handle, we return it.  Otherwise
    // we close it.
    //

    if (ARGUMENT_PRESENT(ServiceHandle)) {
        *ServiceHandle = serviceHandle;
    } else {
        ZwClose(serviceHandle);
    }

    return STATUS_SUCCESS;
}

NTSTATUS
IopGetDeviceInstanceCsConfigFlags(
    IN PUNICODE_STRING DeviceInstance,
    OUT PULONG CsConfigFlags
    )

/*++

Routine Description:

    This routine retrieves the csconfig flags for the specified device.

Arguments:

    DeviceInstance - Supplies a pointer to the devnode's instance path

    CsConfigFlags - Supplies a variable to receive the device's CsConfigFlags

Return Value:

    status

--*/

{
    NTSTATUS status;
    HANDLE handle1, handle2;
    UNICODE_STRING tempUnicodeString;
    PKEY_VALUE_FULL_INFORMATION keyValueInformation;

    PAGED_CODE();

    *CsConfigFlags = 0;

    status = IopOpenRegistryKeyEx( &handle1,
                                   NULL,
                                   &CmRegistryMachineSystemCurrentControlSetHardwareProfilesCurrent,
                                   KEY_READ
                                   );

    if (!NT_SUCCESS(status)) {

        return status;
    }

    //
    // Now, we must open the System\CCS\Enum key under this.
    //
    //
    // Open system\CurrentControlSet under current hardware profile key
    //

    PiWstrToUnicodeString(&tempUnicodeString, REGSTR_PATH_CURRENTCONTROLSET);
    status = IopOpenRegistryKeyEx( &handle2,
                                   handle1,
                                   &tempUnicodeString,
                                   KEY_READ
                                   );
    ZwClose(handle1);

    if (!NT_SUCCESS(status)) {

        return status;
    }

    PiWstrToUnicodeString(&tempUnicodeString, REGSTR_KEY_ENUM);

    status = IopOpenRegistryKeyEx( &handle1,
                                   handle2,
                                   &tempUnicodeString,
                                   KEY_READ
                                   );

    ZwClose(handle2);

    if (!NT_SUCCESS(status)) {

        return status;
    }


    status = IopOpenRegistryKeyEx( &handle2,
                                   handle1,
                                   DeviceInstance,
                                   KEY_READ
                                   );

    ZwClose(handle1);

    if (!NT_SUCCESS(status)) {

        return status;
    }


    status = IopGetRegistryValue( handle2,
                                  REGSTR_VALUE_CSCONFIG_FLAGS,
                                  &keyValueInformation
                                  );

    ZwClose(handle2);

    if (NT_SUCCESS(status)) {
        if ((keyValueInformation->Type == REG_DWORD) &&
            (keyValueInformation->DataLength >= sizeof(ULONG))) {

            *CsConfigFlags = *(PULONG)KEY_VALUE_DATA(keyValueInformation);
        }
        ExFreePool(keyValueInformation);
    }

    return status;
}

NTSTATUS
PipGetServiceInstanceCsConfigFlags(
    IN PUNICODE_STRING ServiceKeyName,
    IN ULONG Instance,
    OUT PULONG CsConfigFlags
    )

/*++

Routine Description:

    This routine retrieves the csconfig flags for the specified device
    which is specified by the instance number under ServiceKeyName\Enum.

Arguments:

    ServiceKeyName - Supplies a pointer to the name of the subkey in the
        system service list (HKEY_LOCAL_MACHINE\CurrentControlSet\Services)
        that caused the driver to load.

    Instance - Supplies the instance value under ServiceKeyName\Enum key

    CsConfigFlags - Supplies a variable to receive the device's CsConfigFlags

Return Value:

    status

--*/

{
    NTSTATUS status;
    HANDLE handle;
    PKEY_VALUE_FULL_INFORMATION keyValueInformation;

    PAGED_CODE();

    *CsConfigFlags = 0;

    status = IopOpenCurrentHwProfileDeviceInstanceKey(&handle,
                                                      ServiceKeyName,
                                                      Instance,
                                                      KEY_READ,
                                                      FALSE
                                                     );
    if(NT_SUCCESS(status)) {
        status = IopGetRegistryValue(handle,
                                     REGSTR_VALUE_CSCONFIG_FLAGS,
                                     &keyValueInformation
                                    );
        if(NT_SUCCESS(status)) {
            if((keyValueInformation->Type == REG_DWORD) &&
               (keyValueInformation->DataLength >= sizeof(ULONG))) {
                *CsConfigFlags = *(PULONG)KEY_VALUE_DATA(keyValueInformation);
            }
            ExFreePool(keyValueInformation);
        }
        ZwClose(handle);
    }
    return status;
}

NTSTATUS
IopOpenCurrentHwProfileDeviceInstanceKey(
    OUT PHANDLE Handle,
    IN  PUNICODE_STRING ServiceKeyName,
    IN  ULONG Instance,
    IN  ACCESS_MASK DesiredAccess,
    IN  BOOLEAN Create
    )

/*++

Routine Description:

    This routine sets the csconfig flags for the specified device
    which is specified by the instance number under ServiceKeyName\Enum.

Arguments:

    ServiceKeyName - Supplies a pointer to the name of the subkey in the
        system service list (HKEY_LOCAL_MACHINE\CurrentControlSet\Services)
        that caused the driver to load. This is the RegistryPath parameter
        to the DriverEntry routine.

    Instance - Supplies the instance value under ServiceKeyName\Enum key

    DesiredAccess - Specifies the desired access that the caller needs to
        the key.

    Create - Determines if the key is to be created if it does not exist.

Return Value:

    status

--*/

{
    NTSTATUS status;
    UNICODE_STRING tempUnicodeString;
    HANDLE profileHandle, profileEnumHandle, tmpHandle;

    //
    // See if we can open current hardware profile
    //

    if (Create) {
        status = IopCreateRegistryKeyEx( &profileHandle,
                                         NULL,
                                         &CmRegistryMachineSystemCurrentControlSetHardwareProfilesCurrent,
                                         KEY_READ,
                                         REG_OPTION_NON_VOLATILE,
                                         NULL
                                         );
    } else {
        status = IopOpenRegistryKeyEx( &profileHandle,
                                       NULL,
                                       &CmRegistryMachineSystemCurrentControlSetHardwareProfilesCurrent,
                                       KEY_READ
                                       );
    }

    if(NT_SUCCESS(status)) {
        //
        // Now, we must open the System\CCS\Enum key under this.
        //
        //
        // Open system\CurrentControlSet under current hardware profile key
        //

        PiWstrToUnicodeString(&tempUnicodeString, REGSTR_PATH_CURRENTCONTROLSET);
        status = IopOpenRegistryKeyEx( &tmpHandle,
                                       profileHandle,
                                       &tempUnicodeString,
                                       DesiredAccess
                                       );
        ZwClose(profileHandle);
        if (!NT_SUCCESS(status)) {
            return status;
        }

        PiWstrToUnicodeString(&tempUnicodeString, REGSTR_KEY_ENUM);

        if (Create) {
            status = IopCreateRegistryKeyEx( &profileEnumHandle,
                                             tmpHandle,
                                             &tempUnicodeString,
                                             KEY_READ,
                                             REG_OPTION_NON_VOLATILE,
                                             NULL
                                             );
        } else {
            status = IopOpenRegistryKeyEx( &profileEnumHandle,
                                           tmpHandle,
                                           &tempUnicodeString,
                                           KEY_READ
                                           );
        }

        ZwClose(tmpHandle);
        if(NT_SUCCESS(status)) {

            status = PipServiceInstanceToDeviceInstance(NULL,
                                                        ServiceKeyName,
                                                        Instance,
                                                        &tempUnicodeString,
                                                        NULL,
                                                        0
                                                       );
            if (NT_SUCCESS(status)) {
                if (Create) {
                    status = IopCreateRegistryKeyEx( Handle,
                                                     profileEnumHandle,
                                                     &tempUnicodeString,
                                                     DesiredAccess,
                                                     REG_OPTION_NON_VOLATILE,
                                                     NULL
                                                     );
                } else {
                    status = IopOpenRegistryKeyEx( Handle,
                                                   profileEnumHandle,
                                                   &tempUnicodeString,
                                                   DesiredAccess
                                                   );
                }
                RtlFreeUnicodeString(&tempUnicodeString);
            }
            ZwClose(profileEnumHandle);
        }
    }
    return status;
}

NTSTATUS
PipApplyFunctionToSubKeys(
    IN     HANDLE BaseHandle OPTIONAL,
    IN     PUNICODE_STRING KeyName OPTIONAL,
    IN     ACCESS_MASK DesiredAccess,
    IN     ULONG Flags,
    IN     PIOP_SUBKEY_CALLBACK_ROUTINE SubKeyCallbackRoutine,
    IN OUT PVOID Context
    )

/*++

Routine Description:

    This routine enumerates all subkeys under the specified key, and calls
    the specified callback routine for each subkey.

Arguments:

    BaseHandle - Optional handle to the base registry path. If KeyName is also
        specified, then KeyName represents a subkey under this path.  If KeyName
        is not specified, the subkeys are enumerated under this handle.  If this
        parameter is not specified, then the full path to the base key must be
        given in KeyName.

    KeyName - Optional name of the key whose subkeys are to be enumerated.

    DesiredAccess - Specifies the desired access that the callback routine
        needs to the subkeys.  If no desired access is specified (i.e.,
        DesiredAccess is zero), then no handle will be opened for the
        subkeys, and the callback will be passed a NULL for its SubKeyHandle
        parameter.

    Flags - Controls the behavior of subkey enumeration.  Currently, the
        following flags are defined:

        FUNCTIONSUBKEY_FLAG_IGNORE_NON_CRITICAL_ERRORS - Specifies whether this
            function should immediately terminate on all errors, or only on
            critical ones.  An example of a non-critical error is when an
            enumerated subkey cannot be opened for the desired access.

        FUNCTION_SUBKEY_DELETE_SUBKEYS - Specifies that each subkey should be
            deleted after the specified SubKeyCallBackRoutine has been performed
            on it.  Note that this is NOT a recursive delete on each of the
            subkeys, just an attempt to delete the subkey itself.  It the subkey
            contains children, this will fail.

    SubKeyCallbackRoutine - Supplies a pointer to a function that will
        be called for each subkey found under the
        specified key.  The prototype of the function
        is as follows:

            typedef BOOLEAN (*PIOP_SUBKEY_CALLBACK_ROUTINE) (
                IN     HANDLE SubKeyHandle,
                IN     PUNICODE_STRING SubKeyName,
                IN OUT PVOID Context
                );

        where SubKeyHandle is the handle to an enumerated subkey under the
        specified key, SubKeyName is its name, and Context is a pointer to
        user-defined data.

        This function should return TRUE to continue enumeration, or
        FALSE to terminate it.

    Context - Supplies a pointer to user-defined data that will be passed
        in to the callback routine at each subkey invocation.

Return Value:

    NT status code indicating whether the subkeys were successfully
    enumerated.  Note that this does not provide information on the
    success or failure of the callback routine--if desired, this
    information should be stored in the Context structure.

--*/

{
    NTSTATUS Status;
    BOOLEAN CloseHandle = FALSE, ContinueEnumeration;
    HANDLE Handle, SubKeyHandle;
    ULONG i, RequiredBufferLength;
    PKEY_BASIC_INFORMATION KeyInformation = NULL;
    // Use an initial key name buffer size large enough for a 20-character key
    // (+ terminating NULL)
    ULONG KeyInformationLength = sizeof(KEY_BASIC_INFORMATION) + (20 * sizeof(WCHAR));
    UNICODE_STRING SubKeyName;

    if(ARGUMENT_PRESENT(KeyName)) {

        Status = IopOpenRegistryKeyEx( &Handle,
                                       BaseHandle,
                                       KeyName,
                                       KEY_READ
                                       );
        if(!NT_SUCCESS(Status)) {
            return Status;
        } else {
            CloseHandle = TRUE;
        }

    } else {

        Handle = BaseHandle;
    }

    //
    // Enumerate the subkeys until we run out of them.
    //
    i = 0;
    SubKeyHandle = NULL;

    for ( ; ; ) {

        if (!KeyInformation) {

            KeyInformation = (PKEY_BASIC_INFORMATION)ExAllocatePool(PagedPool,
                                                                    KeyInformationLength
                                                                   );
            if (!KeyInformation) {
                Status = STATUS_INSUFFICIENT_RESOURCES;
                break;
            }
        }

        Status = ZwEnumerateKey(Handle,
                                i,
                                KeyBasicInformation,
                                KeyInformation,
                                KeyInformationLength,
                                &RequiredBufferLength
                               );

        if (!NT_SUCCESS(Status)) {
            if (Status == STATUS_BUFFER_OVERFLOW ||
                Status == STATUS_BUFFER_TOO_SMALL) {
                //
                // Try again with larger buffer.
                //
                ExFreePool(KeyInformation);
                KeyInformation = NULL;
                KeyInformationLength = RequiredBufferLength;
                continue;

            } else {

                if (Status == STATUS_NO_MORE_ENTRIES) {
                    //
                    // No more subkeys.
                    //
                    Status = STATUS_SUCCESS;
                }
                //
                // break out of loop
                //
                break;
            }
        }

        //
        // Initialize a unicode string with this key name.  Note that this string
        // WILL NOT be NULL-terminated.
        //
        SubKeyName.Length = SubKeyName.MaximumLength = (USHORT)KeyInformation->NameLength;
        SubKeyName.Buffer = KeyInformation->Name;

        //
        // If DesiredAccess is non-zero, open a handle to this subkey.
        //
        if (DesiredAccess) {
            Status = IopOpenRegistryKeyEx( &SubKeyHandle,
                                           Handle,
                                           &SubKeyName,
                                           DesiredAccess
                                           );
            if (!NT_SUCCESS(Status)) {
                //
                // This is a non-critical error.
                //
                if(Flags & FUNCTIONSUBKEY_FLAG_IGNORE_NON_CRITICAL_ERRORS) {
                    goto ContinueWithNextSubKey;
                } else {
                    break;
                }
            }
        }

        //
        // Invoke the supplied callback function for this subkey.
        //
        ContinueEnumeration = SubKeyCallbackRoutine(SubKeyHandle, &SubKeyName, Context);

        if (DesiredAccess) {
            if (ContinueEnumeration &&
                (Flags & FUNCTIONSUBKEY_FLAG_DELETE_SUBKEYS)) {
                //
                // Delete the key when asked to, only if the callback routine
                // was successful, otherwise we may not be able to.
                //
                Status = ZwDeleteKey(SubKeyHandle);
            }
            ZwClose(SubKeyHandle);
        }

        if(!ContinueEnumeration) {
            //
            // Enumeration has been aborted.
            //
            Status = STATUS_SUCCESS;
            break;

        }

ContinueWithNextSubKey:
        if (!(Flags & FUNCTIONSUBKEY_FLAG_DELETE_SUBKEYS)) {
            //
            // Only increment the enumeration index for non-deleted subkeys
            //
            i++;
        }
    }

    if(KeyInformation) {
        ExFreePool(KeyInformation);
    }

    if(CloseHandle) {
        ZwClose(Handle);
    }

    return Status;
}

NTSTATUS
PipRegMultiSzToUnicodeStrings(
    IN  PKEY_VALUE_FULL_INFORMATION KeyValueInformation,
    OUT PUNICODE_STRING *UnicodeStringList,
    OUT PULONG UnicodeStringCount
    )

/*++

Routine Description:

    This routine takes a KEY_VALUE_FULL_INFORMATION structure containing
    a REG_MULTI_SZ value, and allocates an array of UNICODE_STRINGs,
    initializing each one to a copy of one of the strings in the value entry.
    All the resulting UNICODE_STRINGs will be NULL terminated
    (MaximumLength = Length + sizeof(UNICODE_NULL)).

    It is the responsibility of the caller to free the buffers for each
    unicode string, as well as the buffer containing the UNICODE_STRING
    array. This may be done by calling PipFreeUnicodeStringList.

Arguments:

    KeyValueInformation - Supplies the buffer containing the REG_MULTI_SZ
        value entry data.

    UnicodeStringList - Receives a pointer to an array of UNICODE_STRINGs, each
        initialized with a copy of one of the strings in the REG_MULTI_SZ.

    UnicodeStringCount - Receives the number of strings in the
        UnicodeStringList.

Returns:

    NT status code indicating whether the function was successful.

    NOTE: This function is only available during INIT time!

--*/

{
    PWCHAR p, BufferEnd, StringStart;
    ULONG StringCount, i, StringLength;

    //
    // First, make sure this is really a REG_MULTI_SZ value.
    //
    if(KeyValueInformation->Type != REG_MULTI_SZ) {
        return STATUS_INVALID_PARAMETER;
    }

    //
    // Make a preliminary pass through the buffer to count the number of strings
    // There will always be at least one string returned (possibly empty).
    //
    StringCount = 0;
    p = (PWCHAR)KEY_VALUE_DATA(KeyValueInformation);
    BufferEnd = (PWCHAR)((PUCHAR)p + KeyValueInformation->DataLength);
    while(p != BufferEnd) {
        if(!*p) {
            StringCount++;
            if(((p + 1) == BufferEnd) || !*(p + 1)) {
                break;
            }
        }
        p++;
    }
    if(p == BufferEnd) {
        StringCount++;
    }

    *UnicodeStringList = ExAllocatePool(PagedPool, sizeof(UNICODE_STRING) * StringCount);
    if(!(*UnicodeStringList)) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    //
    // Now, make a second pass through the buffer making copies of each string.
    //
    i = 0;
    StringStart = p = (PWCHAR)KEY_VALUE_DATA(KeyValueInformation);
    while(p != BufferEnd) {
        if(!*p) {
            StringLength = (ULONG)((PUCHAR)p - (PUCHAR)StringStart) + sizeof(UNICODE_NULL);
            (*UnicodeStringList)[i].Buffer = ExAllocatePool(PagedPool, StringLength);

            if(!((*UnicodeStringList)[i].Buffer)) {
                PipFreeUnicodeStringList(*UnicodeStringList, i);
                return STATUS_INSUFFICIENT_RESOURCES;
            }

            RtlCopyMemory((*UnicodeStringList)[i].Buffer, StringStart, StringLength);

            (*UnicodeStringList)[i].Length =
                ((*UnicodeStringList)[i].MaximumLength = (USHORT)StringLength)
                - sizeof(UNICODE_NULL);

            i++;

            if(((p + 1) == BufferEnd) || !*(p + 1)) {
                break;
            } else {
                StringStart = p + 1;
            }
        }
        p++;
    }
    if(p == BufferEnd) {
        StringLength = (ULONG)((PUCHAR)p - (PUCHAR)StringStart);
        (*UnicodeStringList)[i].Buffer = ExAllocatePool(PagedPool,
                                                        StringLength + sizeof(UNICODE_NULL)
                                                       );
        if(!((*UnicodeStringList)[i].Buffer)) {
            PipFreeUnicodeStringList(*UnicodeStringList, i);
            return STATUS_INSUFFICIENT_RESOURCES;
        }
        if(StringLength) {
            RtlCopyMemory((*UnicodeStringList)[i].Buffer, StringStart, StringLength);
        }
        (*UnicodeStringList)[i].Buffer[CB_TO_CWC(StringLength)] = UNICODE_NULL;

        (*UnicodeStringList)[i].MaximumLength =
                ((*UnicodeStringList)[i].Length = (USHORT)StringLength)
                + sizeof(UNICODE_NULL);
    }

    *UnicodeStringCount = StringCount;

    return STATUS_SUCCESS;
}

NTSTATUS
PipApplyFunctionToServiceInstances(
    IN     HANDLE ServiceKeyHandle OPTIONAL,
    IN     PUNICODE_STRING ServiceKeyName OPTIONAL,
    IN     ACCESS_MASK DesiredAccess,
    IN     BOOLEAN IgnoreNonCriticalErrors,
    IN     PIOP_SUBKEY_CALLBACK_ROUTINE DevInstCallbackRoutine,
    IN OUT PVOID Context,
    OUT    PULONG ServiceInstanceOrdinal OPTIONAL
    )

/*++

Routine Description:

    This routine enumerates all device instances referenced by the instance
    ordinal entries under a service's volatile Enum key, and calls
    the specified callback routine for each instance's corresponding subkey
    under HKLM\System\Enum.

Arguments:

    ServiceKeyHandle - Optional handle to the service entry. If this parameter
        is not specified, then the service key name must be given in
        ServiceKeyName (if both parameters are specified, then ServiceKeyHandle
        is used, and ServiceKeyName is ignored).

    ServiceKeyName - Optional name of the service entry key (under
        HKLM\CurrentControlSet\Services). If this parameter is not specified,
        then ServiceKeyHandle must contain a handle to the desired service key.

    DesiredAccess - Specifies the desired access that the callback routine
        needs to the enumerated device instance keys.  If no desired access is
        specified (i.e., DesiredAccess is zero), then no handle will be opened
        for the device instance keys, and the callback will be passed a NULL for
        its DeviceInstanceHandle parameter.

    IgnoreNonCriticalErrors - Specifies whether this function should
        immediately terminate on all errors, or only on critical ones.
        An example of a non-critical error is when an enumerated device instance
        key cannot be opened for the desired access.

    DevInstCallbackRoutine - Supplies a pointer to a function that will
        be called for each device instance key referenced by a service instance
        entry under the service's volatile Enum subkey. The prototype of the
        function is as follows:

            typedef BOOLEAN (*PIOP_SUBKEY_CALLBACK_ROUTINE) (
                IN     HANDLE DeviceInstanceHandle,
                IN     PUNICODE_STRING DeviceInstancePath,
                IN OUT PVOID Context
                );

        where DeviceInstanceHandle is the handle to an enumerated device instance
        key, DeviceInstancePath is the registry path (relative to
        HKLM\System\Enum) to this device instance, and Context is a pointer to
        user-defined data.

        This function should return TRUE to continue enumeration, or
        FALSE to terminate it.

    Context - Supplies a pointer to user-defined data that will be passed
        in to the callback routine at each device instance key invocation.

    ServiceInstanceOrdinal - Optionally, receives the service instance ordinal (1 based)
        that terminated the enumeration, or the total number of instances enumerated
        if the enumeration completed without being aborted.

Return Value:

    NT status code indicating whether the device instance keys were successfully
    enumerated.  Note that this does not provide information on the success or
    failure of the callback routine--if desired, this information should be
    stored in the Context structure.

--*/

{
    NTSTATUS Status;
    HANDLE ServiceEnumHandle, SystemEnumHandle, DeviceInstanceHandle;
    UNICODE_STRING TempUnicodeString;
    ULONG ServiceInstanceCount, i, junk;
    PKEY_VALUE_FULL_INFORMATION KeyValueInformation;
    BOOLEAN ContinueEnumeration;

    //
    // First, open up the volatile Enum subkey under the specified service entry.
    //

    if(ARGUMENT_PRESENT(ServiceKeyHandle)) {
        PiWstrToUnicodeString(&TempUnicodeString, REGSTR_KEY_ENUM);
        Status = IopOpenRegistryKeyEx( &ServiceEnumHandle,
                                       ServiceKeyHandle,
                                       &TempUnicodeString,
                                       KEY_READ
                                       );
    } else {
        Status = PipOpenServiceEnumKeys(ServiceKeyName,
                                        KEY_READ,
                                        NULL,
                                        &ServiceEnumHandle,
                                        FALSE
                                       );
    }
    if(!NT_SUCCESS(Status)) {
        return Status;
    }

    //
    // Find out how many instances are referenced in the service's Enum key.
    //

    ServiceInstanceCount = 0;   // assume none.

    Status = IopGetRegistryValue(ServiceEnumHandle,
                                 REGSTR_VALUE_COUNT,
                                 &KeyValueInformation
                                );
    if (NT_SUCCESS(Status)) {

        if((KeyValueInformation->Type == REG_DWORD) &&
           (KeyValueInformation->DataLength >= sizeof(ULONG))) {

            ServiceInstanceCount = *(PULONG)KEY_VALUE_DATA(KeyValueInformation);

        }
        ExFreePool(KeyValueInformation);

    } else if (Status != STATUS_OBJECT_NAME_NOT_FOUND) {
        goto PrepareForReturn;
    } else {
        //
        // If 'Count' value entry not found, consider this to mean there are simply
        // no device instance controlled by this service.
        //
        Status = STATUS_SUCCESS;
    }

    //
    // Now, enumerate each service instance, and call the specified callback function
    // for the corresponding device instance.
    //

    if (ServiceInstanceCount) {

        if (DesiredAccess) {
            Status = IopOpenRegistryKeyEx( &SystemEnumHandle,
                                           NULL,
                                           &CmRegistryMachineSystemCurrentControlSetEnumName,
                                           KEY_READ
                                           );
            if(!NT_SUCCESS(Status)) {
                goto PrepareForReturn;
            }
        } else {
            //
            // Set DeviceInstanceHandle to NULL, since we won't be opening up the
            // device instance keys.
            //
            DeviceInstanceHandle = NULL;
        }
        KeyValueInformation = (PKEY_VALUE_FULL_INFORMATION)ExAllocatePool(
                                                              PagedPool,
                                                              PNP_SCRATCH_BUFFER_SIZE);
        if (!KeyValueInformation) {
            Status = STATUS_INSUFFICIENT_RESOURCES;
            goto PrepareForReturn;
        }

        for (i = 0; ; i++) {

            Status = ZwEnumerateValueKey(
                            ServiceEnumHandle,
                            i,
                            KeyValueFullInformation,
                            KeyValueInformation,
                            PNP_SCRATCH_BUFFER_SIZE,
                            &junk
                            );

            if (!NT_SUCCESS (Status)) {
                if (Status == STATUS_NO_MORE_ENTRIES) {
                    Status = STATUS_SUCCESS;
                    break;
                } else if (IgnoreNonCriticalErrors) {
                    continue;
                } else {
                    break;
                }
            }

            if (KeyValueInformation->Type != REG_SZ) {
                continue;
            }

            ContinueEnumeration = TRUE;
            TempUnicodeString.Length = 0;
            IopRegistryDataToUnicodeString(&TempUnicodeString,
                                           (PWSTR)KEY_VALUE_DATA(KeyValueInformation),
                                           KeyValueInformation->DataLength
                                           );
            if (TempUnicodeString.Length) {

                //
                // We have retrieved a (non-empty) string for this service instance.
                // If the user specified a non-zero value for the DesiredAccess
                // parameter, we will attempt to open up the corresponding device
                // instance key under HKLM\System\Enum.
                //
                if (DesiredAccess) {
                    Status = IopOpenRegistryKeyEx( &DeviceInstanceHandle,
                                                   SystemEnumHandle,
                                                   &TempUnicodeString,
                                                   DesiredAccess
                                                   );
                }

                if (NT_SUCCESS(Status)) {
                    //
                    // Invoke the specified callback routine for this device instance.
                    //
                    ContinueEnumeration = DevInstCallbackRoutine(DeviceInstanceHandle,
                                                                 &TempUnicodeString,
                                                                 Context
                                                                );
                    if (DesiredAccess) {
                        ZwClose(DeviceInstanceHandle);
                    }
                } else if (IgnoreNonCriticalErrors) {
                    continue;
                } else {
                    break;
                }
            } else {
                continue;
            }
            if (!ContinueEnumeration) {
                break;
            }
        }

        if (ARGUMENT_PRESENT(ServiceInstanceOrdinal)) {
            *ServiceInstanceOrdinal = i;
        }

        if (DesiredAccess) {
            ZwClose(SystemEnumHandle);
        }
        ExFreePool(KeyValueInformation);
    }


PrepareForReturn:

    ZwClose(ServiceEnumHandle);

    return Status;
}

BOOLEAN
PipIsDuplicatedDevices(
    IN PCM_RESOURCE_LIST Configuration1,
    IN PCM_RESOURCE_LIST Configuration2,
    IN PHAL_BUS_INFORMATION BusInfo1 OPTIONAL,
    IN PHAL_BUS_INFORMATION BusInfo2 OPTIONAL
    )

/*++

Routine Description:

    This routine compares two set of configurations and bus information to
    determine if the resources indicate the same device.  If BusInfo1 and
    BusInfo2 both are absent, it means caller wants to compare the raw
    resources.

Arguments:

    Configuration1 - Supplies a pointer to the first set of resource.

    Configuration2 - Supplies a pointer to the second set of resource.

    BusInfo1 - Supplies a pointer to the first set of bus information.

    BusInfo2 - Supplies a pointer to the second set of bus information.

Return Value:

    returns TRUE if the two set of resources indicate the same device;
    otherwise a value of FALSE is returned.

--*/

{
    PCM_PARTIAL_RESOURCE_LIST list1, list2;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR descriptor1, descriptor2;

    ULONG i, j;
    ULONG pass = 0;

    //
    // The BusInfo for both resources must be both present or not present.
    //

    if ((ARGUMENT_PRESENT(BusInfo1) && !ARGUMENT_PRESENT(BusInfo2)) ||
        (!ARGUMENT_PRESENT(BusInfo1) && ARGUMENT_PRESENT(BusInfo2))) {

        //
        // Unable to determine.
        //

        return FALSE;
    }

    //
    // Next check resources used by the two devices.
    // Currently, we *only* check the Io ports.
    //

    if (Configuration1->Count == 0 || Configuration2->Count == 0) {

        //
        // If any one of the configuration data is empty, we assume
        // the devices are not duplicates.
        //

        return FALSE;
    }

RedoScan:

    list1 = &(Configuration1->List[0].PartialResourceList);
    list2 = &(Configuration2->List[0].PartialResourceList);

    for(i = 0, descriptor1 = list1->PartialDescriptors;
        i < list1->Count;
        i++, descriptor1++) {

        //
        // If this is an i/o port or a memory range then look for a match
        // in the other list.
        //

        if((descriptor1->Type == CmResourceTypePort) ||
           (descriptor1->Type == CmResourceTypeMemory)) {

            for(j = 0, descriptor2 = list2->PartialDescriptors;
                j < list2->Count;
                j++, descriptor2++) {

                //
                // If the types match then check to see if both addresses
                // match as well.  If bus info was provided then go ahead
                // and translate the ranges first.
                //

                if(descriptor1->Type == descriptor2->Type) {

                    PHYSICAL_ADDRESS range1, range1Translated;
                    PHYSICAL_ADDRESS range2, range2Translated;
                    ULONG range1IoSpace, range2IoSpace;

                    range1 = descriptor1->u.Generic.Start;
                    range2 = descriptor2->u.Generic.Start;

                    if((range1.QuadPart == 0) ||
                       (BusInfo1 == NULL) ||
                       (HalTranslateBusAddress(
                            BusInfo1->BusType,
                            BusInfo1->BusNumber,
                            range1,
                            &range1IoSpace,
                            &range1Translated) == FALSE)) {

                        range1Translated = range1;
                        range1IoSpace =
                            (descriptor1->Type == CmResourceTypePort) ? TRUE :
                                                                        FALSE;
                    }

                    if((range2.QuadPart == 0) ||
                       (BusInfo2 == NULL) ||
                       (HalTranslateBusAddress(
                            BusInfo2->BusType,
                            BusInfo2->BusNumber,
                            range2,
                            &range2IoSpace,
                            &range2Translated) == FALSE)) {

                        range2Translated = range2;
                        range2IoSpace =
                            (descriptor2->Type == CmResourceTypePort) ? TRUE :
                                                                        FALSE;
                    }

                    //
                    // If the ranges are in the same space and start at the
                    // same location then break out and go on to the next
                    // range
                    //

                    if((range1Translated.QuadPart == range2Translated.QuadPart) &&
                       (range1IoSpace == range2IoSpace)) {

                        break;
                    }
                }
            }

            //
            // If we made it all the way through the resource list without
            // finding a match then these are not duplicates.
            //

            if(j == list2->Count) {
                return FALSE;
            }
        }
    }

    //
    // If every resource in list 1 exists in list 2 then we also need to make
    // sure that every resource in list 2 exists in list 1.
    //

    if(pass == 0) {

        PVOID tmp ;

        tmp = Configuration2;
        Configuration2 = Configuration1;
        Configuration1 = tmp;

        tmp = BusInfo2;
        BusInfo2 = BusInfo1;
        BusInfo1 = tmp;

        pass = 1;

        goto RedoScan;
    }

    return TRUE;
}

VOID
PipFreeUnicodeStringList(
    IN PUNICODE_STRING UnicodeStringList,
    IN ULONG StringCount
    )

/*++

Routine Description:

    This routine frees the buffer for each UNICODE_STRING in the specified list
    (there are StringCount of them), and then frees the memory used for the
    string list itself.

Arguments:

    UnicodeStringList - Supplies a pointer to an array of UNICODE_STRINGs.

    StringCount - Supplies the number of strings in the UnicodeStringList array.

Returns:

    None.

    NOTE: This function is only available during INIT time!

--*/

{
    ULONG i;

    if(UnicodeStringList) {
        for(i = 0; i < StringCount; i++) {
            if(UnicodeStringList[i].Buffer) {
                ExFreePool(UnicodeStringList[i].Buffer);
            }
        }
        ExFreePool(UnicodeStringList);
    }
}

NTSTATUS
IopDriverLoadingFailed(
    IN HANDLE ServiceHandle OPTIONAL,
    IN PUNICODE_STRING ServiceName OPTIONAL
    )

/*++

Routine Description:

    This routine is invoked when driver failed to start.  All the device
    instances controlled by this driver/service are marked as failing to
    start.

Arguments:

    ServiceKeyHandle - Optionally, supplies a handle to the driver service node in the
        registry that controls this device instance.  If this argument is not specified,
        then ServiceKeyName is used to specify the service entry.

    ServiceKeyName - Optionally supplies the name of the service entry that controls
        the device instance. This must be specified if ServiceKeyHandle isn't given.

Returns:

    None.

--*/

{
    NTSTATUS status;
    PKEY_VALUE_FULL_INFORMATION keyValueInformation;
    BOOLEAN closeHandle = FALSE, deletePdo;
    HANDLE handle, serviceEnumHandle, controlHandle, devInstHandle;
    HANDLE sysEnumHandle = NULL;
    ULONG deviceFlags, count, newCount, i, j;
    UNICODE_STRING unicodeValueName, deviceInstanceName;
    WCHAR unicodeBuffer[20];

    //
    // Open registry ServiceKeyName\Enum branch
    //

    if (!ARGUMENT_PRESENT(ServiceHandle)) {
        status = PipOpenServiceEnumKeys(ServiceName,
                                        KEY_READ,
                                        &ServiceHandle,
                                        &serviceEnumHandle,
                                        FALSE
                                        );
        closeHandle = TRUE;
    } else {
        PiWstrToUnicodeString(&unicodeValueName, REGSTR_KEY_ENUM);
        status = IopOpenRegistryKeyEx( &serviceEnumHandle,
                                       ServiceHandle,
                                       &unicodeValueName,
                                       KEY_READ
                                       );
    }
    if (!NT_SUCCESS( status )) {

        //
        // No Service Enum key? no device instance.  Return FALSE.
        //

        return status;
    }

    //
    // Set "STARTFAILED" flags.  So, we won't load it again.
    //

    PiWstrToUnicodeString(&unicodeValueName, L"INITSTARTFAILED");
    deviceFlags = 1;
    ZwSetValueKey(
                serviceEnumHandle,
                &unicodeValueName,
                TITLE_INDEX_VALUE,
                REG_DWORD,
                &deviceFlags,
                sizeof(deviceFlags)
                );

    //
    // Find out how many device instances listed in the ServiceName's
    // Enum key.
    //

    status = IopGetRegistryValue ( serviceEnumHandle,
                                   REGSTR_VALUE_COUNT,
                                   &keyValueInformation
                                   );
    count = 0;
    if (NT_SUCCESS(status)) {
        if ((keyValueInformation->Type == REG_DWORD) &&
            (keyValueInformation->DataLength >= sizeof(ULONG))) {

            count = *(PULONG)KEY_VALUE_DATA(keyValueInformation);
        }
        ExFreePool(keyValueInformation);
    }
    if (count == 0) {
        ZwClose(serviceEnumHandle);
        if (closeHandle) {
            ZwClose(ServiceHandle);
        }
        return status;
    }

    //
    // Open HTREE\ROOT\0 key so later we can remove device instance key
    // from its AttachedComponents value name.
    //

    status = IopOpenRegistryKeyEx( &sysEnumHandle,
                                   NULL,
                                   &CmRegistryMachineSystemCurrentControlSetEnumName,
                                   KEY_ALL_ACCESS
                                   );

    //
    // Walk through each registered device instance to mark its Problem and
    // StatusFlags as fail to start and reset its ActiveService
    //

    newCount = count;
    for (i = 0; i < count; i++) {
        deletePdo = FALSE;
        status = PipServiceInstanceToDeviceInstance (
                     ServiceHandle,
                     ServiceName,
                     i,
                     &deviceInstanceName,
                     &handle,
                     KEY_ALL_ACCESS
                     );

        if (NT_SUCCESS(status)) {

            PDEVICE_OBJECT deviceObject;
            PDEVICE_NODE deviceNode;

            //
            // If the device instance is a detected device reported during driver's
            // DriverEntry we need to clean it up.
            //

            deviceObject = IopDeviceObjectFromDeviceInstance(&deviceInstanceName);
            if (deviceObject) {
                deviceNode = (PDEVICE_NODE)deviceObject->DeviceObjectExtension->DeviceNode;
                if (deviceNode) {

                    IopReleaseDeviceResources(deviceNode, TRUE);

                    if ((deviceNode->Flags & DNF_MADEUP) &&
                        ((deviceNode->State == DeviceNodeStarted) ||
                        (deviceNode->State == DeviceNodeStartPostWork))) {

                        //
                        // Now mark this one deleted.
                        //
                        PipSetDevNodeState(deviceNode, DeviceNodeRemoved, NULL);

                        PipSetDevNodeProblem(deviceNode, CM_PROB_DEVICE_NOT_THERE);

                        deletePdo = TRUE;
                    }
                }
                ObDereferenceObject(deviceObject);  // added via IopDeviceObjectFromDeviceInstance
            }

            PiLockPnpRegistry(FALSE);

            PiWstrToUnicodeString(&unicodeValueName, REGSTR_KEY_CONTROL);
            controlHandle = NULL;
            status = IopOpenRegistryKeyEx( &controlHandle,
                                           handle,
                                           &unicodeValueName,
                                           KEY_ALL_ACCESS
                                           );
            if (NT_SUCCESS(status)) {

                status = IopGetRegistryValue(controlHandle,
                                             REGSTR_VALUE_NEWLY_CREATED,
                                             &keyValueInformation);
                if (NT_SUCCESS(status)) {
                    ExFreePool(keyValueInformation);
                }
                if ((status != STATUS_OBJECT_NAME_NOT_FOUND) &&
                    (status != STATUS_OBJECT_PATH_NOT_FOUND)) {

                    //
                    // Remove the instance value name from service enum key
                    //

                    PiUlongToUnicodeString(&unicodeValueName, unicodeBuffer, 20, i);
                    status = ZwDeleteValueKey (serviceEnumHandle, &unicodeValueName);
                    if (NT_SUCCESS(status)) {

                        //
                        // If we can successfaully remove the instance value entry
                        // from service enum key, we then remove the device instance key
                        // Otherwise, we go thru normal path to mark driver loading failed
                        // in the device instance key.
                        //

                        newCount--;

                        ZwDeleteKey(controlHandle);
                        ZwDeleteKey(handle);


                        //
                        // We also want to delete the ROOT\LEGACY_<driver> key
                        //

                        if (sysEnumHandle) {
                            deviceInstanceName.Length -= 5 * sizeof(WCHAR);
                            deviceInstanceName.Buffer[deviceInstanceName.Length / sizeof(WCHAR)] =
                                                 UNICODE_NULL;
                            status = IopOpenRegistryKeyEx( &devInstHandle,
                                                           sysEnumHandle,
                                                           &deviceInstanceName,
                                                           KEY_ALL_ACCESS
                                                           );
                            if (NT_SUCCESS(status)) {
                                ZwDeleteKey(devInstHandle);
                                ZwClose(devInstHandle);
                            }
                        }

                        //
                        // If there is a PDO for this device, remove it
                        //

                        if (deletePdo) {
                            IoDeleteDevice(deviceObject);
                        }

                        ZwClose(controlHandle);
                        ZwClose(handle);
                        IopCleanupDeviceRegistryValues(&deviceInstanceName);

                        ExFreePool(deviceInstanceName.Buffer);
                        PiUnlockPnpRegistry();
                        continue;
                    }
                }
            }

            //
            // Reset Control\ActiveService value name.
            //

            if (controlHandle) {
                PiWstrToUnicodeString(&unicodeValueName, REGSTR_VAL_ACTIVESERVICE);
                ZwDeleteValueKey(controlHandle, &unicodeValueName);
                ZwClose(controlHandle);
            }

            ZwClose(handle);
            ExFreePool(deviceInstanceName.Buffer);

            PiUnlockPnpRegistry();
        }
    }

    //
    // If some instance value entry is deleted, we need to update the count of instance
    // value entries and rearrange the instance value entries under service enum key.
    //

    if (newCount != count) {

        PiLockPnpRegistry(FALSE);

        if (newCount != 0) {
            j = 0;
            i = 0;
            while (i < count) {
                PiUlongToUnicodeString(&unicodeValueName, unicodeBuffer, 20, i);
                status = IopGetRegistryValue(serviceEnumHandle,
                                             unicodeValueName.Buffer,
                                             &keyValueInformation
                                             );
                if (NT_SUCCESS(status)) {
                    if (i != j) {

                        //
                        // Need to change the instance i to instance j
                        //

                        ZwDeleteValueKey(serviceEnumHandle, &unicodeValueName);

                        PiUlongToUnicodeString(&unicodeValueName, unicodeBuffer, 20, j);
                        ZwSetValueKey (serviceEnumHandle,
                                       &unicodeValueName,
                                       TITLE_INDEX_VALUE,
                                       REG_SZ,
                                       (PVOID)KEY_VALUE_DATA(keyValueInformation),
                                       keyValueInformation->DataLength
                                       );
                    }
                    ExFreePool(keyValueInformation);
                    j++;
                }
                i++;
            }
        }

        //
        // Don't forget to update the "Count=" and "NextInstance=" value entries
        //

        PiWstrToUnicodeString( &unicodeValueName, REGSTR_VALUE_COUNT);

        ZwSetValueKey(serviceEnumHandle,
                      &unicodeValueName,
                      TITLE_INDEX_VALUE,
                      REG_DWORD,
                      &newCount,
                      sizeof (newCount)
                      );
        PiWstrToUnicodeString( &unicodeValueName, REGSTR_VALUE_NEXT_INSTANCE);

        ZwSetValueKey(serviceEnumHandle,
                      &unicodeValueName,
                      TITLE_INDEX_VALUE,
                      REG_DWORD,
                      &newCount,
                      sizeof (newCount)
                      );

        PiUnlockPnpRegistry();
    }
    ZwClose(serviceEnumHandle);
    if (closeHandle) {
        ZwClose(ServiceHandle);
    }
    if (sysEnumHandle) {
        ZwClose(sysEnumHandle);
    }

    return STATUS_SUCCESS;
}

VOID
IopDisableDevice(
    IN PDEVICE_NODE DeviceNode
    )

/*++

Routine Description:

    This routine tries to ask a bus driver stopping decoding resources

Arguments:

    DeviceNode - Specifies the device to be disabled.

    Handle - specifies the device instance handle.

Returns:

    None.

--*/

{
    NTSTATUS status;

    //
    // If the device has boot config, we will query-remove and remove the device to free
    // the boot config if possible.
    //

    status = IopRemoveDevice (DeviceNode->PhysicalDeviceObject, IRP_MN_QUERY_REMOVE_DEVICE);

    if (NT_SUCCESS(status)) {

        status = IopRemoveDevice (DeviceNode->PhysicalDeviceObject, IRP_MN_REMOVE_DEVICE);
        ASSERT(NT_SUCCESS(status));
        IopReleaseDeviceResources(DeviceNode, TRUE);

    } else {

        IopRemoveDevice (DeviceNode->PhysicalDeviceObject, IRP_MN_CANCEL_REMOVE_DEVICE);
    }

    if (PipDoesDevNodeHaveProblem(DeviceNode)) {
        ASSERT(PipIsDevNodeProblem(DeviceNode, CM_PROB_NOT_CONFIGURED) ||
               PipIsDevNodeProblem(DeviceNode, CM_PROB_FAILED_INSTALL) ||
               PipIsDevNodeProblem(DeviceNode, CM_PROB_REINSTALL));

        PipClearDevNodeProblem(DeviceNode);
    }

    PipSetDevNodeProblem(DeviceNode, CM_PROB_DISABLED);
}

BOOLEAN
IopIsAnyDeviceInstanceEnabled(
    IN PUNICODE_STRING ServiceKeyName,
    IN HANDLE ServiceHandle OPTIONAL,
    IN BOOLEAN LegacyIncluded
    )

/*++

Routine Description:

    This routine checks if any of the devices instances is turned on for the specified
    service. This routine is used for Pnp Driver only and is temporary function to support
    SUR.

Arguments:

    ServiceKeyName - Specifies the service key unicode name

    ServiceHandle - Optionally supplies a handle to the service key to be checked.

    LegacyIncluded - TRUE, a legacy device instance key is counted as a device instance.
                     FALSE, a legacy device instance key is not counted.

Returns:

    A BOOLEAN value.

--*/

{
    NTSTATUS status;
    PKEY_VALUE_FULL_INFORMATION keyValueInformation;
    HANDLE serviceEnumHandle, handle, controlHandle;
    ULONG i, count, deviceFlags;
    UNICODE_STRING unicodeName, instancePath;
    BOOLEAN enabled, closeHandle = FALSE;
    PDEVICE_OBJECT physicalDeviceObject;
    PDEVICE_NODE deviceNode;

    //
    // Open registry ServiceKeyName\Enum branch
    //

    if (!ARGUMENT_PRESENT(ServiceHandle)) {
        status = PipOpenServiceEnumKeys(ServiceKeyName,
                                        KEY_READ,
                                        &ServiceHandle,
                                        &serviceEnumHandle,
                                        FALSE
                                        );
        closeHandle = TRUE;
    } else {
        PiWstrToUnicodeString(&unicodeName, REGSTR_KEY_ENUM);
        status = IopOpenRegistryKeyEx( &serviceEnumHandle,
                                       ServiceHandle,
                                       &unicodeName,
                                       KEY_READ
                                       );
    }
    if (!NT_SUCCESS( status )) {

        //
        // No Service Enum key? no device instance.  Return FALSE.
        //

        return FALSE;
    }

    //
    // Find out how many device instances listed in the ServiceName's
    // Enum key.
    //

    status = IopGetRegistryValue ( serviceEnumHandle,
                                   REGSTR_VALUE_COUNT,
                                   &keyValueInformation
                                   );
    ZwClose(serviceEnumHandle);
    count = 0;
    if (NT_SUCCESS(status)) {
        if ((keyValueInformation->Type == REG_DWORD) &&
            (keyValueInformation->DataLength >= sizeof(ULONG))) {

            count = *(PULONG)KEY_VALUE_DATA(keyValueInformation);
        }
        ExFreePool(keyValueInformation);
    }
    if (count == 0) {
        if (closeHandle) {
            ZwClose(ServiceHandle);
        }
        return FALSE;
    }

    //
    // Walk through each registered device instance to check it is enabled.
    //

    enabled = FALSE;
    for (i = 0; i < count; i++) {

        //
        // Get device instance handle.  If it fails, we will skip this device
        // instance.
        //

        status = PipServiceInstanceToDeviceInstance (
                     ServiceHandle,
                     NULL,
                     i,
                     &instancePath,
                     &handle,
                     KEY_ALL_ACCESS
                     );
        if (!NT_SUCCESS(status)) {
            continue;
        }

        physicalDeviceObject = IopDeviceObjectFromDeviceInstance(&instancePath);
        ExFreePool(instancePath.Buffer);
        if (physicalDeviceObject) {
            deviceNode = (PDEVICE_NODE)physicalDeviceObject->DeviceObjectExtension->DeviceNode;
            if (deviceNode && (PipIsDevNodeProblem(deviceNode, CM_PROB_DISABLED) || PipIsDevNodeProblem(deviceNode, CM_PROB_HARDWARE_DISABLED))) {
                ZwClose(handle);
                ObDereferenceObject(physicalDeviceObject);
                continue;
            }
        } else {
            deviceNode = NULL;
        }

        //
        // Check if the device instance has been disabled.
        // First check global flag: CONFIGFLAG and then CSCONFIGFLAG.
        //

        deviceFlags = 0;
        status = IopGetRegistryValue(handle,
                                     REGSTR_VALUE_CONFIG_FLAGS,
                                     &keyValueInformation);
        if (NT_SUCCESS(status)) {
            if ((keyValueInformation->Type == REG_DWORD) &&
                (keyValueInformation->DataLength >= sizeof(ULONG))) {

                deviceFlags = *(PULONG)KEY_VALUE_DATA(keyValueInformation);
            }
            ExFreePool(keyValueInformation);
        }

        if (deviceFlags & CONFIGFLAG_DISABLED) {

            //
            // Convert this flag into the hardware profile-specific version, so it'll
            // look the same as the CsConfigFlags we retrieve below.
            //

            deviceFlags = CSCONFIGFLAG_DISABLED;

        } else {

            status = PipGetServiceInstanceCsConfigFlags( ServiceKeyName,
                                                         i,
                                                         &deviceFlags
                                                         );

            if (!NT_SUCCESS(status)) {
                deviceFlags = 0;
            }
        }

        //
        // If the device is disabled (either globally, or specifically for this
        // hardware profile), then mark the devnode as DNF_DISABLED.
        //

        if ((deviceFlags & CSCONFIGFLAG_DISABLED) || (deviceFlags & CSCONFIGFLAG_DO_NOT_START)) {

            if (deviceNode) {
                IopDisableDevice(deviceNode);
            }
        }

        if (physicalDeviceObject) {
            ObDereferenceObject(physicalDeviceObject);
        }

        //
        // Finally, we need to set the STATUSFLAGS of the device instance to
        // indicate if the driver is successfully started.
        //

        if (!(deviceFlags & (CSCONFIGFLAG_DISABLED | CSCONFIGFLAG_DO_NOT_CREATE | CSCONFIGFLAG_DO_NOT_START))) {

            ULONG legacy;

            //
            // Check should legacy instance key be counted as an enabled device
            //

            if (LegacyIncluded == FALSE) {

                //
                // The legacy variable must be initialized to zero.  Because the device
                // instance key may be an enumerated device.  In this case, there is no
                // legacy value name.
                //

                legacy = 0;
                status = IopGetRegistryValue(handle,
                                             REGSTR_VALUE_LEGACY,
                                             &keyValueInformation
                                             );
                if (NT_SUCCESS(status)) {
                    if ((keyValueInformation->Type == REG_DWORD) &&
                        (keyValueInformation->DataLength >= sizeof(ULONG))) {
                        legacy = *(PULONG)KEY_VALUE_DATA(keyValueInformation);
                    }
                    ExFreePool(keyValueInformation);
                }
            } else {
                legacy = 0;
            }

            if (legacy == 0) {

                //
                // Mark that the driver has at least a device instance to work with.
                //

                PiWstrToUnicodeString(&unicodeName, REGSTR_KEY_CONTROL);
                status = IopCreateRegistryKeyEx( &controlHandle,
                                                 handle,
                                                 &unicodeName,
                                                 KEY_ALL_ACCESS,
                                                 REG_OPTION_VOLATILE,
                                                 NULL
                                                 );
                if (NT_SUCCESS(status)) {
                    PiWstrToUnicodeString(&unicodeName, REGSTR_VAL_ACTIVESERVICE);
                    ZwSetValueKey(
                                controlHandle,
                                &unicodeName,
                                TITLE_INDEX_VALUE,
                                REG_SZ,
                                ServiceKeyName->Buffer,
                                ServiceKeyName->Length + sizeof(UNICODE_NULL)
                                );

                    ZwClose(controlHandle);
                }
                enabled = TRUE;
            }
        }
        ZwClose(handle);
    }

    if (closeHandle) {
        ZwClose(ServiceHandle);
    }
    return enabled;
}

BOOLEAN
IopIsDeviceInstanceEnabled(
    IN HANDLE DeviceInstanceHandle      OPTIONAL,
    IN PUNICODE_STRING DeviceInstance,
    IN BOOLEAN DisableIfEnabled
    )

/*++

Routine Description:

    This routine checks if the specified devices instances is enabled.

Arguments:

    DeviceInstanceHandle - Optionally supplies a handle to the device instance
        key to be checked.

    DeviceInstance - Specifies the device instance key unicode name.  Caller
        must at least specified DeviceInstanceHandle or DeviceInstance.

    DisableIfEnabled - If this flag is set, and the device should be disabled
        but is currently disabled, then the device is disabled.

Returns:

    A BOOLEAN value.

--*/

{
    NTSTATUS status;
    PKEY_VALUE_FULL_INFORMATION keyValueInformation;
    HANDLE handle, handle1;
    ULONG deviceFlags;
    BOOLEAN enabled, closeHandle = FALSE;
    UNICODE_STRING unicodeString;
    PDEVICE_OBJECT deviceObject = NULL;
    PDEVICE_NODE deviceNode = NULL;

    //
    // Open registry ServiceKeyName\Enum branch
    //

    if (!ARGUMENT_PRESENT(DeviceInstanceHandle)) {
        status = IopOpenRegistryKeyEx( &handle,
                                       NULL,
                                       &CmRegistryMachineSystemCurrentControlSetEnumName,
                                       KEY_READ
                                       );

        if (NT_SUCCESS( status )) {

            status = IopOpenRegistryKeyEx( &DeviceInstanceHandle,
                                           handle,
                                           DeviceInstance,
                                           KEY_READ
                                           );
            ZwClose(handle);
        }

        if (!NT_SUCCESS( status )) {
            return FALSE;
        }
        closeHandle = TRUE;
    }

    enabled = TRUE;

    //
    // First check the device node
    //

    deviceObject = IopDeviceObjectFromDeviceInstance(DeviceInstance);
    if (deviceObject) {
        deviceNode = (PDEVICE_NODE)deviceObject->DeviceObjectExtension->DeviceNode;
        if (deviceNode && (PipIsDevNodeProblem(deviceNode, CM_PROB_DISABLED) || PipIsDevNodeProblem(deviceNode, CM_PROB_HARDWARE_DISABLED))) {
            enabled = FALSE;
            goto exit;
        }
    }

    //
    // Check if the device instance has been disabled.
    // First check global flag: CONFIGFLAG and then CSCONFIGFLAG.
    //

    deviceFlags = 0;
    status = IopGetRegistryValue(DeviceInstanceHandle,
                                 REGSTR_VALUE_CONFIG_FLAGS,
                                 &keyValueInformation);
    if (NT_SUCCESS(status)) {
        if ((keyValueInformation->Type == REG_DWORD) &&
            (keyValueInformation->DataLength >= sizeof(ULONG))) {
            deviceFlags = *(PULONG)KEY_VALUE_DATA(keyValueInformation);
        }
        ExFreePool(keyValueInformation);
    }
    if (!(deviceFlags & CONFIGFLAG_DISABLED)) {
        enabled = TRUE;

        //
        // See if we can open current hardware profile
        //
        status = IopOpenRegistryKeyEx( &handle1,
                                       NULL,
                                       &CmRegistryMachineSystemCurrentControlSetHardwareProfilesCurrent,
                                       KEY_READ
                                       );

        if (NT_SUCCESS(status) && DeviceInstance != NULL) {

            //
            // Now, we must open the System\CCS\Enum key under this.
            //
            //
            // Open system\CurrentControlSet under current hardware profile key
            //

            PiWstrToUnicodeString(&unicodeString, REGSTR_PATH_CURRENTCONTROLSET);
            status = IopOpenRegistryKeyEx( &handle,
                                           handle1,
                                           &unicodeString,
                                           KEY_READ
                                           );
            ZwClose(handle1);
            if (NT_SUCCESS(status)) {
                PiWstrToUnicodeString(&unicodeString, REGSTR_KEY_ENUM);
                status = IopOpenRegistryKeyEx( &handle1,
                                               handle,
                                               &unicodeString,
                                               KEY_READ
                                               );
                ZwClose(handle);
                if (NT_SUCCESS(status)) {
                    status = IopOpenRegistryKeyEx( &handle,
                                                   handle1,
                                                   DeviceInstance,
                                                   KEY_READ
                                                   );
                    ZwClose(handle1);
                    if (NT_SUCCESS(status)) {
                        status = IopGetRegistryValue(
                                        handle,
                                        REGSTR_VALUE_CSCONFIG_FLAGS,
                                        &keyValueInformation
                                        );
                        if (NT_SUCCESS(status)) {
                            if((keyValueInformation->Type == REG_DWORD) &&
                               (keyValueInformation->DataLength >= sizeof(ULONG))) {
                               deviceFlags = *(PULONG)KEY_VALUE_DATA(keyValueInformation);
                            }
                            ExFreePool(keyValueInformation);
                        }
                        ZwClose(handle);
                        if (NT_SUCCESS(status)) {
                            if ((deviceFlags & CSCONFIGFLAG_DISABLED) ||
                                (deviceFlags & CSCONFIGFLAG_DO_NOT_CREATE) ||
                                (deviceFlags & CSCONFIGFLAG_DO_NOT_START)) {
                                enabled = FALSE;
                            }
                        }
                    }
                }
            }
        }
    } else {
        enabled = FALSE;
    }

    //
    // If the device is disabled and has device node associated with it.
    // disable the device.
    //

    if (enabled == FALSE && deviceNode && DisableIfEnabled) {
        IopDisableDevice(deviceNode);
    }
exit:
    if (deviceObject) {
        ObDereferenceObject(deviceObject);
    }
    if (closeHandle) {
        ZwClose(DeviceInstanceHandle);
    }
    return enabled;
}

ULONG
IopDetermineResourceListSize(
    IN PCM_RESOURCE_LIST ResourceList
    )

/*++

Routine Description:

    This routine determines size of the passed in ResourceList
    structure.

Arguments:

    Configuration1 - Supplies a pointer to the resource list.

Return Value:

    size of the resource list structure.

--*/

{
    ULONG totalSize, listSize, descriptorSize, i, j;
    PCM_FULL_RESOURCE_DESCRIPTOR fullResourceDesc;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR partialDescriptor;

    if (!ResourceList) {
        totalSize = 0;
    } else {
        totalSize = FIELD_OFFSET(CM_RESOURCE_LIST, List);
        fullResourceDesc = &ResourceList->List[0];
        for (i = 0; i < ResourceList->Count; i++) {
            listSize = FIELD_OFFSET(CM_FULL_RESOURCE_DESCRIPTOR,
                                    PartialResourceList) +
                       FIELD_OFFSET(CM_PARTIAL_RESOURCE_LIST,
                                    PartialDescriptors);
            partialDescriptor = &fullResourceDesc->PartialResourceList.PartialDescriptors[0];
            for (j = 0; j < fullResourceDesc->PartialResourceList.Count; j++) {
                descriptorSize = sizeof(CM_PARTIAL_RESOURCE_DESCRIPTOR);
                if (partialDescriptor->Type == CmResourceTypeDeviceSpecific) {
                    descriptorSize += partialDescriptor->u.DeviceSpecificData.DataSize;
                }
                listSize += descriptorSize;
                partialDescriptor = (PCM_PARTIAL_RESOURCE_DESCRIPTOR)
                                        ((PUCHAR)partialDescriptor + descriptorSize);
            }
            totalSize += listSize;
            fullResourceDesc = (PCM_FULL_RESOURCE_DESCRIPTOR)
                                      ((PUCHAR)fullResourceDesc + listSize);
        }
    }
    return totalSize;
}

VOID
PpInitializeDeviceReferenceTable(
    VOID
    )

/*++

Routine Description:

    This routine initializes data structures associated with the device
    reference table.

Arguments:

    None.

Return Value:

    None.

--*/

{
    ExInitializeFastMutex(&PpDeviceReferenceTableLock);
    RtlInitializeGenericTable(  &PpDeviceReferenceTable,
                                PiCompareInstancePath,
                                PiAllocateGenericTableEntry,
                                PiFreeGenericTableEntry,
                                NULL);
}

RTL_GENERIC_COMPARE_RESULTS
NTAPI
PiCompareInstancePath(
    IN  PRTL_GENERIC_TABLE          Table,
    IN  PVOID                       FirstStruct,
    IN  PVOID                       SecondStruct
    )

/*++

Routine Description:

    This routine is the callback for the generic table routines.

Arguments:

    Table       - Table for which this is invoked.

    FirstStruct - An element in the table to compare.

    SecondStruct - Another element in the table to compare.

Return Value:

    RTL_GENERIC_COMPARE_RESULTS.

--*/

{
    PUNICODE_STRING lhs = ((PDEVICE_REFERENCE)FirstStruct)->DeviceInstance;
    PUNICODE_STRING rhs = ((PDEVICE_REFERENCE)SecondStruct)->DeviceInstance;
    LONG            result;

    PAGED_CODE();

    result = RtlCompareUnicodeString(lhs, rhs, TRUE);
    if (result < 0) {

        return GenericLessThan;
    } else if (result > 0) {

        return GenericGreaterThan;
    }
    return GenericEqual;
}

PVOID
NTAPI
PiAllocateGenericTableEntry(
    IN  PRTL_GENERIC_TABLE          Table,
    IN  CLONG                       ByteSize
    )

/*++

Routine Description:

    This routine is the callback for allocation for entries in the generic table.

Arguments:

    Table       - Table for which this is invoked.

    ByteSize    - Amount of memory to allocate.

Return Value:

    Pointer to allocated memory if successful, else NULL.

--*/

{
    PAGED_CODE();

    return ExAllocatePool(PagedPool | POOL_COLD_ALLOCATION, ByteSize);
}

VOID
NTAPI
PiFreeGenericTableEntry(
    IN  PRTL_GENERIC_TABLE          Table,
    IN  PVOID                       Buffer
    )

/*++

Routine Description:

    This routine is the callback for releasing memory for entries in the generic
    table.

Arguments:

    Table       - Table for which this is invoked.

    Buffer      - Buffer to free.

Return Value:

    None.

--*/

{
    PAGED_CODE();

    ExFreePool(Buffer);
}

NTSTATUS
IopMapDeviceObjectToDeviceInstance(
    IN PDEVICE_OBJECT   DeviceObject,
    IN PUNICODE_STRING  DeviceInstance
    )
/*++

Routine Description:

    This routine adds a reference for the specified device to the
    PpDeviceReferenceTable lookup table.

    Note, caller must own the PpRegistryDeviceResource before calling the
    function.

Arguments:

    DeviceObject - supplies a pointer to a physical device object.

    DeviceInstance - supplies a UNICODE_STRING to specify the device instance path.

Return Value:

    Status code that indicates whether or not the function was successful.

--*/

{
    NTSTATUS    status;
    HANDLE      hEnum, hInstance, hControl;
    UNICODE_STRING unicodeKeyName;
    DEVICE_REFERENCE deviceReference;
#if DBG
    PDEVICE_OBJECT oldDeviceObject;
#endif

#if DBG
    oldDeviceObject = IopDeviceObjectFromDeviceInstance(DeviceInstance);
    ASSERT(!oldDeviceObject);
    if (oldDeviceObject) {

        ObDereferenceObject(oldDeviceObject);
    }
#endif

    deviceReference.DeviceObject    = DeviceObject;
    deviceReference.DeviceInstance  = DeviceInstance;
    ExAcquireFastMutex(&PpDeviceReferenceTableLock);
    if (RtlInsertElementGenericTable(&PpDeviceReferenceTable,
                                     (PVOID)&deviceReference,
                                     (CLONG)sizeof(DEVICE_REFERENCE),
                                     NULL)) {
        status = STATUS_SUCCESS;
    } else {

        status = STATUS_UNSUCCESSFUL;
    }
    ExReleaseFastMutex(&PpDeviceReferenceTableLock);

    if (NT_SUCCESS(status)) {
        //
        // Create the volatile Control subkey for this device instance,
        // since user-mode depends on it to be present for non-phantom
        // devices.
        //
        // NTRAID #174944-2000/08/30-jamesca:
        // Remove dependence on the presence of volatile Control subkey
        // for present devices.
        //
        status = IopOpenRegistryKeyEx(&hEnum,
                                      NULL,
                                      &CmRegistryMachineSystemCurrentControlSetEnumName,
                                      KEY_READ);
        if (NT_SUCCESS(status)) {
            status = IopOpenRegistryKeyEx(&hInstance,
                                          hEnum,
                                          DeviceInstance,
                                          KEY_ALL_ACCESS);
            if (NT_SUCCESS(status)) {
                PiWstrToUnicodeString(&unicodeKeyName, REGSTR_KEY_CONTROL);
                status = IopCreateRegistryKeyEx(&hControl,
                                                hInstance,
                                                &unicodeKeyName,
                                                KEY_ALL_ACCESS,
                                                REG_OPTION_VOLATILE,
                                                NULL);
                if (NT_SUCCESS(status)) {
                    ZwClose(hControl);
                }
                ZwClose(hInstance);
            }
            ZwClose(hEnum);
        }

        //
        // The attempt to create the volatile Control subkey should always
        // succeed, but just in case it didn't, make sure to always return
        // STATUS_SUCCESS when the device reference is successfully added to
        // the table.
        //
        ASSERT(NT_SUCCESS(status));
        status = STATUS_SUCCESS;
    }

    return status;
}

PDEVICE_OBJECT
IopDeviceObjectFromDeviceInstance(
    IN PUNICODE_STRING  DeviceInstance
    )

/*++

Routine Description:

    This routine receives a DeviceInstance path (or DeviceInstance handle) and
    returns a reference to a bus device object for the DeviceInstance.

    Note, caller must owner the PpRegistryDeviceResource before calling the function,

Arguments:

    DeviceInstance - supplies a UNICODE_STRING to specify the device instance path.

Returns:

    A reference to the desired bus device object.

--*/

{
    DEVICE_REFERENCE    key;
    PDEVICE_REFERENCE   deviceReference;
    PDEVICE_OBJECT      deviceObject;
    PDEVICE_NODE        deviceNode;

    PAGED_CODE();
    //
    // Look-up the DO in our table.
    //
    deviceObject        = NULL;
    key.DeviceObject    = NULL;
    key.DeviceInstance  = DeviceInstance;
    ExAcquireFastMutex(&PpDeviceReferenceTableLock);

    deviceReference = RtlLookupElementGenericTable(&PpDeviceReferenceTable, (PVOID)&key);
    if (deviceReference) {

        deviceObject = deviceReference->DeviceObject;
        ASSERT(deviceObject);
        if (deviceObject) {

            ASSERT(deviceObject->Type == IO_TYPE_DEVICE);
            if (deviceObject->Type != IO_TYPE_DEVICE) {

                deviceObject = NULL;
            } else {

                deviceNode = (PDEVICE_NODE)deviceObject->DeviceObjectExtension->DeviceNode;
                ASSERT(deviceNode && (deviceNode->PhysicalDeviceObject == deviceObject));
                if (!deviceNode || deviceNode->PhysicalDeviceObject != deviceObject) {

                    deviceObject = NULL;
                }
            }
        }
    }
    //
    // Take a reference if we found the device object.
    //
    if (deviceObject) {

        ObReferenceObject(deviceObject);
    }

    ExReleaseFastMutex(&PpDeviceReferenceTableLock);

    return deviceObject;
}

NTSTATUS
IopDeviceObjectToDeviceInstance (
    IN PDEVICE_OBJECT DeviceObject,
    IN PHANDLE DeviceInstanceHandle,
    IN  ACCESS_MASK DesiredAccess
    )

/*++

Routine Description:

    This routine receives a DeviceObject pointer and returns a handle to the device
    instance path under registry System\ENUM key.

    Note, caller must owner the PpRegistryDeviceResource before calling the function,

Arguments:

    DeviceObject - supplies a pointer to a physical device object.

    DeviceInstanceHandle - Supplies a variable to receive the handle to the registry
             device instance key.

    DesiredAccess - specifies the access that is needed to this key.

Returns:

    NTSTATUS code to indicate success or failure.

--*/

{
    NTSTATUS status;
    HANDLE handle;
    PDEVICE_NODE deviceNode;

    PAGED_CODE();

    status = IopOpenRegistryKeyEx( &handle,
                                   NULL,
                                   &CmRegistryMachineSystemCurrentControlSetEnumName,
                                   KEY_READ
                                   );

    if (!NT_SUCCESS( status )) {
        return status;
    }

    deviceNode = (PDEVICE_NODE) DeviceObject->DeviceObjectExtension->DeviceNode;
    if (deviceNode && (deviceNode->InstancePath.Length != 0)) {
        status = IopOpenRegistryKeyEx( DeviceInstanceHandle,
                                       handle,
                                       &deviceNode->InstancePath,
                                       DesiredAccess
                                       );
    } else {
        status = STATUS_INVALID_DEVICE_REQUEST;
    }
    ZwClose(handle);

    return status;
}

NTSTATUS
IopCleanupDeviceRegistryValues (
    IN PUNICODE_STRING InstancePath
    )

/*++

Routine Description:

    This routine cleans up a device instance key when the device is no
    longer present/enumerated.  If the device is registered to a Service
    the Service's enum key will also been cleaned up.

    Note the caller must lock the RegistryDeviceResource

Arguments:

    InstancePath - supplies a pointer to the name of the device instance key.

Return Value:

    status

--*/

{
    DEVICE_REFERENCE    key;
    NTSTATUS            status;
#if DBG
    PDEVICE_OBJECT      deviceObject;
#endif

    PAGED_CODE();

    //
    // Delete the mapping between this instance path and corresponding DO.
    //
    key.DeviceObject         = NULL;
    key.DeviceInstance       = InstancePath;

    ExAcquireFastMutex(&PpDeviceReferenceTableLock);
    RtlDeleteElementGenericTable(&PpDeviceReferenceTable, (PVOID)&key);
    ExReleaseFastMutex(&PpDeviceReferenceTableLock);
#if DBG
    deviceObject = IopDeviceObjectFromDeviceInstance(InstancePath);
    ASSERT(!deviceObject);
    if (deviceObject) {

        ObDereferenceObject(deviceObject);
    }
#endif

    //
    // Deregister the device from its controlling service's service enum key
    //

    status = PiDeviceRegistration( InstancePath, FALSE, NULL );

    return status;
}

NTSTATUS
IopGetDeviceResourcesFromRegistry (
    IN PDEVICE_OBJECT DeviceObject,
    IN ULONG ResourceType,
    IN ULONG Preference,
    OUT PVOID *Resource,
    OUT PULONG Length
    )

/*++

Routine Description:

    This routine determines the resources decoded by the device specified.
    If the device object is a madeup device, we will try to read the resources
    from registry.  Otherwise, we need to traverse the internal assigned resource
    list to compose the resource list.

Arguments:

    DeviceObject - supplies a pointer to a device object whose registry
        values are to be cleaned up.

    ResourceType - 0 for CM_RESOURCE_LIST and 1 for IO_RESOURCE_REQUIREMENTS_LIS

    Flags - specify the preference.

    Resource - Specified a variable to receive the required resources.

    Length - Specified a variable to receive the length of the resource structure.

Return Value:

    status

--*/

{
    HANDLE handle, handlex;
    NTSTATUS status;
    PKEY_VALUE_FULL_INFORMATION keyValueInformation;
    UNICODE_STRING unicodeName;
    PWCHAR valueName = NULL;

    *Resource = NULL;
    *Length = 0;

    //
    // Open the LogConfig key of the device instance.
    //

    status = IopDeviceObjectToDeviceInstance(DeviceObject, &handlex, KEY_READ);
    if (!NT_SUCCESS(status)) {
        return status;
    }

    if (ResourceType == QUERY_RESOURCE_LIST) {

        //
        // Caller is asking for CM_RESOURCE_LIST
        //

        if (Preference & REGISTRY_ALLOC_CONFIG) {

            //
            // Try alloc config first
            //

            PiWstrToUnicodeString(&unicodeName, REGSTR_KEY_CONTROL);
            status = IopOpenRegistryKeyEx( &handle,
                                           handlex,
                                           &unicodeName,
                                           KEY_READ
                                           );
            if (NT_SUCCESS(status)) {
                status = PipReadDeviceConfiguration (handle, REGISTRY_ALLOC_CONFIG, (PCM_RESOURCE_LIST *)Resource, Length);
                ZwClose(handle);
                if (NT_SUCCESS(status)) {
                    ZwClose(handlex);
                    return status;
                }
            }
        }

        handle = NULL;
        if (Preference & REGISTRY_FORCED_CONFIG) {

            PiWstrToUnicodeString(&unicodeName, REGSTR_KEY_LOG_CONF);
            status = IopOpenRegistryKeyEx( &handle,
                                           handlex,
                                           &unicodeName,
                                           KEY_READ
                                           );
            if (NT_SUCCESS(status)) {
                status = PipReadDeviceConfiguration (handle, REGISTRY_FORCED_CONFIG, (PCM_RESOURCE_LIST *)Resource, Length);
                if (NT_SUCCESS(status)) {
                    ZwClose(handle);
                    ZwClose(handlex);
                    return status;
                }
            } else {
                ZwClose(handlex);
                return status;
            }
        }
        if (Preference & REGISTRY_BOOT_CONFIG) {

            //
            // Try alloc config first
            //

            if (handle == NULL) {
                PiWstrToUnicodeString(&unicodeName, REGSTR_KEY_LOG_CONF);
                status = IopOpenRegistryKeyEx( &handle,
                                               handlex,
                                               &unicodeName,
                                               KEY_READ
                                               );
                if (!NT_SUCCESS(status)) {
                    ZwClose(handlex);
                    return status;
                }
            }
            status = PipReadDeviceConfiguration( handle,
                                                 REGISTRY_BOOT_CONFIG,
                                                 (PCM_RESOURCE_LIST *)Resource,
                                                 Length);
        }
        if (handle) {
            ZwClose(handle);
        }
    } else {

        PiWstrToUnicodeString(&unicodeName, REGSTR_KEY_LOG_CONF);
        status = IopOpenRegistryKeyEx( &handle,
                                       handlex,
                                       &unicodeName,
                                       KEY_READ
                                       );
        if (NT_SUCCESS(status)) {

            if (Preference & REGISTRY_OVERRIDE_CONFIGVECTOR) {
                valueName = REGSTR_VALUE_OVERRIDE_CONFIG_VECTOR;
            } else if (Preference & REGISTRY_BASIC_CONFIGVECTOR) {
                valueName = REGSTR_VALUE_BASIC_CONFIG_VECTOR;
            }
            if (valueName) {

                //
                // Try to read device's configuration vector
                //

                status = IopGetRegistryValue (handle,
                                              valueName,
                                              &keyValueInformation);
                if (NT_SUCCESS(status)) {

                    //
                    // Try to read what caller wants.
                    //

                    if ((keyValueInformation->Type == REG_RESOURCE_REQUIREMENTS_LIST) &&
                        (keyValueInformation->DataLength != 0)) {

                        *Resource = ExAllocatePool(PagedPool,
                                                   keyValueInformation->DataLength);
                        if (*Resource) {
                            PIO_RESOURCE_REQUIREMENTS_LIST ioResource;

                            *Length = keyValueInformation->DataLength;
                            RtlCopyMemory(*Resource,
                                          KEY_VALUE_DATA(keyValueInformation),
                                          keyValueInformation->DataLength);

                            //
                            // Process the io resource requirements list to change undefined
                            // interface type to our default type.
                            //

                            ioResource = *Resource;
                            if (ioResource->InterfaceType == InterfaceTypeUndefined) {
                                ioResource->BusNumber = 0;
                                ioResource->InterfaceType = PnpDefaultInterfaceType;
                            }
                        } else {
                            status = STATUS_INVALID_PARAMETER_2;
                        }
                    }
                    ExFreePool(keyValueInformation);
                }
            }
            ZwClose(handle);
        }
    }
    ZwClose(handlex);
    return status;
}

NTSTATUS
PipReadDeviceConfiguration (
    IN HANDLE Handle,
    IN ULONG Flags,
    OUT PCM_RESOURCE_LIST *CmResource,
    OUT PULONG Length
    )

/*++

Routine Description:

    This routine read the specified ALLOC config or ForcedConfig or Boot config.

Arguments:

    Hanle - supplies a handle to the registry key to read resources.

Return Value:

    status

--*/

{
    NTSTATUS status;
    PKEY_VALUE_FULL_INFORMATION keyValueInformation;
    PWCHAR valueName;

    *CmResource = NULL;
    *Length = 0;

    if (Flags == REGISTRY_ALLOC_CONFIG) {
        valueName = REGSTR_VALUE_ALLOC_CONFIG;
    } else if (Flags == REGISTRY_FORCED_CONFIG) {
        valueName = REGSTR_VALUE_FORCED_CONFIG;
    } else if (Flags == REGISTRY_BOOT_CONFIG) {
        valueName = REGSTR_VALUE_BOOT_CONFIG;
    } else {
        return STATUS_INVALID_PARAMETER_2;
    }

    //
    // Read the registry value of the desired value name
    //

    status = IopGetRegistryValue (Handle,
                                  valueName,
                                  &keyValueInformation);
    if (NT_SUCCESS(status)) {

        //
        // Try to read what caller wants.
        //

        if ((keyValueInformation->Type == REG_RESOURCE_LIST) &&
            (keyValueInformation->DataLength != 0)) {
            *CmResource = ExAllocatePool(PagedPool,
                                         keyValueInformation->DataLength);
            if (*CmResource) {
                if (*CmResource) {
                    *Length = keyValueInformation->DataLength;
                    RtlCopyMemory(*CmResource,
                                  KEY_VALUE_DATA(keyValueInformation),
                                  keyValueInformation->DataLength);
                } else {
                    status = STATUS_INSUFFICIENT_RESOURCES;
                }
            }
            ExFreePool(keyValueInformation);
            if (*CmResource) {
                PCM_RESOURCE_LIST resourceList;
                PCM_FULL_RESOURCE_DESCRIPTOR cmFullDesc;
                PCM_PARTIAL_RESOURCE_DESCRIPTOR cmPartDesc;
                ULONG j, k, size;

                //
                // Process the resource list read from Registry to change undefined
                // interface type to our default interface type.
                //

                resourceList = *CmResource;
                cmFullDesc = &resourceList->List[0];
                for (j = 0; j < resourceList->Count; j++) {
                    if (cmFullDesc->InterfaceType == InterfaceTypeUndefined) {
                        cmFullDesc->BusNumber = 0;
                        cmFullDesc->InterfaceType = PnpDefaultInterfaceType;
                    }
                    cmPartDesc = &cmFullDesc->PartialResourceList.PartialDescriptors[0];
                    for (k = 0; k < cmFullDesc->PartialResourceList.Count; k++) {
                        size = 0;
                        switch (cmPartDesc->Type) {
                        case CmResourceTypeDeviceSpecific:
                             size = cmPartDesc->u.DeviceSpecificData.DataSize;
                             break;
                        }
                        cmPartDesc++;
                        cmPartDesc = (PCM_PARTIAL_RESOURCE_DESCRIPTOR) ((PUCHAR)cmPartDesc + size);
                    }
                    cmFullDesc = (PCM_FULL_RESOURCE_DESCRIPTOR)cmPartDesc;
                }
            }
        } else if (keyValueInformation->Type != REG_RESOURCE_LIST) {
            status = STATUS_UNSUCCESSFUL;
        }
    }
    return status;
}

PIO_RESOURCE_REQUIREMENTS_LIST
IopCmResourcesToIoResources(
    IN ULONG SlotNumber,
    IN PCM_RESOURCE_LIST CmResourceList,
    IN ULONG Priority
    )

/*++

Routine Description:

    This routines converts the input CmResourceList to IO_RESOURCE_REQUIREMENTS_LIST.

Arguments:

    SlotNumber - supplies the SlotNumber the resources refer to.

    CmResourceList - the cm resource list to convert.

    Priority - specifies the priority of the logconfig

Return Value:

    returns a IO_RESOURCE_REQUIREMENTS_LISTST if succeeds.  Otherwise a NULL value is
    returned.

--*/
{
    PIO_RESOURCE_REQUIREMENTS_LIST ioResReqList;
    ULONG count = 0, size, i, j;
    PCM_FULL_RESOURCE_DESCRIPTOR cmFullDesc;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR cmPartDesc;
    PIO_RESOURCE_DESCRIPTOR ioDesc;

    //
    // First determine number of descriptors required.
    //

    cmFullDesc = &CmResourceList->List[0];
    for (i = 0; i < CmResourceList->Count; i++) {
        count += cmFullDesc->PartialResourceList.Count;
        cmPartDesc = &cmFullDesc->PartialResourceList.PartialDescriptors[0];
        for (j = 0; j < cmFullDesc->PartialResourceList.Count; j++) {
            size = 0;
            switch (cmPartDesc->Type) {
            case CmResourceTypeDeviceSpecific:
                 size = cmPartDesc->u.DeviceSpecificData.DataSize;
                 count--;
                 break;
            }
            cmPartDesc++;
            cmPartDesc = (PCM_PARTIAL_RESOURCE_DESCRIPTOR) ((PUCHAR)cmPartDesc + size);
        }
        cmFullDesc = (PCM_FULL_RESOURCE_DESCRIPTOR)cmPartDesc;
    }

    if (count == 0) {
        return NULL;
    }

    //
    // Count the extra descriptors for InterfaceType and BusNumber information.
    //

    count += CmResourceList->Count - 1;

    //
    // Allocate heap space for IO RESOURCE REQUIREMENTS LIST
    //

    count++;           // add one for CmResourceTypeConfigData
    ioResReqList = (PIO_RESOURCE_REQUIREMENTS_LIST)ExAllocatePool(
                       PagedPool,
                       sizeof(IO_RESOURCE_REQUIREMENTS_LIST) +
                           count * sizeof(IO_RESOURCE_DESCRIPTOR)
                       );
    if (!ioResReqList) {
        return NULL;
    }

    //
    // Parse the cm resource descriptor and build its corresponding IO resource descriptor
    //

    ioResReqList->InterfaceType = CmResourceList->List[0].InterfaceType;
    ioResReqList->BusNumber = CmResourceList->List[0].BusNumber;
    ioResReqList->SlotNumber = SlotNumber;
    ioResReqList->Reserved[0] = 0;
    ioResReqList->Reserved[1] = 0;
    ioResReqList->Reserved[2] = 0;
    ioResReqList->AlternativeLists = 1;
    ioResReqList->List[0].Version = 1;
    ioResReqList->List[0].Revision = 1;
    ioResReqList->List[0].Count = count;

    //
    // Generate a CmResourceTypeConfigData descriptor
    //

    ioDesc = &ioResReqList->List[0].Descriptors[0];
    ioDesc->Option = IO_RESOURCE_PREFERRED;
    ioDesc->Type = CmResourceTypeConfigData;
    ioDesc->ShareDisposition = CmResourceShareShared;
    ioDesc->Flags = 0;
    ioDesc->Spare1 = 0;
    ioDesc->Spare2 = 0;
    ioDesc->u.ConfigData.Priority = Priority;
    ioDesc++;

    cmFullDesc = &CmResourceList->List[0];
    for (i = 0; i < CmResourceList->Count; i++) {
        if (i != 0) {

            //
            // Set up descriptor to remember the InterfaceType and BusNumber.
            //

            ioDesc->Option = IO_RESOURCE_PREFERRED;
            ioDesc->Type = CmResourceTypeReserved;
            ioDesc->ShareDisposition = CmResourceShareUndetermined;
            ioDesc->Flags = 0;
            ioDesc->Spare1 = 0;
            ioDesc->Spare2 = 0;
            if (cmFullDesc->InterfaceType == InterfaceTypeUndefined) {
                ioDesc->u.DevicePrivate.Data[0] = PnpDefaultInterfaceType;
            } else {
                ioDesc->u.DevicePrivate.Data[0] = cmFullDesc->InterfaceType;
            }
            ioDesc->u.DevicePrivate.Data[1] = cmFullDesc->BusNumber;
            ioDesc->u.DevicePrivate.Data[2] = 0;
            ioDesc++;
        }
        cmPartDesc = &cmFullDesc->PartialResourceList.PartialDescriptors[0];
        for (j = 0; j < cmFullDesc->PartialResourceList.Count; j++) {
            ioDesc->Option = IO_RESOURCE_PREFERRED;
            ioDesc->Type = cmPartDesc->Type;
            ioDesc->ShareDisposition = cmPartDesc->ShareDisposition;
            ioDesc->Flags = cmPartDesc->Flags;
            ioDesc->Spare1 = 0;
            ioDesc->Spare2 = 0;

            size = 0;
            switch (cmPartDesc->Type) {
            case CmResourceTypePort:
                 ioDesc->u.Port.MinimumAddress = cmPartDesc->u.Port.Start;
                 ioDesc->u.Port.MaximumAddress.QuadPart = cmPartDesc->u.Port.Start.QuadPart +
                                                             cmPartDesc->u.Port.Length - 1;
                 ioDesc->u.Port.Alignment = 1;
                 ioDesc->u.Port.Length = cmPartDesc->u.Port.Length;
                 ioDesc++;
                 break;
            case CmResourceTypeInterrupt:
#if defined(_X86_)
                ioDesc->u.Interrupt.MinimumVector = ioDesc->u.Interrupt.MaximumVector =
                   cmPartDesc->u.Interrupt.Level;
#else
                 ioDesc->u.Interrupt.MinimumVector = ioDesc->u.Interrupt.MaximumVector =
                    cmPartDesc->u.Interrupt.Vector;
#endif
                 ioDesc++;
                 break;
            case CmResourceTypeMemory:
                 ioDesc->u.Memory.MinimumAddress = cmPartDesc->u.Memory.Start;
                 ioDesc->u.Memory.MaximumAddress.QuadPart = cmPartDesc->u.Memory.Start.QuadPart +
                                                               cmPartDesc->u.Memory.Length - 1;
                 ioDesc->u.Memory.Alignment = 1;
                 ioDesc->u.Memory.Length = cmPartDesc->u.Memory.Length;
                 ioDesc++;
                 break;
            case CmResourceTypeDma:
                 ioDesc->u.Dma.MinimumChannel = cmPartDesc->u.Dma.Channel;
                 ioDesc->u.Dma.MaximumChannel = cmPartDesc->u.Dma.Channel;
                 ioDesc++;
                 break;
            case CmResourceTypeDeviceSpecific:
                 size = cmPartDesc->u.DeviceSpecificData.DataSize;
                 break;
            case CmResourceTypeBusNumber:
                 ioDesc->u.BusNumber.MinBusNumber = cmPartDesc->u.BusNumber.Start;
                 ioDesc->u.BusNumber.MaxBusNumber = cmPartDesc->u.BusNumber.Start +
                                                    cmPartDesc->u.BusNumber.Length - 1;
                 ioDesc->u.BusNumber.Length = cmPartDesc->u.BusNumber.Length;
                 ioDesc++;
                 break;
            default:
                 ioDesc->u.DevicePrivate.Data[0] = cmPartDesc->u.DevicePrivate.Data[0];
                 ioDesc->u.DevicePrivate.Data[1] = cmPartDesc->u.DevicePrivate.Data[1];
                 ioDesc->u.DevicePrivate.Data[2] = cmPartDesc->u.DevicePrivate.Data[2];
                 ioDesc++;
                 break;
            }
            cmPartDesc++;
            cmPartDesc = (PCM_PARTIAL_RESOURCE_DESCRIPTOR) ((PUCHAR)cmPartDesc + size);
        }
        cmFullDesc = (PCM_FULL_RESOURCE_DESCRIPTOR)cmPartDesc;
    }
    ioResReqList->ListSize = (ULONG)((ULONG_PTR)ioDesc - (ULONG_PTR)ioResReqList);
    return ioResReqList;
}

NTSTATUS
IopFilterResourceRequirementsList(
    IN PIO_RESOURCE_REQUIREMENTS_LIST IoList,
    IN PCM_RESOURCE_LIST CmList,
    IN OUT PIO_RESOURCE_REQUIREMENTS_LIST *FilteredList,
    OUT PBOOLEAN ExactMatch
    )

/*++

Routine Description:

    This routines adjusts the input IoList based on input BootConfig.


Arguments:

    IoList - supplies the pointer to an IoResourceRequirementsList

    CmList - supplies the pointer to a BootConfig.

    FilteredList - Supplies a variable to receive the filtered resource
             requirements list.

Return Value:

    A NTSTATUS code to indicate the result of the function.

--*/
{
    PIO_RESOURCE_REQUIREMENTS_LIST ioList, newList;
    PIO_RESOURCE_LIST ioResourceList, newIoResourceList, selectedResourceList = NULL;
    PIO_RESOURCE_DESCRIPTOR ioResourceDescriptor, ioResourceDescriptorEnd;
    PIO_RESOURCE_DESCRIPTOR newIoResourceDescriptor, configDataDescriptor;
    LONG ioResourceDescriptorCount = 0;
    USHORT version;
    PCM_FULL_RESOURCE_DESCRIPTOR cmFullDesc;
    PCM_PARTIAL_RESOURCE_DESCRIPTOR cmDescriptor;
    ULONG cmDescriptorCount = 0;
    ULONG size, i, j, oldCount, phase;
    LONG k, alternativeLists;
    BOOLEAN exactMatch;

    PAGED_CODE();

    *FilteredList = NULL;
    *ExactMatch = FALSE;

    //
    // Make sure there is some resource requirements to be filtered.
    // If no, we will convert CmList/BootConfig to an IoResourceRequirementsList
    //

    if (IoList == NULL || IoList->AlternativeLists == 0) {
        if (CmList && CmList->Count != 0) {
            *FilteredList = IopCmResourcesToIoResources (0, CmList, LCPRI_BOOTCONFIG);
        }
        return STATUS_SUCCESS;
    }

    //
    // Make a copy of the Io Resource Requirements List
    //

    ioList = (PIO_RESOURCE_REQUIREMENTS_LIST) ExAllocatePool(PagedPool, IoList->ListSize);
    if (ioList == NULL) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlCopyMemory(ioList, IoList, IoList->ListSize);

    //
    // If there is no BootConfig, simply return the copy of the input Io list.
    //

    if (CmList == NULL || CmList->Count == 0) {
        *FilteredList = ioList;
        return STATUS_SUCCESS;
    }

    //
    // First determine minimum number of descriptors required.
    //

    cmFullDesc = &CmList->List[0];
    for (i = 0; i < CmList->Count; i++) {
        cmDescriptorCount += cmFullDesc->PartialResourceList.Count;
        cmDescriptor = &cmFullDesc->PartialResourceList.PartialDescriptors[0];
        for (j = 0; j < cmFullDesc->PartialResourceList.Count; j++) {
            size = 0;
            switch (cmDescriptor->Type) {
            case CmResourceTypeConfigData:
            case CmResourceTypeDevicePrivate:
                 cmDescriptorCount--;
                 break;
            case CmResourceTypeDeviceSpecific:
                 size = cmDescriptor->u.DeviceSpecificData.DataSize;
                 cmDescriptorCount--;
                 break;
            default:

                 //
                 // Invalid cmresource list.  Ignore it and use io resources
                 //

                 if (cmDescriptor->Type == CmResourceTypeNull ||
                     cmDescriptor->Type >= CmResourceTypeMaximum) {
                     cmDescriptorCount--;
                 }
            }
            cmDescriptor++;
            cmDescriptor = (PCM_PARTIAL_RESOURCE_DESCRIPTOR) ((PUCHAR)cmDescriptor + size);
        }
        cmFullDesc = (PCM_FULL_RESOURCE_DESCRIPTOR)cmDescriptor;
    }

    if (cmDescriptorCount == 0) {
        *FilteredList = ioList;
        return STATUS_SUCCESS;
    }

    //
    // cmDescriptorCount is the number of BootConfig Descriptors needs.
    //
    // For each IO list Alternative ...
    //

    ioResourceList = ioList->List;
    k = ioList->AlternativeLists;
    while (--k >= 0) {
        ioResourceDescriptor = ioResourceList->Descriptors;
        ioResourceDescriptorEnd = ioResourceDescriptor + ioResourceList->Count;
        while (ioResourceDescriptor < ioResourceDescriptorEnd) {
            ioResourceDescriptor->Spare1 = 0;
            ioResourceDescriptor++;
        }
        ioResourceList = (PIO_RESOURCE_LIST) ioResourceDescriptorEnd;
    }

    ioResourceList = ioList->List;
    k = alternativeLists = ioList->AlternativeLists;
    while (--k >= 0) {
        version = ioResourceList->Version;
        if (version == 0xffff) {  // Convert bogus version to valid number
            version = 1;
        }

        //
        // We use Version field to store number of BootConfig found.
        // Count field to store new number of descriptor in the alternative list.
        //

        ioResourceList->Version = 0;
        oldCount = ioResourceList->Count;

        ioResourceDescriptor = ioResourceList->Descriptors;
        ioResourceDescriptorEnd = ioResourceDescriptor + ioResourceList->Count;

        if (ioResourceDescriptor == ioResourceDescriptorEnd) {

            //
            // An alternative list with zero descriptor count
            //

            ioResourceList->Version = 0xffff;  // Mark it as invalid
            ioList->AlternativeLists--;
            continue;
        }

        exactMatch = TRUE;

        //
        // For each Cm Resource descriptor ... except DevicePrivate and
        // DeviceSpecific...
        //

        cmFullDesc = &CmList->List[0];
        for (i = 0; i < CmList->Count; i++) {
            cmDescriptor = &cmFullDesc->PartialResourceList.PartialDescriptors[0];
            for (j = 0; j < cmFullDesc->PartialResourceList.Count; j++) {
                size = 0;
                switch (cmDescriptor->Type) {
                case CmResourceTypeDevicePrivate:
                     break;
                case CmResourceTypeDeviceSpecific:
                     size = cmDescriptor->u.DeviceSpecificData.DataSize;
                     break;
                default:
                    if (cmDescriptor->Type == CmResourceTypeNull ||
                        cmDescriptor->Type >= CmResourceTypeMaximum) {
                        break;
                    }

                    //
                    // Check CmDescriptor against current Io Alternative list
                    //

                    for (phase = 0; phase < 2; phase++) {
                        ioResourceDescriptor = ioResourceList->Descriptors;
                        while (ioResourceDescriptor < ioResourceDescriptorEnd) {
                            if ((ioResourceDescriptor->Type == cmDescriptor->Type) &&
                                (ioResourceDescriptor->Spare1 == 0)) {
                                ULONGLONG min1, max1, min2, max2;
                                ULONG len1 = 1, len2 = 1, align1, align2;
                                UCHAR share1, share2;

                                share2 = ioResourceDescriptor->ShareDisposition;
                                share1 = cmDescriptor->ShareDisposition;
                                if ((share1 == CmResourceShareUndetermined) ||
                                    (share1 > CmResourceShareShared)) {
                                    share1 = share2;
                                }
                                if ((share2 == CmResourceShareUndetermined) ||
                                    (share2 > CmResourceShareShared)) {
                                    share2 = share1;
                                }
                                align1 = align2 = 1;

                                switch (cmDescriptor->Type) {
                                case CmResourceTypePort:
                                case CmResourceTypeMemory:
                                    min1 = cmDescriptor->u.Port.Start.QuadPart;
                                    max1 = cmDescriptor->u.Port.Start.QuadPart + cmDescriptor->u.Port.Length - 1;
                                    len1 = cmDescriptor->u.Port.Length;
                                    min2 = ioResourceDescriptor->u.Port.MinimumAddress.QuadPart;
                                    max2 = ioResourceDescriptor->u.Port.MaximumAddress.QuadPart;
                                    len2 = ioResourceDescriptor->u.Port.Length;
                                    align2 = ioResourceDescriptor->u.Port.Alignment;
                                    break;
                                case CmResourceTypeInterrupt:
                                    max1 = min1 = cmDescriptor->u.Interrupt.Vector;
                                    min2 = ioResourceDescriptor->u.Interrupt.MinimumVector;
                                    max2 = ioResourceDescriptor->u.Interrupt.MaximumVector;
                                    break;
                                case CmResourceTypeDma:
                                    min1 = max1 =cmDescriptor->u.Dma.Channel;
                                    min2 = ioResourceDescriptor->u.Dma.MinimumChannel;
                                    max2 = ioResourceDescriptor->u.Dma.MaximumChannel;
                                    break;
                                case CmResourceTypeBusNumber:
                                    min1 = cmDescriptor->u.BusNumber.Start;
                                    max1 = cmDescriptor->u.BusNumber.Start + cmDescriptor->u.BusNumber.Length - 1;
                                    len1 = cmDescriptor->u.BusNumber.Length;
                                    min2 = ioResourceDescriptor->u.BusNumber.MinBusNumber;
                                    max2 = ioResourceDescriptor->u.BusNumber.MaxBusNumber;
                                    len2 = ioResourceDescriptor->u.BusNumber.Length;
                                    break;
                                default:
                                    ASSERT(0);
                                    break;
                                }
                                if (phase == 0) {
                                    if (share1 == share2 && min2 == min1 && max2 >= max1 && len2 >= len1) {

                                        //
                                        // For phase 0 match, we want near exact match...
                                        //

                                        if (max2 != max1) {
                                            exactMatch = FALSE;
                                        }
                                        ioResourceList->Version++;
                                        ioResourceDescriptor->Spare1 = 0x80;
                                        if (ioResourceDescriptor->Option & IO_RESOURCE_ALTERNATIVE) {
                                            PIO_RESOURCE_DESCRIPTOR ioDesc;

                                            ioDesc = ioResourceDescriptor;
                                            ioDesc--;
                                            while (ioDesc >= ioResourceList->Descriptors) {
                                                ioDesc->Type = CmResourceTypeNull;
                                                ioResourceList->Count--;
                                                if (ioDesc->Option == IO_RESOURCE_ALTERNATIVE) {
                                                    ioDesc--;
                                                } else {
                                                    break;
                                                }
                                            }
                                        }
                                        ioResourceDescriptor->Option = IO_RESOURCE_PREFERRED;
                                        ioResourceDescriptor->Flags = cmDescriptor->Flags;
                                        if (ioResourceDescriptor->Type == CmResourceTypePort ||
                                            ioResourceDescriptor->Type == CmResourceTypeMemory) {
                                            ioResourceDescriptor->u.Port.MinimumAddress.QuadPart = min1;
                                            ioResourceDescriptor->u.Port.MaximumAddress.QuadPart = min1 + len2 - 1;
                                            ioResourceDescriptor->u.Port.Alignment = 1;
                                        } else if (ioResourceDescriptor->Type == CmResourceTypeBusNumber) {
                                            ioResourceDescriptor->u.BusNumber.MinBusNumber = (ULONG)min1;
                                            ioResourceDescriptor->u.BusNumber.MaxBusNumber = (ULONG)(min1 + len2 - 1);
                                        }
                                        ioResourceDescriptor++;
                                        while (ioResourceDescriptor < ioResourceDescriptorEnd) {
                                            if (ioResourceDescriptor->Option & IO_RESOURCE_ALTERNATIVE) {
                                                ioResourceDescriptor->Type = CmResourceTypeNull;
                                                ioResourceDescriptor++;
                                                ioResourceList->Count--;
                                            } else {
                                                break;
                                            }
                                        }
                                        phase = 1;   // skip phase 1
                                        break;
                                    } else {
                                        ioResourceDescriptor++;
                                    }
                                } else {
                                    exactMatch = FALSE;
                                    if (share1 == share2 && min2 <= min1 && max2 >= max1 && len2 >= len1 &&
                                        (min1 & (align2 - 1)) == 0) {

                                        //
                                        // Io range covers Cm range ... Change the Io range to what is specified
                                        // in BootConfig.
                                        //
                                        //

                                        switch (cmDescriptor->Type) {
                                        case CmResourceTypePort:
                                        case CmResourceTypeMemory:
                                            ioResourceDescriptor->u.Port.MinimumAddress.QuadPart = min1;
                                            ioResourceDescriptor->u.Port.MaximumAddress.QuadPart = min1 + len2 - 1;
                                            break;
                                        case CmResourceTypeInterrupt:
                                        case CmResourceTypeDma:
                                            ioResourceDescriptor->u.Interrupt.MinimumVector = (ULONG)min1;
                                            ioResourceDescriptor->u.Interrupt.MaximumVector = (ULONG)max1;
                                            break;
                                        case CmResourceTypeBusNumber:
                                            ioResourceDescriptor->u.BusNumber.MinBusNumber = (ULONG)min1;
                                            ioResourceDescriptor->u.BusNumber.MaxBusNumber = (ULONG)(min1 + len2 - 1);
                                            break;
                                        }
                                        ioResourceList->Version++;
                                        ioResourceDescriptor->Spare1 = 0x80;
                                        ioResourceDescriptor->Flags = cmDescriptor->Flags;
                                        if (ioResourceDescriptor->Option & IO_RESOURCE_ALTERNATIVE) {
                                            PIO_RESOURCE_DESCRIPTOR ioDesc;

                                            ioDesc = ioResourceDescriptor;
                                            ioDesc--;
                                            while (ioDesc >= ioResourceList->Descriptors) {
                                                ioDesc->Type = CmResourceTypeNull;
                                                ioResourceList->Count--;
                                                if (ioDesc->Option == IO_RESOURCE_ALTERNATIVE) {
                                                    ioDesc--;
                                                } else {
                                                    break;
                                                }
                                            }
                                        }
                                        ioResourceDescriptor->Option = IO_RESOURCE_PREFERRED;
                                        ioResourceDescriptor++;
                                        while (ioResourceDescriptor < ioResourceDescriptorEnd) {
                                            if (ioResourceDescriptor->Option & IO_RESOURCE_ALTERNATIVE) {
                                                ioResourceDescriptor->Type = CmResourceTypeNull;
                                                ioResourceList->Count--;
                                                ioResourceDescriptor++;
                                            } else {
                                                break;
                                            }
                                        }
                                        break;
                                    } else {
                                        ioResourceDescriptor++;
                                    }
                                }
                            } else {
                                ioResourceDescriptor++;
                            }
                        } // Don't add any instruction after this ...
                    } // phase
                } // switch

                //
                // Move to next Cm Descriptor
                //

                cmDescriptor++;
                cmDescriptor = (PCM_PARTIAL_RESOURCE_DESCRIPTOR) ((PUCHAR)cmDescriptor + size);
            }

            //
            // Move to next Cm List
            //

            cmFullDesc = (PCM_FULL_RESOURCE_DESCRIPTOR)cmDescriptor;
        }

        if (ioResourceList->Version != (USHORT)cmDescriptorCount) {

            //
            // If the current alternative list does not cover all the boot config
            // descriptors, make it as invalid.
            //

            ioResourceList->Version = 0xffff;
            ioList->AlternativeLists--;
        } else {
            if ((ioResourceList->Count == cmDescriptorCount) ||
                (ioResourceList->Count == (cmDescriptorCount + 1) &&
                 ioResourceList->Descriptors[0].Type == CmResourceTypeConfigData)) {
                if (selectedResourceList) {
                    ioResourceList->Version = 0xffff;
                    ioList->AlternativeLists--;
                } else {
                    selectedResourceList = ioResourceList;
                    ioResourceDescriptorCount += ioResourceList->Count;
                    ioResourceList->Version = version;
                    if (exactMatch) {
                        *ExactMatch = TRUE;
                    }
                }
            } else {
                ioResourceDescriptorCount += ioResourceList->Count;
                ioResourceList->Version = version;
            }
        }
        ioResourceList->Count = oldCount;

        //
        // Move to next Io alternative list.
        //

        ioResourceList = (PIO_RESOURCE_LIST) ioResourceDescriptorEnd;
    }

    //
    // If there is not any valid alternative, convert CmList to Io list.
    //

    if (ioList->AlternativeLists == 0) {
         *FilteredList = IopCmResourcesToIoResources (0, CmList, LCPRI_BOOTCONFIG);
        ExFreePool(ioList);
        return STATUS_SUCCESS;
    }

    //
    // we have finished filtering the resource requirements list.  Now allocate memory
    // and rebuild a new list.
    //

    size = sizeof(IO_RESOURCE_REQUIREMENTS_LIST) +
               sizeof(IO_RESOURCE_LIST) * (ioList->AlternativeLists - 1) +
               sizeof(IO_RESOURCE_DESCRIPTOR) * (ioResourceDescriptorCount);
    newList = (PIO_RESOURCE_REQUIREMENTS_LIST) ExAllocatePool(PagedPool, size);
    if (newList == NULL) {
        ExFreePool(ioList);
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    //
    // Walk through the io resource requirements list and pick up any valid descriptor.
    //

    newList->ListSize = size;
    newList->InterfaceType = CmList->List->InterfaceType;
    newList->BusNumber = CmList->List->BusNumber;
    newList->SlotNumber = ioList->SlotNumber;
    if (ioList->AlternativeLists > 1) {
        *ExactMatch = FALSE;
    }
    newList->AlternativeLists = ioList->AlternativeLists;
    ioResourceList = ioList->List;
    newIoResourceList = newList->List;
    while (--alternativeLists >= 0) {
        ioResourceDescriptor = ioResourceList->Descriptors;
        ioResourceDescriptorEnd = ioResourceDescriptor + ioResourceList->Count;
        if (ioResourceList->Version == 0xffff) {
            ioResourceList = (PIO_RESOURCE_LIST)ioResourceDescriptorEnd;
            continue;
        }
        newIoResourceList->Version = ioResourceList->Version;
        newIoResourceList->Revision = ioResourceList->Revision;

        newIoResourceDescriptor = newIoResourceList->Descriptors;
        if (ioResourceDescriptor->Type != CmResourceTypeConfigData) {
            newIoResourceDescriptor->Option = IO_RESOURCE_PREFERRED;
            newIoResourceDescriptor->Type = CmResourceTypeConfigData;
            newIoResourceDescriptor->ShareDisposition = CmResourceShareShared;
            newIoResourceDescriptor->Flags = 0;
            newIoResourceDescriptor->Spare1 = 0;
            newIoResourceDescriptor->Spare2 = 0;
            newIoResourceDescriptor->u.ConfigData.Priority = LCPRI_BOOTCONFIG;
            configDataDescriptor = newIoResourceDescriptor;
            newIoResourceDescriptor++;
        } else {
            newList->ListSize -= sizeof(IO_RESOURCE_DESCRIPTOR);
            configDataDescriptor = newIoResourceDescriptor;
        }

        while (ioResourceDescriptor < ioResourceDescriptorEnd) {
            if (ioResourceDescriptor->Type != CmResourceTypeNull) {
                *newIoResourceDescriptor = *ioResourceDescriptor;
                newIoResourceDescriptor++;
            }
            ioResourceDescriptor++;
        }
        newIoResourceList->Count = (ULONG)(newIoResourceDescriptor - newIoResourceList->Descriptors);

        //if (newIoResourceList->Count == (cmDescriptorCount + 1)) {
        configDataDescriptor->u.ConfigData.Priority =  LCPRI_BOOTCONFIG;
        //}

        //
        // Move to next Io alternative list.
        //

        newIoResourceList = (PIO_RESOURCE_LIST) newIoResourceDescriptor;
        ioResourceList = (PIO_RESOURCE_LIST) ioResourceDescriptorEnd;
    }
    ASSERT((PUCHAR)newIoResourceList == ((PUCHAR)newList + newList->ListSize));

    *FilteredList = newList;
    ExFreePool(ioList);
    return STATUS_SUCCESS;
}

NTSTATUS
IopMergeFilteredResourceRequirementsList (
    IN PIO_RESOURCE_REQUIREMENTS_LIST IoList1,
    IN PIO_RESOURCE_REQUIREMENTS_LIST IoList2,
    IN OUT PIO_RESOURCE_REQUIREMENTS_LIST *MergedList
    )

/*++

Routine Description:

    This routines merges two IoLists into one.


Arguments:

    IoList1 - supplies the pointer to the first IoResourceRequirementsList

    IoList2 - supplies the pointer to the second IoResourceRequirementsList

    MergedList - Supplies a variable to receive the merged resource
             requirements list.

Return Value:

    A NTSTATUS code to indicate the result of the function.

--*/
{
    NTSTATUS status = STATUS_SUCCESS;
    PIO_RESOURCE_REQUIREMENTS_LIST ioList, newList;
    ULONG size;
    PUCHAR p;

    PAGED_CODE();

    *MergedList = NULL;

    //
    // First handle the easy cases that both IO Lists are empty or any one of
    // them is empty.
    //

    if ((IoList1 == NULL || IoList1->AlternativeLists == 0) &&
        (IoList2 == NULL || IoList2->AlternativeLists == 0)) {
        return status;
    }
    ioList = NULL;
    if (IoList1 == NULL || IoList1->AlternativeLists == 0) {
        ioList = IoList2;
    } else if (IoList2 == NULL || IoList2->AlternativeLists == 0) {
        ioList = IoList1;
    }
    if (ioList) {
        newList = (PIO_RESOURCE_REQUIREMENTS_LIST) ExAllocatePool(PagedPool, ioList->ListSize);
        if (newList == NULL) {
            return STATUS_INSUFFICIENT_RESOURCES;
        }
        RtlCopyMemory(newList, ioList, ioList->ListSize);
        *MergedList = newList;
        return status;
    }

    //
    // Do real work...
    //

    size = IoList1->ListSize + IoList2->ListSize - FIELD_OFFSET(IO_RESOURCE_REQUIREMENTS_LIST, List);
    newList = (PIO_RESOURCE_REQUIREMENTS_LIST) ExAllocatePool(
                          PagedPool,
                          size
                          );
    if (newList == NULL) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }
    p = (PUCHAR)newList;
    RtlCopyMemory(p, IoList1, IoList1->ListSize);
    p += IoList1->ListSize;
    RtlCopyMemory(p,
                  &IoList2->List[0],
                  size - IoList1->ListSize
                  );
    newList->ListSize = size;
    newList->AlternativeLists += IoList2->AlternativeLists;
    *MergedList = newList;
    return status;

}

NTSTATUS
IopMergeCmResourceLists (
    IN PCM_RESOURCE_LIST List1,
    IN PCM_RESOURCE_LIST List2,
    IN OUT PCM_RESOURCE_LIST *MergedList
    )

/*++

Routine Description:

    This routines merges two IoLists into one.


Arguments:

    IoList1 - supplies the pointer to the first CmResourceList

    IoList2 - supplies the pointer to the second CmResourceList

    MergedList - Supplies a variable to receive the merged resource
             list.

Return Value:

    A NTSTATUS code to indicate the result of the function.

--*/
{
    NTSTATUS status = STATUS_SUCCESS;
    PCM_RESOURCE_LIST cmList, newList;
    ULONG size, size1, size2;
    PUCHAR p;

    PAGED_CODE();

    *MergedList = NULL;

    //
    // First handle the easy cases that both IO Lists are empty or any one of
    // them is empty.
    //

    if ((List1 == NULL || List1->Count == 0) &&
        (List2 == NULL || List2->Count == 0)) {
        return status;
    }

    cmList = NULL;
    if (List1 == NULL || List1->Count == 0) {
        cmList = List2;
    } else if (List2 == NULL || List2->Count == 0) {
        cmList = List1;
    }
    if (cmList) {
        size =  IopDetermineResourceListSize(cmList);
        newList = (PCM_RESOURCE_LIST) ExAllocatePool(PagedPool, size);
        if (newList == NULL) {
            return STATUS_INSUFFICIENT_RESOURCES;
        }
        RtlCopyMemory(newList, cmList, size);
        *MergedList = newList;
        return status;
    }

    //
    // Do real work...
    //

    size1 =  IopDetermineResourceListSize(List1);
    size2 =  IopDetermineResourceListSize(List2);
    size = size1 + size2;
    newList = (PCM_RESOURCE_LIST) ExAllocatePool(
                          PagedPool,
                          size
                          );
    if (newList == NULL) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }
    p = (PUCHAR)newList;
    RtlCopyMemory(p, List1, size1);
    p += size1;
    RtlCopyMemory(p,
                  &List2->List[0],
                  size2 - FIELD_OFFSET(CM_RESOURCE_LIST, List)
                  );
    newList->Count = List1->Count + List2->Count;
    *MergedList = newList;
    return status;

}

BOOLEAN
IopIsLegacyDriver (
    IN PDRIVER_OBJECT DriverObject
    )

/*++

Routine Description:

    This routine checks if the driver object specifies a legacy driver.

Arguments:

    DriverObject - supplies a pointer to the driver object to be checked.

Return Value:

    BOOLEAN

--*/

{

    PAGED_CODE();

    //
    // If AddDevice entry is not empty it is a wdm driver
    //

    if (DriverObject->DriverExtension->AddDevice) {
        return FALSE;
    }

    //
    // Else if LEGACY flag is set in the driver object, it's a legacy driver.
    //

    if (DriverObject->Flags & DRVO_LEGACY_DRIVER) {
        return TRUE;
    } else {
        return FALSE;
    }
}

VOID
IopDeleteLegacyKey(
    IN PDRIVER_OBJECT DriverObject
    )

/*++

Routine Description:

    This routine checks if the Legacy= value of the driver's legacy_xxx key
    is one.  If yes, it deletes the Legacy key.

Parameters:

    DriverObject - supplies a pointer to the driver object.

Return Value:

    None.  If anything fails in this routine, the legacy key stays.

--*/

{
    WCHAR buffer[MAX_DEVICE_ID_LEN];
    NTSTATUS status;
    UNICODE_STRING deviceName, instanceName, unicodeName, *serviceName;
    ULONG length;
    HANDLE handle, handle1, handlex, enumHandle;
    ULONG legacy;
    PKEY_VALUE_FULL_INFORMATION keyValueInformation;
    PDEVICE_OBJECT deviceObject;
    PDEVICE_NODE deviceNode;
    BOOLEAN deletedPDO;

    serviceName = &DriverObject->DriverExtension->ServiceKeyName;

    PiLockPnpRegistry(FALSE);

    status = IopOpenRegistryKeyEx( &enumHandle,
                                   NULL,
                                   &CmRegistryMachineSystemCurrentControlSetEnumName,
                                   KEY_ALL_ACCESS
                                   );

    if (!NT_SUCCESS(status)) {
        goto exit;
    }

    status = PipGenerateMadeupNodeName( serviceName,
                                        &deviceName
                                        );
    if (!NT_SUCCESS(status)) {
        goto exit;
    }

    length = _snwprintf(buffer,
                        sizeof(buffer) / sizeof(WCHAR),
                        L"%s\\%s",
                        REGSTR_KEY_ROOTENUM,
                        deviceName.Buffer);

    RtlFreeUnicodeString(&deviceName);

    deviceName.MaximumLength = sizeof(buffer);
    ASSERT(length <= sizeof(buffer) - 10);
    deviceName.Length = (USHORT)(length * sizeof(WCHAR));
    deviceName.Buffer = buffer;

    RtlUpcaseUnicodeString(&deviceName, &deviceName, FALSE);

    status = IopOpenRegistryKeyEx( &handle1,
                                   enumHandle,
                                   &deviceName,
                                   KEY_ALL_ACCESS
                                   );

    if (NT_SUCCESS(status)) {

        deviceName.Buffer[deviceName.Length / sizeof(WCHAR)] =
                   OBJ_NAME_PATH_SEPARATOR;
        deviceName.Length += sizeof(WCHAR);
        PiUlongToInstanceKeyUnicodeString(
                                &instanceName,
                                buffer + deviceName.Length / sizeof(WCHAR),
                                sizeof(buffer) - deviceName.Length,
                                0
                                );
        deviceName.Length += instanceName.Length;

        //
        // deviceName is now the full InstancePath (ROOT\LEGACY_service\0000)
        // and instancePath points to the instance ID (0000)
        //
        status = IopOpenRegistryKeyEx( &handle,
                                       handle1,
                                       &instanceName,
                                       KEY_ALL_ACCESS
                                       );
        if (NT_SUCCESS(status)) {
            legacy = 1;
            status = IopGetRegistryValue (handle,
                                          REGSTR_VALUE_LEGACY,
                                          &keyValueInformation);
            if (NT_SUCCESS(status)) {
                if ((keyValueInformation->Type == REG_DWORD) &&
                    (keyValueInformation->DataLength >= sizeof(ULONG))) {
                    legacy = *(PULONG)KEY_VALUE_DATA(keyValueInformation);
                }
                ExFreePool(keyValueInformation);
            }
            if (legacy != 0) {

                //
                // We also want to delete the madeup device node
                //
                deletedPDO = FALSE;
                deviceObject = IopDeviceObjectFromDeviceInstance(&deviceName);
                if (deviceObject) {

                    PDEVICE_NODE devNodex, devNodey;

                    deviceNode = (PDEVICE_NODE)deviceObject->DeviceObjectExtension->DeviceNode;
                    if (deviceNode != NULL && (deviceNode->Flags & DNF_MADEUP)) {

                        //
                        // Now mark this one deleted.
                        //
                        if (!PipDoesDevNodeHaveProblem(deviceNode)) {

                            PipSetDevNodeState(deviceNode, DeviceNodeRemoved, NULL);
                            PipSetDevNodeProblem(deviceNode, CM_PROB_DEVICE_NOT_THERE);
                        }

                        //
                        // This is actually doing nothing because DeviceNode->ResourceList is NULL.
                        //

                        IopReleaseDeviceResources(deviceNode, FALSE);
                        devNodex = deviceNode;
                        while (devNodex) {
                            devNodey = devNodex;
                            devNodex = (PDEVICE_NODE)devNodey->OverUsed2.NextResourceDeviceNode;
                            devNodey->OverUsed2.NextResourceDeviceNode = NULL;
                            devNodey->OverUsed1.LegacyDeviceNode = NULL;
                        }

                        deviceNode->Flags &= ~DNF_MADEUP;  // remove its boot config if any
                        IoDeleteDevice(deviceObject);
                        deletedPDO = TRUE;
                    }
                    ObDereferenceObject(deviceObject);  // added via IopDeviceObjectFromDeviceInstance
                }

                PiWstrToUnicodeString(&unicodeName, REGSTR_KEY_CONTROL);
                status = IopOpenRegistryKeyEx( &handlex,
                                               handle,
                                               &unicodeName,
                                               KEY_ALL_ACCESS
                                               );
                if (NT_SUCCESS(status)) {
                    ZwDeleteKey(handlex);
                }
                PiWstrToUnicodeString(&unicodeName, REGSTR_KEY_LOG_CONF);
                status = IopOpenRegistryKeyEx( &handlex,
                                               handle,
                                               &unicodeName,
                                               KEY_ALL_ACCESS
                                               );
                if (NT_SUCCESS(status)) {
                    ZwDeleteKey(handlex);
                }

                ZwClose(enumHandle);

                //
                // We need to call IopCleanupDeviceRegistryValue even we are going to
                // delete it.  Because, it also cleans up related value names in other
                // keys.
                //
                if (deletedPDO) {

                    IopCleanupDeviceRegistryValues(&deviceName);
                }
                ZwDeleteKey(handle);
                ZwDeleteKey(handle1);
            } else {
                ZwClose(handle);
                ZwClose(handle1);
                ZwClose(enumHandle);
            }
        } else {
            ZwClose(handle1);
            ZwClose(enumHandle);
        }
    } else {
        ZwClose(enumHandle);
    }
exit:
    PiUnlockPnpRegistry();
    return;
}

NTSTATUS
IopQueryAndSaveDeviceNodeCapabilities (
    IN PDEVICE_NODE DeviceNode
    )

/*++

Routine Description:

    Called after start to refresh Capability flags

Arguments:

    DeviceObject - supplies a pointer to a device object whose registry
        values are to be updated.

Return Value:

    status

--*/

{
    NTSTATUS status;
    DEVICE_CAPABILITIES capabilities;

    PAGED_CODE();

    ASSERT(DeviceNode != NULL);

    //
    // Open the device instance key
    //

    status = PipQueryDeviceCapabilities(DeviceNode, &capabilities);
    if (!NT_SUCCESS(status)) {

        return status;
    }

    return IopSaveDeviceCapabilities(DeviceNode,&capabilities);
}

NTSTATUS
IopSaveDeviceCapabilities (
    IN PDEVICE_NODE DeviceNode,
    IN PDEVICE_CAPABILITIES Capabilities
    )

/*++

Routine Description:

    This routine updates device capabilities, must be called after a valid device instance key has been created
    Called directly from IopProcessNewDeviceNode, and indirecly via IopQueryAndSaveDeviceNodeCapabilities
    after device is started.

Arguments:

    DeviceObject - supplies a pointer to a device object whose registry
        values are to be updated.

Return Value:

    status

--*/

{
    NTSTATUS status;
    UNICODE_STRING unicodeName;
    ULONG value;
    HANDLE handle;

    PAGED_CODE();

    ASSERT(DeviceNode != NULL);
    ASSERT(Capabilities != NULL);

    //
    // Open the device instance key
    //
    status = IopDeviceObjectToDeviceInstance(DeviceNode->PhysicalDeviceObject, &handle, KEY_ALL_ACCESS);
    if (!NT_SUCCESS(status)) {

        return status;
    }

    if (DeviceNode->Flags & DNF_HAS_BOOT_CONFIG) {
        Capabilities->SurpriseRemovalOK = 0;
    }
    //
    // Assert the bit fields are completely contained in a ULONG. This is a
    // public structure, so it shouldn't ever change, but paranoia is a good
    // thing...
    //
    ASSERT((FIELD_OFFSET(DEVICE_CAPABILITIES, Address) -
            FIELD_OFFSET(DEVICE_CAPABILITIES, Version) -
            FIELD_SIZE  (DEVICE_CAPABILITIES, Version)) == sizeof(ULONG));

    DeviceNode->CapabilityFlags =
        *((PULONG) (((PUCHAR) Capabilities) +
        FIELD_OFFSET(DEVICE_CAPABILITIES, Version) +
        FIELD_SIZE(DEVICE_CAPABILITIES, Version)));

    value =    (Capabilities->LockSupported)          |
               (Capabilities->EjectSupported    << 1) |
               (Capabilities->WarmEjectSupported<< 1) |
               (Capabilities->Removable         << 2) |
               (Capabilities->DockDevice        << 3) |
               (Capabilities->UniqueID          << 4) |
               (Capabilities->SilentInstall     << 5) |
               (Capabilities->RawDeviceOK       << 6) |
               (Capabilities->SurpriseRemovalOK << 7) |
               (Capabilities->HardwareDisabled  << 8) |
               (Capabilities->NonDynamic        << 9);

    PiWstrToUnicodeString(&unicodeName, REGSTR_VALUE_CAPABILITIES);
    status = ZwSetValueKey(
                  handle,
                  &unicodeName,
                  TITLE_INDEX_VALUE,
                  REG_DWORD,
                  &value,
                  sizeof(value)
                  );

    PiWstrToUnicodeString(&unicodeName, REGSTR_VALUE_UI_NUMBER);
    value = Capabilities->UINumber;
    if(value != (ULONG)-1) {

        ZwSetValueKey(handle,
                      &unicodeName,
                      TITLE_INDEX_VALUE,
                      REG_DWORD,
                      &value,
                      sizeof(value)
                      );
    } else {

        ZwDeleteValueKey(handle, &unicodeName);
    }

    ZwClose(handle);

    return STATUS_SUCCESS;
}

NTSTATUS
IopRestartDeviceNode(
    IN PDEVICE_NODE DeviceNode
    )
{
    PAGED_CODE();

    PpDevNodeLockTree(PPL_TREEOP_BLOCK_READS_FROM_ALLOW);

    ASSERT(DeviceNode->State == DeviceNodeRemoved ||
           DeviceNode->State == DeviceNodeInitialized );

    ASSERT(!PipDoesDevNodeHaveProblem(DeviceNode));

    ASSERT(DeviceNode->Flags & DNF_ENUMERATED);

    if (!(DeviceNode->Flags & DNF_ENUMERATED)) {

        PpDevNodeUnlockTree(PPL_TREEOP_BLOCK_READS_FROM_ALLOW);
        return STATUS_UNSUCCESSFUL;
    }

    DeviceNode->UserFlags &= ~DNUF_NEED_RESTART;
    DeviceNode->Flags &= ~(DNF_DRIVER_BLOCKED | DNF_HARDWARE_VERIFICATION);

#if DBG_SCOPE
    DeviceNode->FailureStatus = 0;
    if (DeviceNode->PreviousResourceList) {
        ExFreePool(DeviceNode->PreviousResourceList);
        DeviceNode->PreviousResourceList = NULL;
    }
    if (DeviceNode->PreviousResourceRequirements) {
        ExFreePool(DeviceNode->PreviousResourceRequirements);
        DeviceNode->PreviousResourceRequirements = NULL;
    }
#endif

    //
    //     Prepare to set the device state back to DeviceNodeUninitialized. To
    // do this we free any existing devnode strings so we can recreate them
    // during enumeration.
    //
    // ADRIAO N.B. 8/19/2000 -
    //     We don't restore the state to DeviceNodeInitialized to maintain Win2K
    // behavior. We have no idea if anyone actually depends on this. In theory
    // this would let a bus driver get away with changing a child's IDs after a
    // remove.
    //

    if (DeviceNode->State != DeviceNodeUninitialized) {

        DeviceNode->Flags &= ~(DNF_NO_RESOURCE_REQUIRED |
                               DNF_RESOURCE_REQUIREMENTS_CHANGED);

        if (DeviceNode->ServiceName.Length != 0) {
            ExFreePool(DeviceNode->ServiceName.Buffer);
            PiWstrToUnicodeString(&DeviceNode->ServiceName, NULL);
        }

        if (DeviceNode->ResourceRequirements != NULL) {
            ExFreePool(DeviceNode->ResourceRequirements);
            DeviceNode->ResourceRequirements = NULL;
            DeviceNode->Flags &= ~DNF_RESOURCE_REQUIREMENTS_NEED_FILTERED;
        }
    }

    ASSERT(DeviceNode->ServiceName.Length == 0 &&
           DeviceNode->ServiceName.MaximumLength == 0 &&
           DeviceNode->ServiceName.Buffer == NULL);

    ASSERT(!(DeviceNode->Flags &
           ~(DNF_MADEUP | DNF_ENUMERATED | DNF_HAS_BOOT_CONFIG | DNF_IDS_QUERIED |
             DNF_BOOT_CONFIG_RESERVED | DNF_NO_RESOURCE_REQUIRED)));

    PipSetDevNodeState(DeviceNode, DeviceNodeUninitialized, NULL);

    PpDevNodeUnlockTree(PPL_TREEOP_BLOCK_READS_FROM_ALLOW);

    return STATUS_SUCCESS;
}

BOOLEAN
IopDeleteKeyRecursiveCallback(
    IN HANDLE KeyHandle,
    IN PUNICODE_STRING KeyName,
    IN OUT PVOID Context
    )
/*++

Routine Description:

    This is a callback routine to PipApplyFunctionToSubKeys, that gets called
    through IopDeleteKeyRecursive.  This routine prepares a given key for
    deletion by deleting all of its subkeys.  This is done, using
    PipApplyFunctionToSubKeys, with instructions to delete all enumerated
    subkeys, and calling this routine as a callback routine, if necessary, until
    no subkeys remain.  KeyHandle can then be successfully deleted by the
    caller.

Arguments:

    KeyHandle - Handle to a subkey that has been enumerated by
        PipApplyFunctionToSubKeys.

    KeyName - Name of the subkey whose handle is specified by KeyHandle.

    Context - Supplies a pointer to user-defined data that will be passed
        in to the callback routine at each subkey invocation.

Return Value:

    BOOLEAN that returns whether or not the given key can be safely deleted.

--*/
{
    NTSTATUS status = STATUS_SUCCESS;

    PAGED_CODE();

    UNREFERENCED_PARAMETER(KeyName);

    //
    // delete any subkeys, recursively if necessary
    //
    status = PipApplyFunctionToSubKeys(
        KeyHandle,
        NULL,
        KEY_ALL_ACCESS,
        FUNCTIONSUBKEY_FLAG_IGNORE_NON_CRITICAL_ERRORS |
        FUNCTIONSUBKEY_FLAG_DELETE_SUBKEYS,
        IopDeleteKeyRecursiveCallback,
        Context);

    *((NTSTATUS *)Context) = status;
    return (BOOLEAN)NT_SUCCESS(status);
}

NTSTATUS
IopDeleteKeyRecursive(
    IN HANDLE ParentKey OPTIONAL,
    IN PWCHAR KeyName
    )
/*++

Routine Description:

    Recursively deletes all subkeys of KeyName, then deletes KeyName.

Arguments:

    ParentKey - Handle to the parent key of KeyName.  If NULL then KeyName is
        expected to start with \Registry.

    KeyName - Name of subkey to delete, as a NULL terminated UNICODE string.

Return Value:

    STATUS_SUCCESS if no errors, otherwise the appropriate error.

--*/
{
    NTSTATUS       status = STATUS_SUCCESS;
    BOOLEAN        result;
    HANDLE         hKey;
    UNICODE_STRING unicodeKeyName;

    PAGED_CODE();

    //
    // Attempt to open the key name we were given
    //
    RtlInitUnicodeString(&unicodeKeyName, KeyName);
    status = IopOpenRegistryKeyEx( &hKey,
                                   ParentKey,
                                   &unicodeKeyName,
                                   KEY_ALL_ACCESS
                                   );
    if (NT_SUCCESS(status)) {
        //
        // Recusively delete all subkeys
        //
        result = IopDeleteKeyRecursiveCallback(hKey,
                                               &unicodeKeyName,
                                               (PVOID)&status);
        if (result) {
            //
            // It is safe to delete this key
            //
            status = ZwDeleteKey(hKey);
        }
        ZwClose(hKey);
    }

    return status;
}

BOOLEAN
PiRegSzToString(
    IN  PWCHAR RegSzData,
    IN  ULONG  RegSzLength,
    OUT PULONG StringLength  OPTIONAL,
    OUT PWSTR  *CopiedString OPTIONAL
    )

/*++

Routine Description:

    This routine takes as input a REG_SZ data buffer (as returned in the DataOffset area
    of the buffer in a KEY_VALUE_FULL_INFORMATION structure), as well as the length
    of the buffer, in bytes (as specified by the DataLength field in the above mentioned
    struct).  It optionally returns the length of the contained string (in bytes), not
    including the terminating NULL, as well as an optional copy of the string itself
    (properly NULL-terminated).

    It is the responsibility of the caller to free the (PagedPool) buffer allocated
    for the string copy.

Arguments:

    RegSzData - Supplies a pointer to the REG_SZ data buffer.

    RegSzLength - Supplies the length of the RegSzData buffer, in bytes.

    StringLength - Optionally supplies a pointer to a variable that will receive
                   the length, in bytes, of the string (excluding terminating NULL).

    CopiedString - Optionally supplies a pointer to a wide character pointer that
                   will recieve a (properly NULL-terminated) copy of the specified
                   string.  If this paramater is NULL, no copy will be made.

Return Value:

    If success, returns TRUE

    If failure (not able to allocate memory for string copy), returns FALSE

--*/

{
    PWCHAR curPos, endOfRegSzData;
    ULONG actualStringLength;

    //
    // Since we're converting a byte count to a wide-character count (and the
    // compiler is converting it back when adding it to a PWCHAR), we are
    // ensuring that endOfRegSzData is not on an odd-byte boundary, even if
    // the RegSzLength passed in was odd.  This takes care of the case where
    // the REG_SZ buffer retrieved from the registry is bogus (e.g., you have
    // a 5-byte buffer, the 1st unicode character of which is a UNICODE_NULL).
    //
    endOfRegSzData = (curPos = RegSzData) + CB_TO_CWC(RegSzLength);

    while ((curPos < endOfRegSzData) && *curPos) {
        curPos++;
    }

    actualStringLength = (ULONG)((PUCHAR)curPos - (PUCHAR)RegSzData);

    if (ARGUMENT_PRESENT(StringLength)) {
        *StringLength = (ULONG)((PUCHAR)curPos - (PUCHAR)RegSzData);
    }

    if (ARGUMENT_PRESENT(CopiedString)) {
        //
        // Allocate memory for the string (+ terminating NULL)
        //
        *CopiedString = (PWSTR)ExAllocatePool(PagedPool, actualStringLength + sizeof(UNICODE_NULL));
        if (*CopiedString == NULL) {
            return FALSE;
        }

        //
        // Copy the string and NULL-terminate it.
        //
        if (actualStringLength) {
            RtlCopyMemory(*CopiedString, RegSzData, actualStringLength);
        }

        *(PWCHAR)((PUCHAR)(*CopiedString) + actualStringLength) = UNICODE_NULL;
    }

    return TRUE;
}

ULONG
IopDebugPrint (
    IN ULONG    Level,
    IN PCHAR    Format,
    ...
    )
{
    va_list ap;

    va_start(ap, Format);

    vDbgPrintExWithPrefix("", DPFLTR_NTOSPNP_ID, Level, Format, ap);

    va_end(ap);

    return Level;
}


PCHAR
PpConvertMultiSzWstrToStr(
    IN PWCHAR Source,
    IN ULONG Length
    )
{
    NTSTATUS status;
    PWCHAR  s, sourceEnd;
    PCHAR   dest, d;
    ANSI_STRING ansiString;
    UNICODE_STRING unicodeString;

    PAGED_CODE();

    if (Length <= 2) {

        return NULL;
    }

#if DBG
    for (s = Source; *s != UNICODE_NULL; s += wcslen(s) + 1) {
    }
    ASSERT(Length == (ULONG)(s - Source) + 1);
#endif

    dest = ExAllocatePool(PagedPool, Length * sizeof(CHAR));
    if (dest) {

        s = Source;
        for (sourceEnd = s + Length, d = dest;
             s < sourceEnd && *s != UNICODE_NULL;
             s += wcslen(s) + 1) {

            RtlInitUnicodeString(&unicodeString, s);
            status = RtlUnicodeStringToAnsiString(&ansiString, &unicodeString, TRUE);
            if (!NT_SUCCESS(status)) {

                ExFreePool(dest);
                return NULL;
            }
            RtlCopyMemory(d, ansiString.Buffer, ansiString.Length + 1);
            d += ansiString.Length + 1;
            RtlFreeAnsiString(&ansiString);
        }
        if (s < sourceEnd) {

            *d = '\0';
        }
    }

    return dest;
}


PWCHAR
PpConvertMultiSzStrToWstr(
    IN PCHAR Source,
    IN ULONG Length
    )
{
    NTSTATUS status;
    PCHAR s, sourceEnd;
    PWCHAR dest, d;
    ANSI_STRING ansiString;
    UNICODE_STRING unicodeString;

    PAGED_CODE();

    if (Length <= 2) {

        return NULL;
    }

#if DBG
    for (s = Source; *s != '\0'; s += strlen(s) + 1) {
    }
    ASSERT(Length == (ULONG)(s - Source) + 1);
#endif

    dest = ExAllocatePool(PagedPool, Length * sizeof(WCHAR));
    if (dest) {

        s = Source;
        for (sourceEnd = s + Length, d = dest;
             s < sourceEnd && *s != '\0';
             s += strlen(s) + 1) {

            RtlInitAnsiString(&ansiString, s);
            status = RtlAnsiStringToUnicodeString(&unicodeString, &ansiString, TRUE);
            if (!NT_SUCCESS(status)) {

                ExFreePool(dest);
                return NULL;
            }
            RtlCopyMemory(d, unicodeString.Buffer, unicodeString.Length + sizeof(UNICODE_NULL));
            d += (unicodeString.Length + sizeof(UNICODE_NULL)) / sizeof(WCHAR);
            RtlFreeUnicodeString(&unicodeString);
        }
        if (s < sourceEnd) {

            *d = UNICODE_NULL;
        }
    }

    return dest;
}


VOID
PpSystemHiveLimitCallback(
    PSYSTEM_HIVE_LIMITS HiveLimits,
    ULONG Level
    )
{
    PAGED_CODE();

    if (Level >= HiveLimits->High) {

        PpSystemHiveTooLarge = TRUE;

    } else {

        ASSERT(Level <= HiveLimits->Low);
        PpSystemHiveTooLarge = FALSE;
        PpResetProblemDevices(IopRootDeviceNode, CM_PROB_REGISTRY_TOO_LARGE);
        PipRequestDeviceAction(
            IopRootDeviceNode->PhysicalDeviceObject,
            RestartEnumeration,
            FALSE,
            0,
            NULL,
            NULL
            );

    }
}

NTSTATUS
PpBusTypeGuidInitialize(
    VOID
    )

/*++

Routine Description:

    This routine opens the specified subkey.

Arguments:

    None
    
Return Value:

    STATUS_SUCCESS.

--*/

{
    PAGED_CODE();

    PpBusTypeGuidCountMax = 16;
    PpBusTypeGuidArray = ExAllocatePool(PagedPool, sizeof(GUID) * PpBusTypeGuidCountMax);
    if (PpBusTypeGuidArray == NULL) {

        PpBusTypeGuidCountMax = 0;
        return STATUS_INSUFFICIENT_RESOURCES;
    }
    PpBusTypeGuidCount = 0;

    ExInitializeFastMutex(&PpBusTypeGuidLock);

    return STATUS_SUCCESS;
}

USHORT
PpBusTypeGuidGetIndex(
    IN LPGUID BusTypeGuid
    )

/*++

Routine Description:

    This routine looks up the BusTypeGuid and returns its index into the table.

Arguments:
    
    BusTypeGuid - GUID to lookup.
    
Return Value:

    Index into the table iff successful, else 0xFFFF.

--*/

{
    LPGUID p;
    ULONG i;

    PAGED_CODE();

    ExAcquireFastMutex(&PpBusTypeGuidLock);
    //
    // First look it up.
    //
    for (i = 0; i < PpBusTypeGuidCount; i++) {

        if (IopCompareGuid(BusTypeGuid, &PpBusTypeGuidArray[i])) {

            break;
        }
    }
    //
    // If the GUID is not in the table, add it.
    //
    if (i == PpBusTypeGuidCount) {
        //
        // Grow the table if needed.
        // 
        if (i == PpBusTypeGuidCountMax) {
            //  
            // We grow the table one entry at a time. This should not be a 
            // problem since this should not happen often.
            //
            p  = ExAllocatePool(PagedPool, (i + 1) * sizeof(GUID));
            if (p) {
                //
                // Copy the old table.
                //
                RtlCopyMemory(p, PpBusTypeGuidArray, PpBusTypeGuidCount * sizeof(GUID));
                //
                // Update global data.
                //
                PpBusTypeGuidCountMax++;
                if (PpBusTypeGuidArray) {

                    ExFreePool(PpBusTypeGuidArray);
                }
                PpBusTypeGuidArray = p;

            } else {
                //
                // Return invalid index on failure.
                //
                i = (ULONG)-1;
            }
        }
        //
        // Copy the new entry on success.
        //
        if (i != (ULONG)-1) {
            //
            // Copy the new entry.
            //
            RtlCopyMemory(&PpBusTypeGuidArray[PpBusTypeGuidCount], BusTypeGuid, sizeof(GUID));
            //
            // Update global data.
            //
            PpBusTypeGuidCount++;
        }
    }

    ExReleaseFastMutex(&PpBusTypeGuidLock);

    return (USHORT)i;
}

NTSTATUS
PpBusTypeGuidGet(
    IN USHORT Index,
    IN OUT LPGUID BusTypeGuid
    )

/*++

Routine Description:

    This routine return the BusTypeGuid in the table at the specified index.

Arguments:
    
    Index - BusTypeGuid index.
    
    BusTypeGuid - Recieves the GUID.
    
Return Value:

    NTSTATUS.

--*/

{
    NTSTATUS status;

    PAGED_CODE();

    ExAcquireFastMutex(&PpBusTypeGuidLock);

    if (Index < PpBusTypeGuidCount) {

        RtlCopyMemory(BusTypeGuid, &PpBusTypeGuidArray[Index], sizeof(GUID));
        status = STATUS_SUCCESS;
    } else {

        status = STATUS_OBJECT_NAME_NOT_FOUND;
    }

    ExReleaseFastMutex(&PpBusTypeGuidLock);

    return status;
}

ULONG
PiFixupID(
    IN PWCHAR ID,
    IN ULONG MaxIDLength,
    IN BOOLEAN Multi,
    IN ULONG AllowedSeparators,
    IN PUNICODE_STRING LogString OPTIONAL
    )

/*++

Routine Description:

    This routine parses the device instance string and replaces any invalid
    characters (not allowed in a "device instance") with an underscore
    character.

    Invalid characters are:
        c <= 0x20 (' ')
        c >  0x7F
        c == 0x2C (',')

Arguments:

    ID - ID to be fixed up.
    
    MaxIDLength - Maximum allowed size of ID.
    
    Multi - Specifies if the ID is MULTI_SZ or not.
    
    AllowedSeparators - Number of separators allowed in the ID.

Return Value:

    ID length in number of characters.

--*/

{
    PWCHAR p, pMax, lastNull;
    ULONG separators;
    UNICODE_STRING reason;

    PAGED_CODE();

    //
    // BUGBUG - do we need to uppercase these!?
    //
    separators = 0;
    lastNull = NULL;
    for(p = ID, pMax = p + MaxIDLength; p < pMax; p++) {

        if(*p == UNICODE_NULL) {

            if(Multi == FALSE || (lastNull && p == lastNull + 1)) {

                break;
            }
            pMax += MaxIDLength;
            lastNull = p;
            continue;
        }
        if (*p == L' ') {

            *p = L'_';
        } else if ((*p < L' ')  || (*p > (WCHAR)0x7F) || (*p == L',')) {

            IopDbgPrint((IOP_ENUMERATION_ERROR_LEVEL,
                         "PiFixupID: ID at %p has invalid character %02X\n",
                         ID,
                         *p));

            if(LogString) {

                PiWstrToUnicodeString(&reason, L"invalid character");
                PpLogEvent(LogString, &reason, STATUS_PNP_INVALID_ID, p, sizeof(WCHAR));
            }

            return 0;
        } else if ((*p == OBJ_NAME_PATH_SEPARATOR && ++separators > AllowedSeparators)) {

            IopDbgPrint((IOP_ENUMERATION_ERROR_LEVEL,
                         "PiFixupID: ID at %p has too many (%d) separators\n",
                         ID,
                         separators));
            if(LogString) {

                PiWstrToUnicodeString(&reason, L"too many separators");
                PpLogEvent(LogString, &reason, STATUS_PNP_INVALID_ID, &separators, sizeof(ULONG));
            }
            return 0;
        }
    }
    if(p >= pMax || (AllowedSeparators != (ULONG)-1 && separators != AllowedSeparators)) {

        IopDbgPrint((IOP_ENUMERATION_ERROR_LEVEL,
                     "PiFixupID: ID at %p not terminated, or too long or has invalid number (%d) of separators\n",
                     ID,
                     separators));
        if(LogString) {

            PiWstrToUnicodeString(&reason, L"not terminated, too long or invalid number of separators");
            PpLogEvent(LogString, &reason, STATUS_PNP_INVALID_ID, NULL, 0);
        }
        return 0;
    }

    return (ULONG)(ULONG_PTR)(p - ID) + 1;
}

NTSTATUS
PpQueryDeviceID(
    IN PDEVICE_NODE DeviceNode,
    OUT PWCHAR *BusID,
    OUT PWCHAR *DeviceID
    )

/*++

Routine Description:

    This routine queries the Device ID and fixes it up. It also parses the DeviceID 
    and returns the pointers to BusID and DeviceID parts. If this routine fails, 
    BusID and DeviceID will be set to NULL.

Arguments:

    DeviceNode - The devnode whose DeviceID needs to be queried.
    
    BusID - Recieves the pointer to the bus part of DeviceID.

    DeviceID - Recieves the pointer to the device part of DeviceID.
    
Return Value:

    NTSTATUS.

--*/

{
    NTSTATUS status;
    PWCHAR id, separator;
    ULONG idLength;

    PAGED_CODE();

    *BusID = NULL;
    *DeviceID= NULL;

    status = PpQueryID(DeviceNode, BusQueryDeviceID, &id, &idLength);
    if(NT_SUCCESS(status)) {

        ASSERT(id && idLength);

        *BusID = id;
        separator = wcschr(id, OBJ_NAME_PATH_SEPARATOR);

        ASSERT(separator);

        *separator = UNICODE_NULL;
        *DeviceID = separator + 1;

    } else {

        ASSERT(id == NULL && idLength == 0);
    }

    return status;
}

NTSTATUS
PpQueryID(
    IN PDEVICE_NODE DeviceNode,
    IN BUS_QUERY_ID_TYPE IDType,
    OUT PWCHAR *ID,
    OUT PULONG IDLength
    )

/*++

Routine Description:

    This routine queries the specified ID and fixes it up. If this 
    routine fails, ID will be set to NULL.

Arguments:

    DeviceNode - The devnode whose IDs need to be queried.
    
    IDType - Type of ID to be queried.

    ID - Receives the ID returned by the driver if any. The caller 
    is expected to free the storage for ID on success.
    
    IDLength - Receives the length of the ID (including terminating NULL) in bytes.

Return Value:

    NTSTATUS.

--*/

{
    NTSTATUS status;
    UNICODE_STRING reason;

    PAGED_CODE();

    ASSERT(IDType == BusQueryDeviceID || IDType == BusQueryInstanceID || 
          IDType == BusQueryHardwareIDs || IDType == BusQueryCompatibleIDs);

    *IDLength = 0;
    status = PpIrpQueryID(DeviceNode->PhysicalDeviceObject, IDType, ID);
    if(NT_SUCCESS(status)) {

        switch(IDType) {

            case BusQueryDeviceID:

                *IDLength = PiFixupID(*ID, MAX_DEVICE_ID_LEN, FALSE, 1, &DeviceNode->Parent->ServiceName);
                break;

            case BusQueryInstanceID:

                *IDLength = PiFixupID(*ID, MAX_DEVICE_ID_LEN, FALSE, 0, &DeviceNode->Parent->ServiceName);
                break;

            case BusQueryHardwareIDs:
            case BusQueryCompatibleIDs:

                *IDLength = PiFixupID(*ID, MAX_DEVICE_ID_LEN, TRUE, (ULONG)-1, &DeviceNode->Parent->ServiceName);
                break;

            default:

                *IDLength = 0;
                break;
        }
        (*IDLength) *= sizeof(WCHAR);
        if(*IDLength == 0) {

            status = STATUS_PNP_INVALID_ID;
        }
    }

    if(!NT_SUCCESS(status)) {

        if (status == STATUS_PNP_INVALID_ID || IDType == BusQueryDeviceID) {

            PipSetDevNodeProblem(DeviceNode, CM_PROB_INVALID_DATA);
            if ((DeviceNode->Parent->Flags & DNF_CHILD_WITH_INVALID_ID) == 0) {

                DeviceNode->Parent->Flags |= DNF_CHILD_WITH_INVALID_ID;
                PpSetInvalidIDEvent(&DeviceNode->Parent->InstancePath);
            }
        }
        if (status == STATUS_PNP_INVALID_ID) {

            IopDbgPrint((IOP_ENUMERATION_ERROR_LEVEL,
                         "PpQueryID: Bogus ID returned by %wZ\n",
                         &DeviceNode->Parent->ServiceName));
            ASSERT(status != STATUS_PNP_INVALID_ID);

        } else if (IDType == BusQueryDeviceID) {
            //
            // DeviceID is not optional.
            //
            PiWstrToUnicodeString(&reason, L"failed IRP_MN_QUERY_ID-BusQueryDeviceID");
            PpLogEvent(&DeviceNode->Parent->ServiceName, &reason, status, NULL, 0);

            IopDbgPrint((IOP_ENUMERATION_ERROR_LEVEL,
                         "PpIrpQueryID: Failed by %wZ, status = %x\n",
                         &DeviceNode->Parent->ServiceName, status));
            ASSERT(IDType != BusQueryDeviceID);
        }

        if(*ID) {

            ExFreePool(*ID);
            *ID = NULL;
            *IDLength = 0;
        }
    }

    return status;
}

