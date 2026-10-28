#include <Core/Common/Debug.h>

#if defined(_WIN32)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif

namespace Core
{
    void ShowFatalMessageBox(const std::string& msg)
    {
#if defined(_WIN32)
        MessageBoxA(nullptr, msg.c_str(), "Fatal Error",
            MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND);
#else
        (void)msg;
#endif
    }
}