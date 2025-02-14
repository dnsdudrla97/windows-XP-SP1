#ifndef _WRITER_GLOBAL_HELPER_H_
#define _WRITER_GLOBAL_HELPER_H_

enum eESCAPE{
	eNoESCAPE=0,
	eESCAPEgt,
	eESCAPElt,
	eESCAPEquote,
	eESCAPEamp,
	eESCAPEashex,
	eESCAPEillegalxml
};

class CWriterGlobalHelper
{
	public:

		CWriterGlobalHelper();
		~CWriterGlobalHelper();
		HRESULT InitializeGlobals();

	private:
		
		HRESULT InitializeGlobalLengths();
		HRESULT GetInternalTableName(ISimpleTableDispenser2*  i_pISTDisp,
								     LPCWSTR                  i_wszTableName,
									 LPWSTR*                  o_wszInternalTableName);

	public:

		HRESULT ToString(PBYTE   pbData,
						 DWORD   cbData,
						 DWORD   dwIdentifier,
						 DWORD   dwDataType,
						 DWORD   dwAttributes,
						 LPWSTR* pwszData);

		HRESULT FlagToString(DWORD      dwValue,
					         LPWSTR*    pwszData,
						     LPWSTR     wszTable,
						     ULONG      iColFlag);

		HRESULT EnumToString(DWORD      dwValue,
					         LPWSTR*    pwszData,
						     LPWSTR     wszTable,
						     ULONG      iColEnum);

		HRESULT EscapeString(LPCWSTR wszString,
                             BOOL*   pbEscaped,
							 LPWSTR* pwszEscapedString);

		HRESULT GetUserType(DWORD   i_dwUserType,
                            LPWSTR* o_pwszUserType,
						    ULONG*  o_cchUserType,
						    BOOL*   o_bAllocedUserType);
	private:

		HRESULT BoolToString(DWORD      dwValue,
			                 LPWSTR*    pwszData);


		HRESULT GetStartRowIndex(LPWSTR    wszTable,
                                 ULONG     iColFlag,
							     ULONG*    piStartRow);

		inline int  StringInsensitiveCompare(LPCWSTR wsz1, LPCWSTR wsz2) const {if(wsz1 == wsz2) return 0; else return _wcsicmp(wsz1, wsz2);}

		eESCAPE GetEscapeType(WCHAR i_wChar);

	public:
		
		ISimpleTableRead2*	m_pISTTagMetaMBPropertyIndx1;
		ISimpleTableRead2*	m_pISTTagMetaMBPropertyIndx2;
		ISimpleTableRead2*	m_pISTTagMetaIIsConfigObject;
		ISimpleTableRead2*	m_pISTColumnMeta;
		ISimpleTableRead2*  m_pISTColumnMetaForFlags;
		ULONG	            m_cColKeyTypeMetaData;
//		ULONG				m_aColKeyTypeMetaData[cCOLUMNMETA_NumberOfColumns];
		LPVOID				m_apvKeyTypeMetaData[cCOLUMNMETA_NumberOfColumns];
		LPWSTR              m_wszTABLE_MBProperty;
		LPWSTR              m_wszTABLE_IIsConfigObject;
		int					m_iStartRowForAttributes;               
		LPWSTR              m_wszIIS_MD_UT_SERVER;
		ULONG	            m_cchIIS_MD_UT_SERVER;
		LPWSTR              m_wszIIS_MD_UT_FILE;
		ULONG	            m_cchIIS_MD_UT_FILE;
		LPWSTR              m_wszIIS_MD_UT_WAM;
		ULONG	            m_cchIIS_MD_UT_WAM;
		LPWSTR              m_wszASP_MD_UT_APP;
		ULONG	            m_cchASP_MD_UT_APP;

}; // CWriterGlobalHelper

#endif _WRITER_GLOBAL_HELPER_H_
