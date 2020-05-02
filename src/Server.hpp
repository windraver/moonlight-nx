#include <string>
#include <vector>
#include <functional>

extern "C" {
    #include "client.h"
    #include "errors.h"
}

#pragma once

template <typename T>
struct Result {
public:
    static Result success(T value) {
        return result(value, "", true);
    }
    
    static Result failure(std::string error) {
        return result(T(), error, false);
    }
    
    bool isSuccess() const {
        return _isSuccess;
    }
    
    T value() const {
        return _value;
    }
    
    std::string error() const {
        return _error;
    }
    
private:
    static Result result(T value, std::string error, bool isSuccess) {
        Result result;
        result._value = value;
        result._error = error;
        result._isSuccess = isSuccess;
        return result;
    }
    
    T _value;
    std::string _error = "";
    bool _isSuccess = false;
};

#define ServerCallback(T) std::function<void(Result<T>)>

class Server {
public:
    static Server* server() {
        static Server server;
        return &server;
    }

    void set_working_dir(const std::string &dir) {
        m_working_dir = std::string(dir + "/moonlight");
    }
    
    void add_host(std::string address);
    std::vector<std::string> hosts();
    
    void connect(const std::string &address, ServerCallback(SERVER_DATA) callback);
    void pair(SERVER_DATA data, const std::string &pin, ServerCallback(bool) callback);
    void applist(SERVER_DATA data, ServerCallback(PAPP_LIST) callback);
    void start(SERVER_DATA data, STREAM_CONFIGURATION config, int appId, ServerCallback(STREAM_CONFIGURATION) callback);
    
private:
    Server();
    
    std::string m_working_dir;
    std::vector<std::string> m_hosts;
};
