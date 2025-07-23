#include "CCC.h"

std::ifstream ifs;

void CCC::run()
{
    try
    {
        std::ios::sync_with_stdio(false);
        ifs.open(srcfile);
        if (!ifs.is_open())
        {
            std::cerr << "can't open the file: " << srcfile << std::endl;
            return;
        }
        Syntax syntax;
        AST_Node *programNode = syntax.analyse();
        programNode->excute();
        delete programNode;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}