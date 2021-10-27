#include "ReversePolishSolver.h"
#include <iostream>
#include "Tests.h"

int square(int x, int y)
{
    return static_cast<int>(pow(x, y));
}

int main()
{
    Tests tests;
    tests.run();

    ReversePolishSolver solver;
    solver.RegisterOperator<2>("+", std::plus<>());
    solver.RegisterOperator<1>("double", [](int x) { return x * 2; });

    std::cout << solver.SolveWithExceptions("4 double 3 4 + -") << '\n'; // 3

    std::cout << solver.SolveWithExceptions("2 double 3 4 + +") << '\n'; // 11
    solver.RegisterOperator<2>("-", std::minus<>());

    std::cout << solver.SolveWithExceptions("4 double 3 4 + -") << '\n'; // 3

    solver.RegisterOperator<2>("*", [](int x, int y) {return x * y; });
    std::cout << solver.SolveWithExceptions("2 3 + 4 * 1 -") << '\n'; // 19


    solver.RegisterOperator<1>("~", [](int x) {return -x; });
    std::cout << solver.SolveWithExceptions("2 ~ 3 + 4 * 1 double -") << '\n'; // 2

    solver.RegisterOperator<1>("**", square);
    std::cout << solver.SolveWithExceptions("2 2 **") << '\n'; // 4

    //solver.RegisterOperator<1>("^", std::bind(square, std::placeholders::_1, 2));

    //std::cout << solver.SolveWithExceptions("2 ^") << '\n'; // 4


    return 0;
}
