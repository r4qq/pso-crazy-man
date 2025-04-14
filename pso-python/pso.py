import random
import math
import matplotlib.pyplot as plt
import time

from typing import List, Callable

EPOCH = 50
POINTSAMOUNT = 200000
BOUND = [-10.0, 10.0]
ALPHA = 0.5
BETA = 0.3

class Point:
    def __init__(self, position: List[float], velocityVector: List[float]) -> None:
        self.position = position
        self.velocityVector = velocityVector
        self.personalBest = position
        self.grade = float('inf')

    def updateVelocity(self, alpha: float, beta: float, globalBest: List[float]) -> None:
        epsilon1, epsilon2 = random.random(), random.random()
        for i in range(len(self.velocityVector)):
            self.velocityVector[i] += (alpha * epsilon1 * (globalBest[i] - self.position[i]) +
                                        beta * epsilon2 * (self.personalBest[i] - self.position[i]))

    def updatePosition(self) -> None:
        for i in range(len(self.position)):
            self.position[i] += self.velocityVector[i]

    def evalPoint(self, func: Callable) -> None:
        currentGrade = func(self.position)
        if currentGrade < self.grade:  
            self.personalBest = self.position
            self.grade = currentGrade  

class Algo:
    def __init__(self, epoch: int, pointsAmount: int, bound: List[float],
                       alpha: float, beta: float, funcToMinimize: Callable, sameGradepochs: int) -> None:
        self.epoch = epoch
        self.pointsAmount = pointsAmount
        self.bound = bound
        self.alpha = alpha
        self.beta = beta
        self.funcToMinimize = funcToMinimize
        self.sameGradepochs = sameGradepochs
        self.points = self._initPoints()
        self.globalBestPos = None
        self.globalBestVal = float('inf')
        self.bestGradesHistory = []
        self.consecutiveUnchangedEpochs = 0

    def _initPoints(self) -> None:
        points = []
        for _ in range(self.pointsAmount):
            startPos = [random.uniform(self.bound[0], self.bound[1]) for _ in range(2)]
            velocityVector = [random.uniform(-1, 1) for _ in range(2)]
            point = Point(startPos, velocityVector)
            point.grade = self.funcToMinimize(startPos)
            points.append(point)
        return points

    def _updateGlobalBest(self) -> bool:
        tempPoint = min(self.points, key=lambda p: p.grade)
        if tempPoint.grade < self.globalBestVal:
            self.globalBestVal = tempPoint.grade
            self.globalBestPos = tempPoint.position
            return True
        return False

    def optimize(self) -> tuple:
        startTime = time.time()

        for eepoch in range(self.epoch):
            optimized = self._updateGlobalBest()
            self.bestGradesHistory.append(self.globalBestVal)
            for point in self.points:
                point.updateVelocity(self.alpha, self.beta, self.globalBestPos)
                point.updatePosition()
                point.evalPoint(self.funcToMinimize)

            if optimized:
                self.consecutiveUnchangedEpochs = 0
            else:
                self.consecutiveUnchangedEpochs += 1
                if self.consecutiveUnchangedEpochs >= self.sameGradepochs:
                    break
        
        endTime = time.time()
        print(f"Optimization completed in {endTime - startTime:.4f} seconds")
        print(f"in {eepoch} epochs")
        return self.globalBestPos, self.globalBestVal

if __name__ == "__main__":
    def funcToMinimize(vars):
        x, y = vars
        return math.sin(x**4) - math.cos(y**2) + 6 * (x**2) * (y**2)

    pso = Algo(EPOCH, POINTSAMOUNT, BOUND, ALPHA, BETA, funcToMinimize, 15)
    bestPos, bestVal = pso.optimize()

    print("-------------")
    print(bestPos)
    print(bestVal)

    plt.plot(pso.bestGradesHistory)
    plt.xlabel("Epochs")
    plt.ylabel("Grades")
    plt.title("Best Grades Over Epochs")
    plt.show()