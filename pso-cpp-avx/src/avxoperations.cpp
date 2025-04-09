#include <immintrin.h>
#include "avxoperations.h"

std::string getAvxCapabilities() 
{
    std::string result = "CPU supports: ";
    
    #ifdef __AVX512F__
        result += "AVX-512F";
    #elif defined(__AVX2__)
        result += "AVX2";
    #elif defined(__AVX__)
        result += "AVX";
    #else
        result += "No AVX (using scalar operations)";
    #endif
    
    return result;
}

#if defined(__AVX512F__)
void avxVectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) 
{
    size_t vecSize = vA.size();
    for (size_t i = 0; i < vecSize; i += 8) 
    {
        __m512d nVA = _mm512_loadu_pd(vA.data() + i);
        __m512d nVB = _mm512_loadu_pd(vB.data() + i);
        __m512d nVR = _mm512_add_pd(nVA, nVB);
        _mm512_storeu_pd(vResult.data() + i, nVR);
    }
}

void avxVectorSub(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) 
{
    size_t vecSize = vA.size();
    for (size_t i = 0; i < vecSize; i += 8) 
    {
        __m512d nVA = _mm512_loadu_pd(vA.data() + i);
        __m512d nVB = _mm512_loadu_pd(vB.data() + i);
        __m512d nVR = _mm512_sub_pd(nVA, nVB);
        _mm512_storeu_pd(vResult.data() + i, nVR);
    }
}

void avxVectorMul(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) 
{
    size_t vecSize = vA.size();
    for (size_t i = 0; i < vecSize; i += 8) 
    {
        __m512d nVA = _mm512_loadu_pd(vA.data() + i);
        __m512d nVB = _mm512_loadu_pd(vB.data() + i);
        __m512d nVR = _mm512_mul_pd(nVA, nVB);
        _mm512_storeu_pd(vResult.data() + i, nVR);
    }
}


#elif defined(__AVX2__)
void avxVectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) 
{
    size_t vecSize = vA.size();
    for (size_t i = 0; i < vecSize; i += 8) 
    {
        __m256d nVA = _mm256_loadu_pd(vA.data() + i);
        __m256d nVB = _mm256_loadu_pd(vB.data() + i);
        __m256d nVR = _mm256_add_pd(nVA, nVB);
        _mm256_storeu_pd(vResult.data() + i, nVR);
    }
}

void avxVectorSub(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) 
{
    size_t vecSize = vA.size();
    for (size_t i = 0; i < vecSize; i += 8) 
    {
        __m256d nVA = _mm256_loadu_pd(vA.data() + i);
        __m256d nVB = _mm256_loadu_pd(vB.data() + i);
        __m256d nVR = _mm256_sub_pd(nVA, nVB);
        _mm256_storeu_pd(vResult.data() + i, nVR);
    }
}

void avxVectorMul(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) 
{
     size_t vecSize = vA.size();
    for (size_t i = 0; i < vecSize; i += 8) 
    {
        __m256d nVA = _mm256_loadu_pd(vA.data() + i);
        __m256d nVB = _mm256_loadu_pd(vB.data() + i);
        __m256d nVR = _mm256_mul_pd(nVA, nVB);
        _mm256_storeu_pd(vResult.data() + i, nVR);
    }
}

#else
void avxVectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) 
{
    size_t vecSize = vA.size();
    for (size_t i = 0; i < vecSize; ++i) 
    {
        vResult[i] = vA[i] + vB[i];
    }
}

void avxVectorSub(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) 
{
    size_t vecSize = vA.size();
    for (size_t i = 0; i < vecSize; ++i) 
    {
        vResult[i] = vA[i] - vB[i];
    }
}

void avxVectorMul(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult) 
{
    size_t vecSize = vA.size();
    for (size_t i = 0; i < vecSize; ++i) 
    {
        vResult[i] = vA[i] * vB[i];
    }
}
#endif