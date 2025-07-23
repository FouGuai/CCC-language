#ifndef AST_TREE_H
#define AST_TREE_H

/**
 * @file AST_tree.h
 * @author foushen (foushenio@outlook.com)
 * @brief 此文件定义了语法分析书的节点和分析过程， 创建语法分析书的过程是由
 * 语法分析器完成的， 其定义在 Syntax.h 中。
 * @version 0.1
 * @date 2024-06-19
 *
 * @copyright Copyright (c) 2024
 *
 */
// #include "Syntax.h"
#include "Environment.h"
#include <stack>

enum class AST_NodeYype
{
    WHILE,
    IF,
    FOR,
    BIN_OP,
    SIG_OP,
    EXPR,
    STMT
};

class FunctionReturnException
{
public:
    Object returnVal;
    FunctionReturnException(const Object &returnValue) : returnVal(returnValue) {}
};

class BreakException : public std::exception
{
public:
    const char *what() const noexcept 
    {
        return "Break only can be used in for stmt";
    }
};

class ContinueException : public std::exception
{
public:
    const char *what() const noexcept 
    {
        return "continue only can be used in for stmt";
    }
};

class AST_Node
{
public:
    virtual void excute();
    virtual Object getValue();
    virtual void setValue(Object &obj);

    virtual ~AST_Node(){};
};

// 类的前向声明

class If_Node;
class Expr_Node;
class Function_Node;
class Stmt_Node;
class Call_Node;
class While_Node;
class BinaryOp_Node;
class Assign_Node;
class Return_Node;

class Program_Node : public AST_Node
{
public:
    std::vector<AST_Node *> programs;
    void excute() override;
    ~Program_Node();
};

class Stmt_Node : public AST_Node
{
public:
    ~Stmt_Node(){};
    /*empty*/
};

class Expr_Node : public Stmt_Node
{
public:
    AST_Node *next = nullptr;
    Object getValue() override;
    void excute() override
    {
        getValue();
    }
    ~Expr_Node() override;
};

class Stmts_Node : public Stmt_Node
{
public:
    std::vector<Stmt_Node *> stmts;
    void excute() override;
    ~Stmts_Node() override;
};

class Function_Node : public AST_Node
{
public:
    std::string funcName;
    Type *returnType = nullptr;
    Type *funcType = nullptr;

    /**
     * @brief 参数类型信息在funcType中
     *  std::string 为形参名字
     * 语法分析器保证这里的形参类型和funcType中一一对应
     */
    std::vector<std::string> params;

    // Environment *funEnv;
    AST_Node *next;

    void excute() override;
    Object getValue() override;
    ~Function_Node() override;
};

class If_Node : public Stmt_Node
{
public:
    void excute() override;
    Expr_Node *boolExpr = nullptr;
    Stmt_Node *ifStmt = nullptr;
    Stmt_Node *elseStmt = nullptr;
    bool haveElse = false;
    ~If_Node() override;
};

class While_Node : public Stmt_Node
{
public:
    Expr_Node *boolExpr = nullptr;
    Stmt_Node *loopStmt = nullptr;
    void excute() override;
    ~While_Node() override;
};

class For_Node : public Stmt_Node
{
public:
    Expr_Node *first = nullptr;
    Expr_Node *second = nullptr;
    Expr_Node *third = nullptr;
    Stmt_Node *loopStmt = nullptr;
    void excute() override;
    ~For_Node() override;
};

class BinaryOp_Node : public Expr_Node
{
public:
    std::string op;
    Expr_Node *left = nullptr;
    Expr_Node *right = nullptr;

    Object getValue() override;
    ~BinaryOp_Node() override;
};

class SingleOp_Node : public Expr_Node
{
public:
    // 用于自增运算符
    bool isPosfix = false;
    std::string op;
    Expr_Node *expr = nullptr;
    Object getValue() override;
    ~SingleOp_Node() override;
};

class Id_Node : public Expr_Node
{
public:
    // Environment *env;
    std::string idName;

    Object getValue() override;
    void setValue(Object &obj) override;
    ~Id_Node() override;
};

class Assign_Node : public Expr_Node
{
public:
    std::string assignOp;
    Expr_Node *id = nullptr;
    Expr_Node *expr = nullptr;

    Object getValue() override;
    void excute() override;

    ~Assign_Node() override;
};

class Call_Node : public Expr_Node
{
public:
    std::vector<Expr_Node *> param;
    std::string functionName;
    Object getValue() override;
    // void excute() override;

    ~Call_Node() override;
};

class Const_Node : public Expr_Node
{
public:
    Object val;
    Object getValue() override;
    ~Const_Node() override;
};

class Return_Node : public Stmt_Node
{
public:
    Expr_Node *returnVal = nullptr;
    void excute() override;
    ~Return_Node() override;
};

class Declare_Node : public Stmt_Node
{
public:
    void excute() override;

    ~Declare_Node() override;

    Type *type;
    std::string name;

    bool haveExpr = false;
    Expr_Node *expr;
};

class NoneStmt_Node : public Stmt_Node
{
public:
    // 不执行任何操作
    void excute() override {}
};

class NoneExpr_Node : public Expr_Node
{
public:
    Object getValue() override
    {
        return Object(1LL);
    }
};

class Struct_Node : public AST_Node
{
public:
    std::string structName;
    std::unordered_map<std::string, std::string> member;
    void excute() override;
    ~Struct_Node() override {}
};

class Break_Node : public Stmt_Node
{
public:
    void excute() override { throw BreakException(); }
    ~Break_Node() override {}
};

class Continue_Node : public Stmt_Node
{
public:
    void excute() override { throw ContinueException(); }
    ~Continue_Node() override {}
};

class StructObject_Node : public Expr_Node
{
public:
    std::string structName;
    std::string memberName;

    Object getValue() override;
    void setValue(Object &rhs) override;

    ~StructObject_Node() override{};
};

// class AST_tree
// {
// private:
//     std::stack<Environment *> env_stk;
//     AST_Node *root;
// };
#endif