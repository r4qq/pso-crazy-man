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
constexpr int POINTDIMENSION = 2; //set to 2 for func1 or to 4 for func2
constexpr int SAME_GRADE_EPOCHS = 10; 

int main()
{

    auto func1 = [](const std::vector<double>& vars) -> double 
    {
        double x = vars[0];
        double y = vars[1];
        return std::sin(std::pow(x, 4)) - std::cos(std::pow(y, 2)) + 6 * (std::pow(x, 2)) * (std::pow(y, 2));
    };

    
    // auto func2 = [](const std::vector<double>& vars) -> double 
    // {
    //     double x = vars[0];
    //     double y = vars[1];
    //     double z = vars[2];
    //     double w = vars[3];
    //     return std::sin(std::pow(x, 4)) - std::cos(std::pow(y, 2)) 
    //         + std::cos(std::pow(z, 3)) + std::sin(std::pow(w, 2))
    //         + 6 * (std::pow(x, 2)) * (std::pow(y, 2)) 
    //         + 4 * (std::pow(z, 2)) * (std::pow(w, 2));
    // };

    try {
        auto pso = Pso(ALPHA, BETA, EPOCH, POINTSAMOUNT, POINTDIMENSION, BOUND, func1, SAME_GRADE_EPOCHS, 0);
        pso.setMaxVelocity((BOUND.second - BOUND.first) / 5.0); 
        
        std::tuple<std::vector<double>, double, std::chrono::duration<double>> output = pso.optimize();
        
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
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}