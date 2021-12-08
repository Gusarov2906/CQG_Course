#include "Message.h"

std::atomic_uint32_t Message::m_currentId;

uint32_t Message::calculateRemainingTime()
{
    auto time = std::chrono::system_clock::now();
    return duration_cast<seconds>(m_timeToShow - time).count();
}

std::string Message::getReport() 
{
    return m_name + " (in " + std::to_string(calculateRemainingTime()) + " seconds)";
}

std::string Message::getName() const
{
    return m_name;
}

bool Message::getQuitFlag() const
{
    return m_quitFlag;
}

void Message::setQuitFlag(bool val)
{
    m_quitFlag = val;
}

uint32_t Message::getId() const
{
    return m_id;
}
