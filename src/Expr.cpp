#include "Syntax.h"

// left -> id;
// expr ->  = expr | left /= expr |
// left *= expr | left %= expr | left += expr |
// left -= expr | left <<= expr | left >>= expr |
// left &= expr | left ^= expr | left |= expr |
// land expr1

// expr1 -> || land expr1 | null

// land -> or land1

// land1 -> && or land1 | null

// or -> xor or1

// or1 -> | xor or1 | null

// xor -> and xor1

// xor1 -> ^ and xor1 | null

// and -> equal and1

// and1 -> & equal and1 | null

// eqaul -> cmp equal1

// equal1 -> == cmp equal1 | != cmo equal1 | null

// cmp -> sal cmp1

// cmp1 -> > sal cmp1 | >= sal cmp1 | < sal cmp1 | <= sal cmp1 | null

// sal -> add sal1

// sal1 -> << add sal1 | >> add sal1 | null

// add -> term add1

// add1 -> + term add1 | - term add1 | null

// term -> single term1

// term1 -> * single term1 | / single term1 | % single term1 | null

// single -> - facator | + factor | ++ factor | factor ++ | -- factor | factor -- | ! factor | factor

// factor -> (expr) | id | id(params) | digit

// params ->  param params1

// params1 -> ,param params1 | null

// expr -> left = expr | left /= expr |
// left *= expr | left %= expr | left += expr |
// left -= expr | left <<= expr | left >>= expr |
// left &= expr | left ^= expr | left |= expr |
// lor expr1

bool isAssignment(Token *token)
{
    if (token->type == OP)
    {
        std::string op = token->getStringInfo();

        return op == "=" || op == "/=" || op == "*=" ||
               op == "%=" || op == "+=" || op == "-=" ||
               op == "<<=" || op == ">>=" || op == "&=" ||
               op == "^=" || op == "|=";
    }
    // throw SyntaxError("the type of Token isn't match when solve operator");
    return false;
}

bool isExprFirst(Token *t)
{
    if (t->type == ID || t->type == DIGIT || t->type == REAL || t->type == STRING)
    {
        return true;
    }
    else if (t->type == OP)
    {
        std::string op = t->getStringInfo();
        return op == "+" || op == "++" || op == "-" || op == "--";
    }
    else if (t->type == PUNC)
    {
        std::string punc = t->getStringInfo();
        return punc == "(";
    }
    return false;
}

inline bool isLandFirst(Token *t)
{
    return isExprFirst(t);
}
// expr -> lland expr2;

Expr_Node *Syntax::expr()
{
    Token *exprToken = lex.next();
    if (isExprFirst(exprToken))
    {
        lex.last();
        Expr_Node *llandNode = lland();
        try
        {
            Expr_Node *expr2Node = expr2(llandNode);
            return (expr2Node ? expr2Node : llandNode);
        }
        catch (...)
        {
            delete llandNode;
            throw;
        }
    }
    lex.last();
    throw SyntaxError("Unvalid expr syntax.");
    return nullptr;
}

// lland -> land expr1
Expr_Node *Syntax::lland()
{
    Token *token = lex.next();
    if (isLandFirst(token))
    {
        lex.last();
        Expr_Node *landNode = land();
        try
        {
            Expr_Node *exprNode = expr1(landNode);
            return (exprNode ? exprNode : landNode);
        }
        catch (...)
        {
            delete landNode;
            throw;
        };
    }
    lex.last();
    throw SyntaxError("Unvalid expr syntax.");
    return nullptr;
}

// expr2 -> asssigmentop lland expr2 | null
Expr_Node *Syntax::expr2(Expr_Node *left)
{
    Token *opToken = lex.next();
    if (isAssignment(opToken))
    {
        Assign_Node *assignNode = new Assign_Node;
        assignNode->assignOp = opToken->getStringInfo();
        assignNode->id = left;
        try
        {
            assignNode->expr = lland();
            Expr_Node *rls = expr2(assignNode);
            return (rls ? rls : assignNode);
        }
        catch (...)
        {
            delete assignNode;
            throw;
        }
    }
    lex.last();
    return nullptr;
}

// expr1 -> || land expr1 | null
Expr_Node *Syntax::expr1(Expr_Node *left)
{
    Token *token = lex.next();

    if (token->type == OP && token->getStringInfo() == "||")
    {
        BinaryOp_Node *binaryOpNode = new BinaryOp_Node();
        binaryOpNode->op = "||";
        binaryOpNode->left = left;
        try
        {
            binaryOpNode->right = land();
            Expr_Node *rls = expr1(binaryOpNode);
            return (rls ? rls : binaryOpNode);
        }
        catch (...)
        {
            delete binaryOpNode;
            throw;
        }
    }
    else
    {
        lex.last();
        return nullptr;
    }
}

inline bool isOrFirst(Token *t)
{
    return isExprFirst(t);
}

// land -> or land1
Expr_Node *Syntax::land()
{
    Token *token = lex.next();
    if (isOrFirst(token))
    {
        lex.last();
        Expr_Node *orNode = Or();
        try
        {
            Expr_Node *landNode = land1(orNode);
            return (landNode ? landNode : orNode);
        }
        catch (...)
        {
            delete orNode;
            throw;
        }
    }
    lex.last();
    throw SyntaxError("Unvalid expr syntax.");
    return nullptr;
}

// land1 -> && or land1 | null
Expr_Node *Syntax::land1(Expr_Node *left)
{
    Token *token = lex.next();
    if (token->type == OP && token->getStringInfo() == "&&")
    {
        BinaryOp_Node *binaryOpNode = new BinaryOp_Node();
        binaryOpNode->op = "&&";
        binaryOpNode->left = left;
        try
        {
            binaryOpNode->right = Or();
            Expr_Node *rls = land1(binaryOpNode);
            return (rls ? rls : binaryOpNode);
        }
        catch (...)
        {
            delete binaryOpNode;
            throw;
        }
    }
    lex.last();
    return nullptr;
}

inline bool isXorFirst(Token *t)
{
    return isExprFirst(t);
}

// or -> xor or1
Expr_Node *Syntax::Or()
{
    Token *token = lex.next();
    if (isXorFirst(token))
    {
        lex.last();
        Expr_Node *xorNode = Xor();
        try
        {
            Expr_Node *orNode = Or1(xorNode);
            return (orNode ? orNode : xorNode);
        }
        catch (...)
        {
            delete xorNode;
            throw;
        }
    }
    lex.last();
    throw SyntaxError("Unvalid expr syntax.");
    return nullptr;
}

// or1 -> | xor or1 | null
Expr_Node *Syntax::Or1(Expr_Node *left)
{
    Token *token = lex.next();
    if (token->type == OP && token->getStringInfo() == "|")
    {
        BinaryOp_Node *binaryOpNode = new BinaryOp_Node();
        binaryOpNode->op = "|";
        binaryOpNode->left = left;
        try
        {
            binaryOpNode->right = Xor();
            Expr_Node *rls = Or1(binaryOpNode);
            return (rls ? rls : binaryOpNode);
        }
        catch (...)
        {
            delete binaryOpNode;
            throw;
        }
    }
    else
    {
        lex.last();
        return nullptr;
    }
}

inline bool isAndFirst(Token *t)
{
    return isExprFirst(t);
}

// xor -> and xor1
Expr_Node *Syntax::Xor()
{
    Token *token = lex.next();
    if (isAndFirst(token))
    {
        lex.last();
        Expr_Node *andNode = And();
        try
        {
            Expr_Node *xorNode = Xor1(andNode);
            return (xorNode ? xorNode : andNode);
        }
        catch (...)
        {
            delete andNode;
            throw;
        }
    }
    lex.last();
    throw SyntaxError("Unvalid expr syntax.");
    return nullptr;
}

// xor1 -> ^ and xor1 | null
Expr_Node *Syntax::Xor1(Expr_Node *left)
{
    Token *token = lex.next();
    if (token->type == OP && token->getStringInfo() == "^")
    {
        BinaryOp_Node *binaryOpNode = new BinaryOp_Node();
        binaryOpNode->op = "^";
        binaryOpNode->left = left;
        try
        {
            binaryOpNode->right = And();
            Expr_Node *rls = Xor1(binaryOpNode);
            return (rls ? rls : binaryOpNode);
        }
        catch (...)
        {
            delete binaryOpNode;
            throw;
        }
    }
    else
    {
        lex.last();
        return nullptr;
    }
}

inline bool isEqualFirst(Token *t)
{
    return isExprFirst(t);
}

// and -> equal and1
Expr_Node *Syntax::And()
{
    Token *token = lex.next();
    if (isEqualFirst(token))
    {
        lex.last();
        Expr_Node *equalNode = equal();
        try
        {
            Expr_Node *andNode = And1(equalNode);
            return (andNode ? andNode : equalNode);
        }
        catch (...)
        {
            delete equalNode;
            throw;
        }
    }
    lex.last();
    throw SyntaxError("Unvalid expr syntax.");
    return nullptr;
}

// and1 -> & equal and1 | null
Expr_Node *Syntax::And1(Expr_Node *left)
{
    Token *token = lex.next();
    if (token->type == OP && token->getStringInfo() == "&")
    {
        BinaryOp_Node *binaryOpNode = new BinaryOp_Node();
        binaryOpNode->op = "&";
        binaryOpNode->left = left;
        try
        {
            binaryOpNode->right = equal();
            Expr_Node *rls = And1(binaryOpNode);
            return (rls ? rls : binaryOpNode);
        }
        catch (...)
        {
            delete binaryOpNode;
            throw;
        }
    }
    else
    {
        lex.last();
        return nullptr;
    }
}

inline bool isCmpFirst(Token *t)
{
    return isExprFirst(t);
}

// eqaul -> cmp equal1
Expr_Node *Syntax::equal()
{
    Token *token = lex.next();
    if (isCmpFirst(token))
    {
        lex.last();
        Expr_Node *cmpNode = cmp();
        try
        {
            Expr_Node *equalNode = equal1(cmpNode);
            return (equalNode ? equalNode : cmpNode);
        }
        catch (...)
        {
            delete cmpNode;
            throw;
        }
    }
    lex.last();
    throw SyntaxError("Unvalid expr syntax.");
    return nullptr;
}

// equal1 -> == cmp equal1 | != cmp equal1 | null
Expr_Node *Syntax::equal1(Expr_Node *left)
{
    Token *token = lex.next();
    if (token->type == OP)
    {
        std::string op = token->getStringInfo();
        if (op == "==" || op == "!=")
        {
            BinaryOp_Node *binaryOpNode = new BinaryOp_Node();
            binaryOpNode->op = op;
            binaryOpNode->left = left;
            try
            {
                binaryOpNode->right = cmp();
                Expr_Node *rls = equal1(binaryOpNode);
                return (rls ? rls : binaryOpNode);
            }
            catch (...)
            {
                delete binaryOpNode;
                throw;
            }
        }
        else
        {
            lex.last();
            return nullptr;
        }
    }
    else
    {
        lex.last();
        return nullptr;
    }
}

inline bool isSalFirst(Token *t)
{
    return isExprFirst(t);
}

// cmp -> sal cmp1
Expr_Node *Syntax::cmp()
{
    Token *token = lex.next();
    if (isSalFirst(token))
    {
        lex.last();
        Expr_Node *salNode = sal();
        try
        {
            Expr_Node *cmpNode = cmp1(salNode);
            return (cmpNode ? cmpNode : salNode);
        }
        catch (...)
        {
            delete salNode;
            throw;
        }
    }
    lex.last();
    throw SyntaxError("Unvalid expr syntax.");
    return nullptr;
}

// cmp1 -> > sal cmp1 | >= sal cmp1 | < sal cmp1 | <= sal cmp1 | null
Expr_Node *Syntax::cmp1(Expr_Node *left)
{
    Token *token = lex.next();
    if (token->type == OP)
    {
        std::string op = token->getStringInfo();
        if (op == ">" || op == ">=" || op == "<=" || op == "<")
        {
            BinaryOp_Node *binaryOpNode = new BinaryOp_Node();
            binaryOpNode->op = op;
            binaryOpNode->left = left;
            try
            {
                binaryOpNode->right = sal();
                Expr_Node *rls = cmp1(binaryOpNode);
                return (rls ? rls : binaryOpNode);
            }
            catch (...)
            {
                delete binaryOpNode;
                throw;
            }
        }
        else
        {
            lex.last();
            return nullptr;
        }
    }
    else
    {
        lex.last();
        return nullptr;
    }
}

inline bool isAddFirst(Token *t)
{
    return isExprFirst(t);
}

// sal -> add sal1
Expr_Node *Syntax::sal()
{
    Token *token = lex.next();
    if (isAddFirst(token))
    {
        lex.last();
        Expr_Node *addNode = add();
        try
        {
            Expr_Node *salNode = sal1(addNode);
            return (salNode ? salNode : addNode);
        }
        catch (...)
        {
            delete addNode;
            throw;
        }
    }
    lex.last();
    throw SyntaxError("Unvalid expr syntax.");
    return nullptr;
}

// sal1 -> << add sal1 | >> add sal1 | null
Expr_Node *Syntax::sal1(Expr_Node *left)
{
    Token *token = lex.next();
    if (token->type == OP)
    {
        std::string op = token->getStringInfo();
        if (op == "<<" || op == ">>")
        {
            BinaryOp_Node *binaryOpNode = new BinaryOp_Node();
            binaryOpNode->op = op;
            binaryOpNode->left = left;
            try
            {
                binaryOpNode->right = add();
                Expr_Node *rls = sal1(binaryOpNode);
                return (rls ? rls : binaryOpNode);
            }
            catch (...)
            {
                delete binaryOpNode;
                throw;
            }
        }
        else
        {
            lex.last();
            return nullptr;
        }
    }
    else
    {
        lex.last();
        return nullptr;
    }
}

inline bool isTermFirst(Token *t)
{
    return isExprFirst(t);
}

// add -> term add1
Expr_Node *Syntax::add()
{
    Token *token = lex.next();
    if (isTermFirst(token))
    {
        lex.last();
        Expr_Node *termNode = term();
        try
        {
            Expr_Node *addNode = add1(termNode);
            return (addNode ? addNode : termNode);
        }
        catch (...)
        {
            delete termNode;
            throw;
        }
    }
    lex.last();
    throw SyntaxError("Unvalid expr syntax.");
    return nullptr;
}

// add1 -> + term add1 | - term add1 | null
Expr_Node *Syntax::add1(Expr_Node *left)
{
    Token *token = lex.next();
    if (token->type == OP)
    {
        std::string op = token->getStringInfo();
        if (op == "+" || op == "-")
        {
            BinaryOp_Node *binaryOpNode = new BinaryOp_Node();
            binaryOpNode->op = op;
            binaryOpNode->left = left;
            try
            {
                binaryOpNode->right = term();
                Expr_Node *rls = add1(binaryOpNode);
                return (rls ? rls : binaryOpNode);
            }
            catch (...)
            {
                delete binaryOpNode;
                throw;
            }
        }
        else
        {
            lex.last();
            return nullptr;
        }
    }
    else
    {
        lex.last();
        return nullptr;
    }
}

inline bool isSingleFirst(Token *t)
{
    return isExprFirst(t);
}

// term -> single term1
Expr_Node *Syntax::term()
{
    Token *token = lex.next();
    if (isSingleFirst(token))
    {
        lex.last();
        Expr_Node *singleNode = single();
        try
        {
            Expr_Node *termNode = term1(singleNode);
            return (termNode ? termNode : singleNode);
        }
        catch (...)
        {
            delete singleNode;
            throw;
        }
    }
    lex.last();
    throw SyntaxError("Unvalid expr syntax.");
    return nullptr;
}

// term1 -> * single term1 | / single term1 | % single term1 | null
Expr_Node *Syntax::term1(Expr_Node *left)
{
    Token *token = lex.next();
    if (token->type == OP)
    {
        std::string op = token->getStringInfo();
        if (op == "*" || op == "/" || op == "%")
        {
            BinaryOp_Node *binaryOpNode = new BinaryOp_Node();
            binaryOpNode->op = op;
            binaryOpNode->left = left;
            try
            {
                binaryOpNode->right = single();
                Expr_Node *rls = term1(binaryOpNode);
                return (rls ? rls : binaryOpNode);
            }
            catch (...)
            {
                delete binaryOpNode;
                throw;
            }
        }
        else
        {
            lex.last();
            return nullptr;
        }
    }
    else
    {
        lex.last();
        return nullptr;
    }
}

bool isFactorFirst(Token *t)
{
    if (t->type == ID || t->type == DIGIT || t->type == REAL || t->type == STRING)
    {
        return true;
    }
    else if (t->type == PUNC)
    {
        return t->getStringInfo() == "(";
    }
    return false;
}

// single -> - facator | + factor | ++ factor | factor ++ | -- factor | factor -- | ! factor | factor
Expr_Node *Syntax::single()
{
    Token *token = lex.next();
    if (token->type == OP)
    {
        std::string op = token->getStringInfo();
        if (op == "-" || op == "+" || op == "++" || op == "--" || op == "!")
        {
            Expr_Node *expr = factor();
            SingleOp_Node *singleOpNode = new SingleOp_Node();
            singleOpNode->op = op;
            singleOpNode->isPosfix = false;
            singleOpNode->expr = expr;
            return singleOpNode;
        }
        else
        {
            lex.last();
            throw SyntaxError(op + " can't used to single operator");
        }
    }
    else if (isFactorFirst(token))
    {
        lex.last();
        Expr_Node *expr = factor();
        Token *posfixToken = lex.next();
        if (posfixToken->type == OP)
        {
            if (posfixToken->getStringInfo() == "++")
            {
                SingleOp_Node *singleOpNode = new SingleOp_Node();
                singleOpNode->op = "++";
                singleOpNode->isPosfix = true;
                singleOpNode->expr = expr;
                return singleOpNode;
            }
            else if (posfixToken->getStringInfo() == "--")
            {
                SingleOp_Node *singleOpNode = new SingleOp_Node();
                singleOpNode->op = "--";
                singleOpNode->isPosfix = true;
                singleOpNode->expr = expr;
                return singleOpNode;
            }
        }
        lex.last();
        return expr;
    }
    return nullptr;
}

// factor -> (expr) | id | id(params) | digit |real | string
Expr_Node *Syntax::factor()
{
    Token *token1 = lex.next();
    if (token1->type == PUNC && token1->getStringInfo() == "(")
    {
        Expr_Node *rls = expr();
        Token *token2 = lex.next();
        if (!(token2->type == PUNC && token2->getStringInfo() == ")"))
        {
            lex.last();
            throw SyntaxError("In a expr, brack can't match");
        }
        return rls;
    }
    else if (token1->type == DIGIT)
    {
        Object num(token1->getLongLongInfo());
        Const_Node *rls = new Const_Node();
        rls->val = num;
        return rls;
    }
    else if (token1->type == REAL)
    {
        Object num(token1->getRealInfo());
        Const_Node *rls = new Const_Node();
        rls->val = num;
        return rls;
    }
    else if (token1->type == STRING)
    {
        Object str(token1->getStringInfo());
        Const_Node *rls = new Const_Node();
        rls->val = str;
        return rls;
    }
    else if (token1->type == ID)
    {
        Token *token2 = lex.next();
        if (token2->type == PUNC && token2->getStringInfo() == "(")
        {
            Expr_Node *exprNode = params(token1->getStringInfo());
            Token *token3 = lex.next();
            if (!(token3->type == PUNC && token3->getStringInfo() == ")"))
            {
                lex.last();
                delete exprNode;
                throw SyntaxError("In a expr, brack can't match");
            }
            return exprNode;
        }
        else if (token2->type == PUNC && token2->getStringInfo() == ".")
        {
            Token *memberNameToken = lex.next();
            if (!(memberNameToken->type == ID))
            {
                lex.last();
                throw SyntaxError("In a expr, Expect Id");
            }

            StructObject_Node *structObjectNode = new StructObject_Node;
            structObjectNode->memberName = std::move(memberNameToken->getStringInfo());
            structObjectNode->structName = token1->getStringInfo();
            return structObjectNode;
        }
        else
        {
            lex.last();
            std::string idName = token1->getStringInfo();
            Id_Node *rls = new Id_Node();
            rls->idName = idName;
            // rls->env = nowEnv;
            return rls;
        }
    }
    else
    {
        throw SyntaxError("Factor is wrong");
    }
    return nullptr;
    // updatating...
}

// params ->  param params1 | null
// params1 -> ,param params1 | null
Expr_Node *Syntax::params(const std::string &idName)
{
    Token *token1 = lex.next();
    Call_Node *callNode = new Call_Node();
    callNode->functionName = idName;
    if (isExprFirst(token1))
    {
        lex.last();
        callNode->param.push_back(expr());
        params1(callNode);
    }
    else
    {
        lex.last();
    }
    return callNode;
}

void Syntax::params1(Call_Node *callNode)
{
    while (1)
    {
        Token *t1 = lex.next();
        if (t1->type == PUNC && t1->getStringInfo() == ",")
        {
            callNode->param.push_back(expr());
            // params1(callNode);
        }
        else
        {
            lex.last();
            break;
        }
    }
}
