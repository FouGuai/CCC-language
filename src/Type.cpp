#include "Type.h"

bool operator==(const Type &lhs, const Type &rhs)
{
    return lhs.isEqual(rhs);
}

bool operator!=(const Type &lhs, const Type &rhs)
{
    return !(lhs == rhs);
}

bool Type::isEqual(const Type &rhs) const
{
    if (basicType == rhs.basicType && name == rhs.name)
    {
        if (basicType == ObjectType::FUNCTION)
        {
            const FuncType *lhsFunc = dynamic_cast<const FuncType *>(this);
            const FuncType *rhsFunc = dynamic_cast<const FuncType *>(&rhs);

            return lhsFunc->params == rhsFunc->params;
        }
        else if (basicType == ObjectType::STRUCT)
        {
            const StructType *lhsFunc = dynamic_cast<const StructType *>(this);
            const StructType *rhsFunc = dynamic_cast<const StructType *>(&rhs);

            return lhsFunc->member == rhsFunc->member;
        }
        else
        {
            return true;
        }
    }
    return false;
}

FuncTypeParam_t &Type::getParams()
{
    throw TypeException("It's not function");
}

FuncTypeParam_t &FuncType::getParams()
{
    return params;
}

StructTypeMember_t &Type::getMember()
{
    throw TypeException("It's not struct");
}

StructTypeMember_t &StructType::getMember()
{
    return member;
}

Type *matchType(Type *lhs, Type *rhs)
{
    switch (lhs->basicType)
    {
    case ObjectType::INT:
        switch (rhs->basicType)
        {
        case ObjectType::INT:
            return lhs;
            break;
        case ObjectType::DOUBLE:
            return rhs;
            break;
        default:
            return nullptr;
            break;
        }
        break;
    case ObjectType::DOUBLE:
        switch (rhs->basicType)
        {
        case ObjectType::INT:
            return lhs;
            break;
        case ObjectType::DOUBLE:
            return lhs;
            break;
        default:
            return nullptr;
            break;
        }
        break;
    case ObjectType::STRING:
        switch (rhs->basicType)
        {
        case ObjectType::STRING:
            return lhs;
            break;
        default:
            return nullptr;
            break;
        }
        break;

    default:
        if (*lhs == *rhs)
            return lhs;
        return nullptr;
        break;
    }
}

std::pair<Type *, size_t> Type::getArray()
{
    throw TypeException("Not array type");
}

std::pair<Type *, size_t> ArrayType::getArray()
{
    return std::make_pair(arrType, size);
}
