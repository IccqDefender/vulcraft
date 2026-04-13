#include "Logger.h"
#include <iostream>
#include <chrono>
#include <ctime>

std::mutex Logger::s_mutex;
std::ofstream Logger::s_file;
bool Logger::s_fileEnabled = false;
Logger::Level Logger::s_minLevel = Logger::Level::Trace;

void Logger::Init(bool enableFile) {
    s_fileEnabled = enableFile;
    if (s_fileEnabled) {
        s_file.open("engine.log", std::ios::out | std::ios::trunc);
    }
}

void Logger::Shutdown() {
    if (s_file.is_open()) {
        s_file.close();
    }
}

void Logger::SetMinLevel(Level level) {
    s_minLevel = level;
}

const char* Logger::LevelToString(Level level) {
    switch (level) {
        case Level::Trace: return "TRACE";
        case Level::Debug: return "DEBUG";
        case Level::Info: return "INFO";
        case Level::Warn: return "WARN";
        case Level::Error: return "ERROR";
        case Level::Critical: return "CRITICAL";
    }
    return "UNKNOWN";
}

const char* Logger::TagToString(Tag tag) {
    switch (tag) {
        case Tag::Core: return "Core";
        case Tag::Vulkan: return "Vulkan";
        case Tag::IO: return "IO";
        case Tag::Memory: return "Memory";
        case Tag::Renderer: return "Renderer";
        case Tag::World: return "World";
    }
    return "Unknown";
}

const char* Logger::LevelToColor(Level level) {
    switch (level) {
        case Level::Trace: return "\033[37m";
        case Level::Debug: return "\033[36m";
        case Level::Info: return "\033[32m";
        case Level::Warn: return "\033[33m";
        case Level::Error: return "\033[31m";
        case Level::Critical: return "\033[41m";
    }
    return "\033[0m";
}

void Logger::Log(Level level, Tag tag, const std::string &message) {
    if (level < s_minLevel) {
        return;
    }

    std::lock_guard<std::mutex> lock(s_mutex);

    std::string finalMsg =
        std::string(LevelToColor(level)) +
        "[" + LevelToString(level) + "]" +
        "[" + TagToString(tag) + "] " +
        message +
        "\033[0m";

    std::cout << finalMsg << std::endl;

    if (s_fileEnabled && s_file.is_open()) {
        s_file << "[" << LevelToString(level) << "]"
               << "[" << TagToString(tag) << "] "
               << message << std::endl;
    }
}
