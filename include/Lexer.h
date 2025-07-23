#ifndef LEXER_H
#define LEXER_H

#include "Token.h"
#include <vector>
#include <unordered_set>
#include "buffer.h" 

class LexException : public std::exception
{
private:
    std::string error_info;

public:
    LexException(const char *info) : error_info(info) {}
    const char *what() const noexcept override
    {
        return error_info.data();
    }
};

class Lexer
{
private:
    // 存储行号
    int line;

    Buffer buffer;

    // 每个词法单元的下一个位置
    char lookahead = ' ';

    std::vector<Token *> tokens;
    int index = 0;

    Token *scan_op();
    Token *scan_id();
    Token *scan_digit();
    Token *scan_note_opened();
    Token *scan_note_closed();
    Token *scan_punc();
    Token *scan_string();

    static bool isIdChar(char c);
    static bool is_16digit(char c);
    static bool is_8digit(char c);
    static bool is_2digit(char c);
    static bool isOpFirst(char c);
    static bool isIdFirst(char c);
    static bool isDigitFirst(char c);
    static bool isStringFisrst(char c);

    void input();
    void inputToken();

public:
    Token *next();
    Token *now();
    Token *last();
    Token *jump(int step);
    Token *scan();
    int getNowIndex() const;
    void releaseMem();

    ~Lexer();
    Lexer()
    {
        tokens.push_back(new Token(NONE));
    };
};

#endif