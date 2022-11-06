#include <iostream>
#include <Windows.h>

typedef BOOL(__stdcall* ExceptionHandlerProtection)(); //or SquadNativeInterface::ExceptionHandlerProtection()

int main()
{
    HINSTANCE hNative = LoadLibraryA("native.dll");
    if (!hNative) {
        MessageBoxA(NULL, "Error", "NativeBoot", MB_ICONERROR);
    }
    else {
        MessageBoxA(NULL, "Success", "NativeBoot", MB_ICONINFORMATION);
        ExceptionHandlerProtection nativeMethod = (ExceptionHandlerProtection)GetProcAddress(hNative, "ExceptionHandlerProtection"); //or SquadNativeInterface::ExceptionHandlerProtection()
        nativeMethod();
    }
}

