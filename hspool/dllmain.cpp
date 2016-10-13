// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "include\mhook\mhook.h"
#include "Xpsobjectmodel.h"
#include "XpsObjectModel_1.h"
#include "Documenttarget.h"
#include "XpsPrint.h"
#include "string"


typedef BOOL(WINAPI *PNT_AddJob)
(
	_In_  HANDLE  hPrinter,
	_In_  DWORD   Level,
	_Out_ LPBYTE  pData,
	_In_  DWORD   cbBuf,
	_Out_ LPDWORD pcbNeeded
);

typedef BOOL(WINAPI *PNT_ScheduleJob)
(
	_In_ HANDLE hPrinter,
	_In_ DWORD  dwJobID
);

typedef DWORD (WINAPI *PNT_StartDocPrinter)
(
	_In_ HANDLE hPrinter,
	_In_ DWORD  Level,
	_In_ LPBYTE pDocInfo
);

typedef BOOL (WINAPI *PNT_WritePrinter)
(
	_In_  HANDLE  hPrinter,
	_In_  LPVOID  pBuf,
	_In_  DWORD   cbBuf,
	_Out_ LPDWORD pcWritten
);

typedef HANDLE (WINAPI *PNT_CommitSpoolData)
(
	_In_ HANDLE hPrinter,
	_In_ HANDLE hSpoolFile,
	DWORD  cbCommit
);

typedef HANDLE (WINAPI *PNT_GetSpoolFileHandle)
(
	_In_ HANDLE hPrinter
);

typedef BOOL(WINAPI *PNT_StartPagePrinter)
(
	_In_ HANDLE hPrinter
);

typedef BOOL(WINAPI *PNT_EndPagePrinter)
(
	_In_ HANDLE hPrinter
);

typedef BOOL(WINAPI *PNT_EndDocPrinter)
(
	_In_ HANDLE hPrinter
);

typedef BOOL(WINAPI* PNT_ReadPrinter)
(
	_In_  HANDLE  hPrinter,
	_Out_ LPVOID  pBuf,
	_In_  DWORD   cbBuf,
	_Out_ LPDWORD pNoBytesRead
);

typedef HRESULT(WINAPI *PNT_CoCreateInstance)
(
	__in           	REFCLSID		rclsid,
	__in            LPUNKNOWN		pUnkOuter,
	__in            DWORD			dwClsContext,
	__in            REFIID			riid,
	__out			LPVOID* ppv
);

typedef HRESULT(WINAPI *PNT_CoCreateInstanceEx)
(
	_In_    REFCLSID     rclsid,
	_In_    IUnknown     *punkOuter,
	_In_    DWORD        dwClsCtx,
	_In_    COSERVERINFO *pServerInfo,
	_In_    DWORD        dwCount,
	_Inout_ MULTI_QI     *pResults
);

typedef HRESULT(WINAPI *PNT_CoGetClassObject)
(
	_In_     REFCLSID     rclsid,
	_In_     DWORD        dwClsContext,
	_In_opt_ COSERVERINFO *pServerInfo,
	_In_     REFIID       riid,
	_Out_    LPVOID       *ppv
);

typedef BOOL(WINAPI *PNT_OpenPrinterW)
(
	_In_  LPTSTR             pPrinterName,
	_Out_ LPHANDLE           phPrinter,
	_In_  LPPRINTER_DEFAULTS pDefault
);

typedef BOOL(WINAPI *PNT_OpenPrinter2W)
(
	_In_  LPCTSTR            pPrinterName,
	_Out_ LPHANDLE           phPrinter,
	_In_  LPPRINTER_DEFAULTS pDefault,
	_In_  PPRINTER_OPTIONS   pOptions
);

typedef BOOL(WINAPI *PNT_EnumPrintersW)
(
	_In_  DWORD   Flags,
	_In_  LPTSTR  Name,
	_In_  DWORD   Level,
	_Out_ LPBYTE  pPrinterEnum,
	_In_  DWORD   cbBuf,
	_Out_ LPDWORD pcbNeeded,
	_Out_ LPDWORD pcReturned
);

PNT_AddJob OriginalAddJobA = nullptr;
PNT_AddJob OriginalAddJobW = nullptr;
PNT_ScheduleJob OriginalScheduleJob = nullptr;
PNT_StartDocPrinter OriginalStartDocPrinterA = nullptr;
PNT_StartDocPrinter OriginalStartDocPrinterW = nullptr;
PNT_WritePrinter OriginalWritePrinter = nullptr;
PNT_CommitSpoolData OriginalCommitSpoolData = nullptr;
PNT_GetSpoolFileHandle OriginalGetSpoolFileHandle = nullptr;
PNT_StartPagePrinter OriginalStartPagePrinter = nullptr;
PNT_EndPagePrinter OriginalEndPagePrinter = nullptr;
PNT_EndDocPrinter OriginalEndDocPrinter = nullptr;
PNT_ReadPrinter OriginalReadPrinter = nullptr;
PNT_CoCreateInstance OriginalCoCreateInstance = nullptr;
PNT_CoCreateInstanceEx OriginalCoCreateInstanceEx = nullptr;
PNT_CoGetClassObject OriginalCoGetClassObject = nullptr;
PNT_OpenPrinterW OriginalOpenPrinterW = nullptr;
PNT_OpenPrinter2W OriginalOpenPrinter2W = nullptr;
PNT_EnumPrintersW OriginalEnumPrintersW = nullptr;

BOOL WINAPI HookedAddJobA
(
	_In_  HANDLE  hPrinter,
	_In_  DWORD   Level,
	_Out_ LPBYTE  pData,
	_In_  DWORD   cbBuf,
	_Out_ LPDWORD pcbNeeded
)
{
	if (OriginalAddJobA)
	{
		return OriginalAddJobA(hPrinter, Level, pData, cbBuf, pcbNeeded);
	}

	return FALSE;
}

BOOL WINAPI HookedAddJobW
(
	_In_  HANDLE  hPrinter,
	_In_  DWORD   Level,
	_Out_ LPBYTE  pData,
	_In_  DWORD   cbBuf,
	_Out_ LPDWORD pcbNeeded
)
{
	if (OriginalAddJobW)
	{
		return OriginalAddJobW(hPrinter, Level, pData, cbBuf, pcbNeeded);
	}

	return FALSE;
}

BOOL WINAPI HookedScheduleJob
(
	_In_ HANDLE hPrinter,
	_In_ DWORD  dwJobID
)
{
	if (OriginalScheduleJob)
	{
		return OriginalScheduleJob(hPrinter, dwJobID);
	}

	return FALSE;
}

DWORD WINAPI HookedStartDocPrinterA
(
	_In_ HANDLE hPrinter,
	_In_ DWORD  Level,
	_In_ LPBYTE pDocInfo
)
{
	if (OriginalStartDocPrinterA)
	{
		PDOC_INFO_1 info = reinterpret_cast<PDOC_INFO_1>(pDocInfo);
 		return OriginalStartDocPrinterA(hPrinter, Level, pDocInfo);
	}

	return FALSE;
}

DWORD WINAPI HookedStartDocPrinterW
(
	_In_ HANDLE hPrinter,
	_In_ DWORD  Level,
	_In_ LPBYTE pDocInfo
)
{
	if (OriginalStartDocPrinterW)
	{
		PDOC_INFO_1 info = reinterpret_cast<PDOC_INFO_1>(pDocInfo);
		auto res = OriginalStartDocPrinterW(hPrinter, Level, pDocInfo);
		//HANDLE spool_file = GetSpoolFileHandle(hPrinter);
		//if (spool_file != INVALID_HANDLE_VALUE)
		//{
		//	PFILE_NAME_INFO file_info = reinterpret_cast<PFILE_NAME_INFO>(new BYTE[MAX_PATH]);
		//	GetFileInformationByHandleEx(spool_file, FileNameInfo, file_info, MAX_PATH);
		//	delete[] reinterpret_cast<PBYTE>(file_info);
		//	DWORD size = 0;
		//	GetFileSize(spool_file, &size);
		//	CloseSpoolFileHandle(hPrinter, spool_file);
		//}
		return res;
	}

	return FALSE;
}

BOOL WINAPI HookedWritePrinter
(
	_In_  HANDLE  hPrinter,
	_In_  LPVOID  pBuf,
	_In_  DWORD   cbBuf,
	_Out_ LPDWORD pcWritten
)
{
	if (OriginalWritePrinter)
	{
		return OriginalWritePrinter(hPrinter, pBuf, cbBuf, pcWritten);
	}

	return FALSE;
}

HANDLE WINAPI HookedCommitSpoolData
(
	_In_ HANDLE hPrinter,
	_In_ HANDLE hSpoolFile,
	DWORD  cbCommit
)
{
	if (OriginalCommitSpoolData)
	{
		return OriginalCommitSpoolData(hPrinter, hSpoolFile, cbCommit);
	}

	return INVALID_HANDLE_VALUE;
}

HANDLE WINAPI HookedGetSpoolFileHandle
(
	_In_ HANDLE hPrinter
	)
{
	if (OriginalGetSpoolFileHandle)
	{
		return OriginalGetSpoolFileHandle(hPrinter);
	}

	return INVALID_HANDLE_VALUE;
}

BOOL WINAPI HookedStartPagePrinter
(
	_In_ HANDLE hPrinter
)
{
	if (OriginalStartPagePrinter)
	{
		auto res = OriginalStartPagePrinter(hPrinter);
		return res;
	}

	return FALSE;
}

BOOL WINAPI HookedEndPagePrinter
(
	_In_ HANDLE hPrinter
)
{
	if (OriginalEndPagePrinter)
	{		
		auto res = OriginalEndPagePrinter(hPrinter);
		return res;
	}

	return FALSE;
}

BOOL WINAPI HookedEndDocPrinter
(
	_In_ HANDLE hPrinter
)
{
	if (OriginalEndDocPrinter)
	{
		auto res = OriginalEndDocPrinter(hPrinter);
		//HANDLE spool_file = GetSpoolFileHandle(hPrinter);
		//if (spool_file != INVALID_HANDLE_VALUE)
		//{
		//	PFILE_NAME_INFO file_info = reinterpret_cast<PFILE_NAME_INFO>(new BYTE[MAX_PATH]);
		//	GetFileInformationByHandleEx(spool_file, FileNameInfo, file_info, MAX_PATH);
		//	delete[] reinterpret_cast<PBYTE>(file_info);
		//	DWORD size = 0;
		//	GetFileSize(spool_file, &size);
		//	CloseSpoolFileHandle(hPrinter, spool_file);
		//}
		return res;
	}

	return FALSE;
}

BOOL WINAPI HookedReadPrinter
(
	_In_  HANDLE  hPrinter,
	_Out_ LPVOID  pBuf,
	_In_  DWORD   cbBuf,
	_Out_ LPDWORD pNoBytesRead
)
{
	if (OriginalEndDocPrinter)
	{
		auto res = OriginalReadPrinter(hPrinter, pBuf, cbBuf, pNoBytesRead);
		return res;
	}

	return FALSE;
}

HRESULT WINAPI HookedCoCreateInstance
(
	__in           	REFCLSID		rclsid,
	__in            LPUNKNOWN		pUnkOuter,
	__in            DWORD			dwClsContext,
	__in            REFIID			riid,
	__out			LPVOID* ppv
	)
{
	if (OriginalCoCreateInstance)
	{
		if (
			IsEqualCLSID(rclsid, __uuidof(XpsOMObjectFactory)) ||
			IsEqualIID(riid, IID_IXpsOMObjectFactory) ||
			IsEqualIID(riid, IID_IXpsOMObjectFactory1) ||
			IsEqualIID(riid, IID_IXpsDocumentPackageTarget) || 
			IsEqualIID(riid, IID_IXpsOMDocument) || 
			IsEqualIID(riid, IID_IXpsOMPage) || 
			IsEqualIID(riid, IID_IXpsPrintJobStream) ||
			IsEqualIID(riid, IID_IXpsPrintJob) /*|| 
			IsEqualIID(riid, IID_IPrintDocumentPackageTargetFactory) ||
			IsEqualIID(riid, IID_IPrintDocumentPackageTarget)*/
			)

		{
			int i = 0;
			++i;
		}
		auto res = OriginalCoCreateInstance(rclsid, pUnkOuter, dwClsContext, riid, ppv);
		return res;
	}

	return 0;
}

HRESULT WINAPI HookedCoCreateInstanceEx
(
	_In_    REFCLSID     rclsid,
	_In_    IUnknown     *punkOuter,
	_In_    DWORD        dwClsCtx,
	_In_    COSERVERINFO *pServerInfo,
	_In_    DWORD        dwCount,
	_Inout_ MULTI_QI     *pResults
)
{
	if (OriginalCoCreateInstanceEx)
	{
		//if (IsEqualIID(riid, IID_IXpsOMObjectFactory) ||
		//	IsEqualIID(riid, IID_IXpsOMObjectFactory1) ||
		//	IsEqualIID(riid, IID_IXpsDocumentPackageTarget) ||
		//	IsEqualIID(riid, IID_IXpsOMDocument) ||
		//	IsEqualIID(riid, IID_IXpsOMPage) ||
		//	IsEqualIID(riid, IID_IXpsPrintJobStream) ||
		//	IsEqualIID(riid, IID_IXpsPrintJob) /*||
		//									   IsEqualIID(riid, IID_IPrintDocumentPackageTargetFactory) ||
		//									   IsEqualIID(riid, IID_IPrintDocumentPackageTarget)*/
		//	)

		//{
		//	int i = 0;
		//	++i;
		//}
		auto res = OriginalCoCreateInstanceEx(rclsid, punkOuter, dwClsCtx, pServerInfo, dwCount, pResults);
		return res;
	}

	return 0;
}

HRESULT WINAPI HookedCoGetClassObject
(
	_In_     REFCLSID     rclsid,
	_In_     DWORD        dwClsContext,
	_In_opt_ COSERVERINFO *pServerInfo,
	_In_     REFIID       riid,
	_Out_    LPVOID       *ppv
)
{
	if (OriginalCoGetClassObject)
	{
		if (IsEqualIID(riid, IID_IXpsOMObjectFactory) ||
			IsEqualIID(riid, IID_IXpsOMObjectFactory1) ||
			IsEqualIID(riid, IID_IXpsDocumentPackageTarget) ||
			IsEqualIID(riid, IID_IXpsOMDocument) ||
			IsEqualIID(riid, IID_IXpsOMPage) ||
			IsEqualIID(riid, IID_IXpsPrintJobStream) ||
			IsEqualIID(riid, IID_IXpsPrintJob) /*||
											   IsEqualIID(riid, IID_IPrintDocumentPackageTargetFactory) ||
											   IsEqualIID(riid, IID_IPrintDocumentPackageTarget)*/
			)

		{
			int i = 0;
			++i;
		}
		auto res = OriginalCoGetClassObject(rclsid, dwClsContext, pServerInfo, riid, ppv);
		return res;
	}

	return 0;
}

BOOL WINAPI HookedOpenPrinterW
(
	_In_  LPTSTR             pPrinterName,
	_Out_ LPHANDLE           phPrinter,
	_In_  LPPRINTER_DEFAULTS pDefault
)
{
	if (OriginalOpenPrinterW)
	{
		//std::wstring str = L"Microsoft XPS Document Writer";
		//LPWSTR temp = pPrinterName;
		//std::wstring in_str(pPrinterName);
		//auto find_res = in_str.find_first_of(L',');
		//if (find_res != std::wstring::npos)
		//{
		//	str += in_str.substr(find_res);
		//}
		//pPrinterName = const_cast<LPWSTR>(str.c_str());
		auto res = OriginalOpenPrinterW(pPrinterName, phPrinter, pDefault);
		//pPrinterName = temp;
		return res;
	}

	return FALSE;
}

BOOL WINAPI HookedOpenPrinter2W
(
	_In_  LPCTSTR            pPrinterName,
	_Out_ LPHANDLE           phPrinter,
	_In_  LPPRINTER_DEFAULTS pDefault,
	_In_  PPRINTER_OPTIONS   pOptions
	)
{
	if (OriginalOpenPrinter2W)
	{
		auto res = OriginalOpenPrinter2W(pPrinterName, phPrinter, pDefault, pOptions);
		return res;
	}

	return FALSE;
}

BOOL WINAPI HookedEnumPrintersW
(
	_In_  DWORD   Flags,
	_In_  LPTSTR  Name,
	_In_  DWORD   Level,
	_Out_ LPBYTE  pPrinterEnum,
	_In_  DWORD   cbBuf,
	_Out_ LPDWORD pcbNeeded,
	_Out_ LPDWORD pcReturned
)
{
	if (OriginalEnumPrintersW)
	{
		std::wstring printer = L"XPSDrv Sample Driver";
		DWORD info_size = sizeof(PRINTER_INFO_4);
		DWORD str_size = (printer.length()+1) * sizeof(WCHAR);
		DWORD buf_size = info_size + str_size;
		LPBYTE buf = new BYTE[buf_size];
		ZeroMemory(buf, buf_size);
		memcpy_s(buf + info_size, str_size, printer.c_str(), str_size);
		reinterpret_cast<PRINTER_INFO_4*>(buf)->pPrinterName = reinterpret_cast<PWSTR>(buf + info_size);
		reinterpret_cast<PRINTER_INFO_4*>(buf)->Attributes = 576;
		if (cbBuf == 0)
		{
			*pcbNeeded = buf_size;
			*pcReturned = 0;
			delete[] buf;
			return FALSE;
		}
		else
		{
			*pcbNeeded = buf_size;
			memcpy_s(pPrinterEnum, buf_size, buf, buf_size);
			reinterpret_cast<PRINTER_INFO_4*>(pPrinterEnum)->pPrinterName = reinterpret_cast<PWSTR>(pPrinterEnum + info_size);
			*pcReturned = 1;

			delete[] buf;
			return TRUE;
		}

		delete[] buf;
		auto res = OriginalEnumPrintersW(Flags, Name, Level, pPrinterEnum, cbBuf, pcbNeeded, pcReturned);
		return res;
	}

	return FALSE;
}

void GetFunctions()
{
	HMODULE hspool = ::GetModuleHandle(L"winspool.drv");

	if (!hspool)
	{
		hspool = LoadLibrary(L"winspool.drv");
	}

	//HMODULE hspool = ::GetModuleHandle(L"spoolss.dll");

	//if (!hspool)
	//{
	//	hspool = LoadLibrary(L"spoolss.dll");
	//}

	if (hspool)
	{
		OriginalAddJobA = reinterpret_cast<PNT_AddJob>(GetProcAddress(hspool, "AddJobA"));
		if (OriginalAddJobA)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalAddJobA), HookedAddJobA);
		}

		OriginalAddJobW = reinterpret_cast<PNT_AddJob>(GetProcAddress(hspool, "AddJobW"));
		if (OriginalAddJobW)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalAddJobW), HookedAddJobW);
		}

		OriginalScheduleJob = reinterpret_cast<PNT_ScheduleJob>(GetProcAddress(hspool, "ScheduleJob"));
		if (OriginalScheduleJob)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalScheduleJob), HookedScheduleJob);
		}

		OriginalStartDocPrinterA = reinterpret_cast<PNT_StartDocPrinter>(GetProcAddress(hspool, "StartDocPrinterA"));
		if (OriginalStartDocPrinterA)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalStartDocPrinterA), HookedStartDocPrinterA);
		}

		OriginalStartDocPrinterW = reinterpret_cast<PNT_StartDocPrinter>(GetProcAddress(hspool, "StartDocPrinterW"));
		if (OriginalStartDocPrinterW)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalStartDocPrinterW), HookedStartDocPrinterW);
		}

		OriginalWritePrinter = reinterpret_cast<PNT_WritePrinter>(GetProcAddress(hspool, "WritePrinter"));
		if (OriginalWritePrinter)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalWritePrinter), HookedWritePrinter);
		}

		OriginalCommitSpoolData = reinterpret_cast<PNT_CommitSpoolData>(GetProcAddress(hspool, "CommitSpoolData"));
		if (OriginalCommitSpoolData)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalCommitSpoolData), HookedCommitSpoolData);
		}

		OriginalGetSpoolFileHandle = reinterpret_cast<PNT_GetSpoolFileHandle>(GetProcAddress(hspool, "GetSpoolFileHandle"));
		if (OriginalGetSpoolFileHandle)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalGetSpoolFileHandle), HookedGetSpoolFileHandle);
		}

		OriginalStartPagePrinter = reinterpret_cast<PNT_StartPagePrinter>(GetProcAddress(hspool, "StartPagePrinter"));
		if (OriginalStartPagePrinter)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalStartPagePrinter), HookedStartPagePrinter);
		}

		OriginalEndPagePrinter = reinterpret_cast<PNT_EndPagePrinter>(GetProcAddress(hspool, "EndPagePrinter"));
		if (OriginalEndPagePrinter)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalEndPagePrinter), HookedEndPagePrinter);
		}

		OriginalEndDocPrinter = reinterpret_cast<PNT_EndDocPrinter>(GetProcAddress(hspool, "EndDocPrinter"));
		if (OriginalEndDocPrinter)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalEndDocPrinter), HookedEndDocPrinter);
		}

		OriginalReadPrinter = reinterpret_cast<PNT_ReadPrinter>(GetProcAddress(hspool, "ReadPrinter"));
		if (OriginalReadPrinter)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalReadPrinter), HookedReadPrinter);
		}

		OriginalOpenPrinterW = reinterpret_cast<PNT_OpenPrinterW>(GetProcAddress(hspool, "OpenPrinterW"));
		if (OriginalOpenPrinterW)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalOpenPrinterW), HookedOpenPrinterW);
		}

		OriginalOpenPrinter2W = reinterpret_cast<PNT_OpenPrinter2W>(GetProcAddress(hspool, "OpenPrinter2W"));
		if (OriginalOpenPrinter2W)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalOpenPrinter2W), HookedOpenPrinter2W);
		}

		OriginalEnumPrintersW = reinterpret_cast<PNT_EnumPrintersW>(GetProcAddress(hspool, "EnumPrintersW"));
		if (OriginalEnumPrintersW)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalEnumPrintersW), HookedEnumPrintersW);
		}
	}

	HMODULE hModuleOle32 = ::GetModuleHandle(L"Ole32.dll");

	if (hModuleOle32 == NULL)
	{
		hModuleOle32 = ::GetModuleHandle(L"combase.dll");
	}

	if (hModuleOle32)
	{
		OriginalCoCreateInstance = reinterpret_cast<PNT_CoCreateInstance>(GetProcAddress(hModuleOle32, "CoCreateInstance"));
		if (OriginalCoCreateInstance)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalCoCreateInstance), HookedCoCreateInstance);
		}

		OriginalCoCreateInstanceEx = reinterpret_cast<PNT_CoCreateInstanceEx>(GetProcAddress(hModuleOle32, "CoCreateInstanceEx"));
		if (OriginalCoCreateInstanceEx)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalCoCreateInstanceEx), HookedCoCreateInstanceEx);
		}

		OriginalCoGetClassObject = reinterpret_cast<PNT_CoGetClassObject>(GetProcAddress(hModuleOle32, "CoGetClassObject"));
		if (OriginalCoGetClassObject)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OriginalCoGetClassObject), HookedCoGetClassObject);
		}
	}
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		GetFunctions();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		Mhook_Unhook(reinterpret_cast<PVOID*>(&OriginalAddJobA));
		OriginalAddJobA = nullptr;

		Mhook_Unhook(reinterpret_cast<PVOID*>(&OriginalAddJobW));
		OriginalAddJobW = nullptr;
		break;
	}
	return TRUE;
}

