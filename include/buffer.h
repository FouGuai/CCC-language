#ifndef BUFFER_H
#define BUFFER_H

#include "config.h"
#include <iostream>
#include <fstream>

extern std::ifstream ifs;

class Buffer
{
private:
    char *const buff1, *const buff2;
    char *buff_ptr;
    bool chose_buff1;

    void input();

public:
    Buffer() : buff1(new char[BUFF_SIZE]), buff2(new char[BUFF_SIZE]), buff_ptr(buff1), chose_buff1(true)
    {
        input();
        std::ios::sync_with_stdio(false);
    }
    ~Buffer();
    char get();
    char last();
};

#endif
