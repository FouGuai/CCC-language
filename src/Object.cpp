#include "Object.h"
#include <iostream>

bool Object::isMatch(Type *t) const
{
    return matchType(type, t) != nullptr;
}

long long &Object::getIntVal()
{
    return std::get<(int)ObjectType::INT>(obj);
}

double &Object::getDoubleVal()
{
    return std::get<(int)ObjectType::DOUBLE>(obj);
}

std::string &Object::getStringVal()
{
    return std::get<(int)ObjectType::STRING>(obj);
}

StructObject_t &Object::getStruct()
{
    return std::get<(int)ObjectType::STRUCT>(obj);
}

FunctionObject_t &Object::getFunc()
{
    return std::get<(int)ObjectType::FUNCTION>(obj);
}

long long Object::getIntVal() const
{
    return std::get<(int)ObjectType::INT>(obj);
}

// StructObject_t &Object::getStruct()
// {
//     return std::get<(int)ObjectType::STRUCT>(obj);
// }

double Object::getDoubleVal() const
{
    return std::get<(int)ObjectType::DOUBLE>(obj);
}

const std::string &Object::getStringVal() const
{
    return std::get<(int)ObjectType::STRING>(obj);
}

const StructObject_t &Object::getStruct() const
{
    return std::get<(int)ObjectType::STRUCT>(obj);
}

const FunctionObject_t &Object::getFunc() const
{
    return std::get<(int)ObjectType::FUNCTION>(obj);
}

bool Object::getBool() const
{
    switch (type->basicType)
    {
    case ObjectType::INT:
        return getIntVal() != 0;
        break;
    case ObjectType::DOUBLE:
        return getDoubleVal() != 0.0;
        break;
    default:
        throw TypeException("The type can't convert to bool");
        break;
    }
}

void Object::toInt()
{
    switch (type->basicType)
    {
    case ObjectType::INT:
        break;
    case ObjectType::DOUBLE:
        obj = (long long)getDoubleVal();
        break;

    default:
        throw TypeException(type->name + "can't convert to int");
        break;
    }
    type = typeEnv.getType("int");
}

void Object::toDouble()
{
    switch (type->basicType)
    {
    case ObjectType::INT:
        obj = (double)getIntVal();
        break;
    case ObjectType::DOUBLE:
        break;

    default:
        throw TypeException(type->name + "can't convert to double");
        break;
    }
    type = typeEnv.getType("double");
}

// 运算的辅助函数
ObjectType commomObject(Object &lhs, Object &rhs)
{
    switch (lhs.getType()->basicType)
    {
    case ObjectType::INT:
        switch (rhs.getType()->basicType)
        {
        case ObjectType::INT:
            return ObjectType::INT;
            break;
        case ObjectType::DOUBLE:
            lhs.toDouble();
            return ObjectType::DOUBLE;
            break;
        default:
            throw TypeException("Type can't match");
            break;
        }
        break;
    case ObjectType::DOUBLE:
        switch (rhs.getType()->basicType)
        {
        case ObjectType::INT:
            rhs.toDouble();
        case ObjectType::DOUBLE:
            return ObjectType::DOUBLE;
            break;
        default:
            throw TypeException("Type can't match");
            break;
        }
        break;
    case ObjectType::STRING:
        switch (rhs.getType()->basicType)
        {
        case ObjectType::STRING:
            return ObjectType::STRING;
            break;
        default:
            throw TypeException("Type can't match");
            break;
        }
        break;
    case ObjectType::FUNCTION:
        switch (rhs.getType()->basicType)
        {
        case ObjectType::FUNCTION:
            return ObjectType::FUNCTION;
            break;
        default:
            throw TypeException("Type can't match");
            break;
        }
        break;
    case ObjectType::STRUCT:
        switch (rhs.getType()->basicType)
        {
        case ObjectType::STRUCT:
            return ObjectType::STRUCT;
            break;
        default:
            throw TypeException("Type can't match");
            break;
        }
        break;
    }

    throw TypeException("Type can't match");
    return ObjectType::VOID;
}

Object::Object(Type *t) : type(t)
{
    switch (type->basicType)
    {
    case ObjectType::INT:
        obj = 0LL;
        break;
    case ObjectType::DOUBLE:
        obj = 0.0;
        break;
    case ObjectType::STRING:
        obj = "";
        break;
    case ObjectType::STRUCT:
    {
        StructObject_t structObject = StructObject_t();
        auto &typeMap = type->getMember();
        for (auto it = typeMap.begin(); it != typeMap.end(); ++it)
        {
            if (it->second == type)
            {
                throw TypeException("can't use the type in this struct");
            }
            structObject.insert(std::make_pair(it->first, Object(it->second)));
        }
        obj = std::move(structObject);
    }
    break;
    case ObjectType::FUNCTION:
        obj = FunctionObject_t();
        break;
    default:
        break;
    }
}

Object operator+(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object(newLhs.getIntVal() + newRhs.getIntVal());
        break;
    case ObjectType::DOUBLE:
        return Object(newLhs.getDoubleVal() + newRhs.getDoubleVal());
        break;
    case ObjectType::STRING:
        return Object(newLhs.getStringVal() + newRhs.getStringVal());
        break;
    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator-(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object(newLhs.getIntVal() - newRhs.getIntVal());
        break;
    case ObjectType::DOUBLE:
        return Object(newLhs.getDoubleVal() - newRhs.getDoubleVal());
        break;

    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator*(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object(newLhs.getIntVal() * newRhs.getIntVal());
        break;
    case ObjectType::DOUBLE:
        return Object(newLhs.getDoubleVal() * newRhs.getDoubleVal());
        break;

    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator/(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object(newLhs.getIntVal() / newRhs.getIntVal());
        break;
    case ObjectType::DOUBLE:
        return Object(newLhs.getDoubleVal() / newRhs.getDoubleVal());
        break;

    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator%(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object(newLhs.getIntVal() % newRhs.getIntVal());
        break;
    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object equal(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((long long)(newLhs.getIntVal() == newRhs.getIntVal()));
        break;
    case ObjectType::DOUBLE:
        return Object((long long)(newLhs.getDoubleVal() == newRhs.getDoubleVal()));
        break;
    case ObjectType::STRING:
        return Object((long long)(newLhs.getStringVal() == newRhs.getStringVal()));
        break;
    case ObjectType::STRUCT:
        return Object((long long)(newLhs.getStruct() == newRhs.getStruct()));
        break;
    case ObjectType::FUNCTION:
        return Object((long long)(newLhs.getFunc() == newRhs.getFunc()));
        break;
    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator!=(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((long long)(newLhs.getIntVal() != newRhs.getIntVal()));
        break;
    case ObjectType::DOUBLE:
        return Object((long long)(newLhs.getDoubleVal() != newRhs.getDoubleVal()));
        break;
    case ObjectType::STRING:
        return Object((long long)(newLhs.getStringVal() != newRhs.getStringVal()));
        break;
    case ObjectType::STRUCT:
        return Object((long long)(newLhs.getStruct() != newRhs.getStruct()));
        break;
    case ObjectType::FUNCTION:
        return Object((long long)(newLhs.getFunc() != newRhs.getFunc()));
        break;
    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator>(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((long long)(newLhs.getIntVal() > newRhs.getIntVal()));
        break;
    case ObjectType::DOUBLE:
        return Object((long long)(newLhs.getDoubleVal() > newRhs.getDoubleVal()));
        break;
    case ObjectType::STRING:
        return Object((long long)(newLhs.getStringVal() > newRhs.getStringVal()));
        break;

    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator>=(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((long long)(newLhs.getIntVal() >= newRhs.getIntVal()));
        break;
    case ObjectType::DOUBLE:
        return Object((long long)(newLhs.getDoubleVal() >= newRhs.getDoubleVal()));
        break;
    case ObjectType::STRING:
        return Object((long long)(newLhs.getStringVal() >= newRhs.getStringVal()));
        break;

    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator<(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((long long)(newLhs.getIntVal() < newRhs.getIntVal()));
        break;
    case ObjectType::DOUBLE:
        return Object((long long)(newLhs.getDoubleVal() < newRhs.getDoubleVal()));
        break;
    case ObjectType::STRING:
        return Object((long long)(newLhs.getStringVal() < newRhs.getStringVal()));
        break;

    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator<=(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((long long)(newLhs.getIntVal() <= newRhs.getIntVal()));
        break;
    case ObjectType::DOUBLE:
        return Object((long long)(newLhs.getDoubleVal() <= newRhs.getDoubleVal()));
        break;
    case ObjectType::STRING:
        return Object((long long)(newLhs.getStringVal() <= newRhs.getStringVal()));
        break;

    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator&(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((newLhs.getIntVal() & newRhs.getIntVal()));
        break;
    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator|(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((newLhs.getIntVal() | newRhs.getIntVal()));
        break;
    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator^(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((newLhs.getIntVal() ^ newRhs.getIntVal()));
        break;
    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object And(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((long long)(newLhs.getIntVal() && newRhs.getIntVal()));
        break;
    case ObjectType::DOUBLE:
        return Object((long long)(newLhs.getDoubleVal() && newRhs.getDoubleVal()));
        break;

    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator||(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((long long)(newLhs.getIntVal() || newRhs.getIntVal()));
        break;
    case ObjectType::DOUBLE:
        return Object((long long)(newLhs.getDoubleVal() || newRhs.getDoubleVal()));
        break;

    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator!(const Object &lhs)
{
    switch (lhs.getType()->basicType)
    {
    case ObjectType::INT:
        return Object((long long)(!lhs.getIntVal()));
        break;
    case ObjectType::DOUBLE:
        return Object((long long)(!lhs.getDoubleVal()));
        break;
    default:
        throw TypeException("Type can't match");
        break;
    }
}

Object operator>>(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((long long)(newLhs.getIntVal() >> newRhs.getIntVal()));
        break;
    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

Object operator<<(const Object &lhs, const Object &rhs)
{
    Object newLhs = lhs, newRhs = rhs;
    ObjectType c_type = commomObject(newLhs, newRhs);
    switch (c_type)
    {
    case ObjectType::INT:
        return Object((long long)(newLhs.getIntVal() << newRhs.getIntVal()));
        break;
    default:
        throw TypeException("Type can't match");
        break;
    }
    return Object();
}

std::ostream &operator<<(std::ostream &os, const Object &rhs)
{
    switch (rhs.getType()->basicType)
    {
    case ObjectType::INT:
        os << rhs.getIntVal();
        break;
    case ObjectType::DOUBLE:
        os << rhs.getDoubleVal();
        break;
    case ObjectType::STRING:
        os << rhs.getStringVal();
        break;

    default:
        throw TypeException("You can't print the type");
        break;
    }
    return os;
}

bool operator==(const Object &lhs, const Object &rhs)
{
    return *lhs.getType() == *rhs.getType() && lhs.obj == rhs.obj;
}

void Object::copyFrom(const Object &rhs)
{
    if (type->basicType == ObjectType::STRUCT || type->basicType == ObjectType::FUNCTION)
    {
        if (*type == *rhs.type)
        {
            obj = rhs.obj;
        }
    }
    else if (type->basicType == ObjectType::STRING)
    {
        if (rhs.type->basicType == ObjectType::STRING)
            obj = rhs.obj;
        else
            throw TypeException("can't convert to string");
    }
    else
    {
        switch (type->basicType)
        {
        case ObjectType::INT:
            switch (rhs.type->basicType)
            {
            case ObjectType::INT:
                getIntVal() = rhs.getIntVal();
                break;
            case ObjectType::DOUBLE:
                getIntVal() = (long long)rhs.getDoubleVal();
                break;
            default:
                throw TypeException("can't convert to int");
                break;
            }
            break;
        case ObjectType::DOUBLE:
            switch (rhs.type->basicType)
            {
            case ObjectType::INT:
                getDoubleVal() = (double)rhs.getIntVal();
                break;
            case ObjectType::DOUBLE:
                getDoubleVal() = rhs.getDoubleVal();
                break;
            default:
                throw TypeException("can't convert to double");
                break;
            }
        default:
            throw TypeException("Unknown type");
            break;
        }
    }
}

Object operator~(const Object &lhs)
{
    if (lhs.getType()->basicType == ObjectType::INT)
    {
        return Object(~lhs.getIntVal());
    }
    else
    {
        throw TypeException("Can't convert to int.");
    }
    return Object();
}