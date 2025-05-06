#include "Pso.h"
#include "Point.h"
#include "CustomAllocator.h"

#include <cstddef>
#include <iostream>
#include <ostream>
#include <random>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <chrono>
#include <stdexcept>

Pso::Pso(
    float alpha,
    float beta,        
    int epoch,
    int pointsAmount,
    int pointDimensions,
    std::pair<double, double> bound,
    const std::function<double(const std::vector<double, CustomAllocator<double>>&)>& funcToMinimize,
    int sameGradeEpochs,
    int consecutiveUnchangedEpochs
    )
    :
        _alpha(alpha),
        _beta(beta),
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
        
        _points = _initPoints();
    }

std::vector<Point, CustomAllocator<Point>> Pso::_initPoints(void)
{
    std::uniform_int_distribution<> distrPoints(_bound.first, _bound.second);
    std::uniform_real_distribution<> distrVal(-1.0, 1.0);
    
    std::vector<Point, CustomAllocator<Point>> points;
    points.reserve(_pointsAmount);

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

    for (int i = 0; i < _pointsAmount; i++) 
    {
        
        std::vector<double, CustomAllocator<double>> startPos;
        std::vector<double, CustomAllocator<double>> velocityVector;
        for (int j = 0; j < _pointRealDimensions; j++) 
        {
            startPos.push_back(static_cast<double>(distrPoints(_randomEngine)));
            velocityVector.push_back(distrVal(_randomEngine) * _maxVelocity / 2.0);
        }

        startPos.resize(_pointDimensions, 0.0);
        velocityVector.resize(_pointDimensions, 0.0);

        Point point = Point(startPos, velocityVector);
        point.evalPoint(_funcToMinimize);
        points.push_back(std::move(point));
    }   
    std::cout << "Points initialization done" << std::endl;
    return points;
}

bool Pso::updateGlobalBest(void)
{
    auto bestIter = std::min_element(_points.begin(), _points.end(),
        [](const Point& a, const Point& b)
        {
            return a.getGrade() < b.getGrade();
        });

    const Point& tempPoint = *bestIter;

    if (!_hasValidSolution || tempPoint.getGrade() < _globalBestVal)
    {
        _globalBestPos = tempPoint.getPosition();
        _globalBestVal = tempPoint.getGrade();
        _hasValidSolution = true;
        return true;
    } 
    return false;
}

std::tuple<std::vector<double, CustomAllocator<double>>, double, std::chrono::duration<double>> Pso::optimize(void)
{
    auto optimized = updateGlobalBest();
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

            point.updateVelocity(_alpha, _beta, epsilon1, epsilon2, *_globalBestPos);
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
    }
    
    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = endTime - startTime;
    
    if (!_hasValidSolution) 
    {
        throw std::runtime_error("No valid solution found during optimization");
    }
    
    return {*_globalBestPos, _globalBestVal, duration};
}

double Pso::getRandomDouble(double min, double max) 
{
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(_randomEngine);
}