#pragma once
#include <cstdint>


template <class T> 
class smart_shared_ptr
{
private:
    T* m_ptr = nullptr;
    uint32_t* m_countRefs = nullptr;

public:
    smart_shared_ptr(); //конструктор по умолчанию
    smart_shared_ptr(T* ptr); //конструктор, в качестве параметра принимающий обычный указатель
    ~smart_shared_ptr();

    smart_shared_ptr(const smart_shared_ptr& obj); //copy-конструктор
    smart_shared_ptr& operator=(const smart_shared_ptr& obj); //copy-оператор присваивания

    smart_shared_ptr(smart_shared_ptr&& obj); //move-конструктор
    smart_shared_ptr& operator=(smart_shared_ptr&& obj); //move-оператор присваивания

    T& operator*() const;
    T* operator->() const;
    T& get() const;

    bool isNull() const;
    uint32_t getCount() const;

    void reset();
};

#include "smart_shared_ptr.h"

template<class T>
smart_shared_ptr<T>::smart_shared_ptr() : m_ptr(nullptr), m_countRefs(new uint32_t(0))
{
    std::cout << "Constructor" << std::endl;
}

template<class T>
smart_shared_ptr<T>::smart_shared_ptr(T* ptr) : m_ptr(ptr), m_countRefs(new uint32_t(1))
{
    std::cout << "Constructor with param" << std::endl;
}

template<class T>
void smart_shared_ptr<T>::reset()
{
    if (nullptr != m_countRefs)
    {
        (*m_countRefs)--;
        if (*m_countRefs == 0)
        {
            if (nullptr != m_ptr)
                delete m_ptr;
            delete m_countRefs;
            m_ptr = nullptr;
            m_countRefs = nullptr;
        }
    }
}
template<class T>
smart_shared_ptr<T>::~smart_shared_ptr()
{
    std::cout << "Destructor" << std::endl;
    reset();
}

template<class T>
smart_shared_ptr<T>::smart_shared_ptr(const smart_shared_ptr& obj)
{
    std::cout << "Copy constructor" << std::endl;

    m_ptr = obj.m_ptr;
    m_countRefs = obj.m_countRefs;
    if (nullptr != obj.m_ptr)
    {
        (*m_countRefs)++;
    }
}

template<class T>
smart_shared_ptr<T>& smart_shared_ptr<T>::operator=(const smart_shared_ptr& obj)
{
    if (obj.m_ptr != this->m_ptr)
    {
        reset();
        std::cout << "Copy operator" << std::endl;
        m_ptr = obj.m_ptr;
        m_countRefs = obj.m_countRefs;
        if (nullptr != obj.m_ptr)
        {
            (*m_countRefs)++;
        }
    }

    return *this;
}

template<class T>
smart_shared_ptr<T>::smart_shared_ptr(smart_shared_ptr&& obj)
{

    std::cout << "Move constructor" << std::endl;

    m_ptr = obj.m_ptr;
    m_countRefs = obj.m_countRefs;

    obj.m_ptr = nullptr;
    obj.m_countRefs = nullptr;
}

template<class T>
smart_shared_ptr<T>& smart_shared_ptr<T>::operator=(smart_shared_ptr&& obj)
{
    reset();

    std::cout << "Move operator" << std::endl;

    m_ptr = obj.m_ptr;
    m_countRefs = obj.m_countRefs;

    obj.m_ptr = nullptr;
    obj.m_countRefs = nullptr;

    return *this;
}

template<class T>
T& smart_shared_ptr<T>::operator*() const
{
    return *m_ptr;
}

template<class T>
T* smart_shared_ptr<T>::operator->() const
{
    return m_ptr;
}

template<class T>
T& smart_shared_ptr<T>::get() const
{
    return m_ptr;
}

template<class T>
bool smart_shared_ptr<T>::isNull() const
{
    return m_ptr == nullptr;
}

template<class T>
uint32_t smart_shared_ptr<T>::getCount() const
{
    if (nullptr != m_countRefs)
    {
        return *m_countRefs;
    }
    else
    {
        return 0;
    }
}

template<class T> smart_shared_ptr<T> makeShared(T* ptr)
{
    return smart_shared_ptr<T>(ptr);
}
