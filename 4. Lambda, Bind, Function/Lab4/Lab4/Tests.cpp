#include "Tests.h"

void Tests::run()
{
    ReversePolishSolver solver;
    solver.RegisterOperator<2>("+", std::plus<>());

    assert(solver.SolveWithExceptions("1 2 +") == "3");

    assert(solver.SolveWithExceptions("2147483648 2 +") == "Out of range number argument!");
    assert(solver.SolveWithExceptions("-2147483649 -1 +") == "Out of range number argument!");
    assert(solver.SolveWithExceptions("0 2147483648 +") == "Out of range number argument!");
    assert(solver.SolveWithExceptions("0 -2147483649 +") == "Out of range number argument!");


    solver.RegisterOperator<1>("double", [](int x) { return x * 2; });
    assert(solver.SolveWithExceptions("1 double") == "2");
    assert(solver.SolveWithExceptions("1 2 + double") == "6");
    assert(solver.SolveWithExceptions("2 -1 double +") == "0");


    //std::cout << solver.SolveWithExceptions("4 double 3 4 + -") << '\n'; // 3

    //std::cout << solver.SolveWithExceptions("2 double 3 4 + +") << '\n'; // 11
    //solver.RegisterOperator<2>("-", std::minus<>());

    //std::cout << solver.SolveWithExceptions("4 double 3 4 + -") << '\n'; // 3

    //solver.RegisterOperator<2>("*", [](int x, int y) {return x * y; });
    //std::cout << solver.SolveWithExceptions("2 3 + 4 * 1 -") << '\n'; // 19


    //solver.RegisterOperator<1>("~", [](int x) {return -x; });
    //std::cout << solver.SolveWithExceptions("2 ~ 3 + 4 * 1 double -") << '\n'; // 2
}
