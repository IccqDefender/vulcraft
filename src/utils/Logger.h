#ifndef VULCRAFT_LOGGER_H
#define VULCRAFT_LOGGER_H

#include "LoggerMacros.h"

#include <string>
#include <mutex>
#include <fstream>

class Logger {
public:
    enum class Level {
        Trace,
        Debug,
        Info,
        Warn,
        Error,
        Critical,
    };
    enum class Tag {
        Core,
        Vulkan,
        Renderer,
        Memory,
        IO,
        World
    };
public:
    static void Init(bool enableFile = false);
    static void Shutdown();

    static void Log(Level level, Tag tag, const std::string& message);

    static void SetMinLevel(Level level);
private:
    static const char* LevelToString(Level level);
    static const char* TagToString(Tag tag);
    static const char* LevelToColor(Level level);
private:
    static std::mutex s_mutex;
    static std::ofstream s_file;
    static bool s_fileEnabled;
    static Level s_minLevel;
};



#endif //VULCRAFT_LOGGER_H
