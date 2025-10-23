#pragma once

#include "math.h"
#include "stdbool.h"
#include "time.h"

#include "Point.h"

typedef double (*FuncToMinimize)(const double*) ;

typedef struct 
{
        double alpha;
        double beta;
        int epoch;
        int pointsAmount;
        int pointDimensions;
        const double* bound;
        FuncToMinimize minFunc;
        int sameGradeEpochs;
        Point** points;
        double* globalBestPos;
        double globalBestVal;
        int consecutiveUnchangedEpochs;
        double maxVelocity; 
        bool hasValidSolution;
        double intertia;
}PsoData;

typedef struct
{
    double* bestPoint;
    double bestVal;
    double duration;  
    int epochRun;
}outputData;


Point** initPoints(PsoData* data);
bool updateBest(PsoData* data);
static inline double getRandomDouble(double min, double max);
outputData optimize(PsoData* data);
void freePsoData(PsoData* data);