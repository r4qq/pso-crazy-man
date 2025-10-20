#include "Pso.hpp"

//#include <cmath>
#include <cstddef>
#include <iostream>
#include <tuple>
#include <utility>
#include <vector>

constexpr int EPOCH = 1000;
constexpr int POINTSAMOUNT = 50;
constexpr std::pair<double , double> BOUND = {-100.0, 100.0};
constexpr float ALPHA = 1.5;
constexpr float BETA = 1.5; 
constexpr int POINTDIMENSION = 50; //set to 2 for func1, to 4 for func2, 8 fo func3
constexpr int SAME_GRADE_EPOCHS = EPOCH;
constexpr float INTERTIA = 1.0;
constexpr float MAXVELOCITY = 100.0;

int main()
{

    // auto func1 = [](const std::vector<double>& vars) -> double 
    // {
    //     double x = vars[0];
    //     double y = vars[1];
    //     return std::sin(std::pow(x, 4)) - std::cos(std::pow(y, 2)) 
    //            + 6 * (std::pow(x, 2)) * (std::pow(y, 2));
    // };
/*
        auto func2 = [](const std::vector<double>& vars) -> double 
    {
        double x = vars[0];
        double y = vars[1];
        double z = vars[2];
        double w = vars[3];
        return std::sin(std::pow(x, 4)) - std::cos(std::pow(y, 2)) 
            + std::cos(std::pow(z, 3)) + std::sin(std::pow(w, 2))
            + 6 * (std::pow(x, 2)) * (std::pow(y, 2)) 
            + 4 * (std::pow(z, 2)) * (std::pow(w, 2));
    };
*/
/*
    auto func3 = [](const std::vector<double>& vars) -> double 
    {
        double x1 = vars[0];
        double x2 = vars[1];
        double x3 = vars[2];
        double x4 = vars[3];
        double x5 = vars[4];
        double x6 = vars[5];
        double x7 = vars[6];
        double x8 = vars[7];
        
        return std::sin(std::pow(x1, 4)) +
            std::cos(std::pow(x2, 2)) +
            std::sin(std::pow(x3, 3)) +
            std::cos(std::pow(x4, 2)) +
            4 * std::pow(x5, 2) * std::pow(x6, 2) +
            3 * std::pow(x7, 2) * std::pow(x8, 2) +
            2 * (std::pow(x1, 2) + std::pow(x2, 2) + std::pow(x3, 2) + std::pow(x4, 2));
    };
*/

    auto func4 = [](const std::vector<double>& vars) -> double
    {
        double sum = 0;
        for (size_t i = 0; i < POINTDIMENSION; i++) 
        {
            sum += vars[i] * vars[i]; 
        }
        return sum;
    };

    try 
    {
        auto pso = Pso(ALPHA, BETA, INTERTIA, EPOCH, POINTSAMOUNT, POINTDIMENSION, BOUND, func4, SAME_GRADE_EPOCHS, 0);
        pso.setMaxVelocity(MAXVELOCITY); 
        
        std::tuple<std::vector<double>, double, std::chrono::duration<double>> output = pso.optimize();
        
        std::cout << "****************" << std::endl;
        std::cout << "Best position: " << std::endl;
        for (size_t i = 0; i < POINTDIMENSION; i++) 
        {
            std::cout << std::get<0>(output)[i] << " ";
        }
        std::cout << std::endl;
        std::cout << "Best value: " << std::get<1>(output) << std::endl;
        std::cout << "Time elapsed: " << std::get<2>(output).count() << " s" << std::endl; 
        std::cout << "****************" << std::endl;
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}