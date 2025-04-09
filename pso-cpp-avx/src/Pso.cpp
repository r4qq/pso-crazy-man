#include "Pso.h"
#include "Point.h"
#include "avxoperations.h"

#include <iostream>
#include <ostream>
#include <random>
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
    std::pair<int, int> bound,
    const std::function<double(const std::vector<double>&)>& funcToMinimize,
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

std::vector<Point> Pso::_initPoints(void)
{
    std::uniform_int_distribution<> distrPoints(_bound.first, _bound.second);
    std::uniform_real_distribution<> distrVal(-1.0, 1.0);
    std::vector<Point> points;
    
    
    _pointRealDimensions = _pointDimensions;
    if(USINGAVX512F == 1 && USINGAVX2 == 0)
    {
        std::cout << "Using AVX512f, resizing to 8" << std::endl;
        _pointDimensions += 8 - _pointRealDimensions % 8;
    }
    else if (USINGAVX512F == 0 && USINGAVX2 == 1)
    {   
        std::cout << "Using AVX2, resizing to 4" << std::endl;
        _pointDimensions += 4 - _pointRealDimensions % 4;
    }
  

    _alphaVector = std::vector<double>(_pointDimensions, _alpha);
    _betaVector = std::vector<double>(_pointDimensions, _beta);


    for (int i = 0; i < _pointsAmount; i++) 
    {
        std::vector<double> startPos;
        std::vector<double> velocityVector;
        for (int j = 0; j < _pointRealDimensions; j++) 
        {
            startPos.push_back(static_cast<double>(distrPoints(_randomEngine)));
            velocityVector.push_back(distrVal(_randomEngine) * _maxVelocity / 2.0);
        }

        startPos.resize(_pointDimensions, 0.0);
        velocityVector.resize(_pointDimensions, 0.0);

        auto point = Point(startPos, velocityVector);
        point.evalPoint(_funcToMinimize);
        points.push_back(point);
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

std::tuple<std::vector<double>, double, std::chrono::duration<double>> Pso::optimize(void)
{
    auto optimized = updateGlobalBest();
    if (!_globalBestPos.has_value()) 
    {
        throw std::runtime_error("Failed to initialize global best position");
    }
    
    std::vector<double> epsilon1Vector;
    std::vector<double> epsilon2Vector;

    std::cout << "Starting optimization" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    
    for(int i = 0; i < _epoch; i++)
    {
        for(auto& point : _points)
        {
            epsilon1Vector = std::vector<double>(_pointDimensions, getRandomDouble(0.0, 1.0));
            epsilon2Vector = std::vector<double>(_pointDimensions, getRandomDouble(0.0, 1.0));

            point.updateVelocity(_alphaVector, _betaVector, epsilon1Vector, epsilon2Vector, *_globalBestPos);
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
                std::cout << "Early termination after " << (i + 1) << " epochs due to unchanged best value for " 
                          << _consecutiveUnchangedEpochs << " consecutive epochs." << std::endl;
                break;
            }
        }
        
        if ((i + 1) % 10 == 0 || i == 0) {
            std::cout << "Completed " << (i + 1) << " epochs. Current best value: " << _globalBestVal << std::endl;
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