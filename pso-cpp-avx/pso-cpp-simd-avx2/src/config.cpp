#include <immintrin.h>
#include "config.h"

void avx512VectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) {
    size_t vecSize = vA.size();
    size_t remainderElementsAmount = 0;

    for (size_t i = 0; i <= vecSize - 8; i += 8)               //double is 64bit so 512 / 64 is 8
    {
        __m512d nVA = _mm512_loadu_pd(vA.data() + i);       //load vectors to registers
        __m512d nVB = _mm512_loadu_pd(vB.data() + i);
        __m512d nVR = _mm512_add_pd(nVA, nVB);           //add from registers
        
        _mm512_storeu_pd(vResult.data() + i, nVR);       //save result to result vector
        
        remainderElementsAmount += 8;
    }

    for (auto i = remainderElementsAmount; i < vecSize; ++i) 
    {
        vResult[i] = vA[i] + vB[i];
    }
}

void avx2VectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) {
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

void scalarVectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) 
{
    size_t vecSize = vA.size();
    for (size_t i = 0; i < vecSize; ++i) 
    {
        vResult[i] = vA[i] + vB[i];
    }
}