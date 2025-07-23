#include "Lexer.h"

inline std::string toString(char c)
{
    std::string rls;
    rls.push_back(c);
    return rls;
}

void Lexer::input()
{
    lookahead = buffer.get();
}

void Lexer::releaseMem()
{
    while (!tokens.empty())
    {
        delete tokens.back();
        tokens.pop_back();
    }
    index = 0;
}

[[nodiscard]]
Token *Lexer::scan_note_closed()
{
    int state = 0;
    while (1)
    {
        switch (state)
        {
        case 0:
            if (lookahead == '*')
                state = 1;
            input();
            break;
        case 1:
            if (lookahead == '/')
                state = 2;
            else
                state = 0;
            input();
            break;
        case 2:
            return new Token(NOTE);
            break;
        default:
            throw LexException("Unkown state.");
            break;
        }
    }
    return new Token(NOTE);
}

[[nodiscard]]
Token *Lexer::scan_note_opened()
{
    while (lookahead != '\n')
    {
        input();
    }
    ++line;
    return new Token(NOTE);
}

[[nodiscard]]
Token *Lexer::scan_op()
{
    int state = 0;
    Token *rls;
    while (true)
    {
        switch (state)
        {
        case 0:
            switch (lookahead)
            {
            case '+':
                state = 1;
                break;
            case '-':
                state = 2;
                break;
            case '*':
                state = 3;
                break;
            case '/':
                state = 4;
                break;
            case '%':
                state = 5;
                break;
            case '>':
                state = 6;
                break;
            case '<':
                state = 7;
                break;
            case '=':
                state = 8;
                break;
            case '!':
                state = 9;
                break;
            case '&':
                state = 10;
                break;
            case '|':
                state = 11;
                break;
            case '^':
                state = 12;
                break;
            case '~':
                state = 15;
                break;
            default:
                throw LexException("unknown operator");
                break;
            }
            input();
            break;
        case 1: // +
            if (lookahead == '+')
                rls = new Op("++");
            else if (lookahead == '=')
                rls = new Op("+=");
            else
                return new Op("+");
            input();
            return rls;
            break;

        case 2: // -
            if (lookahead == '-')
                rls = new Op("--");
            else if (lookahead == '=')
                rls = new Op("-=");
            else
                return new Op("-");
            input();
            return rls;
            break;
        case 3: // *
            if (lookahead == '=')
                rls = new Op("*=");
            else
                return new Op("*");
            input();
            return rls;
            break;
        case 4: // /
            if (lookahead == '=')
                rls = new Op("/=");
            else if (lookahead == '/')
            {
                input();
                rls = scan_note_opened();
            }
            else if (lookahead == '*')
            {
                input();
                rls = scan_note_closed();
            }
            else
                return new Op("/");
            input();
            return rls;
            break;
        case 5: // %
            if (lookahead == '=')
                rls = new Op("%=");
            else
                return new Op("%");
            input();
            return rls;
            break;
        case 6: // >
            if (lookahead == '>')
            {
                state = 13;
                input();
            }
            else
            {
                if (lookahead == '=')
                    rls = new Op(">=");
                else
                    return new Op(">");
                input();
                return rls;
            }
            break;
        case 7: // <
            if (lookahead == '<')
            {
                state = 14;
                input();
            }
            else
            {
                if (lookahead == '=')
                    rls = new Op("<=");
                else
                    return new Op("<");
                input();
                return rls;
            }
            break;
        case 8: // =
            if (lookahead == '=')
                rls = new Op("==");
            else
                return new Op("=");
            input();
            return rls;
            break;
        case 9: // !
            if (lookahead == '=')
                rls = new Op("!=");
            else
                return new Op("!");
            input();
            return rls;
            break;
        case 10: // &
            if (lookahead == '=')
                rls = new Op("&=");
            else if (lookahead == '&')
                rls = new Op("&&");
            else
                return new Op("&");
            input();
            return rls;
            break;
        case 11: // |
            if (lookahead == '=')
                rls = new Op("|=");
            else if (lookahead == '|')
                rls = new Op("||");
            else
                return new Op("|");
            input();
            return rls;
            break;
        case 12: // ^
            if (lookahead == '=')
                rls = new Op("^=");
            else
                return new Op("^");
            input();
            return rls;
            break;
        case 13: // >>
            if (lookahead == '=')
                rls = new Op(">>=");
            else
                return new Op(">>");
            input();
            return rls;
            break;
        case 14: // <<
            if (lookahead == '=')
                rls = new Op("<<=");
            else
                return new Op("<<");
            input();
            return rls;
            break;
        case 15: // ~
            if (lookahead == '=')
                rls = new Op("~=");
            else
                return new Op("~");
            input();
            return rls;
            break;
        default:
            throw LexException("Unkown state.");
            break;
        }
    }
    return new Token(NONE);
}

bool Lexer::is_16digit(char c)
{
    return isdigit(c) || ('a' <= c && c <= 'f') || ('A' <= c && c <= 'F');
}

bool Lexer::is_8digit(char c)
{
    return '0' <= c && c < '8';
}

bool Lexer::is_2digit(char c)
{
    return c == '0' || c == '1';
}

[[nodiscard]]
Token *Lexer::scan_digit()
{
    int state = 0;
    long long int_val = 0;
    double double_val = 0;
    long long real_digit = 1;
    while (true)
    {
        switch (state)
        {
        case 0:
            if (lookahead == '0')
            {
                state = 1;
                input();
            }
            else if (isdigit(lookahead))
            {
                state = 7;
                int_val = lookahead - '0';
                input();
            }
            else
            {
                throw std::invalid_argument("unknown symbol");
            }
            break;

        case 1:
            if (lookahead == 'x' || lookahead == 'X')
            {
                state = 2;
                input();
            }
            else if (is_8digit(lookahead))
            {
                int_val = lookahead - '0';
                input();
                state = 5;
            }
            else if (lookahead == 'b' || lookahead == 'B')
            {
                state = 4;
                input();
            }
            else if (lookahead == '.')
            {
                state = 8;
                input();
            }
            else
            {
                return new Digit(int_val);
            }
            break;
        case 2:
            if (is_16digit(lookahead))
            {
                if (isdigit(lookahead))
                {
                    int_val += lookahead - '0';
                }
                else
                {
                    lookahead = tolower(lookahead);
                    int_val += lookahead - 'a' + 10;
                }
                state = 9;
                input();
            }
            else
            {
                throw std::invalid_argument("unknown symbol");
            }
            break;
        case 3:
            if (is_8digit(lookahead))
            {
                int_val = lookahead - '0';
                state = 5;
                input();
            }
            else
            {
                return new Digit(int_val);
            }
            break;
        case 4:
            if (is_2digit(lookahead))
            {
                int_val = lookahead - '0';
                state = 6;
                input();
            }
            else
            {
                throw std::invalid_argument("unknown symbol");
            }
            break;
        case 5:
            if (is_8digit(lookahead))
            {
                int_val *= 8;
                int_val += lookahead - '0';
                input();
            }
            else
            {
                return new Digit(int_val);
            }
            break;
        case 6:
            if (is_2digit(lookahead))
            {
                int_val *= 2;
                int_val += lookahead - '0';
                input();
            }
            else
            {
                return new Digit(int_val);
            }
            break;

        case 7:
            if (isdigit(lookahead))
            {
                int_val *= 10;
                int_val += lookahead - '0';
                input();
            }
            else if (lookahead == '.')
            {
                state = 8;
                double_val = int_val;
                int_val = 0;
                input();
            }
            else
            {
                return new Digit(int_val);
            }
            break;
        case 8:
            if (isdigit(lookahead))
            {
                int_val *= 10;
                int_val += lookahead - '0';
                real_digit *= 10;
                input();
                state = 10;
            }
            else
            {
                throw std::invalid_argument("unknown symbol");
            }
            break;
        case 9:
            if (is_16digit(lookahead))
            {
                int_val *= 16;
                if (isdigit(lookahead))
                {
                    int_val += lookahead - '0';
                }
                else
                {
                    lookahead = tolower(lookahead);
                    int_val += lookahead - 'a' + 10;
                }
                input();
            }
            else
            {
                return new Digit(int_val);
            }
            break;
        case 10:
            if (isdigit(lookahead))
            {
                int_val *= 10;
                int_val += lookahead - '0';
                real_digit *= 10;
                input();
            }
            else
            {
                return new Real(double_val + (double)int_val / real_digit);
            }
            break;
        default:
            throw LexException("Unkown state.");
            break;
        }
    }

    return new Token(NONE);
}

bool Lexer::isIdChar(char c)
{
    return isalpha(c) || isdigit(c) || c == '_';
}

[[nodiscard]]
Token *Lexer::scan_id()
{
    std::string id;
    while (isIdChar(lookahead))
    {
        id.push_back(lookahead);
        input();
    }
    if (KEY_SET.find(id) != KEY_SET.end())
        return new Key(id);
    return new Id(id);
}

[[nodiscard]]
Token *Lexer::scan()
{
    // 跳过空格
loop:
    if (lookahead == EOF)
    {
        return new Token(END);
    }

    if (isspace(lookahead))
    {
        while (isspace(lookahead))
        {
            if (lookahead == '\n')
                ++line;
            input();
        }
    }

    if (isDigitFirst(lookahead))
    {
        return scan_digit();
    }

    if (isIdFirst(lookahead))
    {
        return scan_id();
    }

    if (isOpFirst(lookahead))
    {
        Token *rls = scan_op();
        if (rls->type == NOTE)
        {
            delete rls;
            goto loop;
        }
        else
            return rls;
    }

    if (isStringFisrst(lookahead))
    {
        return scan_string();
    }

    if (ispunct(lookahead))
    {
        char cpy = lookahead;
        input();
        return new Punc(toString(cpy));
    }

    return new Token(NONE);
}

[[nodiscard]]
Token *Lexer::scan_string()
{
    int state = 0;
    std::string rls;
    while (true)
    {
        switch (state)
        {
        case 0:
            if (lookahead == '\'')
                state = 1;
            else if (lookahead == '"')
                state = 4;
            else
                throw LexException("unknown symbol");
            input();
            break;
        case 1:
            if (lookahead == '\'')
            {
                input();
                return new String(rls);
            }
            else if (lookahead == '\\')
            {
                input();
                state = 2;
            }
            else
            {
                rls.push_back(lookahead);
                input();
            }
            break;
        case 2:
            state = 3;
            rls.push_back(lookahead);
            input();
            break;
        case 3:
            if (lookahead == '\'')
            {
                input();
                return new String(rls);
            }
            else
            {
                rls.push_back(lookahead);
                input();
                state = 1;
            }
            break;
        case 4:
            if (lookahead == '"')
            {
                input();
                return new String(rls);
            }
            else if (lookahead == '\\')
            {
                input();
                state = 5;
            }
            else
            {
                rls.push_back(lookahead);
                input();
            }
            break;
        case 5:
            state = 6;
            rls.push_back(lookahead);
            input();
            break;
        case 6:
            if (lookahead == '"')
            {
                input();
                return new String(rls);
            }
            else
            {
                state = 4;
                rls.push_back(lookahead);
                input();
            }
            break;

        default:
            throw LexException("Unkown state.");
            break;
        }
    }
    return new Token(NONE);
}

Lexer::~Lexer()
{
    for (int i = 0; i < tokens.size(); ++i)
    {
        delete tokens[i];
    }
}

bool Lexer::isDigitFirst(char c)
{
    return isdigit(c);
}

bool Lexer::isIdFirst(char c)
{
    return isalpha(c) || c == '_';
}

bool Lexer::isOpFirst(char c)
{
    return OP_SET.find(toString(c)) != OP_SET.end();
}

bool Lexer::isStringFisrst(char c)
{
    return c == '"' || c == '\'';
}

void Lexer::inputToken()
{
    while (1)
    {
        Token *token = scan();
#ifdef DEBUG
        std::cout << token << std::endl;
#endif
        tokens.push_back(token);
        if ((token->type == PUNC && token->getStringInfo() == ";") || token->type == END)
        {
            break;
        }
    }
}

/**
 * @brief 得到下一个token
 *
 * @return Token*
 */
Token *Lexer::next()
{
    while (index >= (int)tokens.size() - 1)
    {
        inputToken();
    }

    return tokens[++index];
}

Token *Lexer::now()
{
    while (index >= (int)tokens.size() - 1)
    {
        inputToken();
    }
    return tokens[index];
}

/**
 * @brief 返回到上一个token
 *
 * @return Token*
 */
Token *Lexer::last()
{
    if (index == 0)
        throw std::out_of_range("index must greater than 0");
    return tokens[--index];
}

/**
 * @brief 得到当前位置跳转step步的token
 * step可以为负数
 *
 * @param step
 * @return Token*
 */
Token *Lexer::jump(int step)
{
    while (index + step >= (int)tokens.size() - 1)
    {
        inputToken();
    }
    if (index + step < 0)
        throw std::out_of_range("index must greater than 0");
    index += step;

    return tokens[index];
}

int Lexer::getNowIndex() const
{
    return index;
}