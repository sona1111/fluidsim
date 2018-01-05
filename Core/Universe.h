//
// Created by paul on 12/21/17.
//

#ifndef FLUIDSIM_UNIVERSE_H
#define FLUIDSIM_UNIVERSE_H

#include <array>
#include <cmath>
#include <iostream>
#include "../Constants.h"
#include "Interpolators.h"



/*
 * Variable definition convention
 * Dx/Dt - material derivative, like regular derivative but for one mobile piece of the fluid
 *
 * q(t,x) - q may be any quantity for a particle at x,t (density, velocity, temperature)
 * Dq/Dt = 0 ; the particle is moving around, but not changing in q
 *
 * vec{u} = velocity field, for example if vec{u} = 10 (1D), everywhere the fluid moves
 * 10 m/s to the right
 *
 *
 */

class Universe {

private:

    gridArray pressure; // = {{{5, 8, 2}, {8, 3, 1}, {5, 3, 9}}};
    gridArray u_x;
    gridArray u_y;
    gridArray pressure_buffer; // = {{{5, 8, 2}, {8, 3, 1}, {5, 3, 9}}};
    gridArray u_x_buffer;
    gridArray u_y_buffer;

public:

    double advect(int x_g_x, int x_g_y, gridArray& gridvals, double delta_t);

    double test();

    dataFrames run(double timeStepSize);

    void setState(gridArray u_x_init, gridArray u_y_init, gridArray pressure_init);

    void printArr(gridArray &gridVals);
};


#endif //FLUIDSIM_UNIVERSE_H
