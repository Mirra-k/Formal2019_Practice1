#include <iostream>
#include "automat.cpp"

int main()
{
    std::string reg, word;
    std::cin >> reg >> word;
    checkRegular(reg);
    Automat my_automat(reg);
    std::cout << my_automat.FindLongestPreffix(word) << std::endl;
    return 0;
}