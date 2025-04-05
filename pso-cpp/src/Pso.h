#pragma once

#include <vector>
#include <functional>
#include <random>
#include <algorithm>
#include <chrono>

#include "Point.h"

class Pso
{
    public:
        float alpha;
        float beta;        
        int epoch;
        int pointsAmount;
        int pointDimensions;
        std::pair<int, int> bound;
        std::function<double(const std::vector<int>&)> funcToMinimize;
        int sameGradeEpochs;
        int consecutiveUnchangedEpochs;

        Pso(
        float alpha,
        float beta,        
        int epoch,
        int pointsAmount,
        int pointDimensions,
        std::pair<int, int> bound,
        std::function<double(const std::vector<int>&)> funcToMinimize,
        int sameGradeEpochs,
        int consecutiveUnchangedEpochs);
        
        std::vector<Point> _initPoints(void);
        bool updateGlobalBest(void);
        std::tuple<std::vector<int>, double, std::chrono::duration<double>> optimize(void);
        void returnBest(void);
        double getRandomDouble(double min, double max);

    private:
        float _alpha;
        float _beta;
        int _epoch;
        int _pointsAmount;
        int _pointDimensions;
        std::pair<int, int> _bound;
        std::function<double(const std::vector<int>&)> _funcToMinimize;
        int _sameGradeEpochs;
        std::vector<Point> _points;
        std::vector<int> _globalBestPos;
        double _globalBestVal;
        int _consecutiveUnchangedEpochs;
        std::mt19937 _randomEngine;

};