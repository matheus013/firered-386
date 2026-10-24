#include "hg/log.hpp"

#include <chrono>
#include <cstdio>
#include <mutex>
#include <utility>

namespace hg {

namespace {

std::mutex g_mutex;
LogLevel g_min_level = LogLevel::Info;
LogSink g_sink;
const auto g_start = std::chrono::steady_clock::now();

void default_sink(LogLevel level, std::string_view message)
{
    const auto elapsed = std::chrono::duration<double>(std::chrono::steady_clock::now() - g_start);
    const auto name = log_level_name(level);
    std::fprintf(stderr, "[%9.3f] %-5.*s %.*s\n", elapsed.count(), static_cast<int>(name.size()), name.data(),
                 static_cast<int>(message.size()), message.data());
}

} // namespace

std::string_view log_level_name(LogLevel level)
{
    switch (level) {
    case LogLevel::Debug: return "DEBUG";
    case LogLevel::Info: return "INFO";
    case LogLevel::Warn: return "WARN";
    case LogLevel::Error: return "ERROR";
    }
    return "?";
}

void set_log_level(LogLevel min_level)
{
    std::lock_guard lock(g_mutex);
    g_min_level = min_level;
}

void set_log_sink(LogSink sink)
{
    std::lock_guard lock(g_mutex);
    g_sink = std::move(sink);
}

void log(LogLevel level, std::string_view message)
{
    std::lock_guard lock(g_mutex);
    if (level < g_min_level) {
        return;
    }
    if (g_sink) {
        g_sink(level, message);
    } else {
        default_sink(level, message);
    }
}

} // namespace hg
