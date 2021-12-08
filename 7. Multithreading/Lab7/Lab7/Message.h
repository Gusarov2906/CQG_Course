#pragma once
#include <string>
#include <chrono>
#include <mutex>


using namespace  std::chrono;
class Message
{
public:
    Message(std::string name, system_clock::time_point time) : m_name(name),
        m_timeToShow(time), m_quitFlag(false), m_id(m_currentId++){};
    uint32_t calculateRemainingTime();
    std::string getReport();
    std::string getName() const;
    bool getQuitFlag() const;
    void setQuitFlag(bool val);
    uint32_t getId() const;
private:
    uint32_t m_id;
    static std::atomic_uint32_t m_currentId;
    const std::string m_name;
    const system_clock::time_point m_timeToShow;
    bool m_quitFlag;
};

