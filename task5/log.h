#include <string>
#include <vector>
#include <ctime>
#include <iostream>

enum LogLevel {
    LOG_NORMAL = 0,
    LOG_WARNING = 1,
    LOG_ERROR = 2
};

struct Record {
    time_t timestamp;
    LogLevel level;
    std::string message;
};

class Log {
private:
    std::vector<Record> storage;
    static const size_t MAX_RECORDS = 10;

    Log();
    Log(const Log&) = delete;
    Log& operator=(const Log&) = delete;
    
    std::string level_print(LogLevel level_);
    std::string time_print(time_t timestamp_);
    
public:
    static Log* Instance();
    void message(LogLevel level_, const std::string& message_);
    void print();
};
