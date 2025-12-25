#include "Pso.hpp"
#include "AllignedAllocator.hpp"

#include <algorithm>
#include <cstddef>
#include <immintrin.h>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include <chrono>
#include <stdexcept>
#include <span>

Pso::Pso(
    double alpha,
    double beta,
    double interia,        
    int epoch,
    int pointsAmount,
    int pointDimensions,
    std::pair<double, double> bounds,
    const std::function<double(std::span<const double>)>& funcToMinimize,
    int sameGradeEpochs,
    int consecutiveUnchangedEpochs
    )
    :
        _epoch(epoch),
        _pointsAmount(pointsAmount),
        _pointDimensions(pointDimensions),
        _sameGradeEpochs(sameGradeEpochs),
        _consecutiveUnchangedEpochs(consecutiveUnchangedEpochs),
        _alpha(alpha),
        _beta(beta),
        _intertia(interia),
        _bounds(bounds),
        _funcToMinimize(funcToMinimize),
        _randomEngine(std::random_device{}())
    {
        if (_sameGradeEpochs <= 0) 
        {
            _sameGradeEpochs = _epoch / 10; 
        }
        
        _initPoints();
    }

std::tuple<std::vector<double, AlignedAllocator<double, 64>>, double, std::chrono::duration<double>, int> Pso::optimize(void)
{
    bool optimized = updateGlobalBest();
    if (!_globalBestPos.has_value()) 
    {
        throw std::runtime_error("Failed to initialize global best position");
    }
    
    
    std::cout << "Starting optimization" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    for(size_t i = 0; i < _epoch; i++)
    {
        _epsilon1 = getRandomDouble(0.0, 1.0);
        _epsilon2 = getRandomDouble(0.0, 1.0);

        updateVelocities();
        clampVelocities();
        updatePositions();
        enforceBounds();
        evalPoints();
            
        optimized = updateGlobalBest();

        if (optimized) 
        {
            _consecutiveUnchangedEpochs = 0;
        }
        else 
        {
            _consecutiveUnchangedEpochs += 1;
            if (_consecutiveUnchangedEpochs >= _sameGradeEpochs) 
            {
                break;
            }
        }

        //std::cout << "Epoch " << i << ": Best value = " << _globalBestVal << std::endl;
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = endTime - startTime;
    
    if (!_hasValidSolution) 
    {
        throw std::runtime_error("No valid solution found during optimization");
    }
    
    return {*_globalBestPos, _globalBestVal, duration, _consecutiveUnchangedEpochs};
}

void Pso::_initPoints(void)
{
    std::string message = "CPU supports: ";

    _pointRealDimensions = _pointDimensions;
    
    #ifdef __AVX512F__
        if (_pointRealDimensions % 8 != 0) 
        {
            _pointDimensions = ((_pointRealDimensions + 7) / 8) * 8; // Pad to next multiple of 8
        }
    message += "AVX-512F";
    #elif defined(__AVX2__)
        if (_pointRealDimensions % 4 != 0) 
        {
            _pointDimensions = ((_pointRealDimensions + 3) / 4) * 4; // Pad to next multiple of 4
        }
        message += "AVX2";
    #else
        message += "No AVX (using scalar operations)";
    #endif

    std::cout << message << std::endl;
    std::cout << "Vector Size: " << _pointDimensions << std::endl;

    size_t totalSize = _pointsAmount * _pointDimensions;
    std::uniform_real_distribution<> distrPoints(_bounds.first, _bounds.second);
    std::uniform_real_distribution<> distrVal(-1.0, 1.0);

    _pointsPositions.resize(totalSize);    
    _pointsVelocities.resize(totalSize);    
    _personalBests.resize(totalSize);
    _grades.resize(_pointsAmount, std::numeric_limits<double>::max());

    for (size_t i = 0 ; i < _pointsAmount; i++) 
    {
        size_t idx = i * _pointDimensions;
        for(int j = 0; j < _pointRealDimensions; j++)
        {
            _pointsPositions[idx + j] = distrPoints(_randomEngine);
            _pointsVelocities[idx + j] = distrVal(_randomEngine) * _maxVelocity;
        }
    }
    
    std::copy(_pointsPositions.begin(), _pointsPositions.end(), _personalBests.begin());
    std::cout << "Points initialization done" << std::endl;
}

bool Pso::updateGlobalBest(void)
{
    auto bestIter = std::min_element(_grades.begin(), _grades.end());
    auto tempBestGrade = *bestIter;

    if (!_hasValidSolution || tempBestGrade < _globalBestVal)
    {
        
        auto distance = std::distance(_grades.begin(), bestIter);
        auto startIter = _pointsPositions.begin() + (distance * _pointDimensions);
        auto endIter = startIter + _pointDimensions;
        std::vector<double, AlignedAllocator<double, 64>> tempVec(startIter, endIter);
        _globalBestPos =  tempVec;
        _globalBestVal = tempBestGrade;
        _hasValidSolution = true;
        return true;
    } 
    return false;
}

inline double Pso::getRandomDouble(double min, double max) 
{
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(_randomEngine);
}

void Pso::updateVelocities()
{
    #if defined(__AVX512F__)
    __m512d alphaVec = _mm512_set1_pd(_alpha);
    __m512d betaVec = _mm512_set1_pd(_beta);
    __m512d intertiaVec = _mm512_set1_pd(_intertia);

    for (int i = 0; i < _pointsAmount; i++) 
    {
        double r1 = getRandomDouble(0.0, 1.0);
        double r2 = getRandomDouble(0.0, 1.0);

        __m512d eps1Vec = _mm512_set1_pd(r1);
        __m512d eps2Vec = _mm512_set1_pd(r2);
        
        __m512d cognitiveCoef = _mm512_mul_pd(eps1Vec, alphaVec);
        __m512d socialCoef = _mm512_mul_pd(eps2Vec, betaVec);

        for(int j = 0; j < _pointDimensions; j += 8)
        {
            size_t idx = (i * _pointDimensions) + j;

            __m512d pos = _mm512_load_pd(&_pointsPositions[idx]);
            __m512d vel = _mm512_load_pd(&_pointsVelocities[idx]);
            __m512d pBest = _mm512_load_pd(&_personalBests[idx]);
            __m512d gBest = _mm512_load_pd(_globalBestPos->data() + j); 

            __m512d diffGlobal = _mm512_sub_pd(gBest, pos);
            __m512d diffPersonal = _mm512_sub_pd(pBest, pos);

            __m512d term1 = _mm512_mul_pd(diffGlobal, socialCoef);
            __m512d term2 = _mm512_mul_pd(diffPersonal, cognitiveCoef);

            __m512d update = _mm512_add_pd(term1, term2);

            vel = _mm512_fmadd_pd(vel, intertiaVec, update);

            _mm512_store_pd(&_pointsVelocities[idx], vel);
        }
    }
    #elif defined (__AVX2__)
        __m256d alphaVec = _mm256_set1_pd(_alpha);
        __m256d betaVec = _mm256_set1_pd(_beta);
        __m256d intertiaVec = _mm256_set1_pd(_intertia);

        for (int i = 0; i < _pointsAmount; i++) 
        {
            double r1 = getRandomDouble(0.0, 1.0);
            double r2 = getRandomDouble(0.0, 1.0);

            __m256d eps1Vec = _mm256_set1_pd(r1);
            __m256d eps2Vec = _mm256_set1_pd(r2);
            
            __m256d cognitiveCoef = _mm256_mul_pd(eps1Vec, alphaVec);
            __m256d socialCoef = _mm256_mul_pd(eps2Vec, betaVec);
            for(int j = 0; j < _pointDimensions; j += 4)
            {
                size_t idx = (i * _pointDimensions) + j;

                __m256d pos = _mm256_load_pd(&_pointsPositions[idx]);
                __m256d vel = _mm256_load_pd(&_pointsVelocities[idx]);
                __m256d pBest = _mm256_load_pd(&_personalBests[idx]);
                __m256d gBest = _mm256_load_pd(_globalBestPos->data() + j); 

                __m256d diffGlobal = _mm256_sub_pd(gBest, pos);
                __m256d diffPersonal = _mm256_sub_pd(pBest, pos);

                __m256d term1 = _mm256_mul_pd(diffGlobal, socialCoef);
                __m256d term2 = _mm256_mul_pd(diffPersonal, cognitiveCoef);

                __m256d update = _mm256_add_pd(term1, term2);

                vel = _mm256_fmadd_pd(vel, intertiaVec, update);

                _mm256_store_pd(&_pointsVelocities[idx], vel);
            }
        }
    #endif
}

void Pso::updatePositions()
{
    #if defined(__AVX512F__)
        for (size_t i = 0; i < _pointsAmount; i++) 
        {
            for (size_t j = 0; j < _pointDimensions; j += 8) 
            {
                size_t idx = (i * _pointDimensions) + j;

                __m512d pos = _mm512_load_pd(&_pointsPositions[idx]);
                __m512d vel = _mm512_load_pd(&_pointsVelocities[idx]);
                __m512d newPos = _mm512_add_pd(pos, vel);
                _mm512_store_pd(&_pointsPositions[idx], newPos);
            }
        }
    #elif defined(__AVX2__)
        for (size_t i = 0; i < _pointsAmount; i++) 
        {
            for (size_t j = 0; j < _pointDimensions; j += 4) 
            {
                size_t idx = (i * _pointDimensions) + j;

                __m256d pos = _mm256_load_pd(&_pointsPositions[idx]);
                __m256d vel = _mm256_load_pd(&_pointsVelocities[idx]);
                __m256d newPos = _mm256_add_pd(pos, vel);
                _mm256_store_pd(&_pointsPositions[idx], newPos);
            }
        }
    #endif
}

void Pso::enforceBounds()
{
    #ifdef __AVX512F__
        __m512d minBVec = _mm512_set1_pd(_bounds.first);
        __m512d maxBVec = _mm512_set1_pd(_bounds.second);

        for (size_t i = 0; i < _pointsAmount; i++) 
        {
            for (size_t j = 0; j < _pointDimensions; j += 8)
            {
                size_t idx = (i * _pointDimensions) + j;

                __m512d pos = _mm512_load_pd(&_pointsPositions[idx]);
                pos = _mm512_max_pd(minBVec, _mm512_min_pd(maxBVec, pos));

                _mm512_store_pd(&_pointsPositions[idx], pos);
            }
        }
    #elif defined (__AVX2__)
        __m256d minBVec = _mm256_set1_pd(_bounds.first);
        __m256d maxBVec = _mm256_set1_pd(_bounds.second);

        for (size_t i = 0; i < _pointsAmount; i++) 
        {
            for (size_t j = 0; j < _pointDimensions; j += 4)
            {
                size_t idx = (i * _pointDimensions) + j;

                __m256d pos = _mm256_load_pd(&_pointsPositions[idx]);
                pos = _mm256_max_pd(minBVec, _mm256_min_pd(maxBVec, pos));
                
                _mm256_store_pd(&_pointsPositions[idx], pos);
            }   
        }
    #endif
}

void Pso::clampVelocities()
{
    #ifdef __AVX512F__
        __m512d minVec = _mm512_set1_pd(-_maxVelocity); //minVelocity = -1 * maxvelocity
        __m512d maxVec = _mm512_set1_pd(_maxVelocity);

        for (size_t i = 0; i < _pointsAmount; i++) 
        {
            for (size_t j = 0; j < _pointDimensions; j += 8) 
            {
                size_t idx = (i * _pointDimensions) + j;

                __m512d vel = _mm512_load_pd(&_pointsVelocities[idx]);
                vel = _mm512_max_pd(minVec, _mm512_min_pd(maxVec, vel));

                _mm512_store_pd(&_pointsVelocities[idx], vel);
            }
        }
    #elif defined(__AVX2__)
        __m256d minVec = _mm256_set1_pd(-_maxVelocity);
        __m256d maxVec = _mm256_set1_pd(_maxVelocity);

        for (size_t i = 0; i < _pointsAmount; i++) 
        {
            for(size_t j = 0; j < _pointDimensions; j += 4)
            {
                size_t idx = (i * _pointDimensions) + j;
                
                __m256d vel = _mm256_load_pd((&_pointsVelocities[idx]);
                vel = _mm256_max_pd(minVec, _mm256_min_pd(maxVec, vel));
                
                _mm256_store_pd(&_pointsVelocities[idx], vel);
            }
        }
    #endif
}

void Pso::evalPoints()
{
    for (size_t i = 0; i < _pointsAmount; i++) 
    {
        size_t idx = i * _pointDimensions;

        std::span<const double> particleView(&_pointsPositions[idx], _pointDimensions);

        auto currentGrade = _funcToMinimize(particleView);

        if(currentGrade < _grades[i])
        {
            std::copy_n(
                &_pointsPositions[idx],
                _pointDimensions,
                &_personalBests[idx]
            );
            _grades[i] = currentGrade;
        }
    }
}