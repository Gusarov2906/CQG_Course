#include "ReversePolishSolver.h"
#include <iostream>
#include "Tests.h"
#include "Functions.h"

int main()
{
    Tests tests;
    tests.run();

    ReversePolishSolver solver;
    solver.RegisterOperator<2>("+", std::plus<>());
    solver.RegisterOperator<1>("double", [](int x) { return x * 2; });
    solver.RegisterOperator<2>("-", std::minus<>());
    solver.RegisterOperator<2>("*", [](int x, int y) {return x * y; });
    solver.RegisterOperator<2>("**", Functions::square);
    solver.RegisterOperator<1>("~", [](int x) {return -x; });
    solver.RegisterOperator<1>("^", std::bind(Functions::square, std::placeholders::_1, 2));

    std::cout << solver.SolveWithExceptions("2 double 3 4 + +") << std::endl; // 11
    std::cout << solver.SolveWithExceptions("4 double 3 4 + -") << std::endl; // 3
    std::cout << solver.SolveWithExceptions("2 3 + 4 * 1 -") << std::endl; // 19
    std::cout << solver.SolveWithExceptions("2 ~ 3 + 4 * 1 double -") << std::endl; // 2
    std::cout << solver.SolveWithExceptions("2 2 **") << std::endl; // 4
    std::cout << solver.SolveWithExceptions("2 ^") << std::endl; // 4
    std::cout << solver.SolveWithExceptions("123") << std::endl; // 123

    return 0;
}
