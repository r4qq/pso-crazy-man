#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <vector>
#include <stdexcept>
#include <iostream>

std::string getAvxCapabilities();

#ifdef __AVX512F__
void avx512VectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);
void avx512VectorSub(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);
#endif

#ifdef __AVX2__
void avx2VectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);
void avx2VectorSub(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);
#endif

void scalarVectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);
void scalarVectorSub(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);

#ifdef __AVX512F__
#define USINGAVX512F 1
#define USINGAVX2 0
#elif defined(__AVX2__)
#define USINGAVX512F 0
#define USINGAVX2 1
#else
#define USINGAVX512F 0
#define USINGAVX2 0
#endif

inline void avxVectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult)
{
    if (vA.size() != vB.size() || vA.size() != vResult.size())
    {
        throw std::runtime_error("Vector size mismatch");
    }

#ifdef __AVX512F__
    avx512VectorAdd(vA, vB, vResult);
#elif defined(__AVX2__)
    avx2VectorAdd(vA, vB, vResult);
#else
    scalarVectorAdd(vA, vB, vResult);
#endif
}   

inline void avxVectorSub(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult)
{
    if (vA.size() != vB.size() || vA.size() != vResult.size())
    {
        throw std::runtime_error("Vector size mismatch");
    }

#ifdef __AVX512F__
    avx512VectorSub(vA, vB, vResult);
#elif defined(__AVX2__)
    avx2VectorSub(vA, vB, vResult);
#else
    scalarVectorSub(vA, vB, vResult);
#endif
}   
#endif