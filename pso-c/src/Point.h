#pragma once

typedef struct 
{
    double* position;
    double* velocityVector;
    double* personalBestPosition;
    double grade;
    double personalBestGrade;
    int tabSize;
} Point;

void updateVelocity(Point* point, float alpha, float beta, float epsilon1, float epsilon2,
                    double* globalBest);
void updatePosition(Point* point);
void evalPoint(void);
void doubleClamp(double* valVec, double* bounds, int tabSize);