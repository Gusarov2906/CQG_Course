#include "TokenCounter.h"

TokenCounter::TokenCounter(const std::string& i_substr)
{
    this->m_count = 0;
    this->m_index = 0;
    this->m_substr = i_substr;
    this->prevX = NULL;
}

void TokenCounter::operator()(char x)
{
    if (x == m_substr[m_index])
    {
        m_index++;
        prevX = x;
    }
    else
    {
        if (prevX != NULL)
        {
            if (prevX != x)
            {
                m_index = 0;
            }
        }
        else
        {
            m_index = 0;
        }
    }

    if (m_index == m_substr.length() && m_substr.length() > 0)
    {
        m_index = 0;
        m_count++;
    }
}

size_t TokenCounter::GetTokenCount() const
{
    return m_count;
}
