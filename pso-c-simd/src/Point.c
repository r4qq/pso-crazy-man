#include "math.h"
#include "immintrin.h"
#include "assert.h"

#include "Point.h"

void updatePosition(Point* point)
{
     #ifdef __AVX512F__ 
        for(int i = 0; i < point->tabSize; i += 8)
        {
            __m512d pos = _mm512_loadu_pd(point->position + i);
            __m512d vel = _mm512_loadu_pd(point->velocityVector + i);
            __m512d newPos = _mm512_add_pd(pos, vel);
            _mm512_storeu_pd(point->position + i, newPos);
        }
    #elif defined(__AVX2__)
        for(int i = 0; i < point->tabSize; i += 4)
        {
            __m256d pos = _mm256_loadu_pd(point->position + i);
            __m256d vel = _mm256_loadu_pd(point->velocityVector + i);
            __m256d newPos = _mm256_add_pd(pos, vel);
            _mm256_storeu_pd(point->position + i, newPos);
        }
    #else
        for (int i = 0; i < point->tabSize; ++i) 
        {
            point->position[i] += point->velocityVector[i];
        }
    #endif
}



void updateVelocity(Point* point, float alpha, float beta, float epsilon1, float epsilon2, double *globalBest)
{
    #ifdef __AVX512F__
        __m512d eps1Vec = _mm512_set1_pd(epsilon1);
        __m512d eps2Vec = _mm512_set1_pd(epsilon2);
        __m512d alphaVec = _mm512_set1_pd(alpha);
        __m512d betaVec = _mm512_set1_pd(beta);

        for (size_t i = 0; i < point->tabSize; i += 8)
        {
            __m512d pos = _mm512_loadu_pd(point->position + i);
            __m512d vel = _mm512_loadu_pd(point->velocityVector + i);
            __m512d gBest = _mm512_loadu_pd(globalBest + i);
            __m512d pBest = _mm512_loadu_pd(point->personalBest + i);


            __m512d diffGlobal = _mm512_sub_pd(gBest, pos);
            __m512d diffPersonal = _mm512_sub_pd(pBest, pos);
            __m512d term1 = _mm512_mul_pd(diffGlobal, eps1Vec);
            __m512d term2 = _mm512_mul_pd(diffPersonal, eps2Vec);
            __m512d scaledTerm1 = _mm512_mul_pd(term1, alphaVec);
            __m512d scaledTerm2 = _mm512_mul_pd(term2, betaVec);
            __m512d update = _mm512_add_pd(scaledTerm1, scaledTerm2);
            vel = _mm512_add_pd(vel, update);
            _mm512_storeu_pd(point->velocityVector + i, vel);
        }

    #elif defined(__AVX2__)
        __m256d eps1Vec = _mm256_set1_pd(epsilon1);
        __m256d eps2Vec = _mm256_set1_pd(epsilon2);
        __m256d alphaVec = _mm256_set1_pd(alpha);
        __m256d betaVec = _mm256_set1_pd(beta);

        for (size_t i = 0; i < point->tabSize; i += 4) 
        {
            __m256d pos = _mm256_loadu_pd(point->position + i);
            __m256d vel = _mm256_loadu_pd(point->velocityVector + i);
            __m256d gBest = _mm256_loadu_pd(globalBest + i);
            __m256d pBest = _mm256_loadu_pd(point->personalBest + i);
        
            __m256d diffGlobal = _mm256_sub_pd(gBest, pos);
            __m256d diffPersonal = _mm256_sub_pd(pBest, pos);
            __m256d term1 = _mm256_mul_pd(  diffGlobal, eps1Vec);
            __m256d term2 = _mm256_mul_pd(diffPersonal, eps2Vec);
            __m256d scaledTerm1 = _mm256_mul_pd(term1, alphaVec);
            __m256d scaledTerm2 = _mm256_mul_pd(term2, betaVec);
            __m256d update = _mm256_add_pd(scaledTerm1, scaledTerm2);
            vel = _mm256_add_pd(vel, update);
            _mm256_storeu_pd(point->velocityVector + i, vel);
        } 

    #else
    for (int i = 0; i < point->tabSize; ++i) 
    {
        point->velocityVector[i] += (alpha * epsilon1 * (globalBest[i] - point->position[i]) +
                                     beta * epsilon2 * (point->personalBest[i] - point->position[i]));
    }

    #endif
}

void doubleClamp(double* valVec, double* bounds, int tabSize)
{
    #ifdef __AVX512F__
        __m512d minVec = _mm512_set1_pd(bounds[0]);
        __m512d maxVec = _mm512_set1_pd(bounds[1]);
        
        for (size_t i = 0; i < tabSize; i += 8) 
        {
            __m512d vel = _mm512_loadu_pd(valVec + i);
            vel = _mm512_max_pd(minVec, _mm512_min_pd(maxVec, vel));
            _mm512_storeu_pd(valVec + i, vel);
        }

    #elif defined(__AVX2__)
        __m256d minVec = _mm256_set1_pd(bounds[0]);
        __m256d maxVec = _mm256_set1_pd(bounds[1]);

        for (size_t i = 0; i < tabSize; i += 4) 
        {
            __m256d vel = _mm256_loadu_pd(valVec + i);
            vel = _mm256_max_pd(minVec, _mm256_min_pd(maxVec, vel));
            _mm256_storeu_pd(valVec + i, vel);
        }
    
    #else
        for (int i = 0; i < tabSize; ++i) 
        {
            valVec[i] = fmax(bounds[0], fmin(valVec[i], bounds[1]));
        }
    #endif
}