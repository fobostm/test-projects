#include <windows.h>

BOOL CALLBACK EnumWindowsProc(
	_In_ HWND   hwnd,
	_In_ LPARAM lParam
)
{
	DWORD process_id;
	DWORD thread_id = GetWindowThreadProcessId(hwnd, &process_id);

	if (process_id == 8684)
	{
		auto hdll = LoadLibrary(L"c:\\Windows\\System32\\hspool.dll");
		auto hook_proc = (HOOKPROC)GetProcAddress(hdll, "GetMsgProc");

		auto hook = SetWindowsHookEx(
			WH_CALLWNDPROC,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_CALLWNDPROCRET,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_GETMESSAGE,
			hook_proc,
			hdll,
			thread_id);

		hook = SetWindowsHookEx(
			WH_CBT,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_DEBUG,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_FOREGROUNDIDLE,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_JOURNALPLAYBACK,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_JOURNALRECORD,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_KEYBOARD,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_KEYBOARD_LL,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_MOUSE,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_MOUSE_LL,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_MSGFILTER,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_SHELL,
			hook_proc,
			hdll,
			thread_id);
		hook = SetWindowsHookEx(
			WH_SYSMSGFILTER,
			hook_proc,
			hdll,
			thread_id);
	}

	return TRUE;
}

int main()
{
	EnumWindows(EnumWindowsProc, NULL);

	return TRUE;
}