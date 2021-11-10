#include "Tests.h"
#include "Functions.h"

void Tests::run()
{
    ReversePolishSolver solver;

    assert(solver.SolveWithExceptions("0") == "0");
    assert(solver.SolveWithExceptions("123") == "123");

    solver.RegisterOperator<2>("+", std::plus<>());
    assert(solver.SolveWithExceptions("1 2 +") == "3");
    assert(solver.SolveWithExceptions("99 1 +") == "100");

    solver.RegisterOperator<2>("-", std::minus<>());
    assert(solver.SolveWithExceptions("1 2 -") == "-1");
    assert(solver.SolveWithExceptions("99 1 -") == "98");

    solver.RegisterOperator<2>("*", [](int x, int y) {return x * y; });
    assert(solver.SolveWithExceptions("1 2 *") == "2");
    assert(solver.SolveWithExceptions("99 -2 *") == "-198");

    solver.RegisterOperator<1>("double", [](int x) { return x * 2; });
    assert(solver.SolveWithExceptions("2 double") == "4");
    assert(solver.SolveWithExceptions("-5 double") == "-10");

    solver.RegisterOperator<1>("~", [](int x) {return -x; });
    assert(solver.SolveWithExceptions("2 ~") == "-2");
    assert(solver.SolveWithExceptions("-5 ~") == "5");

    solver.RegisterOperator<2>("**", Functions::square);
    assert(solver.SolveWithExceptions("3 2 **") == "9");
    assert(solver.SolveWithExceptions("-1 3 **") == "-1");

    solver.RegisterOperator<1>("^", std::bind(Functions::square, std::placeholders::_1, 2));
    assert(solver.SolveWithExceptions("3 ^") == "9");
    assert(solver.SolveWithExceptions("-5 ^") == "25");

    solver.RegisterOperator<1>("++", std::bind(std::plus<>(), std::placeholders::_1, 1));
    assert(solver.SolveWithExceptions("3 ++") == "4");
    assert(solver.SolveWithExceptions("-5 ++") == "-4");

    solver.RegisterOperator<1>("--", Decrement());
    assert(solver.SolveWithExceptions("3 --") == "2");
    assert(solver.SolveWithExceptions("-5 --") == "-6");

    assert(solver.SolveWithExceptions("5 6 7 * - ~ 18 - 1 + ^") == "400");
    assert(solver.SolveWithExceptions("2 4 ** 8 - 2 * double") == "32");

    assert(solver.SolveWithExceptions("2147483648 2 +") == "Out of range number argument!");
    assert(solver.SolveWithExceptions("-2147483649 -1 -") == "Out of range number argument!");
    assert(solver.SolveWithExceptions("0 2147483648 *") == "Out of range number argument!");
    assert(solver.SolveWithExceptions("0 -2147483649 **") == "Out of range number argument!");

    assert(solver.SolveWithExceptions("2 +") == "Numbers of argumnets for operation error!");
    assert(solver.SolveWithExceptions("~") == "Numbers of argumnets for operation error!");
    assert(solver.SolveWithExceptions("-") == "Numbers of argumnets for operation error!");

    assert(solver.SolveWithExceptions("54 4 /") == "Unknown operator!");
    assert(solver.SolveWithExceptions("43 1 &") == "Unknown operator!");
    assert(solver.SolveWithExceptions("2  2 +") == "Unknown operator!");

    assert(solver.SolveWithExceptions("43 1 + 4") == "Bad expression. Not enough operators!");
    assert(solver.SolveWithExceptions("6 -1") == "Bad expression. Not enough operators!");
    assert(solver.SolveWithExceptions("4 ~ 4") == "Bad expression. Not enough operators!");

}
