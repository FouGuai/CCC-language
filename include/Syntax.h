#ifndef SYNTAX_H
#define SYNTAX_H

#include "Object.h"
#include <exception>
#include "Lexer.h"
#include "AST_tree.h"

class SyntaxError : public std::exception
{
     std::string info;

public:
    SyntaxError(const std::string &info) : info(info) {}
    const char *what() const noexcept override
    {
        return info.data();
    }
};

class Syntax
{
private:
    Lexer lex;
    // // 存储指针， 用于最后的析构
    // std::unordered_set<Environment *> __env_set;
    // // 用于追踪作用域
    // std::stack<Environment *> __envs;

    Environment *nowEnv;

public:
    AST_Node *analyse();

    // 分析表达式
    Expr_Node *expr2(Expr_Node *left);
    Expr_Node *lland();

    Expr_Node *expr();
    Expr_Node *expr1(Expr_Node *left);

    Expr_Node *land();
    Expr_Node *land1(Expr_Node *left);

    Expr_Node *Or();
    Expr_Node *Or1(Expr_Node *left);

    Expr_Node *Xor();
    Expr_Node *Xor1(Expr_Node *left);

    Expr_Node *And();
    Expr_Node *And1(Expr_Node *left);

    Expr_Node *equal();
    Expr_Node *equal1(Expr_Node *left);

    Expr_Node *cmp();
    Expr_Node *cmp1(Expr_Node *left);

    Expr_Node *sal();
    Expr_Node *sal1(Expr_Node *left);

    Expr_Node *add();
    Expr_Node *add1(Expr_Node *left);

    Expr_Node *term();
    Expr_Node *term1(Expr_Node *left);

    Expr_Node *single();

    Expr_Node *factor();

    Expr_Node *params(const std::string &idName);
    void params1(Call_Node *callNode);

    // 流程控制语句
    Stmt_Node *For();
    Stmt_Node *If();
    Stmt_Node *While();

    Function_Node *Function();
    Struct_Node *Struct();
    Stmts_Node *stmts();
    Stmt_Node *stmt();
};

bool isExprFirst(Token *t);
#endif