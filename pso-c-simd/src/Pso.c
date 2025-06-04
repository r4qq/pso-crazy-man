#include "Pso.h"
#include "Point.h"

#include "stdio.h"
#include "stdlib.h"
#include "float.h"
#include "time.h"
#include "string.h"

void freePoints(PsoData *data)
{
    for (int i = 0; i < data->pointsAmount; ++i) 
    {
        free(data->points[i]->position);
        free(data->points[i]->velocityVector);
        free(data->points[i]->personalBest);
        free(data->points[i]);
    }
        free(data->points);
}

double getRandomDouble(double min, double max)
{
    return min + ((double)rand() / RAND_MAX * (max - min));
}


bool updateGlobalBest(PsoData* data)
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

    char message[128] = "CPU supports: ";

    #ifdef __AVX512F__
        if (data->pointDimensions % 8 != 0) 
        {
            data->pointDimensions = ((data->pointDimensions + 7) / 8) * 8; // Pad to next multiple of 8
        }
    strcat(message, "AVX-512F");

    #elif defined(__AVX2__)
        if (data->pointDimensions % 4 != 0) 
        {
            data->pointDimensions = ((data->pointDimensions + 3) / 4) * 4; // Pad to next multiple of 4
        }
        strcat(message, "AVX2");
    
    #else
        strcat(message, "No AVX (using scalar operations)");
    #endif

    printf("%s\n", message);
    printf("Vector(?) size: %i\n", data->pointDimensions);

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
            points[i]->position[j] = getRandomDouble(data->bound[0], data->bound[1]);
            points[i]->personalBest[j] = points[i]->position[j];
        }
        points[i]->grade = data->minFunc(points[i]->position);
    }
    return points;
}

outputData* optimize(PsoData* data)
{
    outputData* output = malloc(sizeof(outputData));

    clock_t start, end;
    double cpuTimeUsed;
    bool optimized = updateGlobalBest(data);
    if (data->globalBestPos == NULL) 
        perror("Failed to initialize global best position\n");

    double epsilon1 = 0.0;
    double epsilon2 = 0.0;
    //double minVel = -1 * (data->maxVelocity);
    //double maxVel = data->maxVelocity;
    //double velTab[] = {minVel, maxVel};  
    printf("Starting optimization\n");
    start = clock();

    for (int i = 0; i < data->epoch; ++i) 
    {
        for (int j = 0; j < data->pointsAmount; ++j) 
        {
            epsilon1 = getRandomDouble(0.0, 1.0);
            epsilon2 = getRandomDouble(0.0, 0.1);
            updateVelocity(data->points[j], data->alpha, data->beta, 
                           epsilon1, epsilon2, data->globalBestPos);

            //doubleClamp(data->points[j]->velocityVector, 
            //    velTab, data->pointDimensions);               //clamp velocity

            updatePosition(data->points[j]);
            
            //doubleClamp(data->points[j]->position, 
            //    (double*)data->bound, data->pointDimensions); //clamp position
            
            data->points[j]->grade = data->minFunc(data->points[j]->position);//evaluate point
            
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
    }
    end = clock();

    cpuTimeUsed = ((double)(end - start)) / CLOCKS_PER_SEC;

    output->bestPoint = data->globalBestPos;
    output->bestVal = data->globalBestVal;
    output->duration = cpuTimeUsed;

    return output;
}
