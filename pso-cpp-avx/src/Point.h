#pragma once

#include <functional>
#include <vector>
#include <limits>
#include <stdexcept>

#include "CustomAllignedAllocator.h"

class Point 
{
    public:
        Point(const std::vector<double, CustomAllignedAllocator<double>>& position,
              const std::vector<double, CustomAllignedAllocator<double>>& velocityVector);
        void updateVelocity(double alpha, double beta, double epsilon1, double epsilon2, 
                            const std::vector<double, CustomAllignedAllocator<double>>& globalBest);
        void updatePosition(void);
        void evalPoint(const std::function<double(const std::vector<double, CustomAllignedAllocator<double>>&)>& funcToMinimize);
        void enforceBounds(const std::pair<int, int>& bounds);
        void clampVelocity(double maxVelocity);
        
        const std::vector<double, CustomAllignedAllocator<double>>& getPosition() const { return position; }
        const std::vector<double, CustomAllignedAllocator<double>>& getVelocityVector() const { return velocityVector; }
        const std::vector<double, CustomAllignedAllocator<double>>& getPersonalBest() const { return personalBest; }
        double getGrade() const { return grade; }
        bool hasBeenEvaluated() const { return evaluated; }

    private:
        std::vector<double, CustomAllignedAllocator<double>> position;
        std::vector<double, CustomAllignedAllocator<double>> velocityVector;
        std::vector<double, CustomAllignedAllocator<double>> personalBest;
        double grade = std::numeric_limits<double>::max();
        bool evaluated = false;
};
