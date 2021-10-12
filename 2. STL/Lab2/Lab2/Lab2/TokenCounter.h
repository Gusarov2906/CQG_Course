#pragma once
#include <string>

class TokenCounter
{
public:
    TokenCounter(const std::string& i_substr);
    void operator() (char x);
    size_t GetTokenCount() const;

private:
    int m_count;
    std::string m_substr;
    int m_index;
    char prevX;
};

