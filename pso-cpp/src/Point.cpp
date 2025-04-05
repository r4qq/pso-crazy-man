#include "Point.h"
#include <functional>
#include <vector>
#include <algorithm>
#include <cmath>

Point::Point(const std::vector<double>& startPos, const std::vector<double>& startVelocity)
    :
    position(startPos),
    velocityVector(startVelocity),
    personalBest(startPos)
    {}

void Point::evalPoint(const std::function<double(const std::vector<double>&)>& funcToMinimize)
{
    auto currentGrade = funcToMinimize(position);
    evaluated = true;
    
    if (currentGrade < grade) 
    {
        personalBest = position;
        grade = currentGrade;
    }
}

void Point::updatePosition(void)
{
    for(size_t i = 0; i < position.size(); i++)
    {
        position[i] += velocityVector[i];
    }
}

void Point::updateVelocity(float alpha, float beta, float epsilon1, float epsilon2, const std::vector<double>& globalBest)
{
    for(size_t i = 0; i < velocityVector.size(); i++)
    {
        velocityVector[i] += (alpha * epsilon1 * (globalBest[i] - position[i]) + 
                             beta * epsilon2 * (personalBest[i] - position[i]));
    }
}

void Point::enforceBounds(const std::pair<int, int>& bounds)
{
    for(size_t i = 0; i < position.size(); i++)
    {
        position[i] = std::max(static_cast<double>(bounds.first), 
                     std::min(static_cast<double>(bounds.second), position[i]));
    }
}

void Point::clampVelocity(double maxVelocity)
{
    for(size_t i = 0; i < velocityVector.size(); i++)
    {
        velocityVector[i] = std::max(-maxVelocity, std::min(maxVelocity, velocityVector[i]));
    }
}
