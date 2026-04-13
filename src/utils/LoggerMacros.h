#pragma once
#include "Logger.h"

#define LOG_TRACE(tag,msg) Logger::Log(Logger::level::TRACE,tag,msg)
#define LOG_DEBUG(tag,msg) Logger::Log(Logger::level::DEBUG,tag,msg);
#define LOG_INFO(tag,msg) Logger::Log(Logger::level::INFO,tag,msg);
#define LOG_WARN(tag,msg) Logger::Log(Logger::level::WARN,tag,msg);
#define LOG_ERROR(tag,msg) Logger::Log(Logger::level::ERROR,tag,msg);
#define LOG_CRITICAL(tag,msg) Logger::Log(Logger::level::CRITICAL,tag,msg);

#define LOGI(msg) Logger::Log(Logger::level::INFO,msg)
#define LOGE(msg) Logger::Log(Logger::level::ERROR,msg)

