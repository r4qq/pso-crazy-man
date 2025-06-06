#include "math.h"

#include "Point.h"

void updatePosition(Point* point)
{
    for (int i = 0; i < point->tabSize; ++i) 
    {
        point->position[i] += point->velocityVector[i];
    }
}

void updateVelocity(Point* point, float alpha, float beta, float epsilon1, float epsilon2, double *globalBest)
{
    for (int i = 0; i < point->tabSize; ++i) 
    {
        point->velocityVector[i] += (alpha * epsilon1 * (globalBest[i] - point->position[i]) +
                                     beta * epsilon2 * (point->personalBest[i] - point->position[i]));
    }
}

void doubleClamp(double* valVec, double* bounds, int tabSize)
{
    for (int i = 0; i < tabSize; ++i) 
    {
        valVec[i] = fmax(bounds[0], fmin(valVec[i], bounds[1]));
    }
}