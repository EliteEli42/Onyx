#pragma once

#include <Core/Common/Log.h>

#include <cstdlib>
#include <format>
#include <string>

// --------------------------------------------------
// Debug break (cross-platform)
// --------------------------------------------------

#if defined(DEBUG)
#if defined(_MSC_VER)
#include <intrin.h>
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define DEBUG_BREAK() __builtin_debugtrap()
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

namespace Core
{
    void ShowFatalMessageBox(const std::string& msg);
}

#if !defined(DEBUG)
#define SHOW_FATAL_MESSAGE_BOX(msg) ::Core::ShowFatalMessageBox(msg)
#else
#define SHOW_FATAL_MESSAGE_BOX(msg) ((void)0)
#endif

// --------------------------------------------------
// CHECK = warning + debug break (debug builds only)
//
// Usage:
//   CHECK(ptr != nullptr, "ptr is null");
//   CHECK(idx < size, "index {} out of range (size {})", idx, size);
// --------------------------------------------------

#if defined(DEBUG)

#define CHECK(condition, fmt, ...)                                                 \
        do                                                                         \
        {                                                                          \
            if (!(condition))                                                      \
            {                                                                      \
                ::Core::Log::Warn("CHECK FAILED: " fmt __VA_OPT__(,) __VA_ARGS__); \
                DEBUG_BREAK();                                                     \
            }                                                                      \
        } while (0)

#else
#define CHECK(condition, fmt, ...) ((void)0)
#endif

// --------------------------------------------------
// WARN_RETURN = warning log + return (no debug break, active in all builds)
// For expected failures caused by external data (wrong image format, missing file ...)
//
// Usage:
//   WARN_RETURN(img.GetNrChannels() == 4, false, "Expected 4 channels, got {}", img.GetNrChannels());
//   WARN_RETURN(file.is_open(), nullptr, "Could not open '{}'", path);
//   WARN_RETURN(title != nullptr, void(), "Title is null");   // void function
// --------------------------------------------------

#define WARN_RETURN(condition, returnValue, fmt, ...)                           \
    do                                                                          \
    {                                                                           \
        if (!(condition))                                                       \
        {                                                                       \
            ::Core::Log::Warn(fmt __VA_OPT__(,) __VA_ARGS__);                   \
            return returnValue;                                                 \
        }                                                                       \
    } while (0)

// --------------------------------------------------
// VERIFY = error log + debug break (active in all builds), continues
// VERIFY_RETURN = same, but returns the given value afterwards
//
// Usage:
//   VERIFY(ptr != nullptr, "ptr is null");
//   VERIFY_RETURN(ptr != nullptr, false, "ptr is null");
//   VERIFY_RETURN(ptr != nullptr, nullptr, "ptr {} is null", name);
//   VERIFY_RETURN(ptr != nullptr, void(), "ptr is null");   // void function
// --------------------------------------------------

#define VERIFY(condition, fmt, ...)                                             \
    do                                                                          \
    {                                                                           \
        if (!(condition))                                                       \
        {                                                                       \
            ::Core::Log::Error("VERIFY FAILED: " fmt __VA_OPT__(,) __VA_ARGS__);\
            DEBUG_BREAK();                                                      \
        }                                                                       \
    } while (0)

#define VERIFY_RETURN(condition, returnValue, fmt, ...)                         \
    do                                                                          \
    {                                                                           \
        if (!(condition))                                                       \
        {                                                                       \
            ::Core::Log::Error("VERIFY FAILED: " fmt __VA_OPT__(,) __VA_ARGS__);\
            DEBUG_BREAK();                                                      \
            return returnValue;                                                 \
        }                                                                       \
    } while (0)

// --------------------------------------------------
// ASSERT = fatal log + (debug break | message box) + exit (always fatal)
//
// Usage:
//   ASSERT(device != nullptr, "Device creation failed (HRESULT {:#x})", hr);
// --------------------------------------------------

#define ASSERT(condition, fmt, ...)                                             \
    do                                                                          \
    {                                                                           \
        if (!(condition))                                                       \
        {                                                                       \
            const std::string assertMsg__ =                                     \
                std::format("ASSERT FAILED: " fmt __VA_OPT__(,) __VA_ARGS__);   \
            ::Core::Log::Fatal("{}", assertMsg__);                              \
            DEBUG_BREAK();                                                      \
            SHOW_FATAL_MESSAGE_BOX(assertMsg__);                                \
            std::_Exit(EXIT_FAILURE);                                           \
        }                                                                       \
    } while (0)