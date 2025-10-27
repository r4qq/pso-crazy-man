import numpy as np
import random
import math
import matplotlib.pyplot as plt
import time

from typing import List, Callable

EPOCH = 50
POINTS_AMOUNT = 200000
BOUND = [-10.0, 10.0]
ALPHA = 0.5
BETA = 0.3
POINT_DIMENSION = 8
INERTIA = 0.7
SAME_GRADE_EPOCHS = 10
MAX_VELOCITY = 10.0

class Point:
    def __init__(self, position: List[float], velocity_vector: List[float]) -> None:
        self.position = position
        self.velocity_vector = velocity_vector
        self.personal_best = position
        self.grade = float('inf')

    def update_velocity(self, alpha: float, beta: float, globalBest: List[float], inertia: float) -> None:
        epsilon1, epsilon2 = random.random(), random.random()
        for i in range(len(self.velocity_vector)):
            self.velocity_vector[i] = self.velocity_vector[i] * inertia + (alpha * epsilon1 * (globalBest[i] - self.position[i]) +
                                        beta * epsilon2 * (self.personal_best[i] - self.position[i]))

    def update_position(self) -> None:
        for i in range(len(self.position)):
            self.position[i] += self.velocity_vector[i]

    def eval_point(self, func: Callable) -> None:
        current_grade = func(self.position)
        if current_grade < self.grade:  
            self.personal_best = self.position
            self.grade = current_grade  

    def clamp_velocity(self, max_velocity: float) -> None:
        self.velocity_vector = [max(-1 * max_velocity, min(max_velocity, self.velocity_vector[i])) for i in range(len(self.velocity_vector))]

class Algo:
    def __init__(self, epoch: int, points_Amount: int, bound: List[float],
                    alpha: float, beta: float, func_to_minimize: Callable, 
                    same_grade_epochs: int, inertia: float, max_velocity: float) -> None:
        self.epoch = epoch
        self.points_Amount = points_Amount
        self.bound = bound
        self.alpha = alpha
        self.beta = beta
        self.func_to_minimize = func_to_minimize
        self.same_grade_epochs = same_grade_epochs
        self.points = self._initPoints()
        self.global_best_pos = None
        self.global_best_val = float('inf')
        self.best_grades_history = []
        self.consecutive_unchanged_epochs = 0
        self.inertia = inertia
        self.max_velocity = max_velocity

    def _initPoints(self) -> None:
        points = []
        for _ in range(self.points_Amount):
            startPos = [random.uniform(self.bound[0], self.bound[1]) for _ in range(POINT_DIMENSION)]
            velocity_vector = [random.uniform(-1, 1) for _ in range(POINT_DIMENSION)]
            point = Point(startPos, velocity_vector)
            point.grade = self.func_to_minimize(startPos)
            points.append(point)
        return points

    def _updateGlobalBest(self) -> bool:
        tempPoint = min(self.points, key=lambda p: p.grade)
        if tempPoint.grade < self.global_best_val:
            self.global_best_val = tempPoint.grade
            self.global_best_pos = tempPoint.position
            return True
        return False

    def optimize(self) -> tuple:
        start_time = time.time()

        for eepoch in range(self.epoch):
            optimized = self._updateGlobalBest()
            self.best_grades_history.append(self.global_best_val)
            for point in self.points:
                point.update_velocity(self.alpha, self.beta, self.global_best_pos, self.inertia)
                point.clamp_velocity(self.max_velocity)
                point.update_position()
                point.eval_point(self.func_to_minimize)

            if optimized:
                self.consecutive_unchanged_epochs = 0
            else:
                self.consecutive_unchanged_epochs += 1
                if self.consecutive_unchanged_epochs >= self.same_grade_epochs:
                    break
        
        end_time = time.time()
        print(f"Optimization completed in {end_time - start_time:.4f} seconds")
        print(f"in {eepoch} epochs")
        return self.global_best_pos, self.global_best_val

if __name__ == "__main__":

    def func_to_minimize(vars):
        x, y = vars
        return math.sin(x**4) - math.cos(y**2) + 6 * (x**2) * (y**2)

    def func_to_minimize2(vars):
        x1, x2, x3, x4, x5, x6, x7, x8 = vars
    
        return (math.sin(math.pow(x1, 4)) + 
               math.cos(math.pow(x2, 2)) + 
               math.sin(math.pow(x3, 3)) + 
               math.cos(math.pow(x4, 2)) + 
               4 * math.pow(x5, 2) * math.pow(x6, 2) + 
               3 * math.pow(x7, 2) * math.pow(x8, 2) +  
               2 * math.pow(x1, 2) + math.pow(x2, 2) +  math.pow(x3, 2) +  math.pow(x4, 2)) 

    pso = Algo(EPOCH, POINTS_AMOUNT, BOUND, ALPHA, BETA, func_to_minimize2, SAME_GRADE_EPOCHS, INERTIA, MAX_VELOCITY)
    best_pos, best_val = pso.optimize()

    print("-------------")
    print(best_pos)
    print(best_val)

    plt.plot(pso.best_grades_history)
    plt.xlabel("Epochs")
    plt.ylabel("Grades")
    plt.title("Best Grades Over Epochs")
    plt.show()