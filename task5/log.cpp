#include "log.h"
#include <algorithm>
#include <iomanip>
#include <sstream>

Log::Log() {
    storage.reserve(MAX_RECORDS);
}

Log* Log::Instance() {
    static Log element;
    return &element;
}

void Log::message(LogLevel level_, const std::string& message_) {
    Record line;
    line.timestamp = time(nullptr);
    line.level = level_;
    line.message = message_;
    
    if (storage.size() >= MAX_RECORDS) {
        storage.erase(storage.begin());
    }
    storage.push_back(line);
}

void Log::print(){
    std::cout << std::endl;
    if (storage.empty()) {
        std::cout << "Log is empty" << std::endl;
        return;
    }
        
    for (size_t i = 0; i < storage.size(); ++i) {
        auto& line = storage[i];
        std::cout << time_print(line.timestamp) << ", " << level_print(line.level) << ", " << line.message << std::endl;
    }
    std::cout << std::endl;
}

std::string Log::level_print(LogLevel level_){
    switch (level_) {
        case LOG_NORMAL:  
            return "NORMAL";
        case LOG_WARNING: 
            return "WARNING";
        case LOG_ERROR:   
            return "ERROR";
    }
}

std::string Log::time_print(time_t timestamp_){
    char buffer[100];
    struct tm* timeinfo = localtime(&timestamp_);
    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    return std::string(buffer);
}