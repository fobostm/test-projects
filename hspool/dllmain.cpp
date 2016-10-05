// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "include\mhook\mhook.h"


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
PNT_ReadPrinter OrirginalReadPrinter = nullptr;

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
		auto res = OrirginalReadPrinter(hPrinter, pBuf, cbBuf, pNoBytesRead);
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

		OrirginalReadPrinter = reinterpret_cast<PNT_ReadPrinter>(GetProcAddress(hspool, "ReadPrinter"));
		if (OrirginalReadPrinter)
		{
			Mhook_SetHook(reinterpret_cast<PVOID*>(&OrirginalReadPrinter), HookedReadPrinter);
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

