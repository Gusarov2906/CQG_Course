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
    void swapTwoToAnswer(std::vector<std::string>& lexemes,
        int& index, const std::string& ans);
    void swapOneToAnswer(std::vector<std::string>& lexemes,
        int& index, const std::string& ans);
    int count_digits(const std::string& s);
        
    int num1 = 0;
    int num2 = 0;
};

template<size_t argsCount, class T>
inline void ReversePolishSolver::RegisterOperator(const std::string& name, T fn)
{
    using namespace std;
    if constexpr (argsCount == 1)
    {
        m_Funcs.push_back(pair<string, variant<function<int(int)>,
            function<int(int, int)>>>
            (name, static_cast<function<int(int)>>(fn)));
    }
    else if constexpr (argsCount == 2)
    {
        m_Funcs.push_back(pair<string, variant<function<int(int)>,
            function<int(int, int)>>>
            (name, static_cast<function<int(int, int)>>(fn)));
    }
}
