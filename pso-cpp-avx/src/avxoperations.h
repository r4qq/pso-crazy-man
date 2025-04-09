#pragma once

#ifndef AVXOPERATIONS_H
#define AVXOPERATIONS_H

#include <vector>
#include <stdexcept>
#include <string>

std::string getAvxCapabilities();

void avxVectorAdd(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);
void avxVectorSub(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);
void avxVectorMul(const std::vector<double>& vA, const std::vector<double>& vB, std::vector<double>& vResult);



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

#endif