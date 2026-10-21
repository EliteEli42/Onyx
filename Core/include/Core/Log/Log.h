#pragma once

#include <string>
#include <source_location>
#include <mutex>
#include <queue>
#include <chrono>

namespace Core {

    enum class LogLevel
    {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Fatal
    };

    struct LogEntry
    {
        LogLevel level;
        std::string message;

        std::source_location location;
        std::chrono::system_clock::time_point timestamp;
    };

    class Log
    {
    public:


    public:
        inline static void SetLevel(LogLevel level) { sCurrentLevel = level; }
        inline static void EnableTimeStamp(bool enable) { sTimeStamp = enable; }
        inline static void EnableSourceLocation(bool enable) { sSourceLocation = enable; }
        inline static void SetLogFile(const std::string& filePath) { sLogFilePath = filePath; }

        inline static void Trace(const std::string& msg,
            std::source_location loc = std::source_location::current())
        {
            Message(LogLevel::Trace, msg, loc);
        }

        inline static void Debug(const std::string& msg,
            std::source_location loc = std::source_location::current())
        {
            Message(LogLevel::Debug, msg, loc);
        }

        inline static void Info(const std::string& msg,
            std::source_location loc = std::source_location::current())
        {
            Message(LogLevel::Info, msg, loc);
        }

        inline static void Warn(const std::string& msg,
            std::source_location loc = std::source_location::current())
        {
            Message(LogLevel::Warn, msg, loc);
        }

        inline static void Error(const std::string& msg,
            std::source_location loc = std::source_location::current())
        {
            Message(LogLevel::Error, msg, loc);
        }

        inline static void Fatal(const std::string& msg,
            std::source_location loc = std::source_location::current())
        {
            Message(LogLevel::Fatal, msg, loc);
        }

    private:
        static void Message(
            LogLevel level,
            const std::string& message,
            std::source_location location);

    private:
        inline static LogLevel sCurrentLevel = LogLevel::Info;
        inline static bool sTimeStamp = true;
        inline static bool sSourceLocation = true;
        inline static std::mutex sMutex;

        inline static std::string sLogFilePath = "";

        inline static std::queue<LogEntry> sLogQueue;
    };

} // namespace Core
