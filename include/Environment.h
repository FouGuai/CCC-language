#ifndef ENVIRONMENT_H
#define EMVIRONMENT_H

#include "config.h"
#include "Object.h"
#include <unordered_map>
#include "Type.h"

struct EnvironmentException : public std::exception
{
private:
    std::string info;

public:
    EnvironmentException(const std::string &info) : info(info) {}
    const char *what() const noexcept override;
};

class Environment
{
private:
    std::unordered_map<std::string, Object> objects;
    Environment *next = nullptr;

public:
    // Environment() = default;
    Environment(Environment *next = nullptr) : next(next) {}
    void setNext(Environment *next)
    {
        this->next = next;
    }

    bool contains(const std::string &str) const;
    Object &getObject(const std::string &str);
    void insertObject(const std::string &str, Object &object);
};



#endif