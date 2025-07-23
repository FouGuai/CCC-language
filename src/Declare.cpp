#include "Syntax.h"

// extern Environment *total;

inline bool isLeftBrack(Token *t)
{
    return (t->type == PUNC && t->getStringInfo() == "(");
}

inline bool isRightBrack(Token *t)
{
    return (t->type == PUNC && t->getStringInfo() == ")");
}

inline bool isType(Token *t)
{
    if (t->type == KEY)
    {
        std::string keyName = t->getStringInfo();
        return keyName == "int" || keyName == "double" || keyName == "string";
    }
    else if (t->type == ID)
    {
        return typeEnv.contains(t->getStringInfo());
    }
    return false;
}

// type id(params) {stmts}
Function_Node *Syntax::Function()
{
    // Function_Node *functionNode = new Function_Node();
    Token *typeToken = lex.next();
    // type
    if (isType(typeToken))
    {
        std::vector<Type *> typeArr;
        std::vector<std::string> paramNameArr;
        Type *returnType = typeEnv.getType(typeToken->getStringInfo());
        Token *idToken = lex.next();

        // id
        if (idToken->type == ID)
        {
            std::string funcName = idToken->getStringInfo();

            // (
            Token *leftBrack = lex.next();
            if (!isLeftBrack(leftBrack))
            {
                lex.last();
                throw SyntaxError("In function " + funcName + ", Expect '('");
            }

            // parms
            Token *paramTypeToken = lex.next();
            while (isType(paramTypeToken))
            {
                typeArr.push_back(typeEnv.getType(paramTypeToken->getStringInfo()));
                Token *paramNameToken = lex.next();
                if (paramNameToken->type == ID)
                {
                    paramNameArr.push_back(paramNameToken->getStringInfo());
                }
                else
                {
                    throw SyntaxError("In function " + funcName + "'s param, need a id");
                }

                Token *puncToken = lex.next();
                // )
                if (isRightBrack(puncToken))
                {
                    break;
                }
                else if (!(puncToken->type == PUNC && puncToken->getStringInfo() == ","))
                {
                    throw SyntaxError("In function " + funcName + "'s param, need a ',' or ')'");
                }
                paramTypeToken = lex.next();
            }

            // lex.last();

            // Token *rightBrack = lex.next();
            // if (!isRightBrack(rightBrack))
            // {
            //     lex.last();
            //     throw SyntaxError("Exprect )");
            // }

            // {stmt}
            Token *leftStmtBrack = lex.next();
            if (!(leftStmtBrack->type == PUNC && leftStmtBrack->getStringInfo() == "{"))
            {
                lex.last();
                throw SyntaxError("In function " + funcName + ", lack function body");
            }

            Stmts_Node *stmtsNode = stmts();
            Token *rightStmtBrack = lex.next();
            if (!(leftStmtBrack->type == PUNC && leftStmtBrack->getStringInfo() == "{"))
            {
                lex.last();
                throw SyntaxError("In function " + funcName + ", '{' isn't match");
            }

            // 生成 Function_Node
            Function_Node *funcNode = new Function_Node;

            funcNode->funcName = funcName;
            funcNode->next = stmtsNode;
            funcNode->params = paramNameArr;

            Type *funcType = new FuncType(funcName);
            funcType->getParams() = typeArr;
            typeEnv.insertType(funcName, funcType);

            funcNode->funcType = funcType;
            funcNode->returnType = returnType;
            return funcNode;
        }
    }
    lex.last();
    throw SyntaxError("Invalid function");
    return nullptr;
}

Struct_Node *Syntax::Struct()
{
    Token *structToken = lex.next();
    if (structToken->type == KEY && structToken->getStringInfo() == "struct")
    {
        Token *structNameToken = lex.next();
        if (!(structNameToken->type == ID))
        {
            lex.last();
            throw SyntaxError("Expect a id name as a struct name");
        }
        std::string structName = structNameToken->getStringInfo();

        Token *leftStmtToken = lex.next();
        if (!(leftStmtToken->type == PUNC && leftStmtToken->getStringInfo() == "{"))
        {
            lex.last();
            throw SyntaxError("In struct " + structName + ", expect '{'");
        }

        typeEnv.insertType(structName, new StructType(structName));
        std::unordered_map<std::string, std::string> member;
        Token *typeToken = lex.next();
        while (isType(typeToken))
        {
            // lex.last();
            Token *idNameToken = lex.next();
            if (idNameToken->type == ID)
            {
                member.insert(std::make_pair(idNameToken->getStringInfo(), typeToken->getStringInfo()));
                Token *puncToken = lex.next();
                if (!(puncToken->type == PUNC && puncToken->getStringInfo() == ";"))
                {
                    lex.last();
                    throw SyntaxError("In struct " + structName + ", after member " + idNameToken->getStringInfo() + ", expect a ';'");
                }
            }
            else
            {
                lex.last();
                throw SyntaxError("In struct " + structName + ", you need a id as member name");
            }
            typeToken = lex.next();
        }

        Struct_Node *structNode = new Struct_Node;
        structNode->member = member;
        structNode->structName = structName;
        return structNode;
    }
    lex.last();
    throw SyntaxError("Expect struct");
    return nullptr;
}