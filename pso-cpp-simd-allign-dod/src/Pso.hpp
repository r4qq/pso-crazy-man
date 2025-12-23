#pragma once

#include <vector>
#include <functional>
#include <random>
#include <chrono>
#include <optional>

#include "AllignedAllocator.hpp"

class Pso
{
    public:
        Pso
        (
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
        );
        
        std::tuple<std::vector<double, AlignedAllocator<double, 64>>, double, std::chrono::duration<double>, int> optimize(void);
        void setMaxVelocity(double maxVel) { _maxVelocity = maxVel; }
        int getPointDimensions() const {return _pointDimensions;}
        
    private:

        bool _hasValidSolution = false;
        int _epoch;
        int _pointsAmount;
        int _pointDimensions;
        int _pointRealDimensions;
        int _sameGradeEpochs;
        int _consecutiveUnchangedEpochs;
        double _alpha;
        double _beta;
        double _intertia;
        double _globalBestVal = std::numeric_limits<double>::max();
        double _maxVelocity = 4.0;
        std::vector<double, AlignedAllocator<double, 64>> _pointsPositions;
        std::vector<double, AlignedAllocator<double, 64>> _personalBests;
        std::vector<double, AlignedAllocator<double, 64>> _pointsVelocities;
        std::vector<double, AlignedAllocator<double, 64>> _grades;
        std::pair<double, double> _bound;
        std::function<double(const std::vector<double, AlignedAllocator<double, 64>>&)> _funcToMinimize;
        std::optional<std::vector<double, AlignedAllocator<double, 64>>> _globalBestPos;
        std::mt19937 _randomEngine;

        void _initPoints(void);
        bool updateGlobalBest(void);
        double getRandomDouble(double min, double max);
};
