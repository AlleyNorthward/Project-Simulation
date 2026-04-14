#ifndef OH_LOG_H
#define OH_LOG_H

namespace sdust {

enum class LogLevel { ERROR = 0, WARN, INFO, DEBUG };

class Log {
private:
  LogLevel _level = LogLevel::DEBUG;

public:
  static Log &instance();
  void setLevel(LogLevel level) { _level = level; };
  LogLevel level() const { return _level; }

  void info(const char *fmt, ...);
  void error(const char *fmt, ...);
  void debug(const char *fmt, ...);
  void warn(const char *fmt, ...);

private:
  Log() = default;
  ~Log() = default;
  Log(const Log &) = delete;
  Log &operator=(const Log &) = delete;
};

} // namespace sdust

#ifdef LOG_ENABLE

#define LOGE(...) sdust::Log::instance().error(__VA_ARGS__)
#define LOGW(...) sdust::Log::instance().warn(__VA_ARGS__)
#define LOGI(...) sdust::Log::instance().info(__VA_ARGS__)
#define LOGD(...) sdust::Log::instance().debug(__VA_ARGS__)

#else

#define LOGE(...)
#define LOGW(...)
#define LOGI(...)
#define LOGD(...)

#endif

#define LOG_SET_LEVEL(level) sdust::Log::instance().setLevel(level)

#endif
