//
// Created by paul on 1/4/18.
//

#ifndef FLUIDSIM_INTERPOLATORS_H
#define FLUIDSIM_INTERPOLATORS_H

#include <array>
#include <cmath>
#include <iostream>
#include "../Constants.h"

class Interpolators {

public:

    static double bilinear_interpolate(double x, double y, gridArray &gridvals);

    static double linear_interpolate(double x, int i, char dim, gridArray &gridvals);
};


#endif //FLUIDSIM_INTERPOLATORS_H
