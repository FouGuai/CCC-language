#ifndef TYPE_H
#define TYPE_H

#include "config.h"
#include <vector>
#include <unordered_map>

enum class ObjectType
{
    INT,
    DOUBLE,
    STRING,
    STRUCT,
    FUNCTION,
    VOID,
    ARRAY
};

class TypeException : public std::exception
{
private:
    std::string info;

public:
    TypeException(const std::string &info) : info(info) {}
    const char *what() const noexcept override
    {
        return info.data();
    }
};

class Type;

typedef std::vector<Type *> FuncTypeParam_t;

typedef std::unordered_map<std::string, Type *> StructTypeMember_t;

// 每个对象的类型， 每个Type 必须以动态内存指针的形式存在TypeEnvironment中
class Type
{
    friend bool operator==(const Type &lhs, const Type &rhs);
    friend bool operator!=(const Type &lhs, const Type &rhs);

public:
    // 类型名称
    std::string name;
    ObjectType basicType;

    Type() : basicType(ObjectType::INT) {}
    Type(const std::string typeName, ObjectType type = ObjectType::INT) : name(typeName), basicType(type) {}

    virtual FuncTypeParam_t &getParams();
    virtual StructTypeMember_t &getMember();
    virtual std::pair<Type *, size_t> getArray();
    bool isEqual(const Type &rhs) const;
};

class FuncType : public Type
{
public:
    FuncType(const std::string &typeName) : Type(typeName, ObjectType::FUNCTION) {}
    FuncTypeParam_t params;
    FuncTypeParam_t &getParams() override;
};

class StructType : public Type
{
public:
    StructType(const std::string &typeName) : Type(typeName, ObjectType::STRUCT) {}
    StructTypeMember_t member;
    StructTypeMember_t &getMember() override;
};

class ArrayType : public Type
{
public:
    ArrayType() : Type("", ObjectType::ARRAY) {}
    Type *arrType;
    size_t size;

    std::pair<Type *, size_t> getArray() override;
};

/**
 * @brief 给出两个类型的隐式转换
 *
 * @param lhs
 * @param rhs
 * @return Type* 返回nullptr代表两个类型不能转换
 */
Type *matchType(Type *lhs, Type *rhs);

#endif