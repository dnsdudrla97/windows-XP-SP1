/*++ BUILD Version: 0002    // Increment this if a change has global effects

Copyright (c) 1991-1993  Microsoft Corporation

Module Name:

    lmaccess.h

Abstract:

    This file contains structures, function prototypes, and definitions
    for the NetUser, NetUserModals, NetGroup, NetAccess, and NetLogon API.

Author:

    Dan Lafferty (danl) 10-Mar-1991

Environment:

    User Mode - Win32

Notes:

    You must include NETCONS.H before this file, since this file depends
    on values defined in NETCONS.H.

Revision History:

    10-Mar-1991 danl
    Created from LM2.0 header files and NT-LAN API Spec.

    11-Apr-1991 danhi
    Changed usrmod0_passwd_hist_len to usrmod0_password_hist_len to match
    the LM 2.0 name.

    12-Apr-1991 cliffv
    Moved internal info level 99 out of this public file.

    14-Apr-1991 w-shanku
    Changed ParmNum constants to allow consistency with OS/2 parmnums.

    17-Apr-1991 cliffv
    Added OPTIONAL keyword to function prototypes.

    29-Apr-1991 w-shanku
    Fixed USER_LOGON_INFO structure definition - was given without
    'typedef' keyword.

    21-Jun-1991 cliffv
    Converted to UNICODE.  Added info level for each parmnum.

    16-Jul-1991 JohnRo
    Clarify that NetGetDCName's domain name is OPTIONAL (default is primary
    domain.)

    18-Jul-1991 RFirth
        Created XXX_XXX_INFOLEVELs as counterparts of XXX_XXX_PARMNUMs

    21-Jan-1992 madana
        Sundry API changes

    26-Nov-1992 ritaw
        Added NetLocalGroup APIs.

--*/

//
// User Class
//

#ifndef _LMUSER_
#define _LMUSER_

#ifdef __cplusplus
extern "C" {
#endif

#include <lmcons.h>     // unfortunately created a dependence on this

//
// Function Prototypes - User
//

NET_API_STATUS NET_API_FUNCTION
NetUserAdd (
    IN  LPWSTR     servername OPTIONAL,
    IN  DWORD      level,
    IN  LPBYTE     buf,
    OUT LPDWORD    parm_err OPTIONAL
    );

NET_API_STATUS NET_API_FUNCTION
NetUserEnum (
    IN  LPWSTR     servername OPTIONAL,
    IN  DWORD      level,
    IN  DWORD      filter,
    OUT LPBYTE     *bufptr,
    IN  DWORD      prefmaxlen,
    OUT LPDWORD    entriesread,
    OUT LPDWORD    totalentries,
    IN OUT LPDWORD resume_handle OPTIONAL
    );

NET_API_STATUS NET_API_FUNCTION
NetUserGetInfo (
    IN  LPWSTR     servername OPTIONAL,
    IN  LPWSTR     username,
    IN  DWORD      level,
    OUT LPBYTE     *bufptr
    );

NET_API_STATUS NET_API_FUNCTION
NetUserSetInfo (
    IN  LPWSTR    servername OPTIONAL,
    IN  LPWSTR    username,
    IN  DWORD     level,
    IN  LPBYTE    buf,
    OUT LPDWORD   parm_err OPTIONAL
    );

NET_API_STATUS NET_API_FUNCTION
NetUserDel (
    IN  LPWSTR    servername OPTIONAL,
    IN  LPWSTR    username
    );

NET_API_STATUS NET_API_FUNCTION
NetUserGetGroups (
    IN  LPWSTR    servername OPTIONAL,
    IN  LPWSTR    username,
    IN  DWORD     level,
    OUT LPBYTE    *bufptr,
    IN  DWORD     prefmaxlen,
    OUT LPDWORD   entriesread,
    OUT LPDWORD   totalentries
    );

NET_API_STATUS NET_API_FUNCTION
NetUserSetGroups (
    IN  LPWSTR    servername OPTIONAL,
    IN  LPWSTR    username,
    IN  DWORD     level,
    IN  LPBYTE    buf,
    IN  DWORD     num_entries
    );

NET_API_STATUS NET_API_FUNCTION
NetUserGetLocalGroups (
    IN  LPWSTR    servername OPTIONAL,
    IN  LPWSTR    username,
    IN  DWORD     level,
    IN  DWORD     flags,
    OUT LPBYTE    *bufptr,
    IN  DWORD     prefmaxlen,
    OUT LPDWORD   entriesread,
    OUT LPDWORD   totalentries
    );

NET_API_STATUS NET_API_FUNCTION
NetUserModalsGet (
    IN  LPWSTR    servername OPTIONAL,
    IN  DWORD     level,
    OUT LPBYTE    *bufptr
    );

NET_API_STATUS NET_API_FUNCTION
NetUserModalsSet (
    IN  LPWSTR    servername OPTIONAL,
    IN  DWORD     level,
    IN  LPBYTE    buf,
    OUT LPDWORD   parm_err OPTIONAL
    );

//
//  Data Structures - User
//

typedef struct _USER_INFO_0 {
    LPWSTR   usri0_name;
}USER_INFO_0, *PUSER_INFO_0, *LPUSER_INFO_0;

typedef struct _USER_INFO_1 {
    LPWSTR   usri1_name;
    LPWSTR   usri1_password;
    DWORD    usri1_password_age;
    DWORD    usri1_priv;
    LPWSTR   usri1_home_dir;
    LPWSTR   usri1_comment;
    DWORD    usri1_flags;
    LPWSTR   usri1_script_path;
}USER_INFO_1, *PUSER_INFO_1, *LPUSER_INFO_1;

typedef struct _USER_INFO_2 {
    LPWSTR   usri2_name;
    LPWSTR   usri2_password;
    DWORD    usri2_password_age;
    DWORD    usri2_priv;
    LPWSTR   usri2_home_dir;
    LPWSTR   usri2_comment;
    DWORD    usri2_flags;
    LPWSTR   usri2_script_path;
    DWORD    usri2_auth_flags;
    LPWSTR   usri2_full_name;
    LPWSTR   usri2_usr_comment;
    LPWSTR   usri2_parms;
    LPWSTR   usri2_workstations;
    DWORD    usri2_last_logon;
    DWORD    usri2_last_logoff;
    DWORD    usri2_acct_expires;
    DWORD    usri2_max_storage;
    DWORD    usri2_units_per_week;
    PBYTE    usri2_logon_hours;
    DWORD    usri2_bad_pw_count;
    DWORD    usri2_num_logons;
    LPWSTR   usri2_logon_server;
    DWORD    usri2_country_code;
    DWORD    usri2_code_page;
}USER_INFO_2, *PUSER_INFO_2, *LPUSER_INFO_2;

typedef struct _USER_INFO_3 {
    LPWSTR   usri3_name;
    LPWSTR   usri3_password;
    DWORD    usri3_password_age;
    DWORD    usri3_priv;
    LPWSTR   usri3_home_dir;
    LPWSTR   usri3_comment;
    DWORD    usri3_flags;
    LPWSTR   usri3_script_path;
    DWORD    usri3_auth_flags;
    LPWSTR   usri3_full_name;
    LPWSTR   usri3_usr_comment;
    LPWSTR   usri3_parms;
    LPWSTR   usri3_workstations;
    DWORD    usri3_last_logon;
    DWORD    usri3_last_logoff;
    DWORD    usri3_acct_expires;
    DWORD    usri3_max_storage;
    DWORD    usri3_units_per_week;
    PBYTE    usri3_logon_hours;
    DWORD    usri3_bad_pw_count;
    DWORD    usri3_num_logons;
    LPWSTR   usri3_logon_server;
    DWORD    usri3_country_code;
    DWORD    usri3_code_page;
    DWORD    usri3_user_id;
    DWORD    usri3_primary_group_id;
    LPWSTR   usri3_profile;
    LPWSTR   usri3_home_dir_drive;
    DWORD    usri3_password_expired;
}USER_INFO_3, *PUSER_INFO_3, *LPUSER_INFO_3;

typedef struct _USER_INFO_10 {
    LPWSTR   usri10_name;
    LPWSTR   usri10_comment;
    LPWSTR   usri10_usr_comment;
    LPWSTR   usri10_full_name;
}USER_INFO_10, *PUSER_INFO_10, *LPUSER_INFO_10;

typedef struct _USER_INFO_11 {
    LPWSTR   usri11_name;
    LPWSTR   usri11_comment;
    LPWSTR   usri11_usr_comment;
    LPWSTR   usri11_full_name;
    DWORD    usri11_priv;
    DWORD    usri11_auth_flags;
    DWORD    usri11_password_age;
    LPWSTR   usri11_home_dir;
    LPWSTR   usri11_parms;
    DWORD    usri11_last_logon;
    DWORD    usri11_last_logoff;
    DWORD    usri11_bad_pw_count;
    DWORD    usri11_num_logons;
    LPWSTR   usri11_logon_server;
    DWORD    usri11_country_code;
    LPWSTR   usri11_workstations;
    DWORD    usri11_max_storage;
    DWORD    usri11_units_per_week;
    PBYTE    usri11_logon_hours;
    DWORD    usri11_code_page;
}USER_INFO_11, *PUSER_INFO_11, *LPUSER_INFO_11;

typedef struct _USER_INFO_20 {
    LPWSTR   usri20_name;
    LPWSTR   usri20_full_name;
    LPWSTR   usri20_comment;
    DWORD    usri20_flags;
    DWORD    usri20_user_id;
}USER_INFO_20, *PUSER_INFO_20, *LPUSER_INFO_20;

typedef struct _USER_INFO_21 {
    BYTE     usri21_password[ENCRYPTED_PWLEN];
}USER_INFO_21, *PUSER_INFO_21, *LPUSER_INFO_21;

typedef struct _USER_INFO_22 {
    LPWSTR   usri22_name;
    BYTE     usri22_password[ENCRYPTED_PWLEN];
    DWORD    usri22_password_age;
    DWORD    usri22_priv;
    LPWSTR   usri22_home_dir;
    LPWSTR   usri22_comment;
    DWORD    usri22_flags;
    LPWSTR   usri22_script_path;
    DWORD    usri22_auth_flags;
    LPWSTR   usri22_full_name;
    LPWSTR   usri22_usr_comment;
    LPWSTR   usri22_parms;
    LPWSTR   usri22_workstations;
    DWORD    usri22_last_logon;
    DWORD    usri22_last_logoff;
    DWORD    usri22_acct_expires;
    DWORD    usri22_max_storage;
    DWORD    usri22_units_per_week;
    PBYTE    usri22_logon_hours;
    DWORD    usri22_bad_pw_count;
    DWORD    usri22_num_logons;
    LPWSTR   usri22_logon_server;
    DWORD    usri22_country_code;
    DWORD    usri22_code_page;
}USER_INFO_22, *PUSER_INFO_22, *LPUSER_INFO_22;

typedef struct _USER_INFO_1003 {
     LPWSTR  usri1003_password;
} USER_INFO_1003, *PUSER_INFO_1003, *LPUSER_INFO_1003;

typedef struct _USER_INFO_1005 {
     DWORD   usri1005_priv;
} USER_INFO_1005, *PUSER_INFO_1005, *LPUSER_INFO_1005;

typedef struct _USER_INFO_1006 {
     LPWSTR  usri1006_home_dir;
} USER_INFO_1006, *PUSER_INFO_1006, *LPUSER_INFO_1006;

typedef struct _USER_INFO_1007 {
     LPWSTR  usri1007_comment;
} USER_INFO_1007, *PUSER_INFO_1007, *LPUSER_INFO_1007;

typedef struct _USER_INFO_1008 {
     DWORD   usri1008_flags;
} USER_INFO_1008, *PUSER_INFO_1008, *LPUSER_INFO_1008;

typedef struct _USER_INFO_1009 {
     LPWSTR  usri1009_script_path;
} USER_INFO_1009, *PUSER_INFO_1009, *LPUSER_INFO_1009;

typedef struct _USER_INFO_1010 {
     DWORD   usri1010_auth_flags;
} USER_INFO_1010, *PUSER_INFO_1010, *LPUSER_INFO_1010;

typedef struct _USER_INFO_1011 {
     LPWSTR  usri1011_full_name;
} USER_INFO_1011, *PUSER_INFO_1011, *LPUSER_INFO_1011;

typedef struct _USER_INFO_1012 {
     LPWSTR  usri1012_usr_comment;
} USER_INFO_1012, *PUSER_INFO_1012, *LPUSER_INFO_1012;

typedef struct _USER_INFO_1013 {
     LPWSTR  usri1013_parms;
} USER_INFO_1013, *PUSER_INFO_1013, *LPUSER_INFO_1013;

typedef struct _USER_INFO_1014 {
     LPWSTR  usri1014_workstations;
} USER_INFO_1014, *PUSER_INFO_1014, *LPUSER_INFO_1014;

typedef struct _USER_INFO_1017 {
     DWORD   usri1017_acct_expires;
} USER_INFO_1017, *PUSER_INFO_1017, *LPUSER_INFO_1017;

typedef struct _USER_INFO_1018 {
     DWORD   usri1018_max_storage;
} USER_INFO_1018, *PUSER_INFO_1018, *LPUSER_INFO_1018;

typedef struct _USER_INFO_1020 {
    DWORD   usri1020_units_per_week;
    LPBYTE  usri1020_logon_hours;
} USER_INFO_1020, *PUSER_INFO_1020, *LPUSER_INFO_1020;

typedef struct _USER_INFO_1023 {
     LPWSTR  usri1023_logon_server;
} USER_INFO_1023, *PUSER_INFO_1023, *LPUSER_INFO_1023;

typedef struct _USER_INFO_1024 {
     DWORD   usri1024_country_code;
} USER_INFO_1024, *PUSER_INFO_1024, *LPUSER_INFO_1024;

typedef struct _USER_INFO_1025 {
     DWORD   usri1025_code_page;
} USER_INFO_1025, *PUSER_INFO_1025, *LPUSER_INFO_1025;

typedef struct _USER_INFO_1051 {
     DWORD   usri1051_primary_group_id;
} USER_INFO_1051, *PUSER_INFO_1051, *LPUSER_INFO_1051;

typedef struct _USER_INFO_1052 {
     LPWSTR  usri1052_profile;
} USER_INFO_1052, *PUSER_INFO_1052, *LPUSER_INFO_1052;

typedef struct _USER_INFO_1053 {
     LPWSTR  usri1053_home_dir_drive;
} USER_INFO_1053, *PUSER_INFO_1053, *LPUSER_INFO_1053;


//
//  Data Structures - User Modals
//

typedef struct _USER_MODALS_INFO_0 {
    DWORD    usrmod0_min_passwd_len;
    DWORD    usrmod0_max_passwd_age;
    DWORD    usrmod0_min_passwd_age;
    DWORD    usrmod0_force_logoff;
    DWORD    usrmod0_password_hist_len;
}USER_MODALS_INFO_0, *PUSER_MODALS_INFO_0, *LPUSER_MODALS_INFO_0;

typedef struct _USER_MODALS_INFO_1 {
    DWORD    usrmod1_role;
    LPWSTR   usrmod1_primary;
}USER_MODALS_INFO_1, *PUSER_MODALS_INFO_1, *LPUSER_MODALS_INFO_1;

typedef struct _USER_MODALS_INFO_2 {
    LPWSTR  usrmod2_domain_name;
    PSID    usrmod2_domain_id;
}USER_MODALS_INFO_2, *PUSER_MODALS_INFO_2, *LPUSER_MODALS_INFO_2;

typedef struct _USER_MODALS_INFO_3 {
    DWORD   usrmod3_lockout_duration;
    DWORD   usrmod3_lockout_observation_window;
    DWORD   usrmod3_lockout_threshold;
}USER_MODALS_INFO_3, *PUSER_MODALS_INFO_3, *LPUSER_MODALS_INFO_3;

typedef struct _USER_MODALS_INFO_1001 {
     DWORD   usrmod1001_min_passwd_len;
} USER_MODALS_INFO_1001, *PUSER_MODALS_INFO_1001, *LPUSER_MODALS_INFO_1001;

typedef struct _USER_MODALS_INFO_1002 {
     DWORD   usrmod1002_max_passwd_age;
} USER_MODALS_INFO_1002, *PUSER_MODALS_INFO_1002, *LPUSER_MODALS_INFO_1002;

typedef struct _USER_MODALS_INFO_1003 {
     DWORD   usrmod1003_min_passwd_age;
} USER_MODALS_INFO_1003, *PUSER_MODALS_INFO_1003, *LPUSER_MODALS_INFO_1003;

typedef struct _USER_MODALS_INFO_1004 {
     DWORD   usrmod1004_force_logoff;
} USER_MODALS_INFO_1004, *PUSER_MODALS_INFO_1004, *LPUSER_MODALS_INFO_1004;

typedef struct _USER_MODALS_INFO_1005 {
     DWORD   usrmod1005_password_hist_len;
} USER_MODALS_INFO_1005, *PUSER_MODALS_INFO_1005, *LPUSER_MODALS_INFO_1005;

typedef struct _USER_MODALS_INFO_1006 {
     DWORD   usrmod1006_role;
} USER_MODALS_INFO_1006, *PUSER_MODALS_INFO_1006, *LPUSER_MODALS_INFO_1006;

typedef struct _USER_MODALS_INFO_1007 {
     LPWSTR  usrmod1007_primary;
} USER_MODALS_INFO_1007, *PUSER_MODALS_INFO_1007, *LPUSER_MODALS_INFO_1007;


//
// Special Values and Constants - User
//

//
//  Bit masks for field usriX_flags of USER_INFO_X (X = 0/1).
//

#define UF_SCRIPT               0x0001
#define UF_ACCOUNTDISABLE       0x0002
#define UF_HOMEDIR_REQUIRED     0x0008
#define UF_LOCKOUT              0x0010
#define UF_PASSWD_NOTREQD       0x0020
#define UF_PASSWD_CANT_CHANGE   0x0040

//
// Account type bits as part of usri_flags.
//

#define UF_TEMP_DUPLICATE_ACCOUNT       0x0100
#define UF_NORMAL_ACCOUNT               0x0200
#define UF_INTERDOMAIN_TRUST_ACCOUNT    0x0800
#define UF_WORKSTATION_TRUST_ACCOUNT    0x1000
#define UF_SERVER_TRUST_ACCOUNT         0x2000

#define UF_MACHINE_ACCOUNT_MASK ( UF_INTERDOMAIN_TRUST_ACCOUNT | \
                                  UF_WORKSTATION_TRUST_ACCOUNT | \
                                  UF_SERVER_TRUST_ACCOUNT )

#define UF_ACCOUNT_TYPE_MASK         ( \
                    UF_TEMP_DUPLICATE_ACCOUNT | \
                    UF_NORMAL_ACCOUNT | \
                    UF_INTERDOMAIN_TRUST_ACCOUNT | \
                    UF_WORKSTATION_TRUST_ACCOUNT | \
                    UF_SERVER_TRUST_ACCOUNT \
                )

#define UF_DONT_EXPIRE_PASSWD           0x10000


#define UF_SETTABLE_BITS        ( \
                    UF_SCRIPT | \
                    UF_ACCOUNTDISABLE | \
                    UF_LOCKOUT | \
                    UF_HOMEDIR_REQUIRED  | \
                    UF_PASSWD_NOTREQD | \
                    UF_PASSWD_CANT_CHANGE | \
                    UF_ACCOUNT_TYPE_MASK | \
                    UF_DONT_EXPIRE_PASSWD \
                )

//
// bit masks for the NetUserEnum filter parameter.
//

#define FILTER_TEMP_DUPLICATE_ACCOUNT       (0x0001)
#define FILTER_NORMAL_ACCOUNT               (0x0002)
// #define FILTER_PROXY_ACCOUNT                (0x0004)
#define FILTER_INTERDOMAIN_TRUST_ACCOUNT    (0x0008)
#define FILTER_WORKSTATION_TRUST_ACCOUNT    (0x0010)
#define FILTER_SERVER_TRUST_ACCOUNT         (0x0020)

//
// bit masks for the NetUserGetLocalGroups flags
//
#define LG_INCLUDE_INDIRECT         (0x0001)

//
//  Bit masks for field usri2_auth_flags of USER_INFO_2.
//

#define AF_OP_PRINT             0x1
#define AF_OP_COMM              0x2
#define AF_OP_SERVER            0x4
#define AF_OP_ACCOUNTS          0x8
#define AF_SETTABLE_BITS        (AF_OP_PRINT | AF_OP_COMM | \
                                AF_OP_SERVER | AF_OP_ACCOUNTS)

//
//  UAS role manifests under NETLOGON
//

#define UAS_ROLE_STANDALONE     0
#define UAS_ROLE_MEMBER         1
#define UAS_ROLE_BACKUP         2
#define UAS_ROLE_PRIMARY        3

//
//  Values for ParmError for NetUserSetInfo.
//

#define USER_NAME_PARMNUM               1
#define USER_PASSWORD_PARMNUM           3
#define USER_PASSWORD_AGE_PARMNUM       4
#define USER_PRIV_PARMNUM               5
#define USER_HOME_DIR_PARMNUM           6
#define USER_COMMENT_PARMNUM            7
#define USER_FLAGS_PARMNUM              8
#define USER_SCRIPT_PATH_PARMNUM        9
#define USER_AUTH_FLAGS_PARMNUM         10
#define USER_FULL_NAME_PARMNUM          11
#define USER_USR_COMMENT_PARMNUM        12
#define USER_PARMS_PARMNUM              13
#define USER_WORKSTATIONS_PARMNUM       14
#define USER_LAST_LOGON_PARMNUM         15
#define USER_LAST_LOGOFF_PARMNUM        16
#define USER_ACCT_EXPIRES_PARMNUM       17
#define USER_MAX_STORAGE_PARMNUM        18
#define USER_UNITS_PER_WEEK_PARMNUM     19
#define USER_LOGON_HOURS_PARMNUM        20
#define USER_PAD_PW_COUNT_PARMNUM       21
#define USER_NUM_LOGONS_PARMNUM         22
#define USER_LOGON_SERVER_PARMNUM       23
#define USER_COUNTRY_CODE_PARMNUM       24
#define USER_CODE_PAGE_PARMNUM          25
#define USER_PRIMARY_GROUP_PARMNUM      51
#define USER_PROFILE                    52 // ?? Delete when convenient
#define USER_PROFILE_PARMNUM            52
#define USER_HOME_DIR_DRIVE_PARMNUM     53

//
// the new infolevel counterparts of the old info level + parmnum
//

#define USER_NAME_INFOLEVEL             \
            (PARMNUM_BASE_INFOLEVEL + USER_NAME_PARMNUM)
#define USER_PASSWORD_INFOLEVEL         \
            (PARMNUM_BASE_INFOLEVEL + USER_PASSWORD_PARMNUM)
#define USER_PASSWORD_AGE_INFOLEVEL     \
            (PARMNUM_BASE_INFOLEVEL + USER_PASSWORD_AGE_PARMNUM)
#define USER_PRIV_INFOLEVEL             \
            (PARMNUM_BASE_INFOLEVEL + USER_PRIV_PARMNUM)
#define USER_HOME_DIR_INFOLEVEL         \
            (PARMNUM_BASE_INFOLEVEL + USER_HOME_DIR_PARMNUM)
#define USER_COMMENT_INFOLEVEL          \
            (PARMNUM_BASE_INFOLEVEL + USER_COMMENT_PARMNUM)
#define USER_FLAGS_INFOLEVEL            \
            (PARMNUM_BASE_INFOLEVEL + USER_FLAGS_PARMNUM)
#define USER_SCRIPT_PATH_INFOLEVEL      \
            (PARMNUM_BASE_INFOLEVEL + USER_SCRIPT_PATH_PARMNUM)
#define USER_AUTH_FLAGS_INFOLEVEL       \
            (PARMNUM_BASE_INFOLEVEL + USER_AUTH_FLAGS_PARMNUM)
#define USER_FULL_NAME_INFOLEVEL        \
            (PARMNUM_BASE_INFOLEVEL + USER_FULL_NAME_PARMNUM)
#define USER_USR_COMMENT_INFOLEVEL      \
            (PARMNUM_BASE_INFOLEVEL + USER_USR_COMMENT_PARMNUM)
#define USER_PARMS_INFOLEVEL            \
            (PARMNUM_BASE_INFOLEVEL + USER_PARMS_PARMNUM)
#define USER_WORKSTATIONS_INFOLEVEL     \
            (PARMNUM_BASE_INFOLEVEL + USER_WORKSTATIONS_PARMNUM)
#define USER_LAST_LOGON_INFOLEVEL       \
            (PARMNUM_BASE_INFOLEVEL + USER_LAST_LOGON_PARMNUM)
#define USER_LAST_LOGOFF_INFOLEVEL      \
            (PARMNUM_BASE_INFOLEVEL + USER_LAST_LOGOFF_PARMNUM)
#define USER_ACCT_EXPIRES_INFOLEVEL     \
            (PARMNUM_BASE_INFOLEVEL + USER_ACCT_EXPIRES_PARMNUM)
#define USER_MAX_STORAGE_INFOLEVEL      \
            (PARMNUM_BASE_INFOLEVEL + USER_MAX_STORAGE_PARMNUM)
#define USER_UNITS_PER_WEEK_INFOLEVEL   \
            (PARMNUM_BASE_INFOLEVEL + USER_UNITS_PER_WEEK_PARMNUM)
#define USER_LOGON_HOURS_INFOLEVEL      \
            (PARMNUM_BASE_INFOLEVEL + USER_LOGON_HOURS_PARMNUM)
#define USER_PAD_PW_COUNT_INFOLEVEL     \
            (PARMNUM_BASE_INFOLEVEL + USER_PAD_PW_COUNT_PARMNUM)
#define USER_NUM_LOGONS_INFOLEVEL       \
            (PARMNUM_BASE_INFOLEVEL + USER_NUM_LOGONS_PARMNUM)
#define USER_LOGON_SERVER_INFOLEVEL     \
            (PARMNUM_BASE_INFOLEVEL + USER_LOGON_SERVER_PARMNUM)
#define USER_COUNTRY_CODE_INFOLEVEL     \
            (PARMNUM_BASE_INFOLEVEL + USER_COUNTRY_CODE_PARMNUM)
#define USER_CODE_PAGE_INFOLEVEL        \
            (PARMNUM_BASE_INFOLEVEL + USER_CODE_PAGE_PARMNUM)
#define USER_PRIMARY_GROUP_INFOLEVEL    \
            (PARMNUM_BASE_INFOLEVEL + USER_PRIMARY_GROUP_PARMNUM)
#define USER_POSIX_ID_INFOLEVEL         \
            (PARMNUM_BASE_INFOLEVEL + USER_POSIX_ID_PARMNUM)
#define USER_HOME_DIR_DRIVE_INFOLEVEL         \
            (PARMNUM_BASE_INFOLEVEL + USER_HOME_DIR_DRIVE_PARMNUM)

//
//  For SetInfo call (parmnum 0) when password change not required
//

#define NULL_USERSETINFO_PASSWD     "              "

#define TIMEQ_FOREVER               ((unsigned long) -1L)
#define USER_MAXSTORAGE_UNLIMITED   ((unsigned long) -1L)
#define USER_NO_LOGOFF              ((unsigned long) -1L)
#define UNITS_PER_DAY               24
#define UNITS_PER_WEEK              UNITS_PER_DAY * 7

//
// Privilege levels (USER_INFO_X field usriX_priv (X = 0/1)).
//

#define USER_PRIV_MASK      0x3
#define USER_PRIV_GUEST     0
#define USER_PRIV_USER      1
#define USER_PRIV_ADMIN     2

//
// user modals related defaults
//

#define MAX_PASSWD_LEN      PWLEN
#define DEF_MIN_PWLEN       6
#define DEF_PWUNIQUENESS    5
#define DEF_MAX_PWHIST      8

#define DEF_MAX_PWAGE       TIMEQ_FOREVER               // forever
#define DEF_MIN_PWAGE       (unsigned long) 0L          // 0 days
#define DEF_FORCE_LOGOFF    (unsigned long) 0xffffffff  // never
#define DEF_MAX_BADPW       0                           // no limit
#define ONE_DAY             (unsigned long) 01*24*3600  // 01 day

//
// User Logon Validation (codes returned)
//

#define VALIDATED_LOGON         0
#define PASSWORD_EXPIRED        2
#define NON_VALIDATED_LOGON     3

#define VALID_LOGOFF            1

//
// parmnum manifests for user modals
//

#define MODALS_MIN_PASSWD_LEN_PARMNUM       1
#define MODALS_MAX_PASSWD_AGE_PARMNUM       2
#define MODALS_MIN_PASSWD_AGE_PARMNUM       3
#define MODALS_FORCE_LOGOFF_PARMNUM         4
#define MODALS_PASSWD_HIST_LEN_PARMNUM      5
#define MODALS_ROLE_PARMNUM                 6
#define MODALS_PRIMARY_PARMNUM              7
#define MODALS_DOMAIN_NAME_PARMNUM          8
#define MODALS_DOMAIN_ID_PARMNUM            9
#define MODALS_LOCKOUT_DURATION_PARMNUM     10
#define MODALS_LOCKOUT_OBSERVATION_WINDOW_PARMNUM 11
#define MODALS_LOCKOUT_THRESHOLD_PARMNUM    12

//
// the new infolevel counterparts of the old info level + parmnum
//

#define MODALS_MIN_PASSWD_LEN_INFOLEVEL     \
            (PARMNUM_BASE_INFOLEVEL + MODALS_MIN_PASSWD_LEN_PARMNUM)
#define MODALS_MAX_PASSWD_AGE_INFOLEVEL     \
            (PARMNUM_BASE_INFOLEVEL + MODALS_MAX_PASSWD_AGE_PARMNUM)
#define MODALS_MIN_PASSWD_AGE_INFOLEVEL     \
            (PARMNUM_BASE_INFOLEVEL + MODALS_MIN_PASSWD_AGE_PARMNUM)
#define MODALS_FORCE_LOGOFF_INFOLEVEL       \
            (PARMNUM_BASE_INFOLEVEL + MODALS_FORCE_LOGOFF_PARMNUM)
#define MODALS_PASSWD_HIST_LEN_INFOLEVEL    \
            (PARMNUM_BASE_INFOLEVEL + MODALS_PASSWD_HIST_LEN_PARMNUM)
#define MODALS_ROLE_INFOLEVEL               \
            (PARMNUM_BASE_INFOLEVEL + MODALS_ROLE_PARMNUM)
#define MODALS_PRIMARY_INFOLEVEL            \
            (PARMNUM_BASE_INFOLEVEL + MODALS_PRIMARY_PARMNUM)
#define MODALS_DOMAIN_NAME_INFOLEVEL        \
            (PARMNUM_BASE_INFOLEVEL + MODALS_DOMAIN_NAME_PARMNUM)
#define MODALS_DOMAIN_ID_INFOLEVEL          \
            (PARMNUM_BASE_INFOLEVEL + MODALS_DOMAIN_ID_PARMNUM)

#endif // _LMUSER_

//
// Group Class
//

#ifndef _LMGROUP_
#define _LMGROUP_

//
// Function Prototypes
//

NET_API_STATUS NET_API_FUNCTION
NetGroupAdd (
    IN  LPWSTR   servername OPTIONAL,
    IN  DWORD    level,
    IN  LPBYTE   buf,
    OUT LPDWORD  parm_err OPTIONAL
    );

NET_API_STATUS NET_API_FUNCTION
NetGroupAddUser (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   GroupName,
    IN  LPWSTR   username
    );

NET_API_STATUS NET_API_FUNCTION
NetGroupEnum (
    IN  LPWSTR      servername OPTIONAL,
    IN  DWORD       level,
    OUT LPBYTE      *bufptr,
    IN  DWORD       prefmaxlen,
    OUT LPDWORD     entriesread,
    OUT LPDWORD     totalentries,
    IN OUT LPDWORD  resume_handle OPTIONAL
    );

NET_API_STATUS NET_API_FUNCTION
NetGroupGetInfo (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   groupname,
    IN  DWORD    level,
    OUT LPBYTE   *bufptr
    );

NET_API_STATUS NET_API_FUNCTION
NetGroupSetInfo (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   groupname,
    IN  DWORD    level,
    IN  LPBYTE   buf,
    OUT LPDWORD  parm_err OPTIONAL
    );

NET_API_STATUS NET_API_FUNCTION
NetGroupDel (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   groupname
    );

NET_API_STATUS NET_API_FUNCTION
NetGroupDelUser (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   GroupName,
    IN  LPWSTR   Username
    );

NET_API_STATUS NET_API_FUNCTION
NetGroupGetUsers (
    IN  LPWSTR     servername OPTIONAL,
    IN  LPWSTR     groupname,
    IN  DWORD      level,
    OUT LPBYTE     *bufptr,
    IN  DWORD      prefmaxlen,
    OUT LPDWORD    entriesread,
    OUT LPDWORD    totalentries,
    IN OUT LPDWORD ResumeHandle
    );

NET_API_STATUS NET_API_FUNCTION
NetGroupSetUsers (
    IN  LPWSTR     servername OPTIONAL,
    IN  LPWSTR     groupname,
    IN  DWORD      level,
    IN  LPBYTE     buf,
    IN  DWORD      totalentries
    );

//
//  Data Structures - Group
//

typedef struct _GROUP_INFO_0 {
    LPWSTR   grpi0_name;
}GROUP_INFO_0, *PGROUP_INFO_0, *LPGROUP_INFO_0;

typedef struct _GROUP_INFO_1 {
    LPWSTR   grpi1_name;
    LPWSTR   grpi1_comment;
}GROUP_INFO_1, *PGROUP_INFO_1, *LPGROUP_INFO_1;

typedef struct _GROUP_INFO_2 {
    LPWSTR   grpi2_name;
    LPWSTR   grpi2_comment;
    DWORD    grpi2_group_id;
    DWORD    grpi2_attributes;
}GROUP_INFO_2, *PGROUP_INFO_2;

typedef struct _GROUP_INFO_1002 {
     LPWSTR  grpi1002_comment;
} GROUP_INFO_1002, *PGROUP_INFO_1002, *LPGROUP_INFO_1002;

typedef struct _GROUP_INFO_1005 {
     DWORD  grpi1005_attributes;
} GROUP_INFO_1005, *PGROUP_INFO_1005, *LPGROUP_INFO_1005;


typedef struct _GROUP_USERS_INFO_0 {
     LPWSTR  grui0_name;
} GROUP_USERS_INFO_0, *PGROUP_USERS_INFO_0, *LPGROUP_USERS_INFO_0;

typedef struct _GROUP_USERS_INFO_1 {
     LPWSTR  grui1_name;
     DWORD   grui1_attributes;
} GROUP_USERS_INFO_1, *PGROUP_USERS_INFO_1, *LPGROUP_USERS_INFO_1;

//
// Special Values and Constants - Group
//

#define GROUPIDMASK                 0x8000      // MSB set if uid refers
                                                // to a group

//
// Predefined group for all normal users, administrators and guests
// LOCAL is a special group for pinball local security.
//

#define GROUP_SPECIALGRP_USERS      L"USERS"
#define GROUP_SPECIALGRP_ADMINS     L"ADMINS"
#define GROUP_SPECIALGRP_GUESTS     L"GUESTS"
#define GROUP_SPECIALGRP_LOCAL      L"LOCAL"

//
// parmnum manifests for SetInfo calls (only comment is settable)
//

#define GROUP_ALL_PARMNUM           0
#define GROUP_NAME_PARMNUM          1
#define GROUP_COMMENT_PARMNUM       2
#define GROUP_ATTRIBUTES_PARMNUM    3

//
// the new infolevel counterparts of the old info level + parmnum
//

#define GROUP_ALL_INFOLEVEL             \
            (PARMNUM_BASE_INFOLEVEL + GROUP_ALL_PARMNUM)
#define GROUP_NAME_INFOLEVEL            \
            (PARMNUM_BASE_INFOLEVEL + GROUP_NAME_PARMNUM)
#define GROUP_COMMENT_INFOLEVEL         \
            (PARMNUM_BASE_INFOLEVEL + GROUP_COMMENT_PARMNUM)
#define GROUP_ATTRIBUTES_INFOLEVEL      \
            (PARMNUM_BASE_INFOLEVEL + GROUP_ATTRIBUTES_PARMNUM)
#define GROUP_POSIX_ID_INFOLEVEL        \
            (PARMNUM_BASE_INFOLEVEL + GROUP_POSIX_ID_PARMNUM)

#endif  // _LMGROUP_

//
// LocalGroup Class
//

#ifndef _LMLOCALGROUP_
#define _LMLOCALGROUP_

//
// Function Prototypes
//

NET_API_STATUS NET_API_FUNCTION
NetLocalGroupAdd (
    IN  LPWSTR   servername OPTIONAL,
    IN  DWORD    level,
    IN  LPBYTE   buf,
    OUT LPDWORD  parm_err OPTIONAL
    );

NET_API_STATUS NET_API_FUNCTION
NetLocalGroupAddMember (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   groupname,
    IN  PSID     membersid
    );

NET_API_STATUS NET_API_FUNCTION
NetLocalGroupEnum (
    IN  LPWSTR      servername OPTIONAL,
    IN  DWORD       level,
    OUT LPBYTE      *bufptr,
    IN  DWORD       prefmaxlen,
    OUT LPDWORD     entriesread,
    OUT LPDWORD     totalentries,
    IN OUT LPDWORD  resumehandle OPTIONAL
    );

NET_API_STATUS NET_API_FUNCTION
NetLocalGroupGetInfo (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   groupname,
    IN  DWORD    level,
    OUT LPBYTE   *bufptr
    );

NET_API_STATUS NET_API_FUNCTION
NetLocalGroupSetInfo (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   groupname,
    IN  DWORD    level,
    IN  LPBYTE   buf,
    OUT LPDWORD  parm_err OPTIONAL
    );

NET_API_STATUS NET_API_FUNCTION
NetLocalGroupDel (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   groupname
    );

NET_API_STATUS NET_API_FUNCTION
NetLocalGroupDelMember (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   groupname,
    IN  PSID     membersid
    );

NET_API_STATUS NET_API_FUNCTION
NetLocalGroupGetMembers (
    IN  LPWSTR     servername OPTIONAL,
    IN  LPWSTR     localgroupname,
    IN  DWORD      level,
    OUT LPBYTE     *bufptr,
    IN  DWORD      prefmaxlen,
    OUT LPDWORD    entriesread,
    OUT LPDWORD    totalentries,
    IN OUT LPDWORD resumehandle
    );

NET_API_STATUS NET_API_FUNCTION
NetLocalGroupSetMembers (
    IN  LPWSTR     servername OPTIONAL,
    IN  LPWSTR     groupname,
    IN  DWORD      level,
    IN  LPBYTE     buf,
    IN  DWORD      totalentries
    );

//
//  Data Structures - LocalGroup
//

typedef struct _LOCALGROUP_INFO_0 {
    LPWSTR   lgrpi0_name;
}LOCALGROUP_INFO_0, *PLOCALGROUP_INFO_0, *LPLOCALGROUP_INFO_0;

typedef struct _LOCALGROUP_INFO_1 {
    LPWSTR   lgrpi1_name;
    LPWSTR   lgrpi1_comment;
}LOCALGROUP_INFO_1, *PLOCALGROUP_INFO_1, *LPLOCALGROUP_INFO_1;

typedef struct _LOCALGROUP_INFO_1002 {
     LPWSTR  lgrpi1002_comment;
}LOCALGROUP_INFO_1002, *PLOCALGROUP_INFO_1002, *LPLOCALGROUP_INFO_1002;

typedef struct _LOCALGROUP_MEMBERS_INFO_0 {
     PSID    lgrmi0_sid;
} LOCALGROUP_MEMBERS_INFO_0, *PLOCALGROUP_MEMBERS_INFO_0,
  *LPLOCALGROUP_MEMBERS_INFO_0;

typedef struct _LOCALGROUP_MEMBERS_INFO_1 {
     PSID         lgrmi1_sid;
     SID_NAME_USE lgrmi1_sidusage;
     LPWSTR       lgrmi1_name;
} LOCALGROUP_MEMBERS_INFO_1, *PLOCALGROUP_MEMBERS_INFO_1,
  *LPLOCALGROUP_MEMBERS_INFO_1;

typedef struct _LOCALGROUP_USERS_INFO_0 {
     LPWSTR  lgrui0_name;
} LOCALGROUP_USERS_INFO_0, *PLOCALGROUP_USERS_INFO_0,
  *LPLOCALGROUP_USERS_INFO_0;


#define LOCALGROUP_NAME_PARMNUM          1
#define LOCALGROUP_COMMENT_PARMNUM       2

#endif  // _LMLOCALGROUP_

//
// Access Class
//

#ifndef _LMACCESS_
#define _LMACCESS_



//
// Function Prototypes - Access
//
//
// The NetAccess APIs are only available to downlevel
//

#define NetAccessAdd RxNetAccessAdd

NET_API_STATUS NET_API_FUNCTION
NetAccessAdd (
    IN  LPTSTR   servername OPTIONAL,
    IN  DWORD    level,
    IN  LPBYTE   buf,
    OUT LPDWORD  parm_err OPTIONAL
    );

#define NetAccessEnum RxNetAccessEnum

NET_API_STATUS NET_API_FUNCTION
NetAccessEnum (
    IN  LPTSTR     servername OPTIONAL,
    IN  LPTSTR     BasePath,
    IN  DWORD      Recursive,
    IN  DWORD      level,
    OUT LPBYTE     *bufptr,
    IN  DWORD      prefmaxlen,
    OUT LPDWORD    entriesread,
    OUT LPDWORD    totalentries,
    IN OUT LPDWORD resume_handle OPTIONAL
    );

#define NetAccessGetInfo RxNetAccessGetInfo

NET_API_STATUS NET_API_FUNCTION
NetAccessGetInfo (
    IN  LPTSTR   servername OPTIONAL,
    IN  LPTSTR   resource,
    IN  DWORD    level,
    OUT LPBYTE   *bufptr
    );

#define NetAccessSetInfo RxNetAccessSetInfo

NET_API_STATUS NET_API_FUNCTION
NetAccessSetInfo (
    IN  LPTSTR   servername OPTIONAL,
    IN  LPTSTR   resource,
    IN  DWORD    level,
    IN  LPBYTE   buf,
    OUT LPDWORD  parm_err OPTIONAL
    );

#define NetAccessDel RxNetAccessDel

NET_API_STATUS NET_API_FUNCTION
NetAccessDel (
    IN  LPTSTR   servername OPTIONAL,
    IN  LPTSTR   resource
    );

#define NetAccessGetUserPerms RxNetAccessGetUserPerms

NET_API_STATUS NET_API_FUNCTION
NetAccessGetUserPerms (
    IN  LPTSTR   servername OPTIONAL,
    IN  LPTSTR   UGname,
    IN  LPTSTR   resource,
    OUT LPDWORD  Perms
    );

//
// Data Structures - Access
//

typedef struct _ACCESS_INFO_0 {
    LPTSTR   acc0_resource_name;
}ACCESS_INFO_0, *PACCESS_INFO_0, *LPACCESS_INFO_0;

typedef struct _ACCESS_INFO_1 {
    LPTSTR   acc1_resource_name;
    DWORD    acc1_attr;
    DWORD    acc1_count;
}ACCESS_INFO_1, *PACCESS_INFO_1, *LPACCESS_INFO_1;

typedef struct _ACCESS_INFO_1002 {
     DWORD   acc1002_attr;
} ACCESS_INFO_1002, *PACCESS_INFO_1002, *LPACCESS_INFO_1002;


typedef struct _ACCESS_LIST {
    LPTSTR   acl_ugname;
    DWORD    acl_access;
}ACCESS_LIST, *PACCESS_LIST, *LPACCESS_LIST;

//
// Special Values and Constants - Access
//

//
// Maximum number of permission entries for each resource.
//

#define MAXPERMENTRIES      64

//
//  Bit values for the access permissions.  ACCESS_ALL is a handy
//  way to specify maximum permissions.  These are used in
//  acl_access field of access_list structures.
//

#define ACCESS_NONE         0
#define ACCESS_ALL          ( ACCESS_READ | \
                                ACCESS_WRITE | \
                                ACCESS_CREATE | \
                                ACCESS_EXEC | \
                                ACCESS_DELETE | \
                                ACCESS_ATRIB | \
                                ACCESS_PERM \
                            )

#define ACCESS_READ         0x01
#define ACCESS_WRITE        0x02
#define ACCESS_CREATE       0x04
#define ACCESS_EXEC         0x08
#define ACCESS_DELETE       0x10
#define ACCESS_ATRIB        0x20
#define ACCESS_PERM         0x40

#define ACCESS_GROUP        0x8000

//
// Bit values for the acc1_attr field of the ACCESS_INFO_1 structure.
//

#define ACCESS_AUDIT        0x1

#define ACCESS_SUCCESS_OPEN         0x10
#define ACCESS_SUCCESS_WRITE        0x20
#define ACCESS_SUCCESS_DELETE       0x40
#define ACCESS_SUCCESS_ACL          0x80
#define ACCESS_SUCCESS_MASK         0xF0

#define ACCESS_FAIL_OPEN            0x100
#define ACCESS_FAIL_WRITE           0x200
#define ACCESS_FAIL_DELETE          0x400
#define ACCESS_FAIL_ACL             0x800
#define ACCESS_FAIL_MASK            0xF00

#define ACCESS_FAIL_SHIFT           4

//
// Parmnum value for NetAccessSetInfo.
//

#define ACCESS_RESOURCE_NAME_PARMNUM    1
#define ACCESS_ATTR_PARMNUM             2
#define ACCESS_COUNT_PARMNUM            3
#define ACCESS_ACCESS_LIST_PARMNUM      4

//
// the new infolevel counterparts of the old info level + parmnum
//

#define ACCESS_RESOURCE_NAME_INFOLEVEL  \
            (PARMNUM_BASE_INFOLEVEL + ACCESS_RESOURCE_NAME_PARMNUM)
#define ACCESS_ATTR_INFOLEVEL           \
            (PARMNUM_BASE_INFOLEVEL + ACCESS_ATTR_PARMNUM)
#define ACCESS_COUNT_INFOLEVEL          \
            (PARMNUM_BASE_INFOLEVEL + ACCESS_COUNT_PARMNUM)
#define ACCESS_ACCESS_LIST_INFOLEVEL    \
            (PARMNUM_BASE_INFOLEVEL + ACCESS_ACCESS_LIST_PARMNUM)

//
// ACCESS_LETTERS defines a letter for each bit position in
// the acl_access field of struct access_list.  Note that some
// bits have a corresponding letter of ' ' (space).
//

#define ACCESS_LETTERS      "RWCXDAP         "

#endif // _LMACCESS_

//
// Domain Class
//

#ifndef _LMDOMAIN_
#define _LMDOMAIN_

//
// Function Prototypes - Domain
//

NET_API_STATUS NET_API_FUNCTION
NetGetDCName (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   domainname OPTIONAL,
    OUT LPBYTE  *bufptr
    );

NET_API_STATUS NET_API_FUNCTION
NetGetAnyDCName (
    IN  LPWSTR   servername OPTIONAL,
    IN  LPWSTR   domainname OPTIONAL,
    OUT LPBYTE  *bufptr
    );


NET_API_STATUS NET_API_FUNCTION
I_NetLogonControl(
    IN LPWSTR ServerName OPTIONAL,
    IN DWORD FunctionCode,
    IN DWORD QueryLevel,
    OUT LPBYTE *Buffer
    );

NET_API_STATUS NET_API_FUNCTION
I_NetLogonControl2(
    IN LPWSTR ServerName OPTIONAL,
    IN DWORD FunctionCode,
    IN DWORD QueryLevel,
    IN LPBYTE Data,
    OUT LPBYTE *Buffer
    );



//
// Special Values and Constants - Domain
//

//
// FunctionCode values for I_NetLogonControl.
//
// NOTE : if you change the following NETLOGON_CONTROL_* values,
// change them in net\svcdlls\logonsrv\logon.idl file also.
//

#define NETLOGON_CONTROL_QUERY         1    // No-op: just query
#define NETLOGON_CONTROL_REPLICATE     2    // Force replicate on BDC
#define NETLOGON_CONTROL_SYNCHRONIZE   3    // Force synchronize on BDC
#define NETLOGON_CONTROL_PDC_REPLICATE 4    // Force PDC to broadcast change
#define NETLOGON_CONTROL_REDISCOVER    5    // Force to re-discover trusted domain DCs
#define NETLOGON_CONTROL_TC_QUERY      6    // Query status of specified trusted channel status

// Debug function codes

#define NETLOGON_CONTROL_BACKUP_CHANGE_LOG  0xFFFC
#define NETLOGON_CONTROL_TRUNCATE_LOG       0xFFFD
#define NETLOGON_CONTROL_SET_DBFLAG         0xFFFE
#define NETLOGON_CONTROL_BREAKPOINT         0xFFFF

//
// Query level 1 for I_NetLogonControl
//

typedef struct _NETLOGON_INFO_1 {
        DWORD netlog1_flags;
        NET_API_STATUS netlog1_pdc_connection_status;
} NETLOGON_INFO_1, *PNETLOGON_INFO_1;

typedef struct _NETLOGON_INFO_2 {
        DWORD netlog2_flags;
        NET_API_STATUS netlog2_pdc_connection_status;
#ifdef MIDL_PASS
        [string] wchar_t * netlog2_trusted_dc_name;
#else
        LPWSTR netlog2_trusted_dc_name;
#endif // MIDL_PASS
        NET_API_STATUS netlog2_tc_connection_status;
} NETLOGON_INFO_2, *PNETLOGON_INFO_2;

//
// Values of netlog1_flags
//

#define NETLOGON_REPLICATION_NEEDED       0x01  // Database is out of date
#define NETLOGON_REPLICATION_IN_PROGRESS  0x02  // Replication is happening now
#define NETLOGON_FULL_SYNC_REPLICATION    0x04  // full sync replication required/progress

#ifdef __cplusplus
}
#endif

#endif // _LMDOMAIN_
