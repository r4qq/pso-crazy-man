#include "Point.h"
#include "avxoperations.h"

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
    avxVectorAdd(position, velocityVector, position);
}

void Point::updateVelocity(const std::vector<double>& alphaVector, const std::vector<double>& betaVector, const std::vector<double>& epsilon1Vector, const std::vector<double>& epsilon2Vector, const std::vector<double>& globalBest)
{
    std::vector<double> tempVector(position.size());
    

    avxVectorSub(globalBest, position, tempVector);
    avxVectorMul(tempVector, epsilon1Vector, tempVector);
    avxVectorMul(tempVector, alphaVector, tempVector);
    avxVectorAdd(velocityVector, tempVector, velocityVector);

    avxVectorSub(personalBest, position, tempVector);
    avxVectorMul(tempVector, epsilon2Vector, tempVector);
    avxVectorMul(tempVector, betaVector, tempVector);
    avxVectorAdd(velocityVector, tempVector, velocityVector);
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
