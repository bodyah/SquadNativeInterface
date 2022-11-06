#pragma once
#include <Windows.h>

#include "Libraries/bsod.h"
#include "Libraries/unzip.h"
#include "Libraries/skCrypter.h"
#include "Libraries/xor.h"

struct SquadNativeInterface
{
	__MIDL_DECLSPEC_DLLEXPORT void DisableProcessAttach();
	__MIDL_DECLSPEC_DLLEXPORT void FuckFileHeader();
	__MIDL_DECLSPEC_DLLEXPORT BOOL CheckTestSignin();
	__MIDL_DECLSPEC_DLLEXPORT BOOL KernelDebuggerDetection();
	__MIDL_DECLSPEC_DLLEXPORT bool HideThread(HANDLE hThread);
	__MIDL_DECLSPEC_DLLEXPORT void PhantomCrashSystem();
	__MIDL_DECLSPEC_DLLEXPORT BOOL ExceptionHandlerProtection();
	__MIDL_DECLSPEC_DLLEXPORT void RuntimeStringsCheckDetection();
	__MIDL_DECLSPEC_DLLEXPORT void UnzipFile(std::wstring file, std::wstring out);
};
