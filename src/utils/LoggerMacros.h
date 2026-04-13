#pragma once

#define LOG_TRACE(tag,msg) Logger::Log(Logger::Level::Trace,tag,msg)
#define LOG_DEBUG(tag,msg) Logger::Log(Logger::Level::Debug,tag,msg);
#define LOG_INFO(tag,msg) Logger::Log(Logger::Level::Info,tag,msg);
#define LOG_WARN(tag,msg) Logger::Log(Logger::Level::Warn,tag,msg);
#define LOG_ERROR(tag,msg) Logger::Log(Logger::Level::Error,tag,msg);
#define LOG_CRITICAL(tag,msg) Logger::Log(Logger::Level::Critical,tag,msg);

#define LOGI(msg) Logger::Log(Logger::Level::Info,msg)
#define LOGE(msg) Logger::Log(Logger::Level::Error,msg)

