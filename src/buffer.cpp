#include "buffer.h"
#include <iomanip>
void Buffer::input()
{
    ifs >> std::noskipws;
    char *input_ptr, *beg_ptr;
    if (chose_buff1)
    {
        input_ptr = buff2;
        beg_ptr = buff2;
        buff_ptr = buff2;
    }
    else
    {
        input_ptr = buff1;
        beg_ptr = buff1;
        buff_ptr = buff1;
    }

    char c;
    while (1)
    {
        if (ifs >> c)
        {
            *input_ptr = c;
            ++input_ptr;
        }
        else
        {
            *input_ptr = EOF;
            break;
        }

        if (beg_ptr + BUFF_SIZE < input_ptr)
            break;
    }
    chose_buff1 = !chose_buff1;
}

char Buffer::get()
{
    if (chose_buff1)
    {
        if (buff_ptr == buff1 + BUFF_SIZE)
        {
            char rls = *buff_ptr;
            input();
            return rls;
        }
        else
        {
            return *buff_ptr++;
        }
    }
    else
    {
        if (buff_ptr == buff2 + BUFF_SIZE)
        {
            char rls = *buff_ptr;
            input();
            return rls;
        }
        else
        {
            return *buff_ptr++;
        }
    }
}

char Buffer::last()
{
    if (chose_buff1)
    {
        if (buff_ptr == buff1)
        {
            return *(buff2 + BUFF_SIZE);
        }
        else
        {
            return *(buff_ptr - 1);
        }
    }
    else
    {
        if (buff_ptr == buff2)
        {
            return *(buff1 + BUFF_SIZE);
        }
        else
        {
            return *(buff_ptr - 1);
        }
    }
}

Buffer::~Buffer()
{
    delete[] buff1;
    delete[] buff2;
    ifs.close();
}