#include "Point.h"
#include "CustomAllignedAllocator.h"

#include <immintrin.h>
#include <functional>
#include <vector>
#include <algorithm>
#include <stdexcept>

Point::Point(const std::vector<double, CustomAllignedAllocator<double>>& startPos, 
             const std::vector<double, CustomAllignedAllocator<double>>& startVelocity)
    :
    position(startPos),
    velocityVector(startVelocity),
    personalBest(startPos)
    {
        if(startPos.empty() || startVelocity.empty())
        {
            throw std::runtime_error("Point initalized with empty vectors");
        }
    }

void Point::evalPoint(const std::function<double(const std::vector<double,
                      CustomAllignedAllocator<double>>&)>& funcToMinimize)
{
    auto currentGrade = funcToMinimize(position);
    evaluated = true;
    
    if (currentGrade < grade) 
    {
        personalBest = position;
        grade = currentGrade;
    }
}

void Point::updatePosition()
{
    #ifdef __AVX512F__
        for (size_t i = 0; i < position.size(); i += 8)
        {
            __m512d pos = _mm512_loadu_pd(position.data() + i);
            __m512d vel = _mm512_loadu_pd(velocityVector.data() + i);
            __m512d newPos = _mm512_add_pd(pos, vel);
            _mm512_storeu_pd(position.data() + i, newPos);
        }
        
    #elif defined(__AVX2__)
        for (size_t i = 0; i < position.size(); i += 4) 
        {
            __m256d pos = _mm256_loadu_pd(position.data() + i);
            __m256d vel = _mm256_loadu_pd(velocityVector.data() + i);
            __m256d newPos = _mm256_add_pd(pos, vel);
            _mm256_storeu_pd(position.data() + i, newPos);
        }    
    #else
        for (size_t i = 0; i < position.size(); i++) 
        {
            position[i] += velocityVector[i];
        }
    #endif
}

void Point::updateVelocity(double alpha, double beta, double epsilon1, double epsilon2, 
                           const std::vector<double, CustomAllignedAllocator<double>>& globalBest)
{
   #ifdef __AVX512F__
    __m512d eps1Vec = _mm512_set1_pd(epsilon1);
    __m512d eps2Vec = _mm512_set1_pd(epsilon2);
    __m512d alphaVec = _mm512_set1_pd(alpha);
    __m512d betaVec = _mm512_set1_pd(beta);
    
    for (size_t i = 0; i < position.size(); i += 8)
    {
        __m512d pos = _mm512_loadu_pd(position.data() + i);
        __m512d vel = _mm512_loadu_pd(velocityVector.data() + i);
        __m512d gBest = _mm512_loadu_pd(globalBest.data() + i);
        __m512d pBest = _mm512_loadu_pd(personalBest.data() + i);
       

        __m512d diffGlobal = _mm512_sub_pd(gBest, pos);
        __m512d diffPersonal = _mm512_sub_pd(pBest, pos);
        __m512d term1 = _mm512_mul_pd(diffGlobal, eps1Vec);
        __m512d term2 = _mm512_mul_pd(diffPersonal, eps2Vec);
        __m512d scaledTerm1 = _mm512_mul_pd(term1, alphaVec);
        __m512d scaledTerm2 = _mm512_mul_pd(term2, betaVec);
        __m512d update = _mm512_add_pd(scaledTerm1, scaledTerm2);
        vel = _mm512_add_pd(vel, update);
        _mm512_storeu_pd(velocityVector.data() + i, vel);
    }
    
    #elif defined(__AVX2__)
        __m256d eps1Vec = _mm256_set1_pd(epsilon1);
        __m256d eps2Vec = _mm256_set1_pd(epsilon2);
        __m256d alphaVec = _mm256_set1_pd(alpha);
        __m256d betaVec = _mm256_set1_pd(beta);

        for (size_t i = 0; i < position.size(); i += 4) 
        {
            __m256d pos = _mm256_loadu_pd(position.data() + i);
            __m256d vel = _mm256_loadu_pd(velocityVector.data() + i);
            __m256d gBest = _mm256_loadu_pd(globalBest.data() + i);
            __m256d pBest = _mm256_loadu_pd(personalBest.data() + i);
        
            __m256d diffGlobal = _mm256_sub_pd(gBest, pos);
            __m256d diffPersonal = _mm256_sub_pd(pBest, pos);
            __m256d term1 = _mm256_mul_pd(  diffGlobal, eps1Vec);
            __m256d term2 = _mm256_mul_pd(diffPersonal, eps2Vec);
            __m256d scaledTerm1 = _mm256_mul_pd(term1, alphaVec);
            __m256d scaledTerm2 = _mm256_mul_pd(term2, betaVec);
            __m256d update = _mm256_add_pd(scaledTerm1, scaledTerm2);
            vel = _mm256_add_pd(vel, update);
            _mm256_storeu_pd(velocityVector.data() + i, vel);
        }    
    #else
        for (size_t i = 0; i < position.size(); i++) 
        {
            velocityVector[i] += (alpha * epsilon1 * (globalBest[i] - position[i]) +
                                beta * epsilon2 * (personalBest[i] - position[i]));
        }
    #endif
}

void Point::enforceBounds(const std::pair<int, int>& bounds)
{
    double minBound = static_cast<double>(bounds.first);
    double maxBound = static_cast<double>(bounds.second);

    #ifdef __AVX512f__
        __m512d minBVec = _mm512_set1_pd(minBound);
        __m512d maxBVec = _mm512_set1_pd(maxBound);

        for (size_t i = 0; i < position.size(); i += 8)
        {
            __m512d pos = _mm512_loadu_pd(position.data() + i);
            pos = _mm512d_max_pd(minBVec, _mm512_min_pd(maxBVec, pos));
            _mm512_storeu_pd(position.data() + i, pos);
        }
    #elif defined (__AVX2__)
        __m256d minBVec = _mm256_set1_pd(minBound);
        __m256d maxBVec = _mm256_set1_pd(maxBound);

        for (size_t i = 0; i < position.size(); i += 8)
        {
            __m256d pos = _mm256_loadu_pd(position.data() + i);
            pos = _mm256_max_pd(minBVec, _mm256_min_pd(maxBVec, pos));
            _mm256_storeu_pd(position.data() + i, pos);
        }
    #else
        for(size_t i = 0; i < position.size(); i++)
        {
            position[i] = std::max(minBound, std::min(maxBound, position[i]));
        }
    #endif
}

void Point::clampVelocity(double maxVelocity)
{
    double minVel = -maxVelocity;
    #ifdef __AVX512F__
        __m512d minVec = _mm512_set1_pd(minVel);
        __m512d maxVec = _mm512_set1_pd(maxVelocity);

        for (size_t i = 0; i < position.size(); i += 8) 
        {
            __m512d vel = _mm512_loadu_pd(velocityVector.data() + i);
            vel = _mm512_max_pd(minVec, _mm512_min_pd(maxVec, vel));
            _mm512_storeu_pd(velocityVector.data() + i, vel);
        }
    #elif defined(__AVX2__)
        __m256d minVec = _mm256_set1_pd(minVel);
        __m256d maxVec = _mm256_set1_pd(maxVelocity);

        for (size_t i = 0; i < position.size(); i += 4) 
        {
            __m256d vel = _mm256_loadu_pd(velocityVector.data() + i);
            vel = _mm256_max_pd(minVec, _mm256_min_pd(maxVec, vel));
            _mm256_storeu_pd(velocityVector.data() + i, vel);
        }
  
    #else
        for(size_t i = 0; i < velocityVector.size(); i++)
        {
            velocityVector[i] = std::max(-maxVelocity, std::min(maxVelocity, velocityVector[i]));
        }
    #endif
}
