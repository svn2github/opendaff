#ifdef WIN32

#include <windows.h>

BOOL APIENTRY DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpReserved)
{
    return TRUE;
}

#endif // WIN32