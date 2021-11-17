#include "Expression.h"

#include <set>
#include <stdexcept>
#include <unordered_map>
#include <algorithm>

namespace
{

static std::unordered_map<std::string, double> s_variables;

static const std::set<OperatorInfo> sc_operators = {
    { "+", 1, 1, 2, [](const std::vector<double>& args) { return args.size() == 2 ? args[0] + args[1] : args[0]; } },
    { "-", 1, 1, 2, [](const std::vector<double>& args) { return args.size() == 2 ? args[0] - args[1] : -args[0]; } },
    { "*", 2, 2, 2, [](const std::vector<double>& args) { return args[0] * args[1]; } },
    { "/", 2, 2, 2, [](const std::vector<double>& args) { return args[0] / args[1]; } },
    { "^", 3, 2, 2, [](const std::vector<double> &args) { return pow(args[0],args[1]); }},
    { "sin", 0, 1, 1, [](const std::vector<double>& args) { return std::sin(args[0]); } },
    { "cos", 0, 1, 1, [](const std::vector<double>& args) { return std::cos(args[0]); } },
    { "min", 0, 2, 2, [](const std::vector<double>& args) { return std::min(args[0], args[1]); } },
    { "max", 0, 2, 2, [](const std::vector<double>& args) { return std::max(args[0], args[1]); } },
    { "average", 0, 1, INT_MAX, [](const std::vector<double> &args) { 
        int count = 0;
        double sum = 0;
        std::for_each(args.begin(), args.end(), [&count, &sum](const double &val) {
            sum += val;
            count++;
        });
        return (double)sum/count;}},
};
}

const OperatorInfo* FindOperator(const std::string& op)
{
    const auto iter = sc_operators.find(op);
    return iter != sc_operators.end() ? &*iter : nullptr;
}

const OperatorInfo* FindOperator(char op)
{
    return FindOperator(std::string(1, op));
}

int GetPriority(char ch)
{
    const auto operatorInfo = FindOperator(ch);
    return operatorInfo ? operatorInfo->priority : 0;
}

double VariableExpressionNode::Compute() const
{
    const auto iter = s_variables.find(m_identifier);
    if (iter == s_variables.end())
    {
        throw std::runtime_error("Uninitialized variable \"" + m_identifier + "\"");
    }

    return iter->second;
}

double OperatorExpressionNode::Compute() const
{
    std::vector<double> values;
    for (const auto& child : m_children)
        values.push_back(child->Compute());
    try
    {
        auto res = m_op(values);
        if (std::isfinite(res))
        {
            return m_op(values);
        }
        else
        {
            throw std::string("Out of range number argument!");
        }
    }
    catch (const std::out_of_range &e)
    {
        throw std::string("Out of range number argument!");
    }
    catch (const std::invalid_argument &e)
    {
        throw std::string("Not number argument!");
    }

    return 0;
}

double AssignmentExpressionNode::Compute() const
{
    const auto value = m_child->Compute();
    s_variables[m_variable] = value;
    return value;
}
