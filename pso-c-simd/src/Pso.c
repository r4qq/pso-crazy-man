#include "Pso.h"
#include "Point.h"

#include "stdio.h"
#include "stdlib.h"
#include "float.h"
#include "time.h"
#include "string.h"

void freePsoData(PsoData* data)
{
    if (data->points) 
    {
        for (int i = 0; i < data->pointsAmount; ++i) 
        {
            if (data->points[i]) {
                free(data->points[i]->position); 
                free(data->points[i]->velocityVector); 
                free(data->points[i]->personalBestPosition); 
                free(data->points[i]); 
            }
        }
        free(data->points);
    }
    free(data->globalBestPos);
    free(data);
}

double getRandomDouble(double min, double max)
{
    return min + ((double)rand() / RAND_MAX * (max - min));
}


bool updateBest(PsoData* data)
{
    bool improved = false;
    for (int i = 0; i < data->pointsAmount; ++i) 
    {
        //personal
        if (data->points[i]->grade < data->points[i]->personalBestGrade) 
        {
            memcpy(data->points[i]->personalBestPosition, data->points[i]->position, 
                   data->pointDimensions * sizeof(double_t));

            data->points[i]->personalBestGrade = data->points[i]->grade;
        }

        // global
        if (data->points[i]->grade < data->globalBestVal) 
        {
            memcpy(data->globalBestPos, data->points[i]->position, 
                   data->pointDimensions * sizeof(double));

            data->globalBestVal = data->points[i]->grade;
            data->hasValidSolution = true;
            improved = true;
        }
    }
    return improved;
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
        points[i]->personalBestPosition = calloc(data->pointDimensions, sizeof(double));
        points[i]->tabSize = data->pointDimensions;
        
        for (int j = 0; j < data->pointDimensions; ++j)
        {
            points[i]->position[j] = getRandomDouble(-1*((data->maxVelocity)/2.0), 
                                                        ((data->maxVelocity)/2.0));
            points[i]->personalBestPosition[j] = points[i]->position[j];
        }

        points[i]->grade = data->minFunc(points[i]->position);
        points[i]->personalBestGrade = points[i]->grade;
    }
    return points;
}

outputData optimize(PsoData* data)
{
    outputData output = {0};

    clock_t start, end;
    double cpuTimeUsed;
    int tempEpochRun = 0;

    bool optimized = updateBest(data);
    
    if (data->globalBestPos == NULL) 
        perror("Failed to initialize global best position\n");

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
                           epsilon1, epsilon2, data->globalBestPos, data->intertia);

            doubleClamp(data->points[j]->velocityVector, 
                (double*)velBound, data->pointDimensions);               //clamp velocity

            updatePosition(data->points[j]);
            
            doubleClamp(data->points[j]->position, 
                (double*)data->bound, data->pointDimensions); //clamp position
            
            data->points[j]->grade = data->minFunc(data->points[j]->position);//evaluate point
            
        }
        optimized = updateBest(data);
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
