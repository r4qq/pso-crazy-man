#include "stdio.h"
#include "float.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"

#include "Pso.h"
const int EPOCH = 1000;
const int POINTSAMOUNT = 50;
const double BOUND[] = {-1000.0, 1000.0};
const double ALPHA = 2.0;
const double BETA = 1.0; 
const int POINTDIMENSION = 8; //set to 2 for func1, to 4 for func2, 8 fo func3
const int SAME_GRADE_EPOCHS = EPOCH; 
const double INTERTIA  = 0.7;

/*
double func1(const double* vars)
{
    double x1 = vars[0];
    double x2 = vars[1];
    return sin(pow(x1, 4.0)) - cos(pow(x2, 2.0)) + 6 * pow(x1, 2.0) * pow(x2, 2.0);
}

double func2(const double* vars)
{
    double x1 = vars[0];
    double x2 = vars[1];
    double x3 = vars[2];
    double x4 = vars[3];
    return sin(pow(x1, 4.0)) - cos(pow(x2, 2.0)) + cos(pow(x3, 3.0)) + sin(pow(x4, 2.0)) + 6 * pow(x1, 2.0) * pow(x2, 2.0) + 4 * pow(x3, 2.0) * pow(x4, 2.0);
}
*/

double func3(const double* vars)
{
    double x1 = vars[0];
    double x2 = vars[1];
    double x3 = vars[2];
    double x4 = vars[3];
    double x5 = vars[4];
    double x6 = vars[5];
    double x7 = vars[6];
    double x8 = vars[7];
        
    return sin(pow(x1, 4.0)) +
           cos(pow(x2, 2.0)) +
           sin(pow(x3, 3.0)) +
           cos(pow(x4, 2.0)) +
           4 * pow(x5, 2.0) * pow(x6, 2.0) +
           3 * pow(x7, 2.0) * pow(x8, 2.0) +
           2 * (pow(x1, 2.0) + pow(x2, 2.0) + pow(x3, 2.0) + pow(x4, 2.0));
}

int main(void)
{
    srand(time(NULL));
    PsoData* data = malloc(sizeof(PsoData));


    *data = (PsoData){
        .alpha = ALPHA,
        .beta = BETA,
        .epoch = EPOCH,
        .pointsAmount = POINTSAMOUNT,
        .pointDimensions = POINTDIMENSION,
        .bound = BOUND,
        .minFunc = func3,
        .sameGradeEpochs = SAME_GRADE_EPOCHS,
        .points = NULL,
        .globalBestPos = NULL,
        .globalBestVal = DBL_MAX,
        .consecutiveUnchangedEpochs = 0,
        .maxVelocity = 4.0,
        .hasValidSolution = false,
        .intertia = INTERTIA
    };   

    Point** newPoints = initPoints(data);
    data->points = newPoints;
    data->globalBestPos = calloc(data->pointDimensions, sizeof(double));


    outputData output = optimize(data);

    printf("****************\n");
    printf("Best position: \n");
    for (int i = 0; i < POINTDIMENSION; ++i) 
    {
        printf("%f , ", output.bestPoint[i]);
    }
    printf("\n");
    printf("Best value: %f\n", output.bestVal);
    printf("Time elapsed: %f \n", output.duration);
    printf("Epochs optimized: %d\n", output.epochRun); 


    for (int i = 0; i < data->pointsAmount; ++i) 
    {
        free(data->points[i]->position); 
        free(data->points[i]->velocityVector); 
        free(data->points[i]->personalBestPosition); 
        free(data->points[i]); 
    }

    free(data->globalBestPos);
    free(data->points);
    free(data);
    return 0;
}