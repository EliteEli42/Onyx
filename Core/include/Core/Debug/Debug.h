#pragma once

#include <Core/Log/Log.h>
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
// CHECK = warning + debug break
// --------------------------------------------------

#if defined(DEBUG)

    #define CHECK(condition, message)                                   \
            do                                                          \
            {                                                           \
                if (!(condition))                                       \
                {                                                       \
                    ::Core::Log::Warn(                                  \
                        std::string("CHECK FAILED: ") + (message));     \
                    DEBUG_BREAK();                                      \
                }                                                       \
            } while (0)

#else

    #define CHECK(condition, message) ((void)0)

#endif

// --------------------------------------------------
// ASSERT = error + break + abort (always fatal)
// --------------------------------------------------

#define ASSERT(condition, message)                                  \
    do                                                              \
    {                                                               \
        if (!(condition))                                           \
        {                                                           \
            ::Core::Log::Fatal(                                     \
                std::string("ASSERT FAILED: ") + (message));        \
            DEBUG_BREAK();                                          \
            std::exit(EXIT_FAILURE);                                \
        }                                                           \
    } while (0)
