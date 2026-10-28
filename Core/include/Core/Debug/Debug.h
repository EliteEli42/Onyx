#pragma once

#include <Core/Log/Log.h>

#include <cstdlib>
#include <string>

// --------------------------------------------------
// Debug break (cross-platform)
// --------------------------------------------------

#if defined(DEBUG)
#if defined(_MSC_VER)
#include <intrin.h>
#define DEBUG_BREAK() __debugbreak()

#elif defined(__GNUC__) || defined(__clang__)
#define DEBUG_BREAK() __builtin_trap()

#else
#include <csignal>
#define DEBUG_BREAK() std::raise(SIGTRAP)

#endif

#else
#define DEBUG_BREAK() ((void)0)

#endif

// --------------------------------------------------
// Fatal message box (Windows only, non-debug builds only)
// --------------------------------------------------

#if !defined(DEBUG) && defined(_WIN32)

#include <Windows.h>

#define SHOW_FATAL_MESSAGE_BOX(msg)                                     \
            MessageBoxA(nullptr, (msg).c_str(), "Fatal Error",              \
                        MB_OK | MB_ICONERROR | MB_TOPMOST | MB_SETFOREGROUND)

#else

#define SHOW_FATAL_MESSAGE_BOX(msg) ((void)0)

#endif

// --------------------------------------------------
// CHECK = warning + debug break (debug builds only)
// --------------------------------------------------

#if defined(DEBUG)

#define CHECK(condition, message)                                       \
            do                                                              \
            {                                                               \
                if (!(condition))                                           \
                {                                                           \
                    ::Core::Log::Warn(                                      \
                        std::string("CHECK FAILED: ") + (message));         \
                    DEBUG_BREAK();                                          \
                }                                                           \
            } while (0)

#else

#define CHECK(condition, message) ((void)0)

#endif

// --------------------------------------------------
// VERIFY = error log + debug break (active in all builds)
// Returns from the calling function only if a return value is given.
//
// Usage:
//   VERIFY(ptr != nullptr, "ptr is null");           // log only, continues
//   VERIFY(ptr != nullptr, "ptr is null", false);    // returns false
//   VERIFY(ptr != nullptr, "ptr is null", nullptr);  // returns nullptr
//   VERIFY(ptr != nullptr, "ptr is null", void());   // returns from a void function
// --------------------------------------------------

#define VERIFY(condition, message, ...)                                     \
    do                                                                      \
    {                                                                       \
        if (!(condition))                                                   \
        {                                                                   \
            ::Core::Log::Error(                                             \
                std::string("VERIFY FAILED: ") + (message));                \
            DEBUG_BREAK();                                                  \
            __VA_OPT__(return __VA_ARGS__;)                                 \
        }                                                                   \
    } while (0)

// --------------------------------------------------
// ASSERT = fatal log + (debug break | message box) + exit (always fatal)
// --------------------------------------------------

#define ASSERT(condition, message)                                          \
    do                                                                      \
    {                                                                       \
        if (!(condition))                                                   \
        {                                                                   \
            const std::string assertMsg__ =                                 \
                std::string("ASSERT FAILED: ") + (message);                 \
            ::Core::Log::Fatal(assertMsg__);                                \
            DEBUG_BREAK();                                                  \
            SHOW_FATAL_MESSAGE_BOX(assertMsg__);                            \
            std::exit(EXIT_FAILURE);                                        \
        }                                                                   \
    } while (0)
