#include "stdio.h"
#include "stdlib.h"
#include "float.h"
#include "time.h"

#include "Pso.h"
#include "Point.h"

static inline double getRandomDouble(double min, double max)
{
    return min + ((double)rand() / RAND_MAX * (max - min));
}


static inline bool updateGlobalBest(PsoData* data)
{
    for (int i = 0; i < data->pointsAmount; ++i) 
    {
        if (data->points[i]->grade < data->globalBestVal) 
        {
            data->globalBestPos = data->points[i]->position;
            data->globalBestVal = data->points[i]->grade;
            data->hasValidSolution = true;
            return true;
        }
    }
    return false;
}

Point** initPoints(PsoData* data)
{
    Point** points = malloc(sizeof(Point*) * data->pointsAmount);
    if (!points) 
    {
        perror("Points array malloc failed");
        exit(1);
    }

    for (int i = 0; i < data->pointsAmount; ++i)
    {
        points[i] = malloc(sizeof(Point));
        if (!points[i]) 
        {
            perror("Point malloc failed");
            exit(1);
        }

        points[i]->position = calloc(data->pointDimensions, sizeof(double));
        points[i]->velocityVector = calloc(data->pointDimensions, sizeof(double));
        points[i]->personalBest = calloc(data->pointDimensions, sizeof(double));
        points[i]->tabSize = data->pointDimensions;
        
        for (int j = 0; j < data->pointDimensions; ++j)
        {
            points[i]->position[j] = getRandomDouble(-1*((data->maxVelocity)/2.0), ((data->maxVelocity)/2.0));
            points[i]->personalBest[j] = points[i]->position[j];
        }
        points[i]->grade = data->minFunc(points[i]->position);
    }
    return points;
}

outputData optimize(PsoData* data)
{
    outputData output = {0};

    clock_t start, end;
    double cpuTimeUsed;
    int tempEpochRun = 0;
    bool optimized = updateGlobalBest(data);
    if (data->globalBestPos == NULL)
    { 
        perror("Failed to initialize global best position\n");
        exit(1);
    }
    double epsilon1 = 0.0;
    double epsilon2 = 0.0;
    const double velBound[] = {-(data->maxVelocity), data->maxVelocity}; 
    printf("Starting optimization\n");
    start = clock();

    for (int i = 0; i < data->epoch; ++i) 
    {
        for (int j = 0; j < data->pointsAmount; ++j) 
        {
            epsilon1 = getRandomDouble(0.0, 1.0);
            epsilon2 = getRandomDouble(0.0, 1.0);
            updateVelocity(data->points[j], data->alpha, data->beta, 
                           epsilon1, epsilon2, data->globalBestPos);
            doubleClamp(data->points[j]->velocityVector, (double*)velBound, data->pointDimensions); //clamp velocity
            updatePosition(data->points[j]);
            doubleClamp(data->points[j]->position, (double*)data->bound, data->pointDimensions);    //enforce bounds
            data->points[j]->grade = data->minFunc(data->points[j]->position);                      //evaluate point
        }
        optimized = updateGlobalBest(data);
        if(optimized)
        {
            data->consecutiveUnchangedEpochs = 0;
        }
        else 
        {
            data->consecutiveUnchangedEpochs += 1;
            if (data->consecutiveUnchangedEpochs >= data->sameGradeEpochs) 
            {
                break;
            }
        }
        tempEpochRun++;
    }
    end = clock();

    cpuTimeUsed = ((double)(end - start)) / CLOCKS_PER_SEC;

    output.bestPoint = data->globalBestPos;
    output.bestVal = data->globalBestVal;
    output.duration = cpuTimeUsed;
    output.epochRun = tempEpochRun;

    return output;
}
