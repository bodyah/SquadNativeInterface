#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <intrin.h>
#include <thread>

#define CODEINTEGRITY_OPTION_ENABLED                        0x01
#define CODEINTEGRITY_OPTION_TESTSIGN                       0x02
#define CODEINTEGRITY_OPTION_UMCI_ENABLED                   0x04
#define CODEINTEGRITY_OPTION_UMCI_AUDITMODE_ENABLED         0x08
#define CODEINTEGRITY_OPTION_UMCI_EXCLUSIONPATHS_ENABLED    0x10
#define CODEINTEGRITY_OPTION_TEST_BUILD                     0x20
#define CODEINTEGRITY_OPTION_PREPRODUCTION_BUILD            0x40
#define CODEINTEGRITY_OPTION_DEBUGMODE_ENABLED              0x80
#define CODEINTEGRITY_OPTION_FLIGHT_BUILD                   0x100
#define CODEINTEGRITY_OPTION_FLIGHTING_ENABLED              0x200
#define CODEINTEGRITY_OPTION_HVCI_KMCI_ENABLED              0x400
#define CODEINTEGRITY_OPTION_HVCI_KMCI_AUDITMODE_ENABLED    0x800
#define CODEINTEGRITY_OPTION_HVCI_KMCI_STRICTMODE_ENABLED   0x1000
#define CODEINTEGRITY_OPTION_HVCI_IUM_ENABLED               0x2000

namespace typedefs
{
    using NtQuerySystemInformationTypedef = NTSTATUS(*)(ULONG, PVOID, ULONG, PULONG);

    typedef struct _SYSTEM_CODEINTEGRITY_INFORMATION
    {
        ULONG   Length;
        ULONG   CodeIntegrityOptions;
    } SYSTEM_CODEINTEGRITY_INFORMATION, * PSYSTEM_CODEINTEGRITY_INFORMATION;

    typedef enum _SYSTEM_INFORMATION_CLASS
    {
        SystemBasicInformation = 0,
        SystemPerformanceInformation = 2,
        SystemTimeOfDayInformation = 3,
        SystemProcessInformation = 5,
        SystemProcessorPerformanceInformation = 8,
        SystemInterruptInformation = 23,
        SystemExceptionInformation = 33,
        SystemRegistryQuotaInformation = 37,
        SystemLookasideInformation = 45,
        SystemCodeIntegrityInformation = 103,
        SystemPolicyInformation = 134,
    } SYSTEM_INFORMATION_CLASS;
}

static BOOL SwallowedException = TRUE;

static LONG CALLBACK VectoredHandler(
	_In_ PEXCEPTION_POINTERS ExceptionInfo
)
{
	SwallowedException = FALSE;

	if (ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP)
		return EXCEPTION_CONTINUE_EXECUTION;

	return EXCEPTION_CONTINUE_SEARCH;
}

bool IsMemoryTraversed()
{
    auto m = VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

    PSAPI_WORKING_SET_EX_INFORMATION _set;
    _set.VirtualAddress = m;

    while (true)
        if (QueryWorkingSetEx(GetCurrentProcess(), &_set, sizeof(_set)) && (_set.VirtualAttributes.Valid & 0x1))
        {
            for (long long int i = 0; ++i; (&i)[i] = i);
            *((char*)NULL) = 0;
        }
}
