#include "Pso.hpp"
#include "AllignedAllocator.hpp"

#include <algorithm>
#include <asm-generic/errno.h>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <limits>
#include <ostream>
#include <random>
#include <stdalign.h>
#include <string>
#include <utility>
#include <vector>
#include <chrono>
#include <stdexcept>

Pso::Pso(
    double alpha,
    double beta,
    double interia,        
    int epoch,
    int pointsAmount,
    int pointDimensions,
    std::pair<double, double> bound,
    const std::function<double(const std::vector<double, AlignedAllocator<double, 64>>&)>& funcToMinimize,
    int sameGradeEpochs,
    int consecutiveUnchangedEpochs
    )
    :
        _alpha(alpha),
        _beta(beta),
        _intertia(interia),
        _epoch(epoch),
        _pointsAmount(pointsAmount),
        _pointDimensions(pointDimensions),
        _bound(bound),
        _funcToMinimize(funcToMinimize),
        _sameGradeEpochs(sameGradeEpochs),
        _consecutiveUnchangedEpochs(consecutiveUnchangedEpochs),
        _randomEngine(std::random_device{}())
    {
        if (_sameGradeEpochs <= 0) 
        {
            _sameGradeEpochs = _epoch / 10; 
        }
        
        _initPoints();
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
    std::uniform_real_distribution<> distrPoints(_bound.first, _bound.second);
    std::uniform_real_distribution<> distrVal(-1.0, 1.0);

    _pointsPositions.reserve(totalSize);
    std::fill(_pointsPositions.begin(), _pointsPositions.end(), 0.0);
    
    _pointsVelocities.reserve(totalSize);
    std::fill(_pointsVelocities.begin(), _pointsVelocities.end(), 0.0);
    
    _personalBests.reserve(totalSize);
    _grades.resize(_pointsAmount, std::numeric_limits<double>::max());

    for (size_t i = 0 ; i < _pointsAmount; i++) 
    {
        size_t idx = i * _pointDimensions;
        for(int j = 0; j < _pointRealDimensions; j++)
        {
            _pointsPositions[idx + j] = distrPoints(_randomEngine);
            _pointsVelocities[idx + j] = distrPoints(_randomEngine) * _maxVelocity;
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

std::tuple<std::vector<double, AlignedAllocator<double, 64>>, double, std::chrono::duration<double>, int> Pso::optimize(void)
{
    bool optimized = updateGlobalBest();
    if (!_globalBestPos.has_value()) 
    {
        throw std::runtime_error("Failed to initialize global best position");
    }
    
    double epsilon1 = 0;
    double epsilon2 = 0;
    std::cout << "Starting optimization" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    for(size_t i = 0; i < _epoch; i++)
    {
        for(auto& point : _points)
        {
            epsilon1 = getRandomDouble(0.0, 1.0);
            epsilon2 = getRandomDouble(0.0, 1.0);

            point.updateVelocity(_alpha, _beta, _intertia, epsilon1, epsilon2, *_globalBestPos);
            point.clampVelocity(_maxVelocity);
            point.updatePosition();
            point.enforceBounds(_bound);
            point.evalPoint(_funcToMinimize);
            
        }

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

double Pso::getRandomDouble(double min, double max) 
{
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(_randomEngine);
}