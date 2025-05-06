#pragma once

#include <cstddef>
#include <cstdlib>
#include <vector>
#include <functional>
#include <random>
#include <algorithm>
#include <chrono>
#include <optional>

#include "Point.h"
#include "CustomAllocator.h"

class Pso
{
    public:
        Pso(
        float alpha,
        float beta,        
        int epoch,
        int pointsAmount,
        int pointDimensions,
        std::pair<double, double> bound,
        const std::function<double(const std::vector<double, CustomAllocator<double>>&)>& funcToMinimize,
        int sameGradeEpochs,
        int consecutiveUnchangedEpochs);
        
        std::tuple<std::vector<double, CustomAllocator<double>>, double, std::chrono::duration<double>> optimize(void);
        void setMaxVelocity(double maxVel) { _maxVelocity = maxVel; }
        int getPointDimensions() const {return _pointDimensions;}
        
    private:
        std::vector<Point, CustomAllocator<Point>> _initPoints(void);
        bool updateGlobalBest(void);
        double getRandomDouble(double min, double max);

        float _alpha;
        float _beta;
        int _epoch;
        int _pointsAmount;
        int _pointDimensions;
        int _pointRealDimensions;
        std::pair<double, double> _bound;
        std::function<double(const std::vector<double, CustomAllocator<double>>&)> _funcToMinimize;
        int _sameGradeEpochs;
        std::vector<Point> _points;
        std::optional<std::vector<double>> _globalBestPos;
        double _globalBestVal = std::numeric_limits<double>::max();
        int _consecutiveUnchangedEpochs;
        std::mt19937 _randomEngine;
        double _maxVelocity = 4.0; 
        bool _hasValidSolution = false;
};
