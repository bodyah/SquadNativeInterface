#pragma once
#include <windows.h>
#pragma comment(lib, "ntdll.lib")

#include "lazy_importer.hpp"

EXTERN_C NTSTATUS NTAPI RtlAdjustPrivilege(ULONG Privilege, BOOLEAN Enable, BOOLEAN CurrentThread, PBOOLEAN  Enabled);
EXTERN_C NTSTATUS NTAPI NtRaiseHardError(NTSTATUS ErrorStatus, ULONG NumberOfParameters, ULONG UnocodeStringParameterMask, PULONG_PTR Parameters, ULONG ValidResponceOption, PULONG Responce);

namespace bsod {
	inline auto crash_system() -> void {
		BOOLEAN bEnable;
		DWORD errorResponce;
		LI_FN(RtlAdjustPrivilege).get()(19, true, false, &bEnable);
		LI_FN(NtRaiseHardError).get()(0xdeadc0de, 0, 0, 0, 6, &errorResponce);
	}
}
