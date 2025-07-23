#ifndef TYPE_ENVIRONMENT_H
#define TYPE_ENVIRONMENT_H
// type为全局作用域
#include "Type.h"

class TypeEnvironment
{
private:
    // 动态内存

    std::unordered_map<std::string, Type *> types;

public:
    bool contains(const std::string &typeName) const;
    Type *getType(const std::string &typeName);
    void insertType(const std::string &typeName, Type *type);
    TypeEnvironment();

    // 会自动回收内存
    ~TypeEnvironment();
};
#endif