
#ifndef TOKEN_H
#define TOKEN_H

#include "config.h"
#include <iostream>
#include <string>

enum TokenType
{
    ID,
    DIGIT,
    REAL,
    WINT,
    OP,
    PUNC,
    KEY,
    NONE,
    NOTE,
    END,
    STRING
};

struct Token
{
    TokenType type;
    Token(TokenType t) : type(t) {}
#ifdef DEBUG
    virtual std::ostream &display(std::ostream &os) const
    {
        if (type == NONE)
            os << "<NONE: none>";
        else if (type == END)
            os << "<END: EOF>";
        return os;
    }

    friend std::ostream &operator<<(std::ostream &os, const Token *t)
    {
        t->display(os);
        return os;
    }
#endif

    virtual double getRealInfo() const
    {
        return 0.0;
    }

    virtual int getIntInfo() const
    {
        return 0;
    }

    virtual long long getLongLongInfo() const
    {
        return 0;
    }

    virtual char gerCharInfo() const
    {
        return 0;
    }

    virtual std::string getStringInfo() const
    {
        return "";
    }
};

struct Id : public Token
{
    std::string id;
    Id(const std::string &id) : Token(ID), id(id) {}

    std::string getStringInfo() const override
    {
        return id;
    }

#ifdef DEBUG
    std::ostream &display(std::ostream &os) const override
    {
        os << "<ID: " << id << ">";
        return os;
    }

#endif
};

struct Digit : public Token
{
    long long digit;
    Digit(long long d) : Token(DIGIT), digit(d) {}

    long long getLongLongInfo() const override
    {
        return digit;
    }

    int getIntInfo() const override
    {
        return digit;
    }

#ifdef DEBUG
    std::ostream &display(std::ostream &os) const override
    {
        os << "<DIGIT: " << digit << ">";
        return os;
    }

#endif
};

struct Real : public Token
{
    double real;
    Real(double real) : Token(REAL), real(real) {}

    double getRealInfo() const override
    {
        return real;
    }

#ifdef DEBUG
    std::ostream &display(std::ostream &os) const override
    {
        os << "<REAL: " << real << ">";
        return os;
    }

#endif
};

struct Op : public Token
{
    std::string op;
    Op(const std::string &op) : Token(OP), op(op) {}

    std::string getStringInfo() const override
    {
        return op;
    }

#ifdef DEBUG
    std::ostream &display(std::ostream &os) const override
    {
        os << "<OP: " << op << ">";
        return os;
    }

#endif
};

struct Punc : public Token
{
    std::string punc;
    Punc(const std::string &punc) : Token(PUNC), punc(punc) {}

    std::string getStringInfo() const override
    {
        return punc;
    }

#ifdef DEBUG
    std::ostream &display(std::ostream &os) const override
    {
        os << "<PUNC: " << punc << ">";
        return os;
    }

#endif
};

struct Key : public Token
{
    std::string key;
    Key(const std::string &key) : Token(KEY), key(key) {}
    std::string getStringInfo() const override
    {
        return key;
    }

#ifdef DEBUG
    std::ostream &display(std::ostream &os) const override
    {
        os << "<KEY: " << key << ">";
        return os;
    }

#endif
};

struct String : public Token
{
    std::string str;
    String(const std::string &str) : Token(STRING), str(str) {}
    std::string getStringInfo() const override
    {
        return str;
    }

#ifdef DEBUG
    std::ostream &display(std::ostream &os) const override
    {
        os << "<STRING: " << str << ">";
        return os;
    }

#endif
};
#endif