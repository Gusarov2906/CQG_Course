#pragma once
#include <string>
#include <sstream>
#include <vector>
#include <functional>
#include <iostream>
#include <variant>
#include <algorithm>
#include <stack>

class ReversePolishSolver
{
public:
    /// @brief Registers new operator with argsCount operands
    /// @param argsCount - number of operands of given function
    /// @param name - operator name to use in expressions
    /// @param fn - function to call
    template<size_t argsCount, class T>
    void RegisterOperator(const std::string& name, T fn);

    /// @brief Parses and solves given expression using registered earlier operators
    int Solve(const std::string& expression);
    std::string SolveWithExceptions(const std::string& expression);

private:
    std::vector<std::pair<std::string, 
        std::variant<std::function<int(int)>, 
        std::function<int(int, int)>>>> m_Funcs;

    std::vector<std::string> split(std::string strToSplit, const char& delimeter);
    void swapTwoToAnswer(std::vector<std::string>& lexemes, int& index, const std::string& ans);
    void swapOneToAnswer(std::vector<std::string>& lexemes, int& index, const std::string& ans);
    int count_digits(const std::string& s);
        
    int num1 = 0;
    int num2 = 0;
};

template<size_t argsCount, class T>
inline void ReversePolishSolver::RegisterOperator(const std::string& name, T fn)
{
    using namespace std;
    string type = (typeid(T).name());

    if (type == "struct std::plus<void>")
    {
        variant<function<int(int)>, function<int(int, int)>> func = [](int l, int r)
        {
            std::plus<int> plus;
            return plus(l, r);
        };
        m_Funcs.push_back(pair<string, variant<function<int(int)>, function<int(int, int)>>>(name, func));
    }
    else if (type == "struct std::minus<void>")
    {
        variant<function<int(int)>, function<int(int, int)>> func = [](int l, int r)
        {
            std::minus<int> minus;
            return minus(l, r);
        };
        m_Funcs.push_back(pair <string, variant<function<int(int)>, function<int(int, int)>>>(name, func));
    }
    else if (type == "struct std::multiplies<void>")
    {
        variant<function<int(int)>, std::function<int(int, int)>> func = [](int l, int r)
        {
            std::multiplies<int> multiplies;
            return multiplies(l, r);
        };
        m_Funcs.push_back(pair<string, variant<function<int(int)>, function<int(int, int)>>>(name, func));
    }
    else
    {
        std::variant<function<int(int)>, function<int(int, int)>> func = fn;
        m_Funcs.push_back(pair<string, variant<function<int(int)>, function<int(int, int)>>>(name, func));
    }
}
