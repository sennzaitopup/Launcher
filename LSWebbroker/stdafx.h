// stdafx.h : 잘 변경되지 않고 자주 사용하는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows 헤더에서 거의 사용되지 않는 내용을 제외시킵니다.
#endif

// 아래 지정된 플랫폼보다 우선하는 플랫폼을 대상으로 하는 경우 다음 정의를 수정하십시오.
// 다른 플랫폼에 사용되는 해당 값의 최신 정보는 MSDN을 참조하십시오.
#ifndef WINVER				// Windows 95 및 Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define WINVER 0x0500		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINNT 0x0500		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_WINDOWS 0x0410 // Windows Me 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#ifndef _WIN32_IE			// IE 4.0 이후 버전에서만 기능을 사용할 수 있습니다.
#define _WIN32_IE 0x0400	// IE 5.0 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#define __EX
#define SAFEDELETE(x) if(x != NULL) { delete x; x = NULL; }
#define SAFEDELETEARRAY(x) if(x != NULL) { delete [] x; x = NULL; }
#define COMPARE(x,min,max) (((x)>=(min))&&((x)<(max)))

// disable : 'xxxx': 이름이 #pragma deprecated로 표시되었습니다.
#	pragma warning(disable:4995)
#	pragma warning(disable:4267)


#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소
#include <afxext.h>         // MFC 익스텐션

#include <afxdtctl.h>		// Internet Explorer 4 공용 컨트롤에 대한 MFC 지원
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows 공용 컨트롤에 대한 MFC 지원
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "resource.h"

// 국가 코드 페이지. 기본은 Korean 949
#if defined( SRC_ID )
	#define COUNTRY_CODE_PAGE	1252
	#define IDR_ERROR IDR_TXT_ERROR_ID
#elif defined( SRC_SEA )
	#define COUNTRY_CODE_PAGE	1252
	#define IDR_ERROR IDR_TXT_ERROR_NA
#elif defined( SRC_NA )
	#define COUNTRY_CODE_PAGE	1252
	#define IDR_ERROR IDR_TXT_ERROR_NA
#elif defined( SRC_TW )
	#define COUNTRY_CODE_PAGE	950
	#define MULTI_BYTE_CHECK
#define IDR_ERROR IDR_TXT_ERROR_TW
#elif defined( SRC_TH )
	#define COUNTRY_CODE_PAGE	874
	#define MULTI_BYTE_CHECK
	#define IDR_ERROR IDR_TXT_ERROR_TH
#elif defined( SRC_EU )
	#define COUNTRY_CODE_PAGE	1252
	#define IDR_ERROR IDR_TXT_ERROR_EU
#elif defined( SRC_SA )
	#define COUNTRY_CODE_PAGE	1250
	#define IDR_ERROR IDR_TXT_ERROR_SA
#elif defined( SRC_SEA )
	#define COUNTRY_CODE_PAGE	1252
	#define IDR_ERROR IDR_TXT_ERROR_ID
#else
	#define COUNTRY_CODE_PAGE	949
	#define IDR_ERROR IDR_TXT_ERROR
#endif

#define TEXT_RESOURCE "txt"
