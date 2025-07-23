#include "CCC.h"
// #include "cstring"

int main(int argc, char *argv[])
{
    std::ios::sync_with_stdio(false);
    // typeEnv = TypeEnvironment();
#ifndef DEBUG
    if (argc == 2)
#endif
    {
        std::ios::sync_with_stdio(0);
#ifdef DEBUG
        const char *src_file_name = "../pargram.ccc";
#else
        const char *src_file_name = argv[1];
#endif
        CCC ccc;
        ccc.srcfile = src_file_name;
        ccc.run();
    }
#ifndef DEBUG
    else
    {
        std::cout << "Invalid command" << std::endl;
    }
#endif
    return 0;
}