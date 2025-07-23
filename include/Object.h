#ifndef OBJECT_H
#define OBJECT_H

#include "Type.h"
#include "config.h"
#include <vector>
#include <variant>
// #include <unordered_map>
// #include "Environment.h"
#include "TypeEnvironment.h"

class AST_Node;
class Function_Node;

extern TypeEnvironment typeEnv;

struct Field
{
    std::string name;
};

class Object;

/**
 * @brief 定义函数对象类型
 * std::vector<Object> 为实参
 * Function_Node * 为函数代码在AST_tree的地址
 */
class Object;
typedef Function_Node *FunctionObject_t;
typedef std::unordered_map<std::string, Object> StructObject_t;

class Object
{
    friend Object operator+(const Object &lhs, const Object &rhs);
    friend Object operator-(const Object &lhs, const Object &rhs);
    friend Object operator*(const Object &lhs, const Object &rhs);
    friend Object operator/(const Object &lhs, const Object &rhs);
    friend Object operator%(const Object &lhs, const Object &rhs);
    friend bool operator==(const Object &lhs, const Object &rhs);
    friend Object equal(const Object &lhs, const Object &rhs);
    friend Object operator!=(const Object &lhs, const Object &rhs);
    friend Object operator>(const Object &lhs, const Object &rhs);
    friend Object operator>=(const Object &lhs, const Object &rhs);
    friend Object operator<(const Object &lhs, const Object &rhs);
    friend Object operator<=(const Object &lhs, const Object &rhs);
    friend Object operator&(const Object &lhs, const Object &rhs);
    friend Object operator|(const Object &lhs, const Object &rhs);
    friend Object operator^(const Object &lhs, const Object &rhs);
    friend Object And(const Object &lhs, const Object &rhs);
    friend Object operator||(const Object &lhs, const Object &rhs);
    friend Object operator>>(const Object &lhs, const Object &rhs);
    friend Object operator<<(const Object &lhs, const Object &rhs);
    friend Object operator!(const Object &lhs);
    friend Object operator~(const Object &lhs);
    friend std::ostream &operator<<(std::ostream &os, const Object &rhs);

public:
private:
    // object 的类型
    Type *type;

    // 对象的值
    std::variant<long long, double, std::string, StructObject_t, FunctionObject_t> obj;

public:
    Object() : type(typeEnv.getType("int")), obj(0) {}
    explicit Object(long long val) : type(typeEnv.getType("int")), obj(val) {}
    explicit Object(double val) : type(typeEnv.getType("double")), obj(val) {}
    explicit Object(const std::string &str) : type(typeEnv.getType("string")), obj(str) {}
    Object(Type *t);
    void copyFrom(const Object &rhs);

    // 转化为int类型
    void toInt();

    // 转化为double类型
    void toDouble();

    // 得到改类型的bool 类型
    bool getBool() const;

    void toType(Type *type);

    Type *getType() const
    {
        return type;
    }

    /**
     * @brief 该对象是否和类型t匹配
     *
     * @param t
     * @return true
     * @return false
     */
    bool isMatch(Type *t) const;

    long long &getIntVal();
    double &getDoubleVal();
    std::string &getStringVal();
    // std::unordered_map<std::string, Object> &getStruct();
    FunctionObject_t &getFunc();
    StructObject_t &getStruct();

    long long getIntVal() const;
    double getDoubleVal() const;
    const std::string &getStringVal() const;
    const StructObject_t &getStruct() const;
    const FunctionObject_t &getFunc() const;
};

#endif