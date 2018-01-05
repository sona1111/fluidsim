//
// Created by paul on 12/21/17.
//


#include "Universe.h"

/*
 * Looping logic...
 */
dataFrames Universe::run(double timeStepSize){

    //one huge array for output, going to replace with struct at some point
    //for each point you should write xvelocity, yvelocity, and pressure data in that order
    dataFrames frames = {0};



    int cell = 0;
    for(int timeStep=0; timeStep<NUM_FRAMES; timeStep++){
        cell = 0;
        for(int i=0; i<NUM_GRID_CELLS_X; i++){
            for(int j=0; j<NUM_GRID_CELLS_Y; j++){

                //saving to frame stack
                frames.u_x[timeStep*NUM_GRID_CELLS+cell] = u_x[i][j];

                frames.u_y[timeStep*NUM_GRID_CELLS+cell] = u_y[i][j];
                frames.pressure[timeStep*NUM_GRID_CELLS+cell] = pressure[i][j];
                cell++;

                //advect the pressure
                printArr(pressure);
                pressure[i][j] = advect(i, j, pressure, timeStepSize);



                //u_y[i][j] = cell*timeStep;

            }
        }
    }

    return frames;
}

/*
 * Set the value of all elements representing the entire system, usually done prior to beginning the simulation
 */
void Universe::setState(gridArray u_x_init, gridArray u_y_init, gridArray pressure_init){

    u_x = u_x_init;
    u_y = u_y_init;
    pressure = pressure_init;

}

/*
 * location in space of grid point we are looking for: x_g
 * new value of q at that point (return): q_g^{n+1} or q_g_new = q_g_old also
 * location in space of gridpoint of particle one timestep ago: x_p
 * x_g is a exact known value at a grid point (not sure to use on the grid lines or middle of square)
 * x_p is calculated, and will not generally be at an exact grid point
 *
 * this function will advect the quantity in gridvals (pressure, for example)
 * of a single virtual particle at location x, y
 */
double Universe::advect(int x_g_x, int x_g_y, gridArray& gridvals, double delta_t) {




    //step one step of 'forward euler' backward in time to find x_p from x_g
    //we will start by using second order runge kutta and upgrade later
    //I will try this by separating x and y calculations while holding the other constant

    std::cout << "Advecting final position " << x_g_x << " " << x_g_y << std::endl;
    std::cout << "With fluid velocity " << u_x[x_g_x][x_g_y] << " " << u_y[x_g_x][x_g_y] << std::endl;


    double x_mid_x = x_g_x - 0.5 * delta_t * u_x[x_g_x][x_g_y]; //midpoint
    double x_mid_y = x_g_y - 0.5 * delta_t * u_y[x_g_x][x_g_y];

    //std::cout << x_mid_x << std::endl;
    //std::cout << x_mid_y << std::endl;

    double x_p_x = x_g_x - delta_t * Interpolators::bilinear_interpolate(x_mid_x, x_mid_y, u_x); //final result
    double x_p_y = x_g_y - delta_t * Interpolators::bilinear_interpolate(x_mid_x, x_mid_y, u_y);

    std::cout << "Found initial position " << x_p_x << " " << x_p_y << std::endl;

    if(x_p_x < 0 || x_p_y < 0 || x_p_x >= NUM_GRID_CELLS_X || x_p_y >= NUM_GRID_CELLS_Y){
        std::cout << "Initial position off grid, ignoring for now" << std::endl;
        return 0.0;
    }

    // the first interpolation above gives us a good location of the particle at the last time step
    // we now need to interpolate again in order to find the quantity q at the last time step, as it is likely off-grid

    double q_n = Interpolators::bilinear_interpolate(x_p_x, x_p_y, gridvals);

    return q_n;
}



double Universe::test() {

    u_x = {{{5, 8, 2, 1}, {8, 3, 1, 1}, {5, 3, 9, 1}, {5, 3, 9, 1}}};



    double result = Interpolators::bilinear_interpolate(2.6, 3.8, u_x);

    return result;

}

void Universe::printArr(gridArray& gridVals){
    for (int i = 0; i < NUM_GRID_CELLS_X; ++i)
    {
        for (int j = 0; j < NUM_GRID_CELLS_Y; ++j)
        {
            std::cout << gridVals[i][j] << ' ';
        }
        std::cout << std::endl;
    }
}
