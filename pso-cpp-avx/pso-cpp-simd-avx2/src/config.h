#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <stdexcept>

void avx512VectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);
void avx2VectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);
void scalarVectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);

inline void avxVectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult)
{
    if(vA.size() != vB.size() || vA.size() != vResult.size() || vB.size() != vResult.size())
    {
        throw std::runtime_error("Vector size mismatch");
    }

    if(__builtin_cpu_supports("avx512f"))
    {
        avx512VectorAdd(vA, vB, vResult);
    }
    else if (__builtin_cpu_supports("avx2"))
    {
        avx2VectorAdd(vA, vB, vResult);
    }
    else 
    {
        scalarVectorAdd(vA, vB, vResult);
    }
}


#endif 