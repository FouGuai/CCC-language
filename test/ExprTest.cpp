#include "Syntax.h"
// TypeEnvironment typeEnv = TypeEnvironment();
// std::unordered_map<std::string, OperatorType> OP_MAP = {
//     {"+", OperatorType::Addition},           // +
//     {"-", OperatorType::Subtraction},        // -
//     {"*", OperatorType::Multiplication},     // *
//     {"/", OperatorType::Division},           // /
//     {"%", OperatorType::Modulus},            // %
//     {"++", OperatorType::Increment},         // ++
//     {"--", OperatorType::Decrement},         // --
//     {"=", OperatorType::Assignment},         // =
//     {"==", OperatorType::Equal},             // ==
//     {"!=", OperatorType::NotEqual},          // !=
//     {">", OperatorType::Greater},            // >
//     {"<", OperatorType::Less},               // <
//     {">=", OperatorType::GreaterEqual},      // >=
//     {"<=", OperatorType::LessEqual},         // <=
//     {"&&", OperatorType::And},               // &&
//     {"||", OperatorType::Or},                // ||
//     {"!", OperatorType::Not},                // !
//     {"+=", OperatorType::AssAddition},       // +=
//     {"-=", OperatorType::AssSubstraction},   // -=
//     {"*=", OperatorType::AssMultiplication}, // *=
//     {"/=", OperatorType::AssDivision},       // /=
//     {"%=", OperatorType::AssModulus},        // %=
//     {"|", OperatorType::BitOr},              // |
//     {"&", OperatorType::BitAnd},             // &
//     {"^", OperatorType::BitXor},             // ^
//     {"<<", OperatorType::Bitleft},           // <<
//     {">>", OperatorType::BitRight},          // >>
//     {"~", OperatorType::BitNot},             // ~
//     {"|=", OperatorType::AssBitOr},          // |=
//     {"&=", OperatorType::AssBitAnd},         // &=
//     {"^=", OperatorType::AssBitXor},         // ^=
//     {"<<=", OperatorType::AssBitleft},       // <<=
//     {">>=", OperatorType::AssBitRight},      // >>=
//     {"~=", OperatorType::AssNot}             // ~=
// };
int main()
{
    try
    {
        Syntax syntax;
        syntax.analyse();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what();
    }
}