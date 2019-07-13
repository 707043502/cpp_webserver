#ifndef __SYLAR_LOG_H__
#define __SYLAR_LOG_H__

#include <fstream>
#include <list>
#include <memory>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>


namespace sylar {

class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent();

private:
    const char* m_file = nullptr;
    int32_t m_line = 0;
    uint32_t m_elpase = 0;
    uint32_t m_threadId = 0;
    uint32_t m_fiberId = 0;
    uint64_t m_time;
    std::string m_content;
};

class LogLevel {
public:
    enum Level {
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
};

class LogFormatter {
public:
    typedef std::shared_ptr<LogFormatter> ptr;
    std::string format(LogEvent::ptr event);
    LogFormatter(const std::string& pattern);

private:
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
        virtual ~FormatItem() {}
        virtual void format(std::ostream& os, LogEvent::ptr event)=0;
    };

private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
    void init();
}; 
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender() {}

    virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;
    void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
    LogFormatter::ptr getFormatter() const { return m_formatter; }

protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
};

class Logger {
public:
    typedef std::shared_ptr<Logger> ptr;

    Logger(const std::string& name = "root");

    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);

    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) { m_level = val; }

private:
    std::string m_name; // 日志名称
    LogLevel::Level m_level; //日志级别
    std::list<LogAppender::ptr> m_appenders; // Appender集合
};

class StdoutLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    void log(LogLevel::Level level, LogEvent::ptr event) override;

private:
};

class FileLogAppender : public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);

    void log(LogLevel::Level level, LogEvent::ptr event) override;
    bool reopen();

private:
    std::string m_filename;
    std::ofstream m_filestream;
};

}; // namespace sylar

#endif