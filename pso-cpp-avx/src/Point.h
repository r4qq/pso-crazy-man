#pragma once

#include <functional>
#include <vector>
#include <limits>

class Point 
{
    public:
        Point(const std::vector<double>& position, const std::vector<double>& velocityVector);
        void updateVelocity(const std::vector<double>& alphaVector, const std::vector<double>& betaVector, const std::vector<double>& epsilon1Vector, const std::vector<double>& epsilon2Vector, const std::vector<double>& globalBest);
        void updatePosition(void);
        void evalPoint(const std::function<double(const std::vector<double>&)>& funcToMinimize);
        void enforceBounds(const std::pair<int, int>& bounds);
        void clampVelocity(double maxVelocity);
        
        const std::vector<double>& getPosition() const { return position; }
        const std::vector<double>& getVelocityVector() const { return velocityVector; }
        const std::vector<double>& getPersonalBest() const { return personalBest; }
        double getGrade() const { return grade; }
        bool hasBeenEvaluated() const { return evaluated; }

    private:
        std::vector<double> position;
        std::vector<double> velocityVector;
        std::vector<double> personalBest;
        double grade = std::numeric_limits<double>::max();
        bool evaluated = false;
};
