#pragma once

#include <functional>
#include <vector>
#include <limits>
#include <stdexcept>

#include "CustomAllocator.h"

class Point 
{
    public:
        Point(const std::vector<double, CustomAllocator<double>>& position,
              const std::vector<double, CustomAllocator<double>>& velocityVector);
        void updateVelocity(double alpha, double beta, double epsilon1, double epsilon2, 
                            const std::vector<double, CustomAllocator<double>>& globalBest);
        void updatePosition(void);
        void evalPoint(const std::function<double(const std::vector<double, CustomAllocator<double>>&)>& funcToMinimize);
        void enforceBounds(const std::pair<int, int>& bounds);
        void clampVelocity(double maxVelocity);
        
        const std::vector<double, CustomAllocator<double>>& getPosition() const { return position; }
        const std::vector<double, CustomAllocator<double>>& getVelocityVector() const { return velocityVector; }
        const std::vector<double, CustomAllocator<double>>& getPersonalBest() const { return personalBest; }
        double getGrade() const { return grade; }
        bool hasBeenEvaluated() const { return evaluated; }

    private:
        std::vector<double, CustomAllocator<double>> position;
        std::vector<double, CustomAllocator<double>> velocityVector;
        std::vector<double, CustomAllocator<double>> personalBest;
        double grade = std::numeric_limits<double>::max();
        bool evaluated = false;
};
