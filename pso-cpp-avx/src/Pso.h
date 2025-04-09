#pragma once

#include <vector>
#include <functional>
#include <random>
#include <algorithm>
#include <chrono>
#include <optional>

#include "Point.h"

class Pso
{
    public:
        Pso(
        float alpha,
        float beta,        
        int epoch,
        int pointsAmount,
        int pointDimensions,
        std::pair<int, int> bound,
        const std::function<double(const std::vector<double>&)>& funcToMinimize,
        int sameGradeEpochs,
        int consecutiveUnchangedEpochs);
        
        std::tuple<std::vector<double>, double, std::chrono::duration<double>> optimize(void);
        void setMaxVelocity(double maxVel) { _maxVelocity = maxVel; }
        int getRealPointDimensions() const {return _pointRealDimensions;}
        
    private:
        std::vector<Point> _initPoints(void);
        bool updateGlobalBest(void);
        double getRandomDouble(double min, double max);

        float _alpha;
        float _beta;
        std::vector<double> _alphaVector;
        std::vector<double> _betaVector;
        int _epoch;
        int _pointsAmount;
        int _pointDimensions;
        int _pointRealDimensions;
        std::pair<int, int> _bound;
        std::function<double(const std::vector<double>&)> _funcToMinimize;
        int _sameGradeEpochs;
        std::vector<Point> _points;
        std::optional<std::vector<double>> _globalBestPos;
        double _globalBestVal = std::numeric_limits<double>::max();
        int _consecutiveUnchangedEpochs;
        std::mt19937 _randomEngine;
        double _maxVelocity = 4.0; 
        bool _hasValidSolution = false;
};
