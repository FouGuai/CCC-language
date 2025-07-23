#include "Environment.h"

const char *EnvironmentException::what() const noexcept
{
    return info.data();
}

Object &Environment::getObject(const std::string &str)
{
    Environment *nowEnv = this;
    while (nowEnv && nowEnv->objects.find(str) == nowEnv->objects.end())
    {
        nowEnv = nowEnv->next;
    }

    if (!nowEnv)
    {
        std::string exceptionInfo;
        exceptionInfo += str;
        exceptionInfo += " is not existed";
        throw EnvironmentException(exceptionInfo);
    }

    return nowEnv->objects[str];
}

void Environment::insertObject(const std::string &str, Object &object)
{
    if (objects.find(str) != objects.end())
    {
        std::string info;
        info += str;
        info += " is redefine.";
        throw EnvironmentException(info.data());
    }
    else
    {
        objects.insert(std::make_pair(str, object));
    }
}

bool Environment::contains(const std::string &str) const
{
    const Environment *nowEnv = this;
    while (nowEnv && nowEnv->objects.find(str) == nowEnv->objects.end())
    {
        nowEnv = nowEnv->next;
    }
    return nowEnv != nullptr;
    // objects.find(str) != objects.end();
}
