#include <iostream>
#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <stdexcept>

class JsonWriter {
    std::unique_ptr<std::mutex> _mutex;
    std::ofstream _file;
    bool _first_element;
    std::string _filename;
    
public:
    JsonWriter(const std::string& filename) : _mutex(std::make_unique<std::mutex>()), _first_element(true), _filename(filename)
    {
        _file.open(filename);
        if (!_file.is_open()) {
            throw std::runtime_error("Can't open file: " + filename);
        }
        _file << "{\n";
    }
        
    ~JsonWriter() {
        if (_file.is_open()) {
            try {
                _file << "\n}\n";
                _file.close();
            } catch (...) {}
        }
        // указатель удалится сам
    }
        
    JsonWriter(const JsonWriter&) = delete;
    JsonWriter& operator=(const JsonWriter&) = delete;
        
    JsonWriter(JsonWriter&& other) = default; 
    JsonWriter& operator=(JsonWriter&& other) = default;
        
    void Add(const std::string& key, const std::string& value) {
        std::lock_guard<std::mutex> lock(*_mutex);
           
        if (!_file.is_open()) {
            throw std::runtime_error("File isn't open: " + _filename);
        }
            
        if (!_first_element) {
            _file << ",\n";
        } else {
            _first_element = false;
        }
           
        std::string changed_value;
        changed_value.reserve(value.length());
        for (char c : value) {
            if (c == '"') {
                changed_value += "\\\"";
            } else if (c == '\\') {
                changed_value += "\\\\";
            } else {
                changed_value += c;
            }
        }
            
        _file << "  \"" << key << "\": \"" << changed_value << "\"";
            
        if (!_file) {
            throw std::runtime_error("Failed to write to file: " + _filename);
        }
    }
        
    std::string GetFilename() const {
        return _filename;
    }
};
