#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <stdexcept>
#include <string>
#include <sstream>

// 定义类型枚举
enum class Type
{
    Int,
    Float,
    String,
    Struct,
    Function,
    Void,
    // 其他类型可以在这里添加
};

// 类型类
class ASTType
{
public:
    virtual ~ASTType() = default;
    virtual std::string toString() const = 0;
    virtual bool isCompatible(const std::shared_ptr<ASTType> &other) const = 0;
    virtual std::shared_ptr<ASTType> getCommonType(const std::shared_ptr<ASTType> &other) const = 0;
};

class IntType : public ASTType
{
public:
    std::string toString() const override
    {
        return "int";
    }

    bool isCompatible(const std::shared_ptr<ASTType> &other) const override
    {
        return dynamic_cast<IntType *>(other.get()) != nullptr;
    }

    std::shared_ptr<ASTType> getCommonType(const std::shared_ptr<ASTType> &other) const override
    {
        if (isCompatible(other))
        {
            return std::make_shared<IntType>();
        }
        if (dynamic_cast<FloatType *>(other.get()))
        {
            return std::make_shared<FloatType>();
        }
        throw std::runtime_error("Incompatible types");
    }
};

class FloatType : public ASTType
{
public:
    std::string toString() const override
    {
        return "float";
    }

    bool isCompatible(const std::shared_ptr<ASTType> &other) const override
    {
        return dynamic_cast<FloatType *>(other.get()) != nullptr;
    }

    std::shared_ptr<ASTType> getCommonType(const std::shared_ptr<ASTType> &other) const override
    {
        if (isCompatible(other))
        {
            return std::make_shared<FloatType>();
        }
        if (dynamic_cast<IntType *>(other.get()))
        {
            return std::make_shared<FloatType>();
        }
        throw std::runtime_error("Incompatible types");
    }
};

class StringType : public ASTType
{
public:
    std::string toString() const override
    {
        return "string";
    }

    bool isCompatible(const std::shared_ptr<ASTType> &other) const override
    {
        return dynamic_cast<StringType *>(other.get()) != nullptr;
    }

    std::shared_ptr<ASTType> getCommonType(const std::shared_ptr<ASTType> &other) const override
    {
        if (isCompatible(other))
        {
            return std::make_shared<StringType>();
        }
        throw std::runtime_error("Incompatible types");
    }
};

class VoidType : public ASTType
{
public:
    std::string toString() const override
    {
        return "void";
    }

    bool isCompatible(const std::shared_ptr<ASTType> &other) const override
    {
        return dynamic_cast<VoidType *>(other.get()) != nullptr;
    }

    std::shared_ptr<ASTType> getCommonType(const std::shared_ptr<ASTType> &other) const override
    {
        if (isCompatible(other))
        {
            return std::make_shared<VoidType>();
        }
        throw std::runtime_error("Incompatible types");
    }
};

class FunctionType : public ASTType
{
public:
    FunctionType(std::shared_ptr<ASTType> returnType, const std::vector<std::shared_ptr<ASTType>> &paramTypes)
        : returnType(returnType), paramTypes(paramTypes) {}

    std::string toString() const override
    {
        std::ostringstream oss;
        oss << "function (";
        for (size_t i = 0; i < paramTypes.size(); ++i)
        {
            if (i > 0)
                oss << ", ";
            oss << paramTypes[i]->toString();
        }
        oss << ") -> " << returnType->toString();
        return oss.str();
    }

    bool isCompatible(const std::shared_ptr<ASTType> &other) const override
    {
        auto otherFunc = dynamic_cast<FunctionType *>(other.get());
        if (!otherFunc)
            return false;
        if (!returnType->isCompatible(otherFunc->returnType))
            return false;
        if (paramTypes.size() != otherFunc->paramTypes.size())
            return false;
        for (size_t i = 0; i < paramTypes.size(); ++i)
        {
            if (!paramTypes[i]->isCompatible(otherFunc->paramTypes[i]))
                return false;
        }
        return true;
    }

    std::shared_ptr<ASTType> getCommonType(const std::shared_ptr<ASTType> &other) const override
    {
        if (isCompatible(other))
        {
            return std::make_shared<FunctionType>(returnType, paramTypes);
        }
        throw std::runtime_error("Incompatible types");
    }

    std::shared_ptr<ASTType> getReturnType() const
    {
        return returnType;
    }

    const std::vector<std::shared_ptr<ASTType>> &getParamTypes() const
    {
        return paramTypes;
    }

private:
    std::shared_ptr<ASTType> returnType;
    std::vector<std::shared_ptr<ASTType>> paramTypes;
};