#include <iostream>
#include <algorithm>
#include <cassert>

#include "TokenCounter.h"


int getNumberOfDisjointOccurrences(std::string str, std::string substr)
{
    TokenCounter tokenCounter(substr);
    tokenCounter = std::for_each(str.begin(), str.end(), tokenCounter);
    return tokenCounter.GetTokenCount();
}

void runTest()
{
    assert(getNumberOfDisjointOccurrences("aaa", "a") == 3);
    assert(getNumberOfDisjointOccurrences("ab", "ab") == 1);

    assert(getNumberOfDisjointOccurrences("ab", "aba") == 0);
    assert(getNumberOfDisjointOccurrences("ab", "ba") == 0);

    assert(getNumberOfDisjointOccurrences("ab", "") == 0);
    assert(getNumberOfDisjointOccurrences("", "") == 0);
    assert(getNumberOfDisjointOccurrences("", "ads") == 0);

    assert(getNumberOfDisjointOccurrences("aab", "ab") == 1);
    assert(getNumberOfDisjointOccurrences("abb", "ab") == 1);

    assert(getNumberOfDisjointOccurrences("abab", "ab") == 2);
    assert(getNumberOfDisjointOccurrences("babab", "ab") == 2);
    assert(getNumberOfDisjointOccurrences("ababa", "ab") == 2);

    assert(getNumberOfDisjointOccurrences("1111111", "11") == 3);

    assert(getNumberOfDisjointOccurrences("a a", "a") == 2);
}


int main()
{
    runTest();
    while (1)
    {
        std::string str;
        std::string subtr;
        std::cout << "Write str:";
        std::cin >> str;
        std::cout << "Write searching substr:";
        std::cin >> subtr;
        std::cout << "Count: " << getNumberOfDisjointOccurrences(str, subtr) << std::endl << std::endl;
    }
    return 0;
}