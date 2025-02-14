/*++

Copyright (c) 1994  Microsoft Corporation

Module Name:

    scesrvrc.h

Abstract:

    This module defines resource IDs for strings

Author:

    Jin Huang (Jinhuang) 17-Sept.-1997

Revision History:

--*/
#ifndef __scedllrc__
#define __scedllrc__

#include "commonrc.h"

#define SCEDLL_SCP_INIT_ERROR               7101
#define SCEDLL_SCP_INIT_SUCCESS             7102
#define SCEDLL_SCP_READ_PROFILE             7103

#define SCEDLL_SCP_BEGIN_POLICY             7104
#define SCEDLL_SCP_ACCESS_ERROR             7105
#define SCEDLL_SCP_ACCESS_SUCCESS           7106
#define SCEDLL_SCP_AUDIT_ERROR              7107
#define SCEDLL_SCP_AUDIT_SUCCESS            7108

#define SCEDLL_SCP_BEGIN_PRIVILEGES         7109
#define SCEDLL_SCP_PRIVILEGES_ERROR         7110
#define SCEDLL_SCP_PRIVILEGES_SUCCESS       7111

#define SCEDLL_SCP_BEGIN_USERSETTING        7112
#define SCEDLL_SCP_USERSETTING_ERROR        7113
#define SCEDLL_SCP_USERSETTING_SUCCESS      7114

#define SCEDLL_SCP_BEGIN_GROUPMGMT          7115
#define SCEDLL_SCP_GROUPMGMT_ERROR          7116
#define SCEDLL_SCP_GROUPMGMT_SUCCESS        7117

#define SCEDLL_SCP_BEGIN_REGISTRY           7118
#define SCEDLL_SCP_REGISTRY_ERROR           7119
#define SCEDLL_SCP_REGISTRY_SUCCESS         7120

#define SCEDLL_SCP_BEGIN_FILE               7121
#define SCEDLL_SCP_FILE_ERROR               7122
#define SCEDLL_SCP_FILE_SUCCESS             7123

#define SCEDLL_SCP_BEGIN_DS                 7124
#define SCEDLL_SCP_DS_ERROR                 7125
#define SCEDLL_SCP_DS_SUCCESS               7126

#define SCEDLL_SCP_BEGIN_GENERALSVC         7127
#define SCEDLL_SCP_GENERALSVC_ERROR         7128
#define SCEDLL_SCP_GENERALSVC_SUCCESS       7129

#define SCEDLL_SCP_BEGIN_ATTACHMENT         7130
#define SCEDLL_SCP_ATTACHMENT_ERROR         7131
#define SCEDLL_SCP_ATTACHMENT_SUCCESS       7132

#define SCEDLL_SCP_KERBEROS_ERROR           7133
#define SCEDLL_SCP_KERBEROS_SUCCESS         7134

#define SCEDLL_SCP_REGVALUES_ERROR          7135
#define SCEDLL_SCP_REGVALUES_SUCCESS        7136

#define SCEDLL_SCP_UNINIT                   7137

#define SCEDLL_SCP_ADMIN_NOT_ALLOWED        7138
#define SCEDLL_SCP_GUEST_NOT_ALLOWED        7139
#define SCEDLL_SCP_ADD                      7140
#define SCEDLL_SCP_ADDTO                    7141
#define SCEDLL_SCP_CANNOT_REMOVE            7142
#define SCEDLL_SCP_CONFIGURE                7143
#define SCEDLL_SCP_ERROR_ADD                7144
#define SCEDLL_SCP_ERROR_ADDTO              7145
#define SCEDLL_SCP_ERROR_ADMINISTRATOR      7146
#define SCEDLL_SCP_ERROR_CONFIGURE          7147
#define SCEDLL_SCP_ERROR_EVENT_AUDITING     7148
#define SCEDLL_SCP_ERROR_GUEST              7149
#define SCEDLL_SCP_ERROR_LOGSETTINGS        7150
#define SCEDLL_SCP_ERROR_KERBEROS           7151
#define SCEDLL_SCP_ERROR_LOCKOUT            7152
#define SCEDLL_SCP_ERROR_LOGOFF             7153
#define SCEDLL_SCP_ERROR_PASSWORD           7154
#define SCEDLL_SCP_ERROR_REMOVE             7155
#define SCEDLL_SCP_ERROR_UAS                7156
#define SCEDLL_SCP_ERROR_USERSETTINGS       7157
#define SCEDLL_SCP_EVENT_AUDITING           7158
#define SCEDLL_SCP_KERBEROS                 7159
#define SCEDLL_SCP_LOCKOUT                  7160
#define SCEDLL_SCP_LOGOFF                   7161
#define SCEDLL_SCP_LOGSETTINGS              7162
#define SCEDLL_SCP_NOT_SUPPORT              7163
#define SCEDLL_SCP_OTHER_POLICY             7164
#define SCEDLL_SCP_PASSWORD                 7165
#define SCEDLL_SCP_REMOVE                   7166
#define SCEDLL_SCP_RENAME_ADMIN             7167
#define SCEDLL_SCP_RENAME_GUEST             7168
#define SCEDLL_SCP_TAKE_OWNER               7169
#define SCEDLL_SCP_UAS                      7170
#define SCEDLL_SCP_USERSETTINGS             7171
#define SCEDLL_SCP_ERROR_REGVALUES          7172
#define SCEDLL_SCP_ERROR_STOP               7173
#define SCEDLL_SCP_ERROR_OPENFORSTOP        7174
#define SCEDLL_SCP_DISABLE_ADMIN            7175
#define SCEDLL_SCP_ENABLE_ADMIN             7176
#define SCEDLL_SCP_DISABLE_GUEST            7177
#define SCEDLL_SCP_ENABLE_GUEST             7178

#define SCEDLL_SAP_INIT_ERROR               7180
#define SCEDLL_SAP_INIT_SUCCESS             7181
#define SCEDLL_SAP_READ_PROFILE             7182

#define SCEDLL_SAP_BEGIN_POLICY             7183
#define SCEDLL_SAP_ACCESS_ERROR             7184
#define SCEDLL_SAP_ACCESS_SUCCESS           7185
#define SCEDLL_SAP_AUDIT_ERROR              7186
#define SCEDLL_SAP_AUDIT_SUCCESS            7187
#define SCEDLL_SAP_POLICY_ERROR             7188

#define SCEDLL_SAP_BEGIN_PRIVILEGES         7189
#define SCEDLL_SAP_PRIVILEGES_ERROR         7190
#define SCEDLL_SAP_PRIVILEGES_SUCCESS       7191

#define SCEDLL_SAP_BEGIN_USERSETTING        7192
#define SCEDLL_SAP_USERSETTING_ERROR        7193
#define SCEDLL_SAP_USERSETTING_SUCCESS      7194

#define SCEDLL_SAP_BEGIN_GROUPMGMT          7195
#define SCEDLL_SAP_GROUPMGMT_ERROR          7196
#define SCEDLL_SAP_GROUPMGMT_SUCCESS        7197

#define SCEDLL_SAP_BEGIN_REGISTRY           7198
#define SCEDLL_SAP_REGISTRY_ERROR           7199
#define SCEDLL_SAP_REGISTRY_SUCCESS         7200

#define SCEDLL_SAP_BEGIN_FILE               7201
#define SCEDLL_SAP_FILE_ERROR               7202
#define SCEDLL_SAP_FILE_SUCCESS             7203

#define SCEDLL_SAP_BEGIN_DS                 7204
#define SCEDLL_SAP_DS_ERROR                 7205
#define SCEDLL_SAP_DS_SUCCESS               7206

#define SCEDLL_SAP_BEGIN_GENERALSVC         7207
#define SCEDLL_SAP_GENERALSVC_ERROR         7208
#define SCEDLL_SAP_GENERALSVC_SUCCESS       7209

#define SCEDLL_SAP_BEGIN_ATTACHMENT         7210
#define SCEDLL_SAP_ATTACHMENT_ERROR         7211
#define SCEDLL_SAP_ATTACHMENT_SUCCESS       7212

#define SCEDLL_SAP_KERBEROS_ERROR           7213
#define SCEDLL_SAP_KERBEROS_SUCCESS         7214

#define SCEDLL_SAP_REGVALUES_ERROR          7215
#define SCEDLL_SAP_REGVALUES_SUCCESS        7216

#define SCEDLL_SAP_UNINIT                   7217

#define SCEDLL_SAP_ANALYZE                  7220
#define SCEDLL_SAP_ERROR_ACL                7221
#define SCEDLL_SAP_ERROR_ADD                7222
#define SCEDLL_SAP_ERROR_ADMINISTRATOR      7223
#define SCEDLL_SAP_ERROR_ANALYZE            7224
#define SCEDLL_SAP_ERROR_ENUMERATE          7225
#define SCEDLL_SAP_ERROR_EVENT_AUDITING     7226
#define SCEDLL_SAP_ERROR_GUEST              7227
#define SCEDLL_SAP_ERROR_KERBEROS           7228
#define SCEDLL_SAP_ERROR_LOCKOUT            7229
#define SCEDLL_SAP_ERROR_LOGOFF             7230
#define SCEDLL_SAP_ERROR_OUT                7231
#define SCEDLL_SAP_ERROR_PASSWORD           7232
#define SCEDLL_SAP_ERROR_SAVE               7233
#define SCEDLL_SAP_ERROR_SECURITY           7234
#define SCEDLL_SAP_ERROR_UAS                7235
#define SCEDLL_SAP_EVENT_AUDITING           7236
#define SCEDLL_SAP_IGNORE_TEMPLATE          7237
#define SCEDLL_SAP_KERBEROS                 7238
#define SCEDLL_SAP_LOCKOUT                  7239
#define SCEDLL_SAP_LOGOFF                   7240
#define SCEDLL_SAP_LOGSETTINGS              7241
#define SCEDLL_SAP_NOT_SUPPORT              7242
#define SCEDLL_SAP_OTHER_POLICY             7243
#define SCEDLL_SAP_PASSWORD                 7244
#define SCEDLL_SAP_START_SECTION            7245
#define SCEDLL_SAP_UAS                      7246
#define SCEDLL_SAP_ERROR_LSA_ANON_LOOKUP    7247

#define SCEDLL_ACCOUNT_DOMAIN               7250
#define SCEDLL_ADMIN_LOGON                  7251
#define SCEDLL_ADMINISTRATORS_SID           7252
#define SCEDLL_BEGIN_INIT                   7253
#define SCEDLL_BEGIN_RECOVERY               7254
#define SCEDLL_CANNOT_FIND                  7255
#define SCEDLL_CREATE_TABLE                 7256
#define SCEDLL_DELETE_TABLE                 7257
#define SCEDLL_ENGINE_STATUS_RESET          7258
#define SCEDLL_ERROR_ADJUST                 7259
#define SCEDLL_ERROR_ALIAS_MEMBER           7260
#define SCEDLL_ERROR_ALIAS_MEMBEROF         7261
#define SCEDLL_ERROR_ANALYZE_MEMBEROF       7262
#define SCEDLL_ERROR_ANALYZE_MEMBERS        7263
#define SCEDLL_ERROR_BUILD_SD               7264
#define SCEDLL_ERROR_COMPUTESD              7265
#define SCEDLL_ERROR_CONVERT                7266
#define SCEDLL_ERROR_CONVERT_LDAP           7267
#define SCEDLL_ERROR_CONVERT_SECTION        7268
#define SCEDLL_ERROR_CREATE                 7269
#define SCEDLL_ERROR_DELETE                 7270
#define SCEDLL_ERROR_GENERATE               7271
#define SCEDLL_ERROR_IGNORE_POLICY          7249
#define SCEDLL_ERROR_LOAD                   7272
#define SCEDLL_ERROR_LOOKUP                 7273
#define SCEDLL_ERROR_OPEN                   7274
#define SCEDLL_ERROR_PROCESS_UNICODE        7275
#define SCEDLL_ERROR_QUERY_EVENT_AUDITING   7276
#define SCEDLL_ERROR_QUERY_INFO             7277
#define SCEDLL_ERROR_QUERY_LOCKOUT          7278
#define SCEDLL_ERROR_QUERY_LOGOFF           7279
#define SCEDLL_ERROR_QUERY_LOGSETTINGS      7280
#define SCEDLL_ERROR_QUERY_PASSWORD         7281
#define SCEDLL_ERROR_QUERY_SECURITY         7282
#define SCEDLL_ERROR_QUERY_UAS              7283
#define SCEDLL_ERROR_QUERY_VOLUME           7284
#define SCEDLL_ERROR_SAVE_REGISTRY          7285
#define SCEDLL_ERROR_SET_INFO               7286
#define SCEDLL_ERROR_SET_SECURITY           7287
#define SCEDLL_ERROR_START_TRANS            7288
#define SCEDLL_ERROR_TAKE_OWNER             7289
#define SCEDLL_ERROR_USER_MEMBER            7290
#define SCEDLL_ERROR_WRITE_INFO             7291
#define SCEDLL_EVENT_IS_OFF                 7292
#define SCEDLL_EVENT_RESTORED               7293
#define SCEDLL_FIND_DBLOCATION              7294
#define SCEDLL_GENERATE_TEMPLATE            7295
#define SCEDLL_INVALID_GROUP                7296
#define SCEDLL_INVALID_PATH                 7297
#define SCEDLL_INVALID_SECURITY             7298
#define SCEDLL_INVALID_USER                 7299
#define SCEDLL_LOAD_ATTACHMENT              7300
#define SCEDLL_LOGFILE_INVALID              7301
#define SCEDLL_LSA_POLICY                   7302
#define SCEDLL_NO_ACL_SUPPORT               7303
#define SCEDLL_NO_DETAIL                    7304
#define SCEDLL_NO_MAPPINGS                  7305
#define SCEDLL_NOT_GROUP                    7306
#define SCEDLL_PROCESS_TEMPLATE             7307
#define SCEDLL_SEPARATOR                    7308
#define SCEDLL_TAB_SEPARATOR                7309
#define SCEDLL_TIMESTAMP_ERROR              7310
#define SCEDLL_TOTAL_TICKS_ERROR            7311
#define SCEDLL_UNKNOWN_DBLOCATION           7312
#define SCEDLL_UNKNOWN_LOGON_USER           7313
#define SCEDLL_USER_OBJECT                  7314
#define SCEDLL_USERRIGHT_NOT_DEFINED        7315
#define SCEDLL_PROCESS                      7316
#define SCEDLL_COPY_LOCAL                   7317
#define SCEDLL_ERROR_COPY                   7318
#define SCEDLL_ERROR_DELETE_DB              7319
#define SCEDLL_ERROR_RECOVER_DB             7320

#define SCEDLL_STATUS_ERROR                 7321
#define SCEDLL_STATUS_MATCH                 7322
#define SCEDLL_STATUS_MISMATCH              7323
#define SCEDLL_STATUS_NC                    7324
#define SCEDLL_STATUS_NEW                   7325
#define SCEDLL_STATUS_NOACL                 7326

#define SCESRV_ALIAS_NAME_SERVER_OPS        7370
#define SCESRV_ALIAS_NAME_ACCOUNT_OPS       7371
#define SCESRV_ALIAS_NAME_PRINT_OPS         7372
#define SCESRV_ALIAS_CREATE                 7373
#define SCESRV_ALIAS_UNSUPPORTED            7374

#define SCEDLL_COPY_DEFAULT                 7380
#define SCEDLL_ERROR_COPY_DEFAULT           7381

#define IDS_WARNING                         7385
#define IDS_ERROR                           7386

#define IDS_REGISTRY_DELAY_FILTER           7387
#define IDS_ERROR_LOOKUP                    7388
#define IDS_ERROR_CONVERT_SID               7389
#define IDS_COPY_DOMAIN_GPO                 7390
#define IDS_COPY_OU_GPO                     7391
#define IDS_COPY_PRIVILEGE_UPGRADE          7392
#define IDS_COPY_PRIVILEGE_FRESH            7393
#define IDS_ERROR_GET_PROCADDR              7394
#define IDS_COPY_ONE_PRIVILEGE              7395

#define IDS_NO_ANALYSIS                     7396
#define IDS_NO_ANALYSIS_FRESH               7397
#define IDS_ANALYSIS_MISMATCH               7398
#define IDS_ERROR_OPEN_LOG                  7399
#define IDS_ERROR_BIND                      7400
#define IDS_FAIL                            7401

#define SCESRV_POLICY_TATTOO_PREPARE        7405
#define SCESRV_POLICY_TATTOO_ERROR_RESET    7406
#define SCESRV_POLICY_TATTOO_CHECK          7407
#define SCESRV_POLICY_TATTOO_ERROR_CREATE   7408
#define SCESRV_POLICY_TATTOO_ERROR_SETTING  7409
#define SCESRV_POLICY_TATTOO_ERROR_QUERY    7410
#define SCESRV_POLICY_TATTOO_REMOVE_SETTING 7411
#define SCESRV_POLICY_TATTOO_EXIST          7412
#define SCESRV_POLICY_TATTOO_NONEXIST       7413
#define SCESRV_POLICY_TATTOO_QUERY          7414
#define SCESRV_POLICY_TATTOO_ADD            7415
#define SCESRV_POLICY_TATTOO_ERROR_REMOVE   7416
#define SCESRV_POLICY_TATTOO_ARRAY          7417

#define SCEDLL_SCP_BEGIN_REGISTRY_64KEY     7418
#define SCEDLL_SCP_BEGIN_REGISTRY_32KEY     7419

#define SCESRV_ENFORCE_DENY_LOCAL_RIGHT     7420
#define SCESRV_ENFORCE_DENY_NETWORK_RIGHT   7421
#define SCESRV_ENFORCE_NETWORK_RIGHT        7422
#define SCESRV_ENFORCE_LOCAL_RIGHT          7423
#define SCESRV_ERROR_ENFORCE_NETWORK_RIGHT  7424
#define SCESRV_ERROR_ENFORCE_LOCAL_RIGHT    7425

#define SCESRV_POLICY_PENDING_SAM           7430
#define SCESRV_POLICY_PENDING_AUDIT         7431
#define SCESRV_POLICY_PENDING_REMOVE_RIGHTS 7432
#define SCESRV_POLICY_PENDING_RIGHTS        7433
#define SCESRV_POLICY_PENDING_QUERY         7434
#define SCESRV_ERROR_PRIVATE_LSA            7435
#define SCESRV_ERROR_QUERY_ACCOUNT_RIGHTS   7436

#define IDS_ERROR_SAVE_POLICY_GPO_ACCOUNT   7437
#define IDS_ERROR_SAVE_POLICY_GPO_OTHER     7438
#define SCESRV_POLICY_ERROR_SPLAY_INITIALIZE  7439

#define SCEDLL_CONVERT_ROOT_NON_NTFS            7450
#define SCEDLL_CONVERT_ROOT_ERROR_QUERY_VOLUME  7451
#define SCEDLL_CONVERT_ROOT_NOT_FIXED_VOLUME    7452
#define SCEDLL_CONVERT_ROOT_NTFS_VOLUME         7453
#define SCEDLL_CONVERT_ERROR_PROFILES_DIR       7454
#define SCEDLL_CONVERT_SUCCESS_PROFILES_DIR     7455
#define SCEDLL_CONVERT_ERROR_TEMPLATE_APPLY     7456
#define SCEDLL_CONVERT_SUCCESS_TEMPLATE_APPLY   7457
#define SCEDLL_CONVERT_ERROR_DACL               7458
#define SCEDLL_CONVERT_ERROR_MARTA              7459
#define SCEDLL_CONVERT_ERROR_EVENT_WAIT         7460
#define SCEDLL_CONVERT_SUCCESS_EVENT_WAIT       7461
#define SCEDLL_CONVERT_SUCCESS_REGVAL_QUERY     7462
#define SCEDLL_CONVERT_ERROR_REGVAL_QUERY       7463
#define SCEDLL_CONVERT_OUT_OF_MEMORY            7464
#define SCEDLL_CONVERT_NOTEXIST_REGVAL          7465
#define SCEDLL_CONVERT_STATUS_CREATING_THREAD    7466
#define SCEDLL_CONVERT_SUCCESS_MARTA       7467

#define IDS_ERROR_CONVERT_PARAMETER         7470
#define IDS_ERROR_CONVERT_BAD_ENV_VAR       7471
#define IDS_INFO_CONVERT_DRIVE              7472
#define IDS_ERROR_CONVERT_PROD_TYPE         7473

#define SCEDLL_CONVERT_BAD_ENV_VAR          7474
#define SCEDLL_CONVERT_ERROR_DLL_FUNCTION   7475
#define SCEDLL_CONVERT_ERROR_DLL_LOAD       7476
#define SCEDLL_CONVERT_ERROR_EVENT_HANDLE   7477
#define SCEDLL_CONVERT_PROD_TYPE    7478

#define SCEDLL_ERROR_QUERYING_ACCOUNT_RIGHTS 7479
#define SCESRV_POLICY_TATTOO_NOT_REMOVE_MEMBERS  7480
#define IDS_FREE_DISK_SPACE                  7481

#define SCEDLL_SCP_ERROR_LSAPOLICY_QUERY        7490
#define SCEDLL_SCP_ERROR_LSAPOLICY_SET          7491
#define SCEDLL_SCP_ERROR_LSAPOLICY_BUILDDACL    7492
#define SCEDLL_SCP_ERROR_LSAPOLICY_SD_INIT      7493
#define SCEDLL_SCP_ERROR_LSAPOLICY_AUTHZ        7494
#define SCEDLL_SCP_INFO_LSAPOLICY_EXISTING_SDDL       7495
#define SCEDLL_SCP_INFO_LSAPOLICY_COMPUTED_SDDL       7496

#define SCEDLL_SCP_LSAPOLICY    7497
#define SCEDLL_SAP_LSAPOLICY    7498

#define SCEDLL_SCP_ERROR_DISABLE_ADMIN      7500
#define SCEDLL_SCP_ERROR_DISABLE_GUEST      7501
#define SCEDLL_SAP_ERROR_DISABLE_ADMIN      7502
#define SCEDLL_SAP_ERROR_DISABLE_GUEST      7503

#define SCEDLL_CANNOT_FIND_INDS             7505
#define SCEDLL_EMPTY_MEMBERSHIP             7506
#define SCEDLL_SCP_ERROR_NOREMOVE           7507

#define SCESRV_SETUPUPD_ERROR_LMCOMPAT      7508
#define SCESRV_SETUPUPD_IGNORE_LMCOMPAT     7509

#endif


