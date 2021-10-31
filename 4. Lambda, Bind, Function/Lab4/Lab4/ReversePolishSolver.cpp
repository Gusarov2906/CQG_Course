#include "ReversePolishSolver.h"

std::vector<std::string> ReversePolishSolver::split(std::string strToSplit, const char& delimeter)
{
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter))
    {
        splittedStrings.push_back(item);
    }
    return splittedStrings;
}

int ReversePolishSolver::count_digits(const std::string& s)
{
    return std::count_if(s.begin(), s.end(),
        [](unsigned char c) { return std::isdigit(c); });
}

void ReversePolishSolver::swapOneToAnswer(std::vector<std::string>& lexemes, int& index, const std::string& ans)
{
    lexemes.erase(lexemes.begin() + index);
    lexemes[index - 1] = ans;
    index = 0;
}

void ReversePolishSolver::swapTwoToAnswer(std::vector<std::string>& lexemes, int& index, const std::string& ans)
{
    lexemes.erase(lexemes.begin() + index);
    lexemes.erase(lexemes.begin() + (index - 1));
    lexemes[index - 2] = ans;
    index = 0;
}

int ReversePolishSolver::Solve(const std::string& expression)
{
    std::vector<std::string> lexemes = split(expression, ' ');
    int res = 0;

    if (lexemes.size() == 1)
    {
        if ((count_digits(lexemes[0]) != 0))
        {
            return stoi(lexemes[0]);
        }
    }

    bool twoReady = false;
    for (int i = 0; i < lexemes.size(); i++)
    {
        if ((count_digits(lexemes[i]) == 0))
        {
            std::string operation = lexemes[i];
            twoReady = false;
            bool isRealOperator = false;
            std::for_each(m_Funcs.begin(), m_Funcs.end(), [&res, &lexemes, &i, &isRealOperator, this, operation](std::pair <std::string, std::variant<std::function<int(int)>, std::function<int(int, int)>>>& pair)
                {
                    if (pair.first == operation)
                    {
                        try
                        {
                            if (i >= 1)
                            {
                                num1 = stoi(lexemes[i - 1]);
                                std::get<std::function<int(int)>>(pair.second)(num1);
                                res = std::get<std::function<int(int)>>(pair.second)(num1);
                                swapOneToAnswer(lexemes, i, std::to_string(res));
                                isRealOperator = true;
                            }
                            else
                            {
                                throw std::string("Numbers of argumnets for operation error!");
                            }
                        }
                        catch (...)
                        {
                            try
                            {
                                if (i >= 2)
                                {
                                    num1 = stoi(lexemes[i - 2]);
                                    num2 = stoi(lexemes[i - 1]);
                                    std::get<std::function<int(int, int)>>(pair.second)(num1, num2);
                                    res = std::get<std::function<int(int, int)>>(pair.second)(num1, num2);
                                    swapTwoToAnswer(lexemes, i, std::to_string(res));
                                    isRealOperator = true;
                                }
                                else
                                {
                                    throw std::string("Numbers of argumnets for operation error!");
                                }
                            }
                            catch (const std::out_of_range& e) 
                            {
                                throw std::string("Out of range number argument!");
                            }
                            catch (const std::invalid_argument& e)
                            {
                                throw std::string("Not number argument!");
                            }
                            catch (...)
                            {
                                throw std::string("Numbers of argumnets for operation error!");
                            }
                        }
                    }
                });
            if (!isRealOperator)
            {
                throw std::string("Unknown operator!");
            }
        }
    }
    if (lexemes.size() > 1)
    {
        throw std::string("Bad expression. Not enough operators!");
    }
    return res;
}

std::string ReversePolishSolver::SolveWithExceptions(const std::string& expression)
{
    try
    {
        return std::to_string(Solve(expression));
    }
    catch (std::string err)
    {
        return err;
    }
}

