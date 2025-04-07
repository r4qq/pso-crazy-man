#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <stdexcept>
#include <iostream>

#ifdef __AVX512F__
void avx512VectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);
#endif
#ifdef __AVX2__
void avx2VectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);
#endif
void scalarVectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);

inline void avxVectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult)
{
    if (vA.size() != vB.size() || vA.size() != vResult.size())
    {
        throw std::runtime_error("Vector size mismatch");
    }

#ifdef __AVX512F__
    avx512VectorAdd(vA, vB, vResult);
    std::cout << "AVX512 detected" << std::endl;
#elif defined(__AVX2__)
    avx2VectorAdd(vA, vB, vResult);
    std::cout << "AVX2 detected" << std::endl;
#else
    scalarVectorAdd(vA, vB, vResult);
    std::cout << "No avx detected, using scalar" << std::endl;
#endif
}

#endif