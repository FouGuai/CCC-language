#include "Lexer.h"

int main()
{
    Lexer lax;
    while (1)
    {
        Token *t = lax.now();
        lax.next();
        if (t->type == END)
            break;

        std::cout << t << std::endl;
    }
    return 0;
}