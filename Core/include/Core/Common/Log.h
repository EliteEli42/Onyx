#pragma once

#include <chrono>
#include <concepts>
#include <format>
#include <mutex>
#include <source_location>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <functional>

namespace Core
{
    enum class LogLevel
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal,
        Off
    };

    struct LogEntry
    {
        LogLevel level;
        std::string message;

        std::source_location location;
        std::chrono::system_clock::time_point timestamp;
    };

    template<typename... Args>
    struct FormatLoc
    {
        std::format_string<Args...> fmt;
        std::source_location loc;

        template<typename T>
            requires std::convertible_to<const T&, std::string_view>
        consteval FormatLoc(
            const T& str,
            std::source_location l = std::source_location::current())
            : fmt(str)
            , loc(l)
        {}
    };

    class Log
    {
    public:
        static void SetLevel(LogLevel level)
        {
            sCurrentLevel = level;
        }

        template<typename... Args>
        static void Trace(
            FormatLoc<std::type_identity_t<Args>...> f,
            Args&&... args)
        {
            Message(LogLevel::Trace, std::format(f.fmt, std::forward<Args>(args)...), f.loc);
        }

        template<typename... Args>
        static void Debug(
            FormatLoc<std::type_identity_t<Args>...> f,
            Args&&... args)
        {
            Message(LogLevel::Debug, std::format(f.fmt, std::forward<Args>(args)...), f.loc);
        }

        template<typename... Args>
        static void Info(
            FormatLoc<std::type_identity_t<Args>...> f,
            Args&&... args)
        {
            Message(LogLevel::Info, std::format(f.fmt, std::forward<Args>(args)...), f.loc);
        }

        template<typename... Args>
        static void Warn(
            FormatLoc<std::type_identity_t<Args>...> f,
            Args&&... args)
        {
            Message(LogLevel::Warn, std::format(f.fmt, std::forward<Args>(args)...), f.loc);
        }

        template<typename... Args>
        static void Error(
            FormatLoc<std::type_identity_t<Args>...> f,
            Args&&... args)
        {
            Message(LogLevel::Error, std::format(f.fmt, std::forward<Args>(args)...), f.loc);
        }

        template<typename... Args>
        static void Fatal(
            FormatLoc<std::type_identity_t<Args>...> f,
            Args&&... args)
        {
            Message(LogLevel::Fatal, std::format(f.fmt, std::forward<Args>(args)...), f.loc);
        }

        using ErrorCallback = std::function<void(LogEntry)>;
        static void SetErrorCallback(ErrorCallback callback) { sErrorCallback = std::move(callback); }

    private:
        static void Message(
            LogLevel level,
            std::string message,
            std::source_location location);

    private:
        inline static LogLevel sCurrentLevel = LogLevel::Info;
        inline static std::mutex sMutex;
        inline static ErrorCallback sErrorCallback = nullptr;
    };
}