#ifndef CONFIG_H
#define CONFIG_H

#define BUFF_SIZE 4096

// 运算符列表
#include <unordered_map>
#include <unordered_set>
#include <string>

// typedef unsigned int size_t;

extern const std::unordered_set<std::string> OP_SET;

enum class OperatorType
{
    Addition,          // +
    Subtraction,       // -
    Multiplication,    // *
    Division,          // /
    Modulus,           // %
    Increment,         // ++
    Decrement,         // --
    Assignment,        // =
    Equal,             // ==
    NotEqual,          // !=
    Greater,           // >
    Less,              // <
    GreaterEqual,      // >=
    LessEqual,         // <=
    And,               // &&
    Or,                // ||
    Not,               // !
    AssAddition,       // +=
    AssSubstraction,   // -=
    AssMultiplication, // *=
    AssDivision,       // /=
    AssModulus,        // %=
    BitOr,             // |
    BitAnd,            // &
    BitXor,            // ^
    Bitleft,           // <<
    BitRight,          // >>
    BitNot,            // ~
    AssBitOr,          // |=
    AssBitAnd,         // &=
    AssBitXor,         // ^=
    AssBitleft,        // <<=
    AssBitRight,       // >>=
    AssNot             // ~=

};

extern std::unordered_map<std::string, OperatorType> OP_MAP;

extern const std::unordered_set<std::string> KEY_SET;
#if 0
#define DEBUG
#endif

#endif


