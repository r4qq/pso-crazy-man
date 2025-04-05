#include "Pso.h"
#include <cmath>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

constexpr int EPOCH = 50;
constexpr int POINTSAMOUNT = 2000;
constexpr std::pair<int, int> BOUND = {-10, 10};
constexpr float ALPHA = 0.5;
constexpr float BETA = 0.3; 
constexpr int POINTDIMENSION = 2;

int main()
{
    auto func = [](const std::vector<int>& vars) -> double 
    {
        double x = vars[0];
        double y = vars[1];
        return std::sin(std::pow(x, 4)) - std::cos(std::pow(y, 2)) + 6 * (std::pow(x, 2)) * (std::pow(y, 2));
    };

    auto pso =  Pso(ALPHA, BETA, EPOCH, POINTSAMOUNT, POINTDIMENSION, BOUND, func, 10, 0);
    std::tuple<std::vector<int>, double, std::chrono::duration<double>> output = pso.optimize();
    
    std::cout << "****************" << std::endl;
    std::cout << "Best position: " << std::endl;
    for(const auto &element : std::get<0>(output))
    {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    std::cout << "Best value: " << std::get<1>(output) << std::endl;
    std::cout << "Time elapsed: " << std::get<2>(output).count() << " s." << std::endl; 
    std::cout << "****************" << std::endl;
    return 0;
}