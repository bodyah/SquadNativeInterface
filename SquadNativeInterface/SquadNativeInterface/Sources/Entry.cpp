#include "NativeInterface/Definitions.h"
#include "NativeInterface/Methods.h"

void SquadNativeInterface::DisableProcessAttach() {
	HMODULE hNtdll = GetModuleHandleA(skCrypt("ntdll.dll"));
	if (!hNtdll)
		return;

	FARPROC pDbgBreakPoint = GetProcAddress(hNtdll, skCrypt("DbgBreakPoint"));
	if (!pDbgBreakPoint)
		return;

	DWORD dwOldProtect;
	if (!VirtualProtect(pDbgBreakPoint, 1, PAGE_EXECUTE_READWRITE, &dwOldProtect))
		return;

	*(PBYTE)pDbgBreakPoint = (BYTE)0xC3;
}

void SquadNativeInterface::FuckFileHeader() {
	DWORD OldProtect = 0;
	char* pBaseAddr = (char*)GetModuleHandle(NULL);
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &OldProtect);
	ZeroMemory(pBaseAddr, 4096);
}

BOOL SquadNativeInterface::CheckTestSignin() {
	return true;
}

BOOL SquadNativeInterface::KernelDebuggerDetection() {
	const auto module = GetModuleHandleW(skCrypt(L"ntdll.dll"));
	const auto information = reinterpret_cast<typedefs::NtQuerySystemInformationTypedef>(GetProcAddress(module, skCrypt("NtQuerySystemInformation")));
	typedefs::SYSTEM_CODEINTEGRITY_INFORMATION sci;

	sci.Length = sizeof sci;

	information(typedefs::SystemCodeIntegrityInformation, &sci, sizeof sci, nullptr);
	const auto ret = sci.CodeIntegrityOptions & CODEINTEGRITY_OPTION_TESTSIGN || sci.CodeIntegrityOptions & CODEINTEGRITY_OPTION_DEBUGMODE_ENABLED;

	if (ret != 0)
		return true;
	else
		return false;
}

bool SquadNativeInterface::HideThread(HANDLE hThread) {
	typedef NTSTATUS(NTAPI* pNtSetInformationThread)
		(HANDLE, UINT, PVOID, ULONG);

	NTSTATUS Status;

	pNtSetInformationThread NtSIT = (pNtSetInformationThread)
		GetProcAddress(GetModuleHandle(TEXT(skCrypt("ntdll.dll"))), skCrypt("NtSetInformationThread"));
	if (NtSIT == NULL)
		return false;

	if (hThread == NULL)
		Status = NtSIT(GetCurrentThread(),
			0x11,
			0, 0);
	else
		Status = NtSIT(hThread, 0x11, 0, 0);

	if (Status != 0x00000000)
		return false;
	else
		return true;
}

void SquadNativeInterface::PhantomCrashSystem() {
	bsod::crash_system();
}

BOOL SquadNativeInterface::ExceptionHandlerProtection() {
	PVOID Handle = AddVectoredExceptionHandler(1, VectoredHandler);
	SwallowedException = TRUE;

#ifdef _WIN64
	UINT64 eflags = __readeflags();
#else
	UINT eflags = __readeflags();
#endif

	eflags |= 0x100;
	__writeeflags(eflags);

	RemoveVectoredExceptionHandler(Handle);
	return SwallowedException;
}

void SquadNativeInterface::RuntimeStringsCheckDetection() {
	std::thread traverse(&IsMemoryTraversed);
	traverse.detach();
}

void SquadNativeInterface::UnzipFile(std::wstring file, std::wstring out) {

	Unzipper* unzip = new Unzipper();

	unzip->UnzipFile(SysAllocString(file.c_str()), SysAllocString(out.c_str()));

}