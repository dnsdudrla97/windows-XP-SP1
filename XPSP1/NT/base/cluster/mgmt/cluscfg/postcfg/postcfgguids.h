//////////////////////////////////////////////////////////////////////////////
//
//  Copyright (c) 2001 Microsoft Corporation
//
//  Module Name:
//      PostCfgGuids.h
//
//  Description:
//      This file contains the guids used in ClusCfgServer.
//
//  Documentation:
//
//  Maintained By:
//      Galen Barbee (GalenB) 02-FEB-2001
//
//////////////////////////////////////////////////////////////////////////////


// Make sure that this file is included only once per compile path.
#pragma once

//////////////////////////////////////////////////////////////////////////////
// Include Files
//////////////////////////////////////////////////////////////////////////////
#include <InitGuid.h>


//////////////////////////////////////////////////////////////////////////////
// Constant Declarations
//////////////////////////////////////////////////////////////////////////////


// {EB14D973-5260-42c8-BDC5-2C463EFC3BFE}
DEFINE_GUID( TASKID_Minor_Init_MapResTypeGuidToName,
0xeb14d973, 0x5260, 0x42c8, 0xbd, 0xc5, 0x2c, 0x46, 0x3e, 0xfc, 0x3b, 0xfe);

// {E6B4712F-D3C5-4a23-A13E-82249CED7263}
DEFINE_GUID( TASKID_Minor_Init_OutOfMemory,
0xe6b4712f, 0xd3c5, 0x4a23, 0xa1, 0x3e, 0x82, 0x24, 0x9c, 0xed, 0x72, 0x63);

// {0B01B239-72AC-42cd-8083-616E1F79A852}
DEFINE_GUID( TASKID_Minor_Initialize_QI,
0xb01b239, 0x72ac, 0x42cd, 0x80, 0x83, 0x61, 0x6e, 0x1f, 0x79, 0xa8, 0x52);

// {145C7029-B56F-41d3-A667-7B1ADEE65C39}
DEFINE_GUID( TASKID_Minor_CommitChanges_QI_Resources,
0x145c7029, 0xb56f, 0x41d3, 0xa6, 0x67, 0x7b, 0x1a, 0xde, 0xe6, 0x5c, 0x39);

// {B3C49A4C-41F2-40bc-870D-30AA31C5D44E}
DEFINE_GUID( TASKID_Minor_CommitChanges_QI_ClusterInfo,
0xb3c49a4c, 0x41f2, 0x40bc, 0x87, 0xd, 0x30, 0xaa, 0x31, 0xc5, 0xd4, 0x4e);

// {770C99BD-2127-481a-A529-8E194CD206A6}
DEFINE_GUID( TASKID_Minor_CommitChanges_GetCommitMode,
0x770c99bd, 0x2127, 0x481a, 0xa5, 0x29, 0x8e, 0x19, 0x4c, 0xd2, 0x6, 0xa6);

// {F4367721-F216-4a1a-807E-A17EDE71B7F4}
DEFINE_GUID( TASKID_Minor_CommitChanges_CoCreate_ResTypeService,
0xf4367721, 0xf216, 0x4a1a, 0x80, 0x7e, 0xa1, 0x7e, 0xde, 0x71, 0xb7, 0xf4);

// {B057ACFF-74C6-4129-A2A8-942D55393FAF}
DEFINE_GUID( TASKID_Minor_CommitChanges_SetParameters,
0xb057acff, 0x74c6, 0x4129, 0xa2, 0xa8, 0x94, 0x2d, 0x55, 0x39, 0x3f, 0xaf);

// {83149573-151E-4bb6-8594-28EFDC058E48}
DEFINE_GUID( TASKID_Minor_CommitChanges_OpenCluster,
0x83149573, 0x151e, 0x4bb6, 0x85, 0x94, 0x28, 0xef, 0xdc, 0x5, 0x8e, 0x48);

// {1744FBF0-9B5D-4846-98B2-5B95C86B0D29}
DEFINE_GUID( TASKID_Minor_PreCreate_Reset,
0x1744fbf0, 0x9b5d, 0x4846, 0x98, 0xb2, 0x5b, 0x95, 0xc8, 0x6b, 0xd, 0x29);

// {A4D1F1EE-71C4-4a10-AA4E-DEC05F37A0F2}
DEFINE_GUID( TASKID_Minor_PreCreate_CPreCreateServices,
0xa4d1f1ee, 0x71c4, 0x4a10, 0xaa, 0x4e, 0xde, 0xc0, 0x5f, 0x37, 0xa0, 0xf2);

// {36A14852-D2B2-460d-A5F4-52A244E19ADE}
DEFINE_GUID( TASKID_Minor_PreCreate_CPreCreateServices_QI,
0x36a14852, 0xd2b2, 0x460d, 0xa5, 0xf4, 0x52, 0xa2, 0x44, 0xe1, 0x9a, 0xde);

// {D3C86111-25F2-49fe-A1F9-91622A120E9C}
DEFINE_GUID( TASKID_Minor_PreCreate_LoadString_Querying,
0xd3c86111, 0x25f2, 0x49fe, 0xa1, 0xf9, 0x91, 0x62, 0x2a, 0x12, 0xe, 0x9c);

// {C9EF05D3-235C-49ae-B237-0C83F69F125C}
DEFINE_GUID( TASKID_Minor_PreCreate_EnumResources_Next,
0xc9ef05d3, 0x235c, 0x49ae, 0xb2, 0x37, 0xc, 0x83, 0xf6, 0x9f, 0x12, 0x5c);

// {2C02D6FB-8847-4b78-B9A8-BA5BCB81A77E}
DEFINE_GUID( TASKID_Minor_PreCreate_EnumResources_GetName,
0x2c02d6fb, 0x8847, 0x4b78, 0xb9, 0xa8, 0xba, 0x5b, 0xcb, 0x81, 0xa7, 0x7e);

// {5BCE21E7-2FE0-4a52-986A-3CBA1B4A5895}
DEFINE_GUID( TASKID_Minor_PreCreate_EnumResources_IsManaged,
0x5bce21e7, 0x2fe0, 0x4a52, 0x98, 0x6a, 0x3c, 0xba, 0x1b, 0x4a, 0x58, 0x95);

// {9DAA8CDA-1004-4543-BCFF-4ECF774AA8A7}
DEFINE_GUID( TASKID_Minor_PreCreate_EnumResources_IsManaged_False,
0x9daa8cda, 0x1004, 0x4543, 0xbc, 0xff, 0x4e, 0xcf, 0x77, 0x4a, 0xa8, 0xa7);

// {A01A352F-1E7B-4bd8-B160-A273ECA8CE6E}
DEFINE_GUID( TASKID_Minor_PreCreate_EnumResources_QI_pccmrc,
0xa01a352f, 0x1e7b, 0x4bd8, 0xb1, 0x60, 0xa2, 0x73, 0xec, 0xa8, 0xce, 0x6e);

// {53F01DD7-99BB-46e7-9859-7E582E7AECEB}
DEFINE_GUID( TASKID_PreCreate_EnumResources_IsQuorumDevice_S_OK,
0x53f01dd7, 0x99bb, 0x46e7, 0x98, 0x59, 0x7e, 0x58, 0x2e, 0x7a, 0xec, 0xeb);

// {E632F148-25B3-4108-931B-DE6D1B990290}
DEFINE_GUID( TASKID_PreCreate_EnumResources_IsQuorumDevice_Failed,
0xe632f148, 0x25b3, 0x4108, 0x93, 0x1b, 0xde, 0x6d, 0x1b, 0x99, 0x2, 0x90);

// {E466C438-35C6-4d4b-A5ED-31E5AD12F7A7}
DEFINE_GUID( TASKID_PreCreate_EnumResources_SetAssociatedResouce,
0xe466c438, 0x35c6, 0x4d4b, 0xa5, 0xed, 0x31, 0xe5, 0xad, 0x12, 0xf7, 0xa7);

// {B54ED2B2-4762-4d33-A8D1-191DD3933758}
DEFINE_GUID( TASKID_PreCreate_EnumResources_SetName,
0xb54ed2b2, 0x4762, 0x4d33, 0xa8, 0xd1, 0x19, 0x1d, 0xd3, 0x93, 0x37, 0x58);

// {A7E8FD12-9B3F-44d0-82A9-2400164087EC}
DEFINE_GUID( TASKID_PreCreate_EnumResources_SetEntry,
0xa7e8fd12, 0x9b3f, 0x44d0, 0x82, 0xa9, 0x24, 0x0, 0x16, 0x40, 0x87, 0xec);

// {D2E0B087-CE86-4ceb-9295-DCA723994925}
DEFINE_GUID( TASKID_Minor_PreCreate_PreCreate_E_NOTIMPL,
0xd2e0b087, 0xce86, 0x4ceb, 0x92, 0x95, 0xdc, 0xa7, 0x23, 0x99, 0x49, 0x25);

// {05F1200D-2F10-435d-95E4-C45C81E186EF}
DEFINE_GUID( TASKID_Minor_PreCreate_PreCreate_Failed,
0x5f1200d, 0x2f10, 0x435d, 0x95, 0xe4, 0xc4, 0x5c, 0x81, 0xe1, 0x86, 0xef);

// {86E8D0D2-64F7-426d-9E96-C862A48BF319}
DEFINE_GUID( TASKID_Minor_PreCreate_LoadString_Resource_Failed,
0x86e8d0d2, 0x64f7, 0x426d, 0x9e, 0x96, 0xc8, 0x62, 0xa4, 0x8b, 0xf3, 0x19);

// {ADA0F04E-D162-4cb8-A1E1-6695C6A2D010}
DEFINE_GUID( TASKID_Minor_PreCreate_Succeeded,
0xada0f04e, 0xd162, 0x4cb8, 0xa1, 0xe1, 0x66, 0x95, 0xc6, 0xa2, 0xd0, 0x10);

// {389D9670-1408-4cca-A527-E5EAD5680B43}
DEFINE_GUID( TASKID_Minor_PreCreate_Finished,
0x389d9670, 0x1408, 0x4cca, 0xa5, 0x27, 0xe5, 0xea, 0xd5, 0x68, 0xb, 0x43);

// {3B387B9F-9D23-49ff-A010-5E1A2C618663}
DEFINE_GUID( TASKID_Minor_CreateGroups_Begin,
0x3b387b9f, 0x9d23, 0x49ff, 0xa0, 0x10, 0x5e, 0x1a, 0x2c, 0x61, 0x86, 0x63);

// {625DE61F-1678-4e97-A7E6-17578AD106B4}
DEFINE_GUID( TASKID_Minor_CreateGroups_GetCountOfTypeDependencies,
0x625de61f, 0x1678, 0x4e97, 0xa7, 0xe6, 0x17, 0x57, 0x8a, 0xd1, 0x6, 0xb4);

// {26E49D7F-15EF-40c3-8792-537328FF9E1D}
DEFINE_GUID( TASKID_Minor_CreateGroups_GetTypeDependencyPtr,
0x26e49d7f, 0x15ef, 0x40c3, 0x87, 0x92, 0x53, 0x73, 0x28, 0xff, 0x9e, 0x1d);

// {ECEFA23F-6A16-4fa4-83B7-B9EBE1566C66}
DEFINE_GUID( TASKID_Minor_CreateGroups_FormatString_NetName,
0xecefa23f, 0x6a16, 0x4fa4, 0x83, 0xb7, 0xb9, 0xeb, 0xe1, 0x56, 0x6c, 0x66);

// {4AE83FA3-CA8D-4997-97D0-2E8C43A68314}
DEFINE_GUID( TASKID_Minor_CreateGroups_AddTypeDependency,
0x4ae83fa3, 0xca8d, 0x4997, 0x97, 0xd0, 0x2e, 0x8c, 0x43, 0xa6, 0x83, 0x14);

// {0527E435-5B42-4a2c-9334-EA918C3892E0}
DEFINE_GUID( TASKID_Minor_CreateGroups_FormatString_IPAddress,
0x527e435, 0x5b42, 0x4a2c, 0x93, 0x34, 0xea, 0x91, 0x8c, 0x38, 0x92, 0xe0);

// {13F32529-0BA2-4ff6-84FE-7982D4002D64}
DEFINE_GUID( TASKID_Minor_CreateGroups_NetName_AddDependent,
0x13f32529, 0xba2, 0x4ff6, 0x84, 0xfe, 0x79, 0x82, 0xd4, 0x0, 0x2d, 0x64);

// {1C3A7C27-E6E7-4a39-B630-70B8B4D6E6D8}
DEFINE_GUID( TASKID_Minor_CreateGroups_IPAddress_AddDependent,
0x1c3a7c27, 0xe6e7, 0x4a39, 0xb6, 0x30, 0x70, 0xb8, 0xb4, 0xd6, 0xe6, 0xd8);

// {022D8BF0-93EB-4dc9-B11E-3772B8BE43B5}
DEFINE_GUID( TASKID_Minor_CreateGroups_QuorumDisk_AddDependent,
0x22d8bf0, 0x93eb, 0x4dc9, 0xb1, 0x1e, 0x37, 0x72, 0xb8, 0xbe, 0x43, 0xb5);

// {5B9958C4-DC00-474f-86CB-8B893F0FE8FB}
DEFINE_GUID( TASKID_Minor_CreateGroups_GetTypePtr,
0x5b9958c4, 0xdc00, 0x474f, 0x86, 0xcb, 0x8b, 0x89, 0x3f, 0xf, 0xe8, 0xfb);

// {9B69E548-0334-4c4b-888E-12574174B99D}
DEFINE_GUID( TASKID_Minor_CreateGroups_GetClassTypePtr,
0x9b69e548, 0x334, 0x4c4b, 0x88, 0x8e, 0x12, 0x57, 0x41, 0x74, 0xb9, 0x9d);

// {0AA1F2BD-CC88-4bf5-87B3-A1953C731AC1}
DEFINE_GUID( TASKID_Minor_CreateGroups_GetFlags,
0xaa1f2bd, 0xcc88, 0x4bf5, 0x87, 0xb3, 0xa1, 0x95, 0x3c, 0x73, 0x1a, 0xc1);

// {7B64039A-79B2-4086-ABF2-5E1520DACE8C}
DEFINE_GUID( TASKID_Minor_CreateGroups_SetFlags,
0x7b64039a, 0x79b2, 0x4086, 0xab, 0xf2, 0x5e, 0x15, 0x20, 0xda, 0xce, 0x8c);

// {4FB598B1-D9EE-4f75-B274-B639941839A3}
DEFINE_GUID( TASKID_Minor_CreateGroups_Resource_AddDependent,
0x4fb598b1, 0xd9ee, 0x4f75, 0xb2, 0x74, 0xb6, 0x39, 0x94, 0x18, 0x39, 0xa3);

// {E7ECD079-1AC8-43c5-A577-0826500A4262}
DEFINE_GUID( TASKID_Minor_CreateGroups_GetName,
0xe7ecd079, 0x1ac8, 0x43c5, 0xa5, 0x77, 0x8, 0x26, 0x50, 0xa, 0x42, 0x62);

// {04989473-AE35-4a5d-A35F-C502C1E87E8B}
DEFINE_GUID( TASKID_Minor_CreateGroups_MissingDependent,
0x4989473, 0xae35, 0x4a5d, 0xa3, 0x5f, 0xc5, 0x2, 0xc1, 0xe8, 0x7e, 0x8b);

// {CA40367F-04AB-44d1-86C9-7E28318AD868}
DEFINE_GUID( TASKID_Minor_CreateGroups_GetAssociateResource,
0xca40367f, 0x4ab, 0x44d1, 0x86, 0xc9, 0x7e, 0x28, 0x31, 0x8a, 0xd8, 0x68);

// {0FE4B1B0-24C1-46c5-87E5-DE6DF8446895}
DEFINE_GUID( TASKID_Minor_CreateGroups_QI_pccmri,
0xfe4b1b0, 0x24c1, 0x46c5, 0x87, 0xe5, 0xde, 0x6d, 0xf8, 0x44, 0x68, 0x95);

// {677F301F-74A9-4176-9D89-7A505AD2DFE6}
DEFINE_GUID( TASKID_Minor_CreateGroups_SetManaged,
0x677f301f, 0x74a9, 0x4176, 0x9d, 0x89, 0x7a, 0x50, 0x5a, 0xd2, 0xdf, 0xe6);

// {CA048D82-325F-4566-904C-99FB67BE46B9}
DEFINE_GUID( TASKID_Minor_CreateGroups_Creating,
0xca048d82, 0x325f, 0x4566, 0x90, 0x4c, 0x99, 0xfb, 0x67, 0xbe, 0x46, 0xb9);

// {A4B491D2-DB89-461e-8423-59A9A6BFE327}
DEFINE_GUID( TASKID_Minor_CreateGroups_Open_ClusterGroup,
0xa4b491d2, 0xdb89, 0x461e, 0x84, 0x23, 0x59, 0xa9, 0xa6, 0xbf, 0xe3, 0x27);

// {6E6830D6-E2A4-4324-BE48-DE7874ABA52E}
DEFINE_GUID( TASKID_Minor_CreateGroups_OpenClusterGroup,
0x6e6830d6, 0xe2a4, 0x4324, 0xbe, 0x48, 0xde, 0x78, 0x74, 0xab, 0xa5, 0x2e);

// {413D510B-73BF-4230-AE1F-A5A8963B5401}
DEFINE_GUID( TASKID_Minor_CreateGroups_Create_CGroupHandle,
0x413d510b, 0x73bf, 0x4230, 0xae, 0x1f, 0xa5, 0xa8, 0x96, 0x3b, 0x54, 0x1);

// {7029A765-0ED6-49d3-952B-24974ACA3FA2}
DEFINE_GUID( TASKID_Minor_CreateGroups_GetCountOfTypeDependencies2,
0x7029a765, 0xed6, 0x49d3, 0x95, 0x2b, 0x24, 0x97, 0x4a, 0xca, 0x3f, 0xa2);

// {265219CA-B5C3-4978-A9C6-254C5EDF9654}
DEFINE_GUID( TASKID_Minor_CreateGroups_FormatString_Group,
0x265219ca, 0xb5c3, 0x4978, 0xa9, 0xc6, 0x25, 0x4c, 0x5e, 0xdf, 0x96, 0x54);

// {D302B68A-0743-4f66-9258-2DE440BB8092}
DEFINE_GUID( TASKID_Minor_CreateGroups_CreateClusterGroup,
0xd302b68a, 0x743, 0x4f66, 0x92, 0x58, 0x2d, 0xe4, 0x40, 0xbb, 0x80, 0x92);

// {E8CFA468-ED52-415c-A682-311648AA7026}
DEFINE_GUID( TASKID_Minor_CreateGroups_OnlineClusterGroup,
0xe8cfa468, 0xed52, 0x415c, 0xa6, 0x82, 0x31, 0x16, 0x48, 0xaa, 0x70, 0x26);

// {42F93B4E-0AB4-4851-983A-D792A5D24094}
DEFINE_GUID( TASKID_Minor_CreateGroups_Create_CGroupHandle2,
0x42f93b4e, 0xab4, 0x4851, 0x98, 0x3a, 0xd7, 0x92, 0xa5, 0xd2, 0x40, 0x94);

// {3122D49C-9FA4-44a1-8DC3-FFBF79AE630C}
DEFINE_GUID( TASKID_Minor_CreateGroups_Created,
0x3122d49c, 0x9fa4, 0x44a1, 0x8d, 0xc3, 0xff, 0xbf, 0x79, 0xae, 0x63, 0xc);

// {9AEA3DE8-220A-4a76-8A70-B3271EFF63BA}
DEFINE_GUID( TASKID_Minor_CreateGroups_Finished,
0x9aea3de8, 0x220a, 0x4a76, 0x8a, 0x70, 0xb3, 0x27, 0x1e, 0xff, 0x63, 0xba);

// {AF9FE752-2F27-473a-A7B0-86CEFDE922CA}
DEFINE_GUID( TASKID_Minor_CreateResources_LoadString_Creating,
0xaf9fe752, 0x2f27, 0x473a, 0xa7, 0xb0, 0x86, 0xce, 0xfd, 0xe9, 0x22, 0xca);

// {A9282CC8-F6C9-4b38-BADE-2F1CEE657E7B}
DEFINE_GUID( TASKID_Minor_CreateResources_Starting,
0xa9282cc8, 0xf6c9, 0x4b38, 0xba, 0xde, 0x2f, 0x1c, 0xee, 0x65, 0x7e, 0x7b);

// {3F08ABDA-13FD-4f02-B4B4-925FF239C206}
DEFINE_GUID( TASKID_Minor_CreateResources_Finished,
0x3f08abda, 0x13fd, 0x4f02, 0xb4, 0xb4, 0x92, 0x5f, 0xf2, 0x39, 0xc2, 0x6);

// {93779961-39A9-4cdf-BECC-04F1410640D5}
DEFINE_GUID( TASKID_Minor_PostCreateResources_LoadString_Starting,
0x93779961, 0x39a9, 0x4cdf, 0xbe, 0xcc, 0x4, 0xf1, 0x41, 0x6, 0x40, 0xd5);

// {997EEAB6-01F1-45b9-BC96-E2E728E32CCC}
DEFINE_GUID( TASKID_Minor_PostCreateResources_Starting,
0x997eeab6, 0x1f1, 0x45b9, 0xbc, 0x96, 0xe2, 0xe7, 0x28, 0xe3, 0x2c, 0xcc);

// {ABE9C1C8-13CC-44dc-B4D3-8B803DE38358}
DEFINE_GUID( TASKID_Minor_PostCreateResources_Finished,
0xabe9c1c8, 0x13cc, 0x44dc, 0xb4, 0xd3, 0x8b, 0x80, 0x3d, 0xe3, 0x83, 0x58);

// {4F5EAD56-5172-4b2a-97A8-6812ADAE2F9C}
DEFINE_GUID( TASKID_Minor_FindNextSharedStorage_GetClassTypePtr,
0x4f5ead56, 0x5172, 0x4b2a, 0x97, 0xa8, 0x68, 0x12, 0xad, 0xae, 0x2f, 0x9c);

// {EFB1E60B-2967-43ea-8309-41912038B7A7}
DEFINE_GUID( TASKID_Minor_FindNextSharedStorage_GetFlags,
0xefb1e60b, 0x2967, 0x43ea, 0x83, 0x9, 0x41, 0x91, 0x20, 0x38, 0xb7, 0xa7);

// {68984825-C701-4470-AD63-5BC2F336848E}
DEFINE_GUID( TASKID_Minor_HrAttemptToAssignStorageToResource_NoMoreStorage,
0x68984825, 0xc701, 0x4470, 0xad, 0x63, 0x5b, 0xc2, 0xf3, 0x36, 0x84, 0x8e);

// {EBCDD0EB-2073-4f14-BD27-90AB30A8E586}
DEFINE_GUID( TASKID_Minor_HrAttemptToAssignStorageToResource_SetFlags,
0xebcdd0eb, 0x2073, 0x4f14, 0xbd, 0x27, 0x90, 0xab, 0x30, 0xa8, 0xe5, 0x86);

// {2BCED0CA-CEFD-47c2-8740-AB02A6D3DBFB}
DEFINE_GUID( TASKID_Minor_HrAttemptToAssignStorageToResource_AddDependent,
0x2bced0ca, 0xcefd, 0x47c2, 0x87, 0x40, 0xab, 0x2, 0xa6, 0xd3, 0xdb, 0xfb);

// {FE025462-CFCE-4fed-ACB5-3DDBD9811AF0}
DEFINE_GUID( TASKID_Minor_HrMovedDependentsToAnotherResource_GetCountOfDependents,
0xfe025462, 0xcfce, 0x4fed, 0xac, 0xb5, 0x3d, 0xdb, 0xd9, 0x81, 0x1a, 0xf0);

// {A03679D1-5E6C-40d5-88D9-2B147DFFD000}
DEFINE_GUID( TASKID_Minor_HrMovedDependentsToAnotherResource_GetDependent,
0xa03679d1, 0x5e6c, 0x40d5, 0x88, 0xd9, 0x2b, 0x14, 0x7d, 0xff, 0xd0, 0x0);

// {741F73BA-8B43-464f-BBC3-C7AFB9E232A3}
DEFINE_GUID( TASKID_Minor_HrMovedDependentsToAnotherResource_AddDependent,
0x741f73ba, 0x8b43, 0x464f, 0xbb, 0xc3, 0xc7, 0xaf, 0xb9, 0xe2, 0x32, 0xa3);

// {31641A0B-AC8E-4856-AD83-811103EF4451}
DEFINE_GUID( TASKID_Minor_HrMovedDependentsToAnotherResource_ClearDependents,
0x31641a0b, 0xac8e, 0x4856, 0xad, 0x83, 0x81, 0x11, 0x3, 0xef, 0x44, 0x51);

// {401343DB-742F-493b-81C2-C272A01E9685}
DEFINE_GUID( TASKID_Minor_HrSetGroupOnResourceAndItsDependents_SetGroupHandle,
0x401343db, 0x742f, 0x493b, 0x81, 0xc2, 0xc2, 0x72, 0xa0, 0x1e, 0x96, 0x85);

// {CF60103B-2393-478e-82F0-BD1041097D9E}
DEFINE_GUID( TASKID_Minor_HrSetGroupOnResourceAndItsDependents_GetCountOfDependents,
0xcf60103b, 0x2393, 0x478e, 0x82, 0xf0, 0xbd, 0x10, 0x41, 0x9, 0x7d, 0x9e);

// {2179E00E-00AE-432b-8910-F9DF3B8C4701}
DEFINE_GUID( TASKID_Minor_HrSetGroupOnResourceAndItsDependents_GetDependent,
0x2179e00e, 0xae, 0x432b, 0x89, 0x10, 0xf9, 0xdf, 0x3b, 0x8c, 0x47, 0x1);

// {71C819A2-59D4-4a20-8A06-695BFC5A3B50}
DEFINE_GUID( TASKID_Minor_HrFindGroupFromResourceOrItsDependents_GetGroupHandle,
0x71c819a2, 0x59d4, 0x4a20, 0x8a, 0x6, 0x69, 0x5b, 0xfc, 0x5a, 0x3b, 0x50);

// {220CBC7E-F52C-4fb0-AC58-D6DAA7F3330E}
DEFINE_GUID( TASKID_Minor_HrFindGroupFromResourceOrItsDependents_Create_CGroupHandle,
0x220cbc7e, 0xf52c, 0x4fb0, 0xac, 0x58, 0xd6, 0xda, 0xa7, 0xf3, 0x33, 0xe);

// {F3E4E9A6-E114-4718-9EC7-1DB72154BA6B}
DEFINE_GUID( TASKID_Minor_HrFindGroupFromResourceOrItsDependents_OpenClusterGroup,
0xf3e4e9a6, 0xe114, 0x4718, 0x9e, 0xc7, 0x1d, 0xb7, 0x21, 0x54, 0xba, 0x6b);

// {348C670F-06C7-41f8-8EFB-15C2F8DD4E11}
DEFINE_GUID( TASKID_Minor_HrFindGroupFromResourceOrItsDependents_GetClusterResourceState,
0x348c670f, 0x6c7, 0x41f8, 0x8e, 0xfb, 0x15, 0xc2, 0xf8, 0xdd, 0x4e, 0x11);

// {9961A93D-1112-4121-949D-7A43DAF77BD7}
DEFINE_GUID( TASKID_Minor_HrFindGroupFromResourceOrItsDependents_GetCountOfDependents,
0x9961a93d, 0x1112, 0x4121, 0x94, 0x9d, 0x7a, 0x43, 0xda, 0xf7, 0x7b, 0xd7);

// {84710A53-2DE0-49ac-8BA1-3202F3FAC6E8}
DEFINE_GUID( TASKID_Minor_HrFindGroupFromResourceOrItsDependents_GetDependent,
0x84710a53, 0x2de0, 0x49ac, 0x8b, 0xa1, 0x32, 0x2, 0xf3, 0xfa, 0xc6, 0xe8);

// {3590BB47-0B80-4a7a-A27A-F27728742FA1}
DEFINE_GUID( TASKID_Minor_HrFindGroupFromResourceOrItsDependents_OutOfMemory,
0x3590bb47, 0xb80, 0x4a7a, 0xa2, 0x7a, 0xf2, 0x77, 0x28, 0x74, 0x2f, 0xa1);

// {D25F1C89-6452-42aa-93F9-93F0721CCF23}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_Create_CCreateServices,
0xd25f1c89, 0x6452, 0x42aa, 0x93, 0xf9, 0x93, 0xf0, 0x72, 0x1c, 0xcf, 0x23);

// {9F3C4CA9-CFCB-4e19-886C-4569172D4D9F}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_Create_CCreateServices_QI,
0x9f3c4ca9, 0xcfcb, 0x4e19, 0x88, 0x6c, 0x45, 0x69, 0x17, 0x2d, 0x4d, 0x9f);

// {A3555B22-732C-416a-BB51-51F21FD8DC09}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_SetEntry,
0xa3555b22, 0x732c, 0x416a, 0xbb, 0x51, 0x51, 0xf2, 0x1f, 0xd8, 0xdc, 0x9);

// {26699073-04DA-4b24-9138-B8FFA4CC25CA}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_IsConfigured,
0x26699073, 0x4da, 0x4b24, 0x91, 0x38, 0xb8, 0xff, 0xa4, 0xcc, 0x25, 0xca);

// {4BCEF420-01B6-4237-B0ED-E75F4E91C160}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_SetConfigured,
0x4bcef420, 0x1b6, 0x4237, 0xb0, 0xed, 0xe7, 0x5f, 0x4e, 0x91, 0xc1, 0x60);

// {C91BFB6C-5AF3-451b-886A-BC3C54B51F93}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_GetName,
0xc91bfb6c, 0x5af3, 0x451b, 0x88, 0x6a, 0xbc, 0x3c, 0x54, 0xb5, 0x1f, 0x93);

// {72058AEF-0B59-4eef-9EA0-37449B22636D}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_GetGroupHandle,
0x72058aef, 0xb59, 0x4eef, 0x9e, 0xa0, 0x37, 0x44, 0x9b, 0x22, 0x63, 0x6d);

// {D976AB04-40FD-4afb-A989-B52FE2EC1A45}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_GetHandle,
0xd976ab04, 0x40fd, 0x4afb, 0xa9, 0x89, 0xb5, 0x2f, 0xe2, 0xec, 0x1a, 0x45);

// {1F229FDE-D692-40bc-9DA9-C625033421D8}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_GetAssociatedResource,
0x1f229fde, 0xd692, 0x40bc, 0x9d, 0xa9, 0xc6, 0x25, 0x3, 0x34, 0x21, 0xd8);

// {3BB6DCD7-8BAC-4858-815E-F83D9625CE82}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_Create_Failed,
0x3bb6dcd7, 0x8bac, 0x4858, 0x81, 0x5e, 0xf8, 0x3d, 0x96, 0x25, 0xce, 0x82);

// {64B25912-4472-41d0-8DF9-5ED3407C9D42}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_LoadString_CreateFailed,
0x64b25912, 0x4472, 0x41d0, 0x8d, 0xf9, 0x5e, 0xd3, 0x40, 0x7c, 0x9d, 0x42);

// {34731F38-4B93-426c-9188-3463693D302D}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_GetTypePtr,
0x34731f38, 0x4b93, 0x426c, 0x91, 0x88, 0x34, 0x63, 0x69, 0x3d, 0x30, 0x2d);

// {D00DDDF7-4B8F-48d2-B5FC-AA174FF43B46}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_PcszLookupTypeNameByGUID,
0xd00dddf7, 0x4b8f, 0x48d2, 0xb5, 0xfc, 0xaa, 0x17, 0x4f, 0xf4, 0x3b, 0x46);

// {EC8EB861-EFDA-4d0d-9420-0C4EA69536FA}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_InvalidData_GetTypePtr,
0xec8eb861, 0xefda, 0x4d0d, 0x94, 0x20, 0xc, 0x4e, 0xa6, 0x95, 0x36, 0xfa);

// {A2BB1F77-F8C8-4658-B849-9A2CCAC9642D}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_NetworkName_QI_pccrc,
0xa2bb1f77, 0xf8c8, 0x4658, 0xb8, 0x49, 0x9a, 0x2c, 0xca, 0xc9, 0x64, 0x2d);

// {3812CC1D-103D-4ca9-8AE6-ADA8804EFBA5}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_NetworkName_SetPropertyString,
0x3812cc1d, 0x103d, 0x4ca9, 0x8a, 0xe6, 0xad, 0xa8, 0x80, 0x4e, 0xfb, 0xa5);

// {9F64F1C0-6B4E-41fd-9995-4C7331B064EA}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_IPAddress_QI_pccrc,
0x9f64f1c0, 0x6b4e, 0x41fd, 0x99, 0x95, 0x4c, 0x73, 0x31, 0xb0, 0x64, 0xea);

// {45455DEB-2B1B-4834-86D9-2018D382CF25}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_NothingNew,
0x45455deb, 0x2b1b, 0x4834, 0x86, 0xd9, 0x20, 0x18, 0xd3, 0x82, 0xcf, 0x25);

// {6FF6610B-7EF1-4dbd-A1B9-9959FB590AE6}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_Dependents_GetCountOfDependents,
0x6ff6610b, 0x7ef1, 0x4dbd, 0xa1, 0xb9, 0x99, 0x59, 0xfb, 0x59, 0xa, 0xe6);

// {1E32E142-10EC-4d17-ADF9-26B2826CE2E7}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_Dependents_GetDependent,
0x1e32e142, 0x10ec, 0x4d17, 0xad, 0xf9, 0x26, 0xb2, 0x82, 0x6c, 0xe2, 0xe7);

// {4AD23667-65C4-42cc-82EA-E1A5137536FC}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_Dependents_GetName,
0x4ad23667, 0x65c4, 0x42cc, 0x82, 0xea, 0xe1, 0xa5, 0x13, 0x75, 0x36, 0xfc);

// {43CE9A36-7CE7-4e22-87F7-D481F37867DB}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_Dependents_GetHResource,
0x43ce9a36, 0x7ce7, 0x4e22, 0x87, 0xf7, 0xd4, 0x81, 0xf3, 0x78, 0x67, 0xdb);

// {B861DFAF-DB48-4f28-A84E-9510B8F3976F}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_Dependents_AddClusterResourceDependency,
0xb861dfaf, 0xdb48, 0x4f28, 0xa8, 0x4e, 0x95, 0x10, 0xb8, 0xf3, 0x97, 0x6f);

// {111C9950-9E61-4733-80D5-06C58631D423}
DEFINE_GUID( TASKID_Minor_HrCreateResourceAndDependents_Dependents_Succeeded,
0x111c9950, 0x9e61, 0x4733, 0x80, 0xd5, 0x6, 0xc5, 0x86, 0x31, 0xd4, 0x23);

// {AFD6FEDC-BC4F-4cd6-8CFE-0D0458DF15DF}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_IsConfigured,
0xafd6fedc, 0xbc4f, 0x4cd6, 0x8c, 0xfe, 0xd, 0x4, 0x58, 0xdf, 0x15, 0xdf);

// {CA02A3CF-121A-4231-882F-CAF8DB761D25}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_SetConfigured,
0xca02a3cf, 0x121a, 0x4231, 0x88, 0x2f, 0xca, 0xf8, 0xdb, 0x76, 0x1d, 0x25);

// {079D7A3A-E23F-48d5-BF92-7D7A9FD273C6}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_GetName,
0x79d7a3a, 0xe23f, 0x48d5, 0xbf, 0x92, 0x7d, 0x7a, 0x9f, 0xd2, 0x73, 0xc6);

// {D7F92431-5E00-4f77-8A3F-70A7EF394127}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_OpenClusterResource,
0xd7f92431, 0x5e00, 0x4f77, 0x8a, 0x3f, 0x70, 0xa7, 0xef, 0x39, 0x41, 0x27);

// {DC2961C2-E31C-486d-8349-962FD670B688}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_OpenClusterResourcePending,
0xdc2961c2, 0xe31c, 0x486d, 0x83, 0x49, 0x96, 0x2f, 0xd6, 0x70, 0xb6, 0x88);

// {FA3957AC-F30C-4d67-8DBC-70D28C922A92}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_ClusterResourceStateUnknown,
0xfa3957ac, 0xf30c, 0x4d67, 0x8d, 0xbc, 0x70, 0xd2, 0x8c, 0x92, 0x2a, 0x92);

// {C09CF48A-3D3D-4012-83DA-7191C9237BAE}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_ClusterResourceOffline,
0xc09cf48a, 0x3d3d, 0x4012, 0x83, 0xda, 0x71, 0x91, 0xc9, 0x23, 0x7b, 0xae);

// {21A09A70-8EE2-4785-9DEC-97EDBEB45FE2}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_ClusterResourceFailed,
0x21a09a70, 0x8ee2, 0x4785, 0x9d, 0xec, 0x97, 0xed, 0xbe, 0xb4, 0x5f, 0xe2);

// {DCD49407-576F-48f3-8458-D5B3A2C382A6}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_OnlineClusterResource_Failed,
0xdcd49407, 0x576f, 0x48f3, 0x84, 0x58, 0xd5, 0xb3, 0xa2, 0xc3, 0x82, 0xa6);

// {6CE388C2-A01C-4b5a-8AAD-F4FCF5EAAF06}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_SetClusterQuorumResource,
0x6ce388c2, 0xa01c, 0x4b5a, 0x8a, 0xad, 0xf4, 0xfc, 0xf5, 0xea, 0xaf, 0x6);

// {BB0551C3-CDA5-405f-A261-35B27FA5E0A7}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_SetClusterQuorumResource_Succeeded,
0xbb0551c3, 0xcda5, 0x405f, 0xa2, 0x61, 0x35, 0xb2, 0x7f, 0xa5, 0xe0, 0xa7);

// {058BFA08-E687-410b-BD86-E42E269E82ED}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_FormatMessage_SetQuorum,
0x58bfa08, 0xe687, 0x410b, 0xbd, 0x86, 0xe4, 0x2e, 0x26, 0x9e, 0x82, 0xed);

// {E93C6B14-2F68-43c7-94DA-9543000F4D07}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_GetAssociatedResource,
0xe93c6b14, 0x2f68, 0x43c7, 0x94, 0xda, 0x95, 0x43, 0x0, 0xf, 0x4d, 0x7);

// {A771D973-E9BA-4266-BF49-7A6F8475CFAA}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_Create_CPostCreateServices,
0xa771d973, 0xe9ba, 0x4266, 0xbf, 0x49, 0x7a, 0x6f, 0x84, 0x75, 0xcf, 0xaa);

// {A991E33C-D8F6-4281-A1EF-BEFA5BDE0A07}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_Create_CPostCreateServices_QI_ppcr,
0xa991e33c, 0xd8f6, 0x4281, 0xa1, 0xef, 0xbe, 0xfa, 0x5b, 0xde, 0xa, 0x7);

// {2C9F4269-5FA0-4506-91EA-FD29224F0A09}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_SetEntry,
0x2c9f4269, 0x5fa0, 0x4506, 0x91, 0xea, 0xfd, 0x29, 0x22, 0x4f, 0xa, 0x9);

// {FD517153-B577-420d-A6E6-0B5DE9C4E351}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_PostCreate_E_NOTIMPL,
0xfd517153, 0xb577, 0x420d, 0xa6, 0xe6, 0xb, 0x5d, 0xe9, 0xc4, 0xe3, 0x51);

// {FAFA10F6-1DFD-46d8-BD64-A649B29BF411}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_PostCreate_Failed,
0xfafa10f6, 0x1dfd, 0x46d8, 0xbd, 0x64, 0xa6, 0x49, 0xb2, 0x9b, 0xf4, 0x11);

// {EDD4CEDC-89DA-4329-ADE8-705543137E14}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_FormatMessage_PostCreate_Failed,
0xedd4cedc, 0x89da, 0x4329, 0xad, 0xe8, 0x70, 0x55, 0x43, 0x13, 0x7e, 0x14);

// {6C7E2678-883F-4890-8F89-BE4219DA813A}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_FormatMessage_ResourcePostCreate_Failed,
0x6c7e2678, 0x883f, 0x4890, 0x8f, 0x89, 0xbe, 0x42, 0x19, 0xda, 0x81, 0x3a);

// {D45B9470-45BB-4add-B05C-4BB5DE303EE0}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_PostCreate_Succeeded,
0xd45b9470, 0x45bb, 0x4add, 0xb0, 0x5c, 0x4b, 0xb5, 0xde, 0x30, 0x3e, 0xe0);

// {0EFCF386-D992-4223-B87F-849F6F3C47BD}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_PostCreate_NotNeeded,
0xefcf386, 0xd992, 0x4223, 0xb8, 0x7f, 0x84, 0x9f, 0x6f, 0x3c, 0x47, 0xbd);

// {1733454E-5747-4831-BCA7-11E0671A4FBA}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_GetCountOfDependents,
0x1733454e, 0x5747, 0x4831, 0xbc, 0xa7, 0x11, 0xe0, 0x67, 0x1a, 0x4f, 0xba);

// {B0D040AB-7FB3-4404-AC3E-1B15C61B4D0F}
DEFINE_GUID( TASKID_Minor_HrPostCreateResourceAndDependents_GetDependent,
0xb0d040ab, 0x7fb3, 0x4404, 0xac, 0x3e, 0x1b, 0x15, 0xc6, 0x1b, 0x4d, 0xf);

// {FAFF1E59-9012-4143-AFCE-69B953FD4FDD}
DEFINE_GUID( TASKID_Minor_HrNotifyMemberSetChangeListeners_CoCreate_StdComponentCategoriesMgr,
0xfaff1e59, 0x9012, 0x4143, 0xaf, 0xce, 0x69, 0xb9, 0x53, 0xfd, 0x4f, 0xdd);

// {9A84E38B-52C1-4591-982E-D6515FCB90C9}
DEFINE_GUID( TASKID_Minor_HrNotifyMemberSetChangeListeners_EnumClassesOfCategories,
0x9a84e38b, 0x52c1, 0x4591, 0x98, 0x2e, 0xd6, 0x51, 0x5f, 0xcb, 0x90, 0xc9);

// {A93FE316-0894-4a80-83A3-880962450ADA}
DEFINE_GUID( TASKID_Minor_HrNotifyMemberSetChangeListeners_Next,
0xa93fe316, 0x894, 0x4a80, 0x83, 0xa3, 0x88, 0x9, 0x62, 0x45, 0xa, 0xda);

// {59ADF6E8-3EBC-463d-BABF-0D9F1B363D9F}
DEFINE_GUID( TASKID_Minor_HrNotifyMemberSetChangeListeners_HrProcessMemberSetChangeListener,
0x59adf6e8, 0x3ebc, 0x463d, 0xba, 0xbf, 0xd, 0x9f, 0x1b, 0x36, 0x3d, 0x9f);

// {6D8CF7D7-B09C-42dd-946F-0E7BBD702C35}
DEFINE_GUID( TASKID_Minor_HrProcessMemberSetChangeListener_CoCreate_Listener,
0x6d8cf7d7, 0xb09c, 0x42dd, 0x94, 0x6f, 0xe, 0x7b, 0xbd, 0x70, 0x2c, 0x35);

// {497FDB97-5A16-46e7-9A10-E8ABFDBD42D2}
DEFINE_GUID( TASKID_Minor_HrProcessMemberSetChangeListener_Notify,
0x497fdb97, 0x5a16, 0x46e7, 0x9a, 0x10, 0xe8, 0xab, 0xfd, 0xbd, 0x42, 0xd2);

// {085E2A1D-335E-46a8-9F71-B83998C59C77}
DEFINE_GUID( TASKID_Minor_HrConfigureResTypes_CoCreate_CategoriesMgr,
0x85e2a1d, 0x335e, 0x46a8, 0x9f, 0x71, 0xb8, 0x39, 0x98, 0xc5, 0x9c, 0x77);

// {BD3CA031-912E-430e-8B99-1E0E457EAA26}
DEFINE_GUID( TASKID_Minor_HrConfigureResTypes_Enum,
0xbd3ca031, 0x912e, 0x430e, 0x8b, 0x99, 0x1e, 0xe, 0x45, 0x7e, 0xaa, 0x26);

// {EF9DF26D-748A-4b89-87F8-760F6CC903C0}
DEFINE_GUID( TASKID_Minor_HrConfigureResTypes_Next,
0xef9df26d, 0x748a, 0x4b89, 0x87, 0xf8, 0x76, 0xf, 0x6c, 0xc9, 0x3, 0xc0);

// {A06BDBAA-CC10-4e04-8C45-646CFB55E56E}
DEFINE_GUID( TASKID_Minor_HrConfigureResTypes_HrProcessResType,
0xa06bdbaa, 0xcc10, 0x4e04, 0x8c, 0x45, 0x64, 0x6c, 0xfb, 0x55, 0xe5, 0x6e);

// {13915CAB-E9E3-4e2c-8AEA-2EDDB6FB0589}
DEFINE_GUID( TASKID_Minor_HrProcessResType_CoCreate_ResTypeClsid,
0x13915cab, 0xe9e3, 0x4e2c, 0x8a, 0xea, 0x2e, 0xdd, 0xb6, 0xfb, 0x5, 0x89);

// {5E3C6D32-2631-4b13-8BDF-C644DC73EE2C}
DEFINE_GUID( TASKID_Minor_HrProcessResType_QI_pcci,
0x5e3c6d32, 0x2631, 0x4b13, 0x8b, 0xdf, 0xc6, 0x44, 0xdc, 0x73, 0xee, 0x2c);

// {264074E3-65E1-4d44-9A14-C4593C816F1F}
DEFINE_GUID( TASKID_Minor_HrProcessResType_Initialize,
0x264074e3, 0x65e1, 0x4d44, 0x9a, 0x14, 0xc4, 0x59, 0x3c, 0x81, 0x6f, 0x1f);

// {E01E4DBD-336F-40aa-B25B-708BB5EB2E62}
DEFINE_GUID( TASKID_Minor_HrProcessResType_GetTypeName,
0xe01e4dbd, 0x336f, 0x40aa, 0xb2, 0x5b, 0x70, 0x8b, 0xb5, 0xeb, 0x2e, 0x62);

// {5DC20322-81F1-4112-A1E9-C21B29CE11DA}
DEFINE_GUID( TASKID_Minor_HrProcessResType_AboutToConfigureType,
0x5dc20322, 0x81f1, 0x4112, 0xa1, 0xe9, 0xc2, 0x1b, 0x29, 0xce, 0x11, 0xda);

// {EE6D7073-3DCE-47c1-B6BC-5A70D5C07F8E}
DEFINE_GUID( TASKID_Minor_HrProcessResType_CommitChanges,
0xee6d7073, 0x3dce, 0x47c1, 0xb6, 0xbc, 0x5a, 0x70, 0xd5, 0xc0, 0x7f, 0x8e);

// {20952BFB-1BA0-4fa9-A29A-5AD71B917836}
DEFINE_GUID( TASKID_Minor_HrProcessResType_GetTypeGUID,
0x20952bfb, 0x1ba0, 0x4fa9, 0xa2, 0x9a, 0x5a, 0xd7, 0x1b, 0x91, 0x78, 0x36);

// {ADF325A5-1B9C-42eb-B450-E0982C13C145}
DEFINE_GUID( TASKID_Minor_HrProcessResType_HrMapResTypeGUIDToName,
0xadf325a5, 0x1b9c, 0x42eb, 0xb4, 0x50, 0xe0, 0x98, 0x2c, 0x13, 0xc1, 0x45);

// {2AC4FA49-4CFC-4abc-A501-B98A3FDB8FAD}
DEFINE_GUID( TASKID_Minor_HrProcessResType_NoGuid,
0x2ac4fa49, 0x4cfc, 0x4abc, 0xa5, 0x1, 0xb9, 0x8a, 0x3f, 0xdb, 0x8f, 0xad);

// {E6C5CB87-DB90-4ac7-AE72-1D33C009F11A}
DEFINE_GUID( TASKID_HrMapResTypeGUIDToName_InvalidArg,
0xe6c5cb87, 0xdb90, 0x4ac7, 0xae, 0x72, 0x1d, 0x33, 0xc0, 0x9, 0xf1, 0x1a);

// {905FE364-0D87-4196-A6BC-C380EA2E7162}
DEFINE_GUID( TASKID_HrMapResTypeGUIDToName_OutOfMemory_NewMap,
0x905fe364, 0xd87, 0x4196, 0xa6, 0xbc, 0xc3, 0x80, 0xea, 0x2e, 0x71, 0x62);

// {1CD5A0E3-A2F9-4981-87A6-B65F35EEDF0B}
DEFINE_GUID( TASKID_HrMapResTypeGUIDToName_OutOfMemory_TypeName,
0x1cd5a0e3, 0xa2f9, 0x4981, 0x87, 0xa6, 0xb6, 0x5f, 0x35, 0xee, 0xdf, 0xb);

// {B4AEF793-57B0-4ad5-ADCB-8D4778B55697}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_LoadString_LocateExistingQuorum,
0xb4aef793, 0x57b0, 0x4ad5, 0xad, 0xcb, 0x8d, 0x47, 0x78, 0xb5, 0x56, 0x97);

// {81A54EB8-B4C3-4390-982A-E6739D4D8D84}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_OutOfMemory,
0x81a54eb8, 0xb4c3, 0x4390, 0x98, 0x2a, 0xe6, 0x73, 0x9d, 0x4d, 0x8d, 0x84);

// {A42EFFD3-D165-4d05-81C5-201C43235CF9}
DEFINE_GUID( TASKID_Minor_HrGetCoreClusterResourceNames_GetResourceName,
0xa42effd3, 0xd165, 0x4d05, 0x81, 0xc5, 0x20, 0x1c, 0x43, 0x23, 0x5c, 0xf9);

// {8BE2BCE1-D605-4c5c-8D23-9F7BB90AD3E8}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_IP_SetName,
0x8be2bce1, 0xd605, 0x4c5c, 0x8d, 0x23, 0x9f, 0x7b, 0xb9, 0xa, 0xd3, 0xe8);

// {7EB2D7B4-6FE9-45e1-AB7E-D2A325C44068}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_IP_SetType,
0x7eb2d7b4, 0x6fe9, 0x45e1, 0xab, 0x7e, 0xd2, 0xa3, 0x25, 0xc4, 0x40, 0x68);

// {8587CA58-B046-42a6-8EE3-E7D6E23AC41B}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_IP_SetClassType,
0x8587ca58, 0xb046, 0x42a6, 0x8e, 0xe3, 0xe7, 0xd6, 0xe2, 0x3a, 0xc4, 0x1b);

// {1AA31CCC-12A4-409b-925B-67FDF399BD61}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_IP_SetFlags,
0x1aa31ccc, 0x12a4, 0x409b, 0x92, 0x5b, 0x67, 0xfd, 0xf3, 0x99, 0xbd, 0x61);

// {45148990-9648-4be0-8122-5B6FF302C7B7}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_IP_SetHResource,
0x45148990, 0x9648, 0x4be0, 0x81, 0x22, 0x5b, 0x6f, 0xf3, 0x2, 0xc7, 0xb7);

// {2364D4FD-BFF5-4ccd-A127-3AAD778652EB}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_LoadString_ClusterName,
0x2364d4fd, 0xbff5, 0x4ccd, 0xa1, 0x27, 0x3a, 0xad, 0x77, 0x86, 0x52, 0xeb);

// {DB491103-3EE8-4d31-9F91-478548C9828E}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_Name_SetName,
0xdb491103, 0x3ee8, 0x4d31, 0x9f, 0x91, 0x47, 0x85, 0x48, 0xc9, 0x82, 0x8e);

// {1A1EB096-74B3-4169-A14A-73FE0F710E7F}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_Name_SetType,
0x1a1eb096, 0x74b3, 0x4169, 0xa1, 0x4a, 0x73, 0xfe, 0xf, 0x71, 0xe, 0x7f);

// {B5C400E7-0827-473b-B4B7-697976E7DA96}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_Name_SetClassType,
0xb5c400e7, 0x827, 0x473b, 0xb4, 0xb7, 0x69, 0x79, 0x76, 0xe7, 0xda, 0x96);

// {C973FCA2-3E37-4a30-8B27-CBAABFDB5699}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_Name_SetFlags,
0xc973fca2, 0x3e37, 0x4a30, 0x8b, 0x27, 0xcb, 0xaa, 0xbf, 0xdb, 0x56, 0x99);

// {4022DB05-0EC7-4bbf-B47F-BE6017529B2D}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_Name_AddTypeDependency,
0x4022db05, 0xec7, 0x4bbf, 0xb4, 0x7f, 0xbe, 0x60, 0x17, 0x52, 0x9b, 0x2d);

// {F72888A8-AB7D-4e69-B602-84A1B3F850EC}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_Name_SetHResource,
0xf72888a8, 0xab7d, 0x4e69, 0xb6, 0x2, 0x84, 0xa1, 0xb3, 0xf8, 0x50, 0xec);

// {1382A7B9-9E7F-49dd-99B1-7B71520E716C}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_Quorum_SetName,
0x1382a7b9, 0x9e7f, 0x49dd, 0x99, 0xb1, 0x7b, 0x71, 0x52, 0xe, 0x71, 0x6c);

// {6859B087-B309-4b23-A9F7-89B201DCA838}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_Quorum_SetType,
0x6859b087, 0xb309, 0x4b23, 0xa9, 0xf7, 0x89, 0xb2, 0x1, 0xdc, 0xa8, 0x38);

// {5C696EBC-E7F1-4386-9898-5EB5507D1BCD}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_Quorum_SetClassType,
0x5c696ebc, 0xe7f1, 0x4386, 0x98, 0x98, 0x5e, 0xb5, 0x50, 0x7d, 0x1b, 0xcd);

// {34F4E188-A8C4-4148-9735-50EEA7D01CF7}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_Quorum_SetFlags,
0x34f4e188, 0xa8c4, 0x4148, 0x97, 0x35, 0x50, 0xee, 0xa7, 0xd0, 0x1c, 0xf7);

// {072BD24E-AECF-4b6a-A5CE-CBE87D205FDC}
DEFINE_GUID( TASKID_Minor_HrPreInitializeExistingResources_Quorum_SetHResource,
0x72bd24e, 0xaecf, 0x4b6a, 0xa5, 0xce, 0xcb, 0xe8, 0x7d, 0x20, 0x5f, 0xdc);

// {989E8A0F-8303-4f90-A86E-AE98F0EE3FF6}
DEFINE_GUID( TASKID_Minor_HrAddSpecialResource_SetName,
0x989e8a0f, 0x8303, 0x4f90, 0xa8, 0x6e, 0xae, 0x98, 0xf0, 0xee, 0x3f, 0xf6);

// {58878B7E-E298-48de-A2E5-C369A3FA4916}
DEFINE_GUID( TASKID_Minor_HrAddSpecialResource_SetType,
0x58878b7e, 0xe298, 0x48de, 0xa2, 0xe5, 0xc3, 0x69, 0xa3, 0xfa, 0x49, 0x16);

// {AA216E27-79AB-4d91-99B6-EBE8608E08A2}
DEFINE_GUID( TASKID_Minor_HrAddSpecialResource_SetClassType,
0xaa216e27, 0x79ab, 0x4d91, 0x99, 0xb6, 0xeb, 0xe8, 0x60, 0x8e, 0x8, 0xa2);

// {A482F18F-E935-4173-B5BE-72A2260BA118}
DEFINE_GUID( TASKID_Minor_HrAddSpecialResource_OutOfMemory,
0xa482f18f, 0xe935, 0x4173, 0xb5, 0xbe, 0x72, 0xa2, 0x26, 0xb, 0xa1, 0x18);

// {E7524063-8582-4e3e-85DA-0870546FBA78}
DEFINE_GUID( TASKID_Minor_HrCreateResourceInstance_GetName,
0xe7524063, 0x8582, 0x4e3e, 0x85, 0xda, 0x8, 0x70, 0x54, 0x6f, 0xba, 0x78);

// {365E90E9-EE39-4c5c-8F05-5D2F55F60CB1}
DEFINE_GUID( TASKID_Minor_HrCreateResourceInstance_CreateClusterResource,
0x365e90e9, 0xee39, 0x4c5c, 0x8f, 0x5, 0x5d, 0x2f, 0x55, 0xf6, 0xc, 0xb1);

// {80232E1F-A383-49dc-BC6F-FB5EC78591B4}
DEFINE_GUID( TASKID_Minor_HrCreateResourceInstance_CreateClusterResource_Successful,
0x80232e1f, 0xa383, 0x49dc, 0xbc, 0x6f, 0xfb, 0x5e, 0xc7, 0x85, 0x91, 0xb4);

// {48189324-673A-4fb6-9171-9C5939515EE1}
DEFINE_GUID( TASKID_Minor_HrCreateResourceInstance_FoundExistingResource,
0x48189324, 0x673a, 0x4fb6, 0x91, 0x71, 0x9c, 0x59, 0x39, 0x51, 0x5e, 0xe1);

// {9379E242-E26B-40ee-A025-0D3D7B525FD6}
DEFINE_GUID( TASKID_Minor_HrCreateResourceInstance_ChangeClusterResourceGroup,
0x9379e242, 0xe26b, 0x40ee, 0xa0, 0x25, 0xd, 0x3d, 0x7b, 0x52, 0x5f, 0xd6);

// {0466DF38-0715-43cf-B74D-6D055F316B1A}
DEFINE_GUID( TASKID_Minor_HrCreateResourceInstance_SetHResource,
0x466df38, 0x715, 0x43cf, 0xb7, 0x4d, 0x6d, 0x5, 0x5f, 0x31, 0x6b, 0x1a);

// {04B5E793-3A69-43e1-9FE8-8443FEAC725E}
DEFINE_GUID( TASKID_Minor_HrCreateResourceInstance_Configure,
0x4b5e793, 0x3a69, 0x43e1, 0x9f, 0xe8, 0x84, 0x43, 0xfe, 0xac, 0x72, 0x5e);

// {AC28D932-39DC-4504-A9E5-F650A12AABC5}
DEFINE_GUID( TASKID_Minor_HrCreateResourceInstance_LoadString_CreatingResource,
0xac28d932, 0x39dc, 0x4504, 0xa9, 0xe5, 0xf6, 0x50, 0xa1, 0x2a, 0xab, 0xc5);

// {F4A50885-A4B9-4c4d-B67C-9E4DD94A315E}
DEFINE_GUID( CLSID_TaskType,
0xf4a50885, 0xa4b9, 0x4c4d, 0xb6, 0x7c, 0x9e, 0x4d, 0xd9, 0x4a, 0x31, 0x5e);

// {FF5C9A64-4B2F-4f66-B20C-E0D1A5F29E2A}
DEFINE_GUID( TASKID_Minor_CreateGroups_Get_ClusterNameGroup,
0xff5c9a64, 0x4b2f, 0x4f66, 0xb2, 0xc, 0xe0, 0xd1, 0xa5, 0xf2, 0x9e, 0x2a);

// {008B56BA-4AAD-4250-9041-FB990433196D}
DEFINE_GUID( TASKID_Minor_PreCreate_EnumResources_IsLocalQuorum,
0x8b56ba, 0x4aad, 0x4250, 0x90, 0x41, 0xfb, 0x99, 0x4, 0x33, 0x19, 0x6d);

// {D3B10E74-4862-47ea-ABA5-E5E9CBA22662}
//DEFINE_GUID(
//0xd3b10e74, 0x4862, 0x47ea, 0xab, 0xa5, 0xe5, 0xe9, 0xcb, 0xa2, 0x26, 0x62);

// {4836C986-C2E9-4b30-9DDD-3F05DF4EDDA2}
//DEFINE_GUID(
//0x4836c986, 0xc2e9, 0x4b30, 0x9d, 0xdd, 0x3f, 0x5, 0xdf, 0x4e, 0xdd, 0xa2);

// {A1825B45-7C6C-44c2-AF59-F79F92B776F3}
//DEFINE_GUID(
//0xa1825b45, 0x7c6c, 0x44c2, 0xaf, 0x59, 0xf7, 0x9f, 0x92, 0xb7, 0x76, 0xf3);

