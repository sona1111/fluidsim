//
// Created by paul on 1/4/18.
//

#include "Interpolators.h"


/*
 * (not yet used)
 *
 *
 */
double Interpolators::linear_interpolate(double x, int y, gridArray& gridvals) {

    //get the locations of the surrounding grid points where we have actual values
    auto x_1 = (int) floor(x);
    auto x_2 = (int) ceil(x);

    double result = gridvals[x_1][y] + ((gridvals[x_2][y] - gridvals[x_1][y]) / (x_2-x_1)) * (x-x_1);



    return result;
}

/*
 * using https://en.wikipedia.org/wiki/Bilinear_interpolation
 * Define:
 * Q_12 ----- R_2 ----- Q_22
 *      -----  P  -----
 * Q_11 ----- R_1 ----- Q_21
 *
 *
 *
 */
double Interpolators::bilinear_interpolate(double x, double y, gridArray& gridvals) {



    //get the locations of the surrounding grid points where we have actual values
    double x_1 = floor(x);
    double x_2 = x_1 + 1; //(int) ceil(x);
    double y_1 = floor(y);
    double y_2 = y_1 + 1; //(int) ceil(y);


    //huge interpolation
    double result = (1.0 / ((x_2-x_1)*(y_2-y_1))) *
                    (
                            (gridvals[x_1][y_1] * (x_2 - x) * (y_2 - y)) +
                            (gridvals[x_2][y_1] * (x - x_1) * (y_2 - y)) +
                            (gridvals[x_1][y_2] * (x_2 - x) * (y - y_1)) +
                            (gridvals[x_2][y_2] * (x - x_1) * (y - y_1))
                    );

    std::cout << "---" << std::endl;
    std::cout << x_1 << std::endl;
    std::cout << x_2 << std::endl;
    std::cout << y_1 << std::endl;
    std::cout << y_2 << std::endl;
    std::cout << gridvals[x_1][y_1] << std::endl;
    std::cout << gridvals[x_2][y_1] << std::endl;
    std::cout << gridvals[x_1][y_2] << std::endl;
    std::cout << gridvals[x_2][y_2] << std::endl;
    std::cout << result << std::endl;
    std::cout << "---" << std::endl;

    return result;
}