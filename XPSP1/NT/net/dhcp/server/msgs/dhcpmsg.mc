;/*++ BUILD Version: 0001    // Increment this if a change has global effects
;
;Copyright (c) 1994  Microsoft Corporation
;
;Module Name:
;
;    dhcpmsg.h
;
;Abstract:
;
;    Definitions for DHCP messages.
;
;Author:
;
;    Madan Appiah (madana)  10-Sep-1993
;    Manny Weiser 8-Dec-1992
;
;Revision History:
;
;Notes:
;
;    This file is generated by the MC tool from the dhcpmsg.mc file.
;
;    ?? NOTE : These message should go into winerror.h, until then use a
;    error code range which is not used by the system.
;
;--*/
;
;#ifndef _DHCPMESSAGE_
;#define _DHCPMESSAGE_
;

;//
;//  20000 - 20099   Reserved for DHCP service specific errors.
;//

MessageId=20000 SymbolicName=ERROR_DHCP_REGISTRY_INIT_FAILED
Language=English
The DHCP server initialization parameters are incorrect.
.

MessageId=20001 SymbolicName=ERROR_DHCP_DATABASE_INIT_FAILED
Language=English
The DHCP server was unable to open the DHCP client database.
.

MessageId=20002 SymbolicName=ERROR_DHCP_RPC_INIT_FAILED
Language=English
The DHCP server was unable to start as an RPC server.
.

MessageId=20003 SymbolicName=ERROR_DHCP_NETWORK_INIT_FAILED
Language=English
The DHCP server was unable to establish a socket connection.
.

MessageId=20004 SymbolicName=ERROR_DHCP_SUBNET_EXITS
Language=English
The specified subnet already exists.
.

MessageId=20005 SymbolicName=ERROR_DHCP_SUBNET_NOT_PRESENT
Language=English
The specified subnet does not exist.
.

MessageId=20006 SymbolicName=ERROR_DHCP_PRIMARY_NOT_FOUND
Language=English
The primary host information for the specified subnet was not found.
.

MessageId=20007 SymbolicName=ERROR_DHCP_ELEMENT_CANT_REMOVE
Language=English
The specified DHCP element cannot be removed because it has been used.
.

MessageId=20009 SymbolicName=ERROR_DHCP_OPTION_EXITS
Language=English
The specified option already exists.
.

MessageId=20010 SymbolicName=ERROR_DHCP_OPTION_NOT_PRESENT;
Language=English
The specified option does not exist.
.

MessageId=20011 SymbolicName=ERROR_DHCP_ADDRESS_NOT_AVAILABLE;
Language=English
The specified address is not available.
.

MessageId=20012 SymbolicName=ERROR_DHCP_RANGE_FULL;
Language=English
The specified IP address range is full.
.

MessageId=20013 SymbolicName=ERROR_DHCP_JET_ERROR;
Language=English
An error occurred while accessing the DHCP database. Look at the
DHCP server event log for more information on this error.
.

MessageId=20014 SymbolicName=ERROR_DHCP_CLIENT_EXISTS;
Language=English
The specified client already exists in the database.
.

MessageId=20015 SymbolicName=ERROR_DHCP_INVALID_DHCP_MESSAGE;
Language=English
The DHCP server received a message that is not valid.
.

MessageId=20016 SymbolicName=ERROR_DHCP_INVALID_DHCP_CLIENT;
Language=English
The DHCP server received a message from a client that is not valid.
.

MessageId=20017 SymbolicName=ERROR_DHCP_SERVICE_PAUSED;
Language=English
The DHCP server service is paused.
.

MessageId=20018 SymbolicName=ERROR_DHCP_NOT_RESERVED_CLIENT;
Language=English
The specified DHCP client is not a reserved client.
.

MessageId=20019 SymbolicName=ERROR_DHCP_RESERVED_CLIENT;
Language=English
The specified DHCP client is a reserved client.
.

MessageId=20020 SymbolicName=ERROR_DHCP_RANGE_TOO_SMALL;
Language=English
The specified IP address range is too small.
.

MessageId=20021 SymbolicName=ERROR_DHCP_IPRANGE_EXITS;
Language=English
The specified IP address range already exists.
.

MessageId=20022 SymbolicName=ERROR_DHCP_RESERVEDIP_EXITS;
Language=English
The specified IP address or hardware address is being used by another client.
.

MessageId=20023 SymbolicName=ERROR_DHCP_INVALID_RANGE;
Language=English
The specified range either overlaps an existing range or is
not valid.
.

MessageId=20024 SymbolicName=ERROR_DHCP_RANGE_EXTENDED;
Language=English
The specified range is an extension of an existing range.
.

MessageId=20025 SymbolicName=ERROR_EXTEND_TOO_SMALL;
Language=English
The range extension specified is too small.  The range must be
extended by multiples of 32 addresses.
.

MessageId=20026 SymbolicName=WARNING_EXTENDED_LESS;
Language=English
The range was extended less than the specified backward extension.
Extend the range by multiples of 32 addresses.
.

MessageId=20027 SymbolicName=ERROR_DHCP_JET_CONV_REQUIRED;
Language=English
DHCP database needs to be upgraded to a newer format.  Look at the
DHCP server event log for more information.
.

MessageId=20028 SymbolicName=ERROR_SERVER_INVALID_BOOT_FILE_TABLE
Language=English
The format of the bootstrap protocol file table is incorrect.  
The correct format is:
<requested boot file name1>,<boot file server name1>,<boot file name1>
<requested boot file name2>,<boot file server name2>,<boot file name2>
etc...
.

MessageId=20029 SymbolicName=ERROR_SERVER_UNKNOWN_BOOT_FILE_NAME
Language=English
.

MessageId=20030 SymbolicName=ERROR_DHCP_SUPER_SCOPE_NAME_TOO_LONG;
Language=English
The specified superscope name is too long.
.

MessageId=20032 SymbolicName=ERROR_DHCP_IP_ADDRESS_IN_USE;
Language=English
This IP address is already in use.
.

MessageId=20033 SymbolicName=ERROR_DHCP_LOG_FILE_PATH_TOO_LONG
Language=English
The DHCP audit-log file path is too long.
.

MessageId=20034 SymbolicName=ERROR_DHCP_UNSUPPORTED_CLIENT
Language=English
The DHCP service received a request for a valid IP address 
that is not administered by this server.
.

MessageId=20035 SymbolicName=EVENT_SERVER_INTERFACE_NOTIFICATION
Language=English
The DHCP Server failed to receive a notification of interface 
list changes.  Some of the interfaces will not be enabled in
the DHCP service.
.

MessageId=20036 SymbolicName=ERROR_DHCP_JET97_CONV_REQUIRED;
Language=English
The DHCP database needs to be upgraded to the current Jet format.  
Look at the DHCP service event log for more information.
.

MessageId=20037 SymbolicName=ERROR_DHCP_ROGUE_INIT_FAILED
Language=English
The DHCP Server is not servicing any clients on the network 
because it could not determine if it is authorized to run.  
This might be due to network problems or insufficient resources.
.

MessageId=20038 SymbolicName=ERROR_DHCP_ROGUE_SAMSHUTDOWN
Language=English
The DHCP service is shutting down because another DHCP server 
with the IP address %1 is active on the network.
.

MessageId=20039 SymbolicName=ERROR_DHCP_ROGUE_NOT_AUTHORIZED
Language=English
The DHCP service is not servicing any clients on the network 
because it has determined that it is not authorized to run.
.

MessageId=20040 SymbolicName=ERROR_DHCP_ROGUE_DS_UNREACHABLE
Language=English
The DHCP service is unable to contact the directory service 
for domain %1.  The DHCP service will continue to attempt 
to contact the directory service.  During this time, no clients 
on the network will be serviced.
.

MessageId=20041 SymbolicName=ERROR_DHCP_ROGUE_DS_CONFLICT
Language=English
The DHCP service is not servicing any clients on the network 
because its authorization information conflicts with another 
DHCP server whose IP address is %1 and is active on domain %2.
.

MessageId=20042 SymbolicName=ERROR_DHCP_ROGUE_NOT_OUR_ENTERPRISE
Language=English
The DHCP service is ignoring a request from another DHCP 
service because it is on a different directory service 
enterprise (Directory Service Enterprise root = %1)
.

MessageId=20043 SymbolicName=ERROR_DHCP_ROGUE_STANDALONE_IN_DS
Language=English
The DHCP service has detected a directory service environment
on the network.  If there is directory service on the network, 
DHCP service can only run on a server which is part of the directory
service.  Since this server belongs to a workgroup, the DHCP service 
is terminating.
.

MessageId=20044 SymbolicName=ERROR_DHCP_CLASS_NOT_FOUND
Language=English
The class name being used is unknown or incorrect.
.

MessageId=20045 SymbolicName=ERROR_DHCP_CLASS_ALREADY_EXISTS
Language=English
The class name is already in use or the class information 
is already in use.
.

MessageId=20046 SymbolicName=ERROR_DHCP_SCOPE_NAME_TOO_LONG
Language=English
The specified scope name is too long.  The name is limited to
a maximum of 256 characters.
.

MessageId=20047 SymbolicName=ERROR_DHCP_DEFAULT_SCOPE_EXITS
Language=English
There is already a default scope configured on the server.
.

MessageId=20048 SymbolicName=ERROR_DHCP_CANT_CHANGE_ATTRIBUTE
Language=English
The Dynamic BOOTP attribute cannot be turned on or off.
.

MessageId=20049 SymbolicName=ERROR_DHCP_IPRANGE_CONV_ILLEGAL
Language=English
Conversion of a scope to a DHCP Only scope or to a BOOTP Only scope 
is not allowed when DHCP and BOOTP clients both exist in the scope.
Manually delete either the DHCP or the BOOTP clients from the scope,
as appropriate for the type of scope being created.
.

MessageId=20050 SymbolicName=ERROR_DHCP_NETWORK_CHANGED
Language=English
The network has changed.  Retry this operation after checking for the 
network changes.  Network changes may be caused by interfaces that are 
new or no longer valid, or by IP addresses that are new or no longer 
valid.
.

MessageId=20051 SymbolicName=ERROR_DHCP_CANNOT_MODIFY_BINDINGS
Language=English
The bindings to internal IP addresses cannot be modified.
.

MessageId=20052 SymbolicName=ERROR_DHCP_SUBNET_EXISTS
Language=English
The scope parameters are incorrect.  Either the scope already
exists or its subnet address and mask is inconsistent with the
subnet address and mask of an existing scope.
.

MessageId=20053 SymbolicName=ERROR_DHCP_MSCOPE_EXISTS
Language=English
The multicast scope parameters are incorrect.  Either the 
scope  already exists or its properties are inconsistent with 
the properties of another existing scope.
.

MessageId=20054 SymbolicName=ERROR_MSCOPE_RANGE_TOO_SMALL
Language=English
The multicast scope range must have atleast 256 IP addresses.
.

;//
;//  20070 + are the message used here..
;//

MessageId=20070 SymbolicName=ERROR_DDS_NO_DS_AVAILABLE
Language=English
The DHCP service could not contact Active Directory.
.

MessageId=20071 SymbolicName=ERROR_DDS_NO_DHCP_ROOT
Language=English
The DHCP service root could not be found in the Active Directory.
.

MessageId=20072 SymbolicName=ERROR_DDS_UNEXPECTED_ERROR
Language=English
An unexpected error occurred while accessing the Active Directory.

.

MessageId=20073 SymbolicName=ERROR_DDS_TOO_MANY_ERRORS
Language=English
There were too many errors to proceed.
.

MessageId=20074 SymbolicName=ERROR_DDS_DHCP_SERVER_NOT_FOUND
Language=English
A DHCP service could not be found.
.

MessageId=20075 SymbolicName=ERROR_DDS_OPTION_ALREADY_EXISTS
Language=English
The specified options are already present in the directory service.
.

MessageId=20076 SymbolicName=ERROR_DDS_OPTION_DOES_NOT_EXIST
Language=English
The specified options are not present in the directory service.
.

MessageId=20077 SymbolicName=ERROR_DDS_CLASS_EXISTS
Language=English
The specified classes already exist in the directory service.
.

MessageId=20078 SymbolicName=ERROR_DDS_CLASS_DOES_NOT_EXIST
Language=English
The specified classes do not exist in the directory service.
.

MessageId=20079 SymbolicName=ERROR_DDS_SERVER_ALREADY_EXISTS
Language=English
The specified servers are already present in the directory service.
.

MessageId=20080 SymbolicName=ERROR_DDS_SERVER_DOES_NOT_EXIST
Language=English
The specified servers are not present in the directory service.
.

MessageId=20081 SymbolicName=ERROR_DDS_SERVER_ADDRESS_MISMATCH
Language=English
The specified server address does not belong to the identified 
server name.
.

MessageId=20082 SymbolicName=ERROR_DDS_SUBNET_EXISTS
Language=English
The specified subnets already exist in the directory service.
.

MessageId=20083 SymbolicName=ERROR_DDS_SUBNET_HAS_DIFF_SSCOPE
Language=English
The specified subnet belongs to a different superscope.
.

MessageId=20084 SymbolicName=ERROR_DDS_SUBNET_NOT_PRESENT
Language=English
The specified subnet is not present in the directory service.
.

MessageId=20085 SymbolicName=ERROR_DDS_RESERVATION_NOT_PRESENT
Language=English
The specified reservation is not present in the directory service.
.

MessageId=20086 SymbolicName=ERROR_DDS_RESERVATION_CONFLICT
Language=English
The reservation specified conflicts with an existing reservation
in the directory service.
.

MessageId=20087 SymbolicName=ERROR_DDS_POSSIBLE_RANGE_CONFLICT
Language=English
The specified IP range conflicts with some existing IP range in the
directory service.
.

MessageId=20088 SymbolicName=ERROR_DDS_RANGE_DOES_NOT_EXIST
Language=English
The specified IP range is not present in the directory service.
.



;//
;//   This is the last error message for the DHCP service.  Should not be 
;//   used internally except for place marker. All new errors should be
;//   filled before this.  No localization needed for this.
;//

MessageId=20089 SymbolicName=ERROR_LAST_DHCP_SERVER_ERROR
Language=English
.


;//
;//  1000 - 1099   Reserved for DHCP server events.
;//

MessageId=1000 SymbolicName=EVENT_SERVER_UNKNOWN_OPTION
Language=English
The DHCP service received the unknown option %1, with a length of %2. 
The raw option data is given below.
.

MessageId=1001 SymbolicName=EVENT_SERVER_FAILED_REGISTER_SC
Language=English
The DHCP service failed to register with Service Controller. 
The following error occurred: %n%1.
.

MessageId=1002 SymbolicName=EVENT_SERVER_INIT_DATA_FAILED
Language=English
The DHCP service failed to initialize its global parameters. 
The following error occurred: %n%1
.

MessageId=1003 SymbolicName=EVENT_SERVER_INIT_REGISTRY_FAILED
Language=English
The DHCP service failed to initialize its registry parameters. 
The following error occurred: %n%1
.

MessageId=1004 SymbolicName=EVENT_SERVER_INIT_DATABASE_FAILED
Language=English
The DHCP service failed to initialize the database. 
The following error occurred: %n%1
.

MessageId=1005 SymbolicName=EVENT_SERVER_INIT_WINSOCK_FAILED
Language=English
The DHCP service failed to initialize Winsock startup. 
The following error occurred: %n%1
.

MessageId=1006 SymbolicName=EVENT_SERVER_INIT_RPC_FAILED
Language=English
The DHCP service failed to start as a RPC server. 
The following error occurred : %n%1
.

MessageId=1007 SymbolicName=EVENT_SERVER_INIT_SOCK_FAILED
Language=English
The DHCP service failed to initialize Winsock data. 
The following error occurred: %n%1
.

MessageId=1008 SymbolicName=EVENT_SERVER_SHUTDOWN
Language=English
The DHCP service is shutting down due to the following error: %n%1
.

MessageId=1009 SymbolicName=EVENT_SERVER_CLIENT_CLEANUP
Language=English
The DHCP service encountered the following error while cleaning 
up the pending client records: %n%1
.

MessageId=1010 SymbolicName=EVENT_SERVER_DATABASE_CLEANUP
Language=English
The DHCP service encountered the following error while cleaning 
up the database: %n%1
.

MessageId=1011 SymbolicName=EVENT_SERVER_LEASE_NACK
Language=English
The DHCP service issued a NACK (negative acknowledgement message)
to the client, %2, for the address, %1.
.

MessageId=1012 SymbolicName=EVENT_SERVER_LEASE_DECLINED
Language=English
The DHCP client, %2, declined the address %1.
.

MessageId=1013 SymbolicName=EVENT_SERVER_LEASE_RELEASE
Language=English
The DHCP Client, %2, released the address %1.
.

MessageId=1014 SymbolicName=EVENT_SERVER_JET_ERROR
Language=English

The following problem occurred with the Jet database %1:
Reading or writing to the Jet database failed. If this message
appears frequently, either there was insufficient disk space for
database OR backup database or the database might be corrupt. To
correct this problem, either increase available disk space or refer
to the documentation on restoring the database. After the database is
restored, make sure that conflict detection is enabled in the server properties. 


Additional Debug Information: %2.
.

MessageId=1015 SymbolicName=EVENT_SERVER_JET_WARNING
Language=English
The JET Database call returned the following warning : %1.

Additional Debug Information: %2.
.

MessageId=1016 SymbolicName=EVENT_SERVER_DATABASE_BACKUP
Language=English
The DHCP service encountered the following error when 
backing up the database: %n%1
.

MessageId=1017 SymbolicName=EVENT_SERVER_CONFIG_BACKUP
Language=English
The DHCP service encountered the following error when 
backing up the registry configuration: %n%1
.

MessageId=1018 SymbolicName=EVENT_SERVER_DATABASE_RESTORE_FAILED
Language=English
The DHCP service failed to restore the database. 
The following error occurred: %n%1
.

MessageId=1019 SymbolicName=EVENT_SERVER_CONFIG_RESTORE_FAILED
Language=English
The DHCP service failed to restore the DHCP registry configuration. 
The following error occurred: %n%1
.

MessageId=1020 SymbolicName=EVENT_SERVER_LOW_ADDRESS_WARNING
Language=English
Scope, %1, is %2 percent full with only %3 IP addresses remaining.
.

MessageId=1021 SymbolicName=EVENT_SERVER_LOAD_JET_FAILED
Language=English
The DHCP service could not load the JET database library successfully.
.

MessageId=1022 SymbolicName=EVENT_SERVER_JET_CONV_REQUIRED
Language=English
The DHCP service could not use the database. If this service
was started for the first time after the upgrade from NT 3.51 
or earlier, you need to run the utility, upg351db.exe, on the 
DHCP database to convert it to the new JET database format. 
Restart the DHCP service after you have upgraded the database.
.

MessageId=1023 SymbolicName=EVENT_SERVER_JET_CONV_IN_PROGRESS
Language=English
The DHCP service will now terminate because the existing database
needs conversion to Windows 2000 format.  The conversion via the 
jetconv process, has initiated.  Do not reboot or stop the jetconv
process.  

The conversion may take up to 10 minutes depending on the size of 
the database.  Terminate DHCP now by clicking OK. This is required 
for the database conversion to succeed.

NOTE: The DHCP service will be restarted automatically when the 
conversion is completed.  To check conversion status, look at the
Application event log for the jetconv process.
.

MessageId=1024 SymbolicName=EVENT_SERVER_INIT_AND_READY
Language=English
The DHCP service has initialized and is ready
.

MessageId=1025 SymbolicName=EVENT_SERVER_BOOT_FILE_TABLE
Language=English
The DHCP service was unable to read the BOOTP file table from 
the registry.  The DHCP service will be unable to respond to 
BOOTP requests that specify the boot file name.
.
MessageId=1026 SymbolicName=EVENT_SERVER_BOOT_FILE_NAME
Language=English
The DHCP service was unable to read the global BOOTP file name 
from the registry.
.

MessageId=1027 SymbolicName=EVENT_SERVER_AUDIT_LOG_APPEND_FAILED
Language=English
The audit log file cannot be appended.
.

MessageId=1028 SymbolicName=EVENT_SERVER_INIT_AUDIT_LOG_FAILED
Language=English
The DHCP service failed to initialize the audit log. 
The following error occurred: %n%1
.

MessageId=1029 SymbolicName=EVENT_SERVER_PING_FAILED
Language=English
The DHCP service was unable to ping for a new IP address. 
The address was leased to the client.
.

MessageId=1030 SymbolicName=EVENT_SERVER_MOVE_AUDIT_LOG_FAILED
Language=English
The audit log file could not be backed up.  The following error
occurred: %n%1
.

MessageId=1031 SymbolicName=EVENT_SERVER_CALLOUT_UNHANDLED_EXCEPTION
Language=English
The installed server callout .dll file has caused an exception.
The exception was: %n%1.  The server has ignored this exception. 
All further exceptions will be ignored.
.

MessageId=1032 SymbolicName=EVENT_SERVER_CALLOUT_LOAD_EXCEPTION
Language=English
The installed server callout .dll file has caused an exception.
The exception was: %n%1.  The server has ignored this exception 
and the .dll file could not be loaded.
.

MessageId=1033 SymbolicName=EVENT_SERVER_CALLOUT_LOAD_SUCCESS
Language=English
The DHCP service has successfully loaded one or more callout DLLs.
.

MessageId=1034 SymbolicName=EVENT_SERVER_CALLOUT_LOAD_FAILED
Language=English
The DHCP service has failed to load one or more callout DLLs.  
The following error occured: %n%1
.

MessageId=1035 SymbolicName=EVENT_SERVER_READ_ONLY_GROUP_ERROR
Language=English

The DHCP service was unable to create or lookup the DHCP Users
local group on this computer.  The error code is in the data.
.

MessageId=1036 SymbolicName=EVENT_SERVER_ADMIN_GROUP_ERROR
Language=English

The DHCP server was unable to create or lookup the DHCP Administrators 
local group on this computer.  The error code is in the data.
.

MessageId=1037 SymbolicName=EVENT_SERVER_CLEANUP_STARTED
Language=English

The DHCP service has started to clean up the database.
.

MessageId=1038 SymbolicName=EVENT_SERVER_IPCLEANUP_FINISHED
Language=English

The DHCP service has cleaned up the database for unicast 
IP addresses -- %1 leases have been recovered and %2 records 
have been removed from the database.
.

MessageId=1039 SymbolicName=EVENT_SERVER_MCASTCLEANUP_FINISHED
Language=English

The DHCP service has cleaned up the database for multicast 
IP addresses -- %1 leases have expired (been marked for deletion)
and %2 records have been removed from the database.
.

MessageId=1040 SymbolicName=EVENT_SERVER_DATABASE_RESTORE_SUCCEEDED
Language=English
The DHCP service successfully restored the database.
.

MessageId=1041 SymbolicName=DHCP_ROGUE_EVENT_NO_NETWORK
Language=English.
The DHCP service is not servicing any clients because none of 
the active network interfaces have statically configured IP 
addresses, or there are no active interfaces.
.


MessageId=1042 SymbolicName=DHCP_ROGUE_EVENT_UNAUTHORIZED_INFO
Language=English
The DHCP/BINL service running on this machine has detected the following
servers on the network.  Their domains are listed below as well as the 
authorization status of the local machine as verified against the directory
service enterprises of each of these domains.  If the servers do not belong 
to any domain, the domain is listed as empty.  The IP address of each of 
these servers are listed in parentheses.

%1

.

MessageId=1043 SymbolicName=DHCP_ROGUE_EVENT_STARTED
Language=English
The DHCP/BINL service on the local machine has determined that it is
authorized to start.  It is servicing clients now.
.

MessageId=1044 SymbolicName=DHCP_ROGUE_EVENT_STARTED_DOMAIN
Language=English
The DHCP/BINL service on the local machine, belonging to the Windows
Administrative domain %2, has determined that it is authorized to start.
It is servicing clients now.
.

MessageId=1045 SymbolicName=DHCP_ROGUE_EVENT_STOPPED
Language=English
The DHCP/BINL service on the local machine has determined that it is
not authorized to start.  It has stopped servicing clients.  The
following are some possible reasons for this: 
%n%tThis machine belongs to a workgroup and has encountered another DHCP 
Server (belonging to a Windows Administrative Domain) servicing 
the same network.
%n%n%tAn unexpected network error occurred.
.

MessageId=1046 SymbolicName=DHCP_ROGUE_EVENT_STOPPED_DOMAIN
Language=English
The DHCP/BINL service on the local machine, belonging to the Windows
Administrative domain %2, has determined that it is not authorized
to start.  It has stopped servicing clients.  The following are
some possible reasons for this:
%n%tThis machine is part of a directory service enterprise and is 
not authorized in the same domain.  (See help on the DHCP Service 
Management Tool for additional information).
%n%n%tThis machine cannot reach its directory service enterprise and 
it has encountered another DHCP service on the network belonging to 
a directory service enterprise on which the local machine is not authorized.
%n%n%tSome unexpected network error occurred.
.

MessageId=1047 SymbolicName=DHCP_ROGUE_EVENT_JUST_UPGRADED
Language=English
The DHCP/BINL service on the local machine has determined that it is
authorized to start.  It is servicing clients now.
%nThe DHCP/BINL service has determined that the machine was recently 
upgraded.  If the machine is intended to belong to a directory service 
enterprise, the DHCP service must be authorized in the directory service
for it to start servicing clients.  (See help on DHCP Service Management 
Tool for authorizing the server).
.

MessageId=1048 SymbolicName=DHCP_ROGUE_EVENT_JUST_UPGRADED_DOMAIN
Language=English
The DHCP/BINL Service on the local machine, belonging to Windows Domain
%2, has determined that it is authorized to start.  

It is servicing clients now.

It has determined that the computer was recently upgraded.  It has also 
determined that either there is no directory service enterprise for the 
domain or that the computer is not authorized in the directory service. 

All DHCP services that belong to a directory service enterprise should 
be authorized in the directory service to service clients.   (See help
on the DHCP Service Management Tool for authorizing a DHCP service in
the directory service).
.

MessageId=1049 SymbolicName=DHCP_ROGUE_EVENT_CANT_FIND_DOMAIN
Language=English
The DHCP/BINL service on the local machine encountered an error while
trying to find the domain of the local machine.  The error was: %3.
.

MessageId=1050 SymbolicName=DHCP_ROGUE_EVENT_NETWORK_FAILURE
Language=English
The DHCP/BINL service on the local machine encountered a network error.
The error was: %3.
.

MessageId=1051 SymbolicName=DHCP_ROGUE_EVENT_UNAUTHORIZED
Language=English
The DHCP/BINL service has determined that it is not authorized to
service clients on this network for the Windows domain: %2.
.

MessageId=1052 SymbolicName=DHCP_ROGUE_EVENT_OTHER_SERVER
Language=English
The DHCP/BINL service on this workgroup server has encountered another
server with IP Address, %1, belonging to the domain %2.
.

MessageId=1053 SymbolicName=DHCP_ROGUE_EVENT_SAM_OTHER_SERVER
Language=English
The DHCP/BINL service on this Small Business Server
has encountered another server on this network with 
IP Address, %1, belonging to the domain: %2.
.

MessageId=1054 SymbolicName=DHCP_ROGUE_EVENT_SHUTDOWN
Language=English
The DHCP/BINL service on this computer is shutting down. 
See the previous event log messages for reasons.
.

MessageId=1055 SymbolicName=DHCP_EVENT_DNS_REGPARAMS_FAILURE
Language=English
The DHCP service was unable to impersonate the credentials necessary 
for DNS registrations: %n%1.  

The local system credentials is being used.
.

MessageId=1056 SymbolicName=DHCP_EVENT_NO_DNSCREDENTIALS_ON_DC
Language=English
The DHCP service has detected that it is running on a DC and has 
no credentials configured for use with Dynamic DNS registrations 
initiated by the DHCP service.   This is not a recommended security
configuration. 

Credentials for Dynamic DNS registrations may be configured using
the command line "netsh dhcp server set dnscredentials" or via the 
DHCP Administrative tool.

.

MessageId=1057 SymbolicName=EVENT_SERVER_DATABASE_CONVERSION
Language=English
The DHCP service was unable to convert the temporary database to 
ESE format: %n%1.
.

MessageId=1058 SymbolicName=EVENT_SERVER_INIT_CONFIG_FAILED
Language=English
The DHCP service failed to initialize its configuration parameters. 
The following error occurred: %n%1
.

MessageId=1059 SymbolicName=EVENT_SERVER_COULDNT_SEE_DS
Language=English
The DHCP service failed to see a directory server for authorization.
.

MessageId=1060 SymbolicName=EVENT_SERVER_AUDITLOG_PATH_NOT_ACCESSIBLE
Language=English
The DHCP service was unable to access path specified for the audit log.
.

MessageId=1061 SymbolicName=EVENT_SERVER_BACKUP_PATH_NOT_ACCESSIBLE
Language=English
The DHCP service was unable to access path specified for the database backups.
.

MessageId=1062 SymbolicName=EVENT_SERVER_DB_PATH_NOT_ACCESSIBLE
Language=English
The DHCP service was unable to access path specified for the database
.



;//
;// Audit log strings
;//

;//
;// BEGIN: these strings should be <= 10 characters in length ???
;//

MessageId=+1 SymbolicName=DHCP_ROGUE_STRING_FMT_UNAUTHORIZED
Language=English
    The DHCP/BINL service is not authorized in the directory service domain "%2" (Server IP Address %1)
.

MessageId=+1 SymbolicName=DHCP_ROGUE_STRING_FMT_AUTHORIZED
Language=English
    The DHCP/BINL service is authorized in the directory service domain "%2" (Server IP Address %1)
.

MessageId=+1 SymbolicName=DHCP_ROGUE_STRING_FMT_NOT_CHECKED
Language=English
    The DHCP/BINL service has not determined if it is authorized in directory domain "%2" (Server IP Address %1)
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_ASSIGN_NAME
Language=English
Assign%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_BOOTP_NAME
Language=English
Bootp%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_DYNBOOTP_NAME
Language=English
Dynamic Bootp%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_RENEW_NAME
Language=English
Renew%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_RELEASE_NAME
Language=English
Release%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_CONFLICT_NAME
Language=English
Conflict%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_RANGE_FULL_NAME
Language=English
Scope Full%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_START_NAME
Language=English
Started%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_STOP_NAME
Language=English
Stopped%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_DISK_SPACE_LOW_NAME
Language=English
Audit Log Paused%0
.


MessageId=+1 SymbolicName=DHCP_IP_LOG_FILE_NAME
Language=English
dhcpsrv.log%0
.

MessageId=+1 SymbolicName=DHCP_BAD_ADDRESS_NAME
Language=English
BAD_ADDRESS%0
.

MessageId=+1 SymbolicName=DHCP_BAD_ADDRESS_INFO
Language=English
This address is already in use%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_NACK_NAME
Language=English
NACK%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_DELETED_NAME
Language=English
Deleted%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_EXPIRED_NAME
Language=English
Expired%0
.

MessageId=+1 SymbolicName=DHCP_IP_LOG_HEADER
Language=English
                     Microsoft DHCP Service Activity Log


Event ID  Meaning
00        The log was started.
01        The log was stopped.
02        The log was temporarily paused due to low disk space.
10        A new IP address was leased to a client.
11        A lease was renewed by a client.
12        A lease was released by a client.
13        An IP address was found to be in use on the network.
14        A lease request could not be satisfied because the scope's
          address pool was exhausted.
15        A lease was denied.
16        A lease was deleted.
17        A lease was expired.
20        A BOOTP address was leased to a client.
21        A dynamic BOOTP address was leased to a client.
22        A BOOTP request could not be satisfied because the scope's
          address pool for BOOTP was exhausted.
23        A BOOTP IP address was deleted after checking to see it was
          not in use.
50+       Codes above 50 are used for Rogue Server Detection information.

ID Date,Time,Description,IP Address,Host Name,MAC Address
.

MessageId=+1 SymbolicName=DHCP_IP_BOOTP_LOG_RANGE_FULL_NAME
Language=English
BOOTP Range Full%0
.

MessageId=+1 SymbolicName=DHCP_IP_BOOTP_LOG_DELETED_NAME
Language=English
BOOTP deleted%0
.

;//
;// days of the week strings used for file name by auditlogging code
;//

MessageId=+1 SymbolicName=DHCP_LOG_FILE_NAME_SUNDAY
Language=English
DhcpSrvLog-Sun.log%0
.

MessageId=+1 SymbolicName=DHCP_LOG_FILE_NAME_MONDAY
Language=English
DhcpSrvLog-Mon.log%0
.

MessageId=+1 SymbolicName=DHCP_LOG_FILE_NAME_TUESDAY
Language=English
DhcpSrvLog-Tue.log%0
.

MessageId=+1 SymbolicName=DHCP_LOG_FILE_NAME_WEDNESDAY
Language=English
DhcpSrvLog-Wed.log%0
.

MessageId=+1 SymbolicName=DHCP_LOG_FILE_NAME_THURSDAY
Language=English
DhcpSrvLog-Thu.log%0
.

MessageId=+1 SymbolicName=DHCP_LOG_FILE_NAME_FRIDAY
Language=English
DhcpSrvLog-Fri.log%0
.

MessageId=+1 SymbolicName=DHCP_LOG_FILE_NAME_SATURDAY
Language=English
DhcpSrvLog-Sat.log%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_COULDNT_SEE_DS
Language=English
Unreachable Domain%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_AUTHORIZED
Language=English
Authorization succeeded%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_UPGRADED
Language=English
Server Upgraded%0
.

Messageid=+1 SymbolicName=DHCP_ROGUE_LOG_PREAUTHORIZED
Language=English
Cached authorization%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_UNAUTHORIZED
Language=English
Authorization failed%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_STARTED
Language=English
Authorized(servicing)%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_STOPPED
Language=English
Authorization failure, stopped servicing%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_SAME_DOMAIN
Language=English
Server found in our domain%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_CANT_FIND_DOMAIN
Language=English
Could not find domain%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_NETWORK_FAILURE
Language=English
Network failure%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_NO_DSDC
Language=English
No DC is DS Enabled%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_OTHER_SERVER
Language=English
Server found that belongs to DS domain%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_SAM_OTHER_SERVER
Language=English
Another server was found%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_RESTART
Language=English
Restarting rogue detection%0
.

MessageId=+1 SymbolicName=DHCP_ROGUE_LOG_NO_NETWORK
Language=English
No static IP address bound to DHCP server%0
.

MessageId=+1 SymbolicName=DHCP_MSFT50_CLASS_NAME
Language=English
Microsoft Windows 2000 Options%0
.

MessageId=+1 SymbolicName=DHCP_MSFT50_CLASS_DESCR_NAME
Language=English
Microsoft vendor-specific options for Windows 2000 Clients%0
.

MessageId=+1 SymbolicName=DHCP_MSFT98_CLASS_NAME
Language=English
Microsoft Windows 98 Options%0
.

MessageId=+1 SymbolicName=DHCP_MSFT98_CLASS_DESCR_NAME
Language=English
Microsoft vendor-specific options for Windows 98 Clients%0
.

MessageId=+1 SymbolicName=DHCP_MSFT_CLASS_NAME
Language=English
Microsoft Options%0
.

MessageId=+1 SymbolicName=DHCP_MSFT_CLASS_DESCR_NAME
Language=English
Microsoft vendor-specific options applicable to Windows 98 and Windows 2000 Clients%0
.

MessageId=+1 SymbolicName=DHCP_NETBIOS_VENDOR_OPTION_NAME
Language=English
Microsoft Disable Netbios Option %0
.

MessageId=+1 SymbolicName=DHCP_NETBIOS_VENDOR_DESCR_NAME
Language=English
Option for enabling or disabling Netbios for Microsoft Windows 2000 Clients%0
.

MessageId=+1 SymbolicName=DHCP_RELEASE_SHUTDOWN_VENDOR_OPTION_NAME
Language=English
Microsoft Release DHCP Lease On Shutdown Option%0
.

MessageId=+1 SymbolicName=DHCP_RELEASE_SHUTDOWN_VENDOR_DESCR_NAME
Language=English
Option for enabling or disabling Windows 2000 Clients to release DHCP lease on shutdown%0
.

MessageId=+1 SymbolicName=DHCP_METRICBASE_VENDOR_OPTION_NAME
Language=English
Microsoft Default Router Metric Base%0
.

MessageId=+1 SymbolicName=DHCP_METRICBASE_VENDOR_DESCR_NAME
Language=English
Default Router Base Metrics for Microsoft Windows 2000 Clients%0
.

MessageId=+1 SymbolicName=DHCP_CLUSTER_CONNECTION_NAME
Language=English
Internal%0
.

MessageId=+1 SymbolicName=DHCP_USERS_GROUP_NAME
Language=English
DHCP Users%0
.

MessageId=+1 SymbolicName=DHCP_USERS_GROUP_DESCRIPTION
Language=English
Members who have view-only access to the DHCP service%0
.

MessageId=+1 SymbolicName=DHCP_ADMINS_GROUP_NAME
Language=English
DHCP Administrators%0
.

MessageId=+1 SymbolicName=DHCP_ADMINS_GROUP_DESCRIPTION
Language=English
Members who have administrative access to DHCP service%0
.

MessageId=+1 SymbolicName=DHCP_MSFT_RRAS_CLASS_NAME
Language=English
Default Routing and Remote Access Class%0
.

MessageId=+1 SymbolicName=DHCP_MSFT_RRAS_CLASS_DESCR_NAME
Language=English
User class for remote access clients%0
.

MessageId=+1 SymbolicName=DHCP_MSFT_DYNBOOTP_CLASS_NAME
Language=English
Default BOOTP Class%0
.

MessageId=+1 SymbolicName=DHCP_MSFT_DYNBOOTP_CLASS_DESCR_NAME
Language=English
User class for BOOTP Clients%0
.

MessageId=+1 SymbolicName=DHCP_MSFT_CSR_OPTION_NAME
Language=English
Classless Static Routes%0
.

MessageId=+1 SymbolicName=DHCP_MSFT_CSR_DESCR_NAME
Language=English
Destination, mask and router IP addresses in priority order%0
.

;//
;// Keep this message as the last string that is needed by the DHCP service.
;//

MessageId=+1 SymbolicName=DHCP_LAST_STRING_DUMMY_MESSAGE
Language=English
  %0
.

;#endif // _DHCPMESSAGE_
