#include "Syntax.h"

// stmts -> stmt stmts | null
// stmt -> for (expr; expr; expr) stmt
// stmt -> if (epxr) elseStmt;
// elseStmt -> null | else stmt
// stmt -> while (expr) stmt
// stmt -> {stmt}
// stmt -> expr;

bool isStmtFirst(Token *t)
{
    if (isExprFirst(t))
        return true;
    else if (t->type == PUNC && t->getStringInfo() == "{")
        return true;
    else if (t->type == KEY)
        return true;
    return false;
}

inline bool isLeftBrack(Token *t)
{
    return (t->type == PUNC && t->getStringInfo() == "(");
}

inline bool isRightBrack(Token *t)
{
    return (t->type == PUNC && t->getStringInfo() == ")");
}

Stmt_Node *Syntax::For()
{
    Token *token = lex.next();
    if (token->type == KEY && token->getStringInfo() == "for")
    {
        For_Node *forNode = new For_Node;
        // Environment *newEnv = new Environment(__envs.top());
        // __env_set.insert(newEnv);
        // __envs.push(newEnv);
        // nowEnv = __envs.top();

        // (
        Token *t1 = lex.next();
        if (!isLeftBrack(t1))
            throw SyntaxError("syntax in for statate");

        // expr1
        // Token *t2 = lex.next();
        Token *t2 = lex.next();
        lex.last();
        if (isExprFirst(t2))
            forNode->first = expr();
        else
            forNode->first = new NoneExpr_Node();

        // ;
        Token *t3 = lex.next();
        if (!(t3->type == PUNC && t3->getStringInfo() == ";"))
            throw SyntaxError("syntax in for statate");

        // expr2
        Token *t4 = lex.next();
        lex.last();
        if (isExprFirst(t4))
            forNode->second = expr();
        else
            forNode->second = new NoneExpr_Node();

        // ;
        Token *t5 = lex.next();
        if (!(t5->type == PUNC && t5->getStringInfo() == ";"))
            throw SyntaxError("syntax in for statate");

        // expr3
        Token *t6 = lex.next();
        lex.last();
        if (isExprFirst(t6))
            forNode->third = expr();
        else
            forNode->third = new NoneExpr_Node();

        // )
        Token *t7 = lex.next();
        if (!isRightBrack(t7))
            throw SyntaxError("syntax in for statate");

        Token *t8 = lex.next();
        if (!isStmtFirst(t8))
            throw SyntaxError("please input a stmt");
        lex.last();
        forNode->loopStmt = stmt();

        // __envs.pop();
        return forNode;
    }
    throw SyntaxError("syntax in for statate");
    return nullptr;
}

// if -> "if" (epxr) elseStmt;
// elseStmt -> null | else stmt
Stmt_Node *Syntax::If()
{
    Token *ifToken = lex.next();

    // if
    if (ifToken->type == KEY && ifToken->getStringInfo() == "if")
    {
        If_Node *ifNode = new If_Node;
        ifNode->haveElse = false;
        Token *t1 = lex.next();

        // (
        if (!isLeftBrack(t1))
        {
            lex.last();
            throw SyntaxError("syntax in if statate");
        }

        // expr;
        ifNode->boolExpr = expr();

        // )
        Token *t2 = lex.next();
        if (!isRightBrack(t2))
        {
            throw SyntaxError("brack isn't match in if statate");
        }

        // ifStmt
        ifNode->ifStmt = stmt();

        // else
        Token *elseToken = lex.next();
        if (elseToken->type == KEY && elseToken->getStringInfo() == "else")
        {
            ifNode->haveElse = true;
            // elseStmt
            ifNode->elseStmt = stmt();
        }
        else
        {
            lex.last();
        }
        return ifNode;
    }
    throw SyntaxError("syntax in if statate");
    return nullptr;
}

// while -> "while" (expr) stmt
Stmt_Node *Syntax::While()
{
    Token *whileToken = lex.next();
    if (whileToken->type == KEY && whileToken->getStringInfo() == "while")
    {
        While_Node *whileNode = new While_Node;
        Token *leftBrack = lex.next();

        // (
        if (!isLeftBrack(leftBrack))
        {
            lex.last();
            throw SyntaxError("syntax in if statate");
        }

        whileNode->boolExpr = expr();

        Token *rightBrack = lex.next();
        if (!isRightBrack(rightBrack))
        {
            lex.last();
            throw SyntaxError("brack isn't match in while statate");
        }

        whileNode->loopStmt = stmt();
        return whileNode;
    }
    lex.last();
    throw SyntaxError("syntax in for statate");
    return nullptr;
}

// stmt -> while | if | for | expr; | type id; | block | {stmts}
// stmts -> stmt stmts | null
Stmt_Node *Syntax::stmt()
{
    Token *token = lex.next();
    if (token->type == KEY)
    {
        if (token->getStringInfo() == "while")
        {
            lex.last();
            return While();
        }
        else if (token->getStringInfo() == "if")
        {
            lex.last();
            return If();
        }
        else if (token->getStringInfo() == "for")
        {
            lex.last();
            Stmt_Node *forNode = For();
            return forNode;
        }
        else if (token->getStringInfo() == "int" || token->getStringInfo() == "double" || token->getStringInfo() == "string")
        {
            Token *idNameToken = lex.next();
            if (idNameToken->type == ID)
            {
                Declare_Node *declareNode = new Declare_Node();
                declareNode->name = idNameToken->getStringInfo();
                declareNode->type = typeEnv.getType(token->getStringInfo());
                try
                {
                    Token *equalToken = lex.next();
                    if (equalToken->type == OP && equalToken->getStringInfo() == "=")
                    {
                        Token *exprToken = lex.next();
                        lex.last();
                        if (isExprFirst(exprToken))
                        {
                            declareNode->haveExpr = true;
                            declareNode->expr = expr();
                        }
                    }
                    else
                    {
                        lex.last();
                    }

                    Token *punc = lex.next();

                    if (!(punc->getStringInfo() == ";"))
                    {
                        throw SyntaxError("Expect ;");
                    }

                    return declareNode;
                }
                catch (...)
                {
                    delete declareNode;
                    throw;
                }
            }
            else
            {
                throw SyntaxError("Expect id");
            }
        }
        else if (token->getStringInfo() == "return")
        {
            Return_Node *returnNode = nullptr;
            Token *exprToken = lex.next();
            if (isExprFirst(exprToken))
            {
                lex.last();
                returnNode = new Return_Node;
                returnNode->returnVal = expr();
            }
            else
            {
                lex.last();
                throw SyntaxError("Need a expr");
            }

            Token *punc = lex.next();
            if (!(punc->getStringInfo() == ";"))
            {
                throw SyntaxError("Expect ;");
            }

            return returnNode;
        }
        else if (token->getStringInfo() == "break")
        {
            Token *punc = lex.next();
            if (!(punc->getStringInfo() == ";"))
            {
                throw SyntaxError("Expect ;");
            }
            return new Break_Node();
        }
        else if (token->getStringInfo() == "continue")
        {
            Token *punc = lex.next();
            if (!(punc->getStringInfo() == ";"))
            {
                throw SyntaxError("Expect ;");
            }
            return new Continue_Node();
        }
    }
    else if (token->type == PUNC)
    {
        if (token->getStringInfo() == "{")
        {
            Stmt_Node *stmtNode = stmts();
            Token *puncToken = lex.next();
            if (!(puncToken->type == PUNC && puncToken->getStringInfo() == "}"))
            {
                lex.last();
                throw SyntaxError("{ is not match");
            }
            return stmtNode;
        }
        else if (token->getStringInfo() == ";")
        {
            return new NoneStmt_Node;
        }
    }
    else if (token->type == ID && typeEnv.contains(token->getStringInfo()) && typeEnv.getType(token->getStringInfo())->basicType != ObjectType::FUNCTION)
    {
        Token *idNameToken = lex.next();
        if (idNameToken->type == ID)
        {
            Declare_Node *declareNode = new Declare_Node();
            declareNode->name = idNameToken->getStringInfo();
            declareNode->type = typeEnv.getType(token->getStringInfo());
            try
            {
                Token *equalToken = lex.next();
                if (equalToken->type == OP && equalToken->getStringInfo() == "=")
                {
                    Token *exprToken = lex.next();
                    lex.last();
                    if (isExprFirst(exprToken))
                    {
                        declareNode->haveExpr = true;
                        declareNode->expr = expr();
                    }
                }
                else
                {
                    lex.last();
                }

                Token *punc = lex.next();

                if (!(punc->getStringInfo() == ";"))
                {
                    throw SyntaxError("Expect ;");
                }

                return declareNode;
            }
            catch (...)
            {
                delete declareNode;
                throw;
            }
        }
        else
        {
            throw SyntaxError("Expect id");
        }
    }
    else if (isExprFirst(token))
    {
        lex.last();
        Expr_Node *exprNode = expr();
        Token *punc = lex.next();
        if (!(punc->getStringInfo() == ";"))
        {
            throw SyntaxError("Expect ;");
        }
        return exprNode;
    }
    throw SyntaxError("Illegal stmt");
}

Stmts_Node *Syntax::stmts()
{
    Stmts_Node *stmtsNode = new Stmts_Node;
    Token *stmtToken = lex.next();
    while (1)
    {
        lex.last();
        if (!isStmtFirst(stmtToken))
            break;
        stmtsNode->stmts.push_back(stmt());
        stmtToken = lex.next();
    }
    return stmtsNode;
};

bool isFuncFirst(Token *t)
{
    return (t->type == KEY || t->type == ID) && typeEnv.contains(t->getStringInfo());
}

// 全局变量
// pargram -> type id;
// pargram -> function;
AST_Node *Syntax::analyse()
{
    Program_Node *paramNode = new Program_Node();
    Token *t = lex.next();
    while (1)
    {
        lex.last();
        if (isFuncFirst(t))
            paramNode->programs.push_back(Function());
        else if (t->type == KEY && t->getStringInfo() == "struct")
            paramNode->programs.push_back(Struct());
        else
            break;
        t = lex.next();
    }

    // paramNode->excute();
    lex.releaseMem();
    return paramNode;
}