#include "Core/Log/Log.h"

#include "ConsoleColor.h"

#include <iostream>
#include <mutex>
#include <filesystem>

namespace Core
{
    static const char* LevelToString(LogLevel level) {
        switch (level)
        {
        case LogLevel::Trace:   return "[TRACE]   ";
        case LogLevel::Debug:   return "[DEBUG]   ";
        case LogLevel::Info:    return "[INFO]    ";
        case LogLevel::Warn:    return "[WARN]    ";
        case LogLevel::Error:   return "[ERROR]   ";
        case LogLevel::Fatal:   return "[FATAL]   ";

        default:                return "[UNKNOWN] ";
        }
    }

    static const char* LevelToColor(LogLevel level) {
        switch (level)
        {
        case LogLevel::Trace:   return ConsoleColor::BrightBlack;
        case LogLevel::Debug:   return ConsoleColor::Cyan;
        case LogLevel::Info:    return ConsoleColor::BrightWhite;
        case LogLevel::Warn:    return ConsoleColor::BrightYellow;
        case LogLevel::Error:   return ConsoleColor::BrightRed;
        case LogLevel::Fatal:   return ConsoleColor::Red;

        default:                return ConsoleColor::Reset;
        }
    }

    static std::string TimePointToString(const std::chrono::system_clock::time_point& timePoint) {
        std::time_t timeT = std::chrono::system_clock::to_time_t(timePoint);

        std::tm localTime{};
#ifdef _WIN32
        localtime_s(&localTime, &timeT);
#else
        localtime_r(&timeT, &localTime);
#endif

        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &localTime);

        return std::string(buffer);
    }

    void Log::Message(LogLevel level, const std::string& message, std::source_location location) {

        auto now = std::chrono::system_clock::now();

        if (level < sCurrentLevel)
            return;

        std::string logMessage = "";

        logMessage.append(LevelToColor(level));

        if (sTimeStamp)
        {
            logMessage.append("[");
            logMessage.append(TimePointToString(now));
            logMessage.append("] ");
        }

        logMessage.append(LevelToString(level));

        if (sSourceLocation)
        {
            logMessage.append("[");
            logMessage.append(std::filesystem::path(location.file_name()).filename().string());
            logMessage.append(":");
            logMessage.append(std::to_string(location.line()));
            logMessage.append("] ");
        }

        std::lock_guard<std::mutex> lock(sMutex);

        std::cout << logMessage << message << ConsoleColor::Reset << "\n";
    }

} // namespace Core
