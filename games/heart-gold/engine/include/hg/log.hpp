#pragma once

#include <functional>
#include <string_view>

namespace hg {

enum class LogLevel { Debug, Info, Warn, Error };

std::string_view log_level_name(LogLevel level);

using LogSink = std::function<void(LogLevel, std::string_view)>;

// Mensagens abaixo do nível mínimo são descartadas.
void set_log_level(LogLevel min_level);

// Troca o destino das mensagens (padrão: stderr). Passe nullptr para voltar ao padrão.
void set_log_sink(LogSink sink);

void log(LogLevel level, std::string_view message);

inline void log_debug(std::string_view message) { log(LogLevel::Debug, message); }
inline void log_info(std::string_view message) { log(LogLevel::Info, message); }
inline void log_warn(std::string_view message) { log(LogLevel::Warn, message); }
inline void log_error(std::string_view message) { log(LogLevel::Error, message); }

} // namespace hg
