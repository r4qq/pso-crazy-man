#pragma once

#include "AllignedAllocator.hpp"

#include <functional>
#include <vector>
#include <limits>

class Point 
{
    public:
        Point(const std::vector<double, AlignedAllocator<double, 64>>& position,
              const std::vector<double, AlignedAllocator<double, 64>>& velocityVector);
        void updateVelocity(double alpha, double beta, double _intertia, double epsilon1, double epsilon2, 
                            const std::vector<double, AlignedAllocator<double, 64>>& globalBest);
        void updatePosition(void);
        void evalPoint(const std::function<double(const std::vector<double, AlignedAllocator<double, 64>>&)>& funcToMinimize);
        void enforceBounds(const std::pair<double, double>& bounds);
        void clampVelocity(double maxVelocity);
        
        const std::vector<double, AlignedAllocator<double, 64>>& getPosition() const { return position; }
        const std::vector<double, AlignedAllocator<double, 64>>& getVelocityVector() const { return velocityVector; }
        const std::vector<double, AlignedAllocator<double, 64>>& getPersonalBest() const { return personalBest; }
        double getGrade() const { return grade; }
        bool hasBeenEvaluated() const { return evaluated; }

    private:
        std::vector<double, AlignedAllocator<double, 64>> position;
        std::vector<double, AlignedAllocator<double, 64>> velocityVector;
        std::vector<double, AlignedAllocator<double, 64>> personalBest;
        double grade = std::numeric_limits<double>::max();
        bool evaluated = false;
};
