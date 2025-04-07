#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <immintrin.h>

void addVectors(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult)
{
    if(vA.size() != vB.size() || vA.size() != vResult.size() || vB.size() != vResult.size())
    {
        throw std::runtime_error("Vector size mismatch");
    }

    size_t vecSize = vA.size();
    size_t remainderElementsAmount = 0;

    for (size_t i = 0; i <= vecSize - 4; i += 4)               //double is 64bit so 256 / 64 is 4
    {
        __m256d nVA = _mm256_loadu_pd(vA.data() + i);       //load vectors to registers
        __m256d nVB = _mm256_loadu_pd(vB.data() + i);
        __m256d nVR = _mm256_add_pd(nVA, nVB);           //add from registers
        
        _mm256_storeu_pd(vResult.data() + i, nVR);       //save result to result vector
        
        remainderElementsAmount += 4;
    }

    for (auto i = remainderElementsAmount; i < vecSize; ++i) 
    {
        vResult[i] = vA[i] + vB[i];
    }
}

int main()
{
    std::vector<double> vA = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    std::vector<double> vB = {8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
    std::vector<double> vResult(vA.size());

    addVectors(vA, vB, vResult);

    for (const auto val : vResult) 
    {
        std::cout << val << " ";
    }
    std::cout << std::endl; 

    return 0;
}