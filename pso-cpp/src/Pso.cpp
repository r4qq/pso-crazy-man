#include "Pso.h"
#include "Point.h"
#include <iostream>
#include <ostream>
#include <random>
#include <utility>
#include <vector>
#include <algorithm>
#include <chrono>

Pso::Pso(
    float alpha,
    float beta,        
    int epoch,
    int pointsAmount,
    int pointDimensions,
    std::pair<int, int> bound,
    std::function<double(const std::vector<int>&)> funcToMinimize,
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
        _consecutiveUnchangedEpochs(consecutiveUnchangedEpochs)
        
    {
        _points = _initPoints();
    }

std::vector<Point> Pso::_initPoints(void)
{
    std::random_device random;
    std::mt19937 gen(random());
    std::uniform_int_distribution<> distrPoints(_bound.first, _bound.second);
    std::uniform_real_distribution<> distrVal(-1, 1);
    std::vector<Point> points;
    
    for (auto i  = 0; i < _pointsAmount; i++) 
    {
        std::vector<int> startPos;
        std::vector<double> velocityVector;
        for (auto j = 0; j < _pointDimensions; j++) 
        {
            startPos.push_back(distrPoints(gen));
            velocityVector.push_back(distrVal(gen));
        }
        auto point = Point(startPos, velocityVector);
        point.evalPoint(_funcToMinimize);
        points.push_back(point);
    }   
    std::cout << "Points initalization done" << std::endl;
    return points;
}

bool Pso::updateGlobalBest(void)
{

    auto bestIter = std::min_element(_points.begin(), _points.end(),
        [](const Point& a, const Point& b)
        {
            return a._grade < b._grade;
        });

    const Point& tempPoint = *bestIter;

    if (tempPoint._grade < _globalBestVal)
    {
        _globalBestPos = tempPoint._position;
        _globalBestVal = tempPoint._grade;
        return true;
    } 
    return false;
}


std::tuple<std::vector<int>, double, std::chrono::duration<double>> Pso::optimize(void)
{
    auto optimized = updateGlobalBest();
    std::cout << "Starting optimization" << std::endl;
    auto startTime = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < _epoch; i++)
    {
        
        for(auto& point : _points)
        {
            const auto epsilon1 = getRandomDouble(0.0, 1.0); 
            const auto epsilon2 = getRandomDouble(0.0, 1.0);
             
            point.updateVelocity(_alpha, _beta, epsilon1, epsilon2, _globalBestPos);
            point.updatePoisiton();
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
    return {_globalBestPos, _globalBestVal,  duration};
}

double Pso::getRandomDouble(double min, double max) 
{
    static thread_local std::uniform_real_distribution<double> distribution(min, max);
    return distribution(_randomEngine);
}