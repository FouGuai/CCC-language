#include "AST_tree.h"
#include "Syntax.h"

std::stack<Environment *> envStk;
Environment *total;

void Program_Node::excute()
{
    envStk.push(new Environment());
    total = envStk.top();
    for (int i = 0; i < programs.size(); ++i)
    {
        programs[i]->excute();
    }
    delete envStk.top();
    envStk.pop();
}

Object AST_Node::getValue()
{
    throw SyntaxError("The expr don't return a value");
    return Object();
}

void AST_Node::excute()
{
    throw SyntaxError("Invalid stmt");
}

void AST_Node::setValue(Object &obj)
{
    throw SyntaxError("syntax error");
}

void Stmts_Node::excute()
{
    try
    {
        envStk.push(new Environment(envStk.top()));

        for (int i = 0; i < stmts.size(); ++i)
        {
            stmts[i]->excute();
        }

        delete envStk.top();
        envStk.pop();
    }
    catch (const FunctionReturnException &returnExc)
    {
        delete envStk.top();
        envStk.pop();
        throw;
    }
}

void If_Node::excute()
{
    Object boolObj = boolExpr->getValue();
    if (!boolObj.getBool())
    {
        if (haveElse)
            elseStmt->excute();
    }
    else
    {
        ifStmt->excute();
    }
}

void While_Node::excute()
{
    while (1)
    {
        Object boolObj = boolExpr->getValue();
        // boolObj.toInt();
        if (!boolObj.getBool())
        {
            break;
        }
        try
        {
            loopStmt->excute();
        }
        catch (const ContinueException &c)
        {
            continue;
        }
        catch (const BreakException &b)
        {
            break;
        }
    }
}

void For_Node::excute()
{
    first->getValue();
    while (1)
    {
        Object boolObj = second->getValue();
        if (!boolObj.getBool())
            break;
        try
        {
            loopStmt->excute();
        }
        catch (const BreakException &b)
        {
            break;
        }
        catch (const ContinueException &c)
        {
            /*empty*/
        }
        third->getValue();
    }
}

Object Expr_Node::getValue()
{
    return next->getValue();
}

Object BinaryOp_Node::getValue()
{

    // 短路运算
    if (op == "&&")
    {
        Object leftObj = left->getValue();
        switch (leftObj.getType()->basicType)
        {
        case ObjectType::INT:
            return (leftObj.getIntVal() == 0 ? Object(0LL) : And(leftObj, right->getValue()));
            break;
        case ObjectType::DOUBLE:
            return (leftObj.getDoubleVal() == 0.0 ? Object(0.0) : And(leftObj, right->getValue()));
        default:
            throw TypeException("In" + op + " Type can't match");
            break;
        }
    }
    else if (op == "||")
    {
        Object leftObj = left->getValue();
        switch (leftObj.getType()->basicType)
        {
        case ObjectType::INT:
            return (leftObj.getIntVal() != 0 ? Object(1LL) : leftObj || right->getValue());
            break;
        case ObjectType::DOUBLE:
            return (leftObj.getDoubleVal() != 0.0 ? Object(1.0) : leftObj || right->getValue());
        default:
            throw TypeException("In" + op + " Type can't match");
            break;
        }
    }
    else
    {
        Object leftObj = left->getValue();
        Object rightObj = right->getValue();

        switch (OP_MAP[op])
        {
        case OperatorType::Addition:
            return leftObj + rightObj;
            break;
        case OperatorType::Subtraction:
            return leftObj - rightObj;
            break;
        case OperatorType::Multiplication:
            return leftObj * rightObj;
            break;
        case OperatorType::Division:
            return leftObj / rightObj;
            break;
        case OperatorType::Modulus:
            return leftObj % rightObj;
            break;
        case OperatorType::Equal:
            return equal(leftObj, rightObj);
            break;
        case OperatorType::NotEqual:
            return leftObj != rightObj;
            break;
        case OperatorType::Greater:
            return leftObj > rightObj;
            break;
        case OperatorType::Less:
            return leftObj < rightObj;
            break;
        case OperatorType::GreaterEqual:
            return leftObj >= rightObj;
            break;
        case OperatorType::LessEqual:
            return leftObj <= rightObj;
            break;
        case OperatorType::BitOr:
            return leftObj | rightObj;
            break;
        case OperatorType::BitAnd:
            return leftObj & rightObj;
            break;
        case OperatorType::BitXor:
            return leftObj ^ rightObj;
            break;
        case OperatorType::Bitleft:
            return leftObj << rightObj;
            break;
        case OperatorType::BitRight:
            return leftObj >> rightObj;
            break;
        }
    }
    throw SyntaxError("The operator is undefined.");
}

// Object BinaryOp_Node::getValue()
// {
//     switch (OP_MAP[op])
//     {
//     case OperatorType::Not:
//         break;

//     default:
//         break;
//     }
// }

Object Id_Node::getValue()
{
    return envStk.top()->getObject(idName);
}

void Id_Node::setValue(Object &obj)
{
    // if (!envStk.top()->contains(idName))
    //     envStk.top()->insertObject(idName, obj);
    // else
    envStk.top()->getObject(idName).copyFrom(obj);
}

Object Assign_Node::getValue()
{
    Object obj = expr->getValue();
    Object idValue = id->getValue();
#ifdef DEBUG
    std::cout << assignOp << std::endl;
#endif
    switch (OP_MAP[assignOp])
    {
    case OperatorType::Assignment:
        break;
    case OperatorType::AssAddition:
        obj = obj + idValue;
        break;
    case OperatorType::AssSubstraction:
        obj = obj - idValue;
        break;
    case OperatorType::AssMultiplication:
        obj = obj * idValue;
        break;
    case OperatorType::AssDivision:
        obj = obj / idValue;
        break;
    case OperatorType::AssModulus:
        obj = obj % idValue;
        break;
    case OperatorType::AssBitAnd:
        obj = obj & idValue;
        break;
    case OperatorType::AssBitOr:
        obj = obj | idValue;
        break;
    case OperatorType::AssBitXor:
        obj = obj ^ idValue;
        break;
    case OperatorType::AssBitleft:
        obj = obj << idValue;
        break;
    case OperatorType::AssBitRight:
        obj = obj >> idValue;
        break;
    default:
        break;
    }
    id->setValue(obj);
    return obj;
}

/**
 * @brief 检查函数类型和参数是否匹配
 *
 * @param lhs 函数的类型
 * @param rhs 参数列表
 * @return true
 * @return false
 */
bool functionMatchType(const FuncTypeParam_t &lhs, const std::vector<Object> &rhs)
{
    if (lhs.size() == rhs.size())
    {
        for (int i = 0; i < lhs.size(); ++i)
        {
            if (*lhs[i] != *rhs[i].getType())
                return false;
        }
        return true;
    }
    return false;
}

Object Call_Node::getValue()
{
    if (functionName == "print")
    {
        for (int i = 0; i < param.size(); ++i)
        {
            std::cout << param[i]->getValue() << ' ';
        }
        std::cout << '\n';
        return Object(typeEnv.getType("void"));
    }

    Environment *nowEnv = envStk.top();
    const Object &funcObj = nowEnv->getObject(functionName);
    const FunctionObject_t &func = funcObj.getFunc();

    // 计算函数的参数
    std::vector<Object> paramObjects(param.size());
    for (int i = 0; i < param.size(); ++i)
    {
        paramObjects[i] = param[i]->getValue();
    }

    Type *funcType = funcObj.getType();

    if (!functionMatchType(funcType->getParams(), paramObjects))
    {
        throw SyntaxError("Function type can't match");
    }

    // 增添作用域
    envStk.push(new Environment(total));

    for (int i = 0; i < param.size(); ++i)
    {
        envStk.top()->insertObject(func->params[i], paramObjects[i]);
    }

    Object returnValue = func->getValue();

    // 过程调用结束, 删除函数的作用域
    delete envStk.top();
    envStk.pop();

    return returnValue;
}

Object Function_Node::getValue()
{
    try
    {
        next->excute();
    }
    catch (const FunctionReturnException &funcReturnException)
    {
        if (funcReturnException.returnVal.getType() != returnType)
            throw TypeException("In" + funcName + ", Return value can't match the return type");

        return funcReturnException.returnVal;
    }

    if (returnType != typeEnv.getType("void"))
        throw TypeException("The function " + funcName + " need a return value");
    return Object(typeEnv.getType("void"));
}

void Assign_Node::excute()
{
    Object obj = expr->getValue();
    id->setValue(obj);
}

Object Const_Node::getValue()
{
    return val;
}

void Return_Node::excute()
{
    throw FunctionReturnException(returnVal->getValue());
}

inline bool isIncreaseOp(const std::string &op)
{
    return op == "++" || op == "--";
}

Object SingleOp_Node::getValue()
{
    if (isIncreaseOp(op))
    {
        if (op == "++")
        {
            if (isPosfix)
            {
                Object returnVal = expr->getValue();
                Object rls = returnVal + Object(1LL);
                expr->setValue(rls);
                return returnVal;
            }
            else
            {
                Object returnVal = expr->getValue() + Object(1LL);
                expr->setValue(returnVal);
                return returnVal;
            }
        }
        else
        {
            if (isPosfix)
            {
                Object returnVal = expr->getValue();
                Object rls = returnVal - Object(1LL);
                expr->setValue(rls);
                return returnVal;
            }
            else
            {
                Object returnVal = expr->getValue() - Object(1LL);
                expr->setValue(returnVal);
                return returnVal;
            }
        }
    }
    else
    {
        Object exprValue = expr->getValue();
        if (op == "+")
        {
            return exprValue;
        }
        else if (op == "-")
        {
            if (exprValue.getType()->basicType == ObjectType::INT)
            {
                return Object(0LL) - exprValue;
            }
            else if (exprValue.getType()->basicType == ObjectType::DOUBLE)
            {
                return Object(0.0) - exprValue;
            }
            else
            {
                throw SyntaxError("The operator " + op + " can't match the type.");
            }
        }
        if (op == "!")
        {
            return !exprValue;
        }
        else if (op == "~")
        {
            return ~exprValue;
        }
        else
        {
            throw LexException("Unknown operator");
        }
    }
    return Object();
}

void Function_Node::excute()
{
    Object funcObj(funcType);
    funcObj.getFunc() = this;
    envStk.top()->insertObject(funcName, funcObj);

    if (funcName == "main")
    {
        getValue();
    }
}

void Declare_Node::excute()
{
    Object obj(type);
    envStk.top()->insertObject(name, obj);

    if (haveExpr)
    {
        Object exprObject = expr->getValue();
        if (*exprObject.getType() == *type)
            envStk.top()->getObject(name) = exprObject;
        else
            throw TypeException("In declare, type can't match, you need a " + type->name + " value");
    }
}

void Struct_Node::excute()
{
    Type *structType = typeEnv.getType(structName);
    structType->basicType = ObjectType::STRUCT;

    for (auto it = member.begin(); it != member.end(); ++it)
    {
        std::string typeName = std::move(it->second);
        std::string memberName = std::move(it->first);

        structType->getMember().insert(std::make_pair(memberName, typeEnv.getType(typeName)));
    }
}

bool matchStructType(Type *type, const std::string &memberName)
{
    if (type->basicType != ObjectType::STRUCT)
    {
        return false;
    }
    auto &typeMap = type->getMember();
    return typeMap.find(memberName) != typeMap.end();
}

Object StructObject_Node::getValue()
{
    Object &structObject = envStk.top()->getObject(structName);
    Type *type = structObject.getType();
    if (matchStructType(type, memberName))
    {
        return structObject.getStruct()[memberName];
    }
    else
    {
        throw SyntaxError(memberName + " isn't the member of struct " + structName);
    }
    return Object();
}

void StructObject_Node::setValue(Object &rhs)
{
    Object &structObject = envStk.top()->getObject(structName);
    if (matchStructType(structObject.getType(), memberName))
    {
        structObject.getStruct()[memberName].copyFrom(rhs);
    }
    else
    {
        throw SyntaxError(memberName + " isn't the member of struct " + structName);
    }
}

// 析构函数

Expr_Node::~Expr_Node()
{
    if (next)
        delete next;
    next = nullptr;
}

Stmts_Node::~Stmts_Node()
{
    for (int i = 0; i < stmts.size(); ++i)
    {
        if (stmts[i])
            delete stmts[i];
    }
    stmts.clear();
}

Function_Node::~Function_Node()
{
    if (next)
        delete next;
    next = nullptr;
}

If_Node::~If_Node()
{
    if (boolExpr)
        delete boolExpr;
    if (ifStmt)
        delete ifStmt;
    if (elseStmt)
        delete elseStmt;

    boolExpr = nullptr;
    ifStmt = nullptr;
    elseStmt = nullptr;
}

While_Node::~While_Node()
{
    if (boolExpr)
        delete boolExpr;
    if (loopStmt)
        delete loopStmt;
    boolExpr = nullptr;
    loopStmt = nullptr;
}

For_Node::~For_Node()
{
    if (first)
        delete first;
    if (second)
        delete second;
    if (third)
        delete third;
    if (loopStmt)
        delete loopStmt;
    first = nullptr;
    second = nullptr;
    third = nullptr;
    loopStmt = nullptr;
}

BinaryOp_Node::~BinaryOp_Node()
{
    if (left)
        delete left;
    left = nullptr;
    if (right)
        delete right;
    right = nullptr;
}

SingleOp_Node::~SingleOp_Node()
{
    if (expr)
        delete expr;
    expr = nullptr;
}

Id_Node::~Id_Node() {}

Assign_Node::~Assign_Node()
{
    if (id)
        delete id;
    id = nullptr;
    if (expr)
        delete expr;
    expr = nullptr;
}

Call_Node::~Call_Node() {}

Const_Node::~Const_Node() {}

Return_Node::~Return_Node()
{
    if (returnVal)
        delete returnVal;
    returnVal = nullptr;
}

Program_Node::~Program_Node()
{
    for (int i = 0; i < programs.size(); ++i)
    {
        if (programs[i])
            delete programs[i];
    }
    programs.clear();
}

Declare_Node::~Declare_Node()
{
    if (expr)
        delete expr;
}
