#include "config.h"
#include <iostream>
#include <vector>
#include <immintrin.h>

int main()
{
    std::vector<double> vA = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    std::vector<double> vB = {8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
    std::vector<double> vResult(vA.size());

    avxVectorAdd(vA, vB, vResult);

    for (const auto val : vResult) 
    {
        std::cout << val << " ";
    }
    std::cout << std::endl; 

    return 0;
}