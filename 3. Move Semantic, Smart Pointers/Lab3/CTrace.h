#pragma once
#include <iostream>

class CTrace
{
public:
    CTrace(int i_a) : m_a(i_a)
    {
        std::cout << "ctor" << m_a << std::endl;
    }

    ~CTrace()
    {
        std::cout << "dtor" << m_a << std::endl;
    }

    int m_a;
};

