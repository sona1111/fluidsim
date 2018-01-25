//
// Created by paul on 12/21/17.
//



#include "Universe.h"

/*
 * Looping logic...
 */
dataFrames Universe::run(double timeStepSize){

    //one huge array for output, going to replace with struct at some point
    //for each point you should write xvelocity, yvelocity, and density data in that order
    dataFrames frames = {0};

    gridArray buffer_swap_tmp; //holds the pointer while buffers are being flipped

    int cell = 0;
    for(int timeStep=0; timeStep<NUM_FRAMES; timeStep++){

        std::cout << "---------------------FRAME------------------------" << std::endl;

        cell = 0;
        for(int i=0; i<NUM_GRID_CELLS_X; i++){
            for(int j=0; j<NUM_GRID_CELLS_Y; j++){

                //saving to frame stack
                frames.u_x[timeStep*NUM_GRID_CELLS+cell] = u_x[i][j];

                frames.u_y[timeStep*NUM_GRID_CELLS+cell] = u_y[i][j];
                frames.density[timeStep*NUM_GRID_CELLS+cell] = density[i][j];
                frames.contents[timeStep*NUM_GRID_CELLS+cell] = contents[i][j];
                cell++;

                //advect the density
                //printArr(density);
                density_buffer[i][j] = advect_q(i, j, density, timeStepSize);

                //advect the velocity
                std::tie(u_x_buffer_a[i][j], u_y_buffer_a[i][j]) = advect_velocity(i, j, timeStepSize);

            }
        }
//        for(int i=0; i<NUM_GRID_CELLS_X-1; i++){
//            //update all bottom row stored velocities
//            u_x_buffer[i][0] = update_velocity_edge(i, 0, timeStepSize);
//        }
//        for(int j=0; j<NUM_GRID_CELLS_Y-1; j++){
//            //update all bottom row stored velocities
//            u_y_buffer[0][j] = update_velocity_edge(0, j, timeStepSize);
//        }
        for(int i=0; i<NUM_GRID_CELLS_X-1; i++){
            for(int j=0; j<NUM_GRID_CELLS_Y-1; j++){

                    //projection step

                    null_project(i, j, timeStepSize);
                    //update_velocity2(i, j, timeStepSize);

            }
        }

        //flip the buffers


        buffer_swap_tmp = density;
        density = density_buffer;
        density_buffer = buffer_swap_tmp;


        // current velocity points to buffer_b (the final buffer)
        // buffer_b points to buffer_a
        // buffer_a points to old velocity
        buffer_swap_tmp = u_x;
        u_x = u_x_buffer_b;
        u_x_buffer_b = u_x_buffer_a;
        u_x_buffer_a = buffer_swap_tmp;

        buffer_swap_tmp = u_y;
        u_y = u_y_buffer_b;
        u_y_buffer_b = u_x_buffer_a;
        u_y_buffer_a = buffer_swap_tmp;

    }

    return frames;
}

/*
 * Set the value of all elements representing the entire system, usually done prior to beginning the simulation
 */
void Universe::setState(gridArray u_x_init, gridArray u_y_init, gridArray pressure_init, gridArray_char contents_init){

    u_x = u_x_init;
    u_y = u_y_init;
    density = pressure_init;
    contents = contents_init;
    u_x_buffer_a = u_x_init;
    u_y_buffer_a = u_y_init;
    u_x_buffer_b = u_x_init;
    u_y_buffer_b = u_y_init;
    density_buffer = pressure_init;

}

/*
 * Get the velocity vector at the point where the density / T values are usually stored (center of grid square)
 * This boils down to simple averaging as described by the first of three formulas on page 24
 */
std::pair<double, double> Universe::velocity_at_center(int x, int y){

    double u_x_avg = (u_x[x][y] + u_x[x+1][y]) / 2.0;
    double u_y_avg = (u_y[x][y] + u_y[x][y+1]) / 2.0;

    return std::make_pair(u_x_avg, u_y_avg);

    //can use in caller
    //double x, y;
    //std::tie(x,y) = foo();

};

/*
 * Get the velocity vector at the vertical face between x and x+1 at halfway up (u_{i+1/2,j})
 * This boils down to simple averaging as described by the second of three formulas on page 24
 */
std::pair<double, double> Universe::velocity_at_vert_face(int x, int y){

    double u_x_avg = u_x[x+1][y];
    double u_y_avg = (u_y[x][y] + u_y[x][y+1] + u_y[x+1][y] + u_y[x+1][y+1]) / 4.0;

    return std::make_pair(u_x_avg, u_y_avg);
};

/*
 * Get the velocity vector at the horizontal face between y and y+1 at halfway across (u_{i,j+1/2})
 * This boils down to simple averaging as described by the second of three formulas on page 24
 */
std::pair<double, double> Universe::velocity_at_horiz_face(int x, int y){

    double u_x_avg = (u_x[x][y] + u_x[x][y+1] + u_x[x+1][y] + u_x[x+1][y+1]) / 4.0;
    double u_y_avg = u_y[x+1][y];

    return std::make_pair(u_x_avg, u_y_avg);
};

void Universe::null_project(int x, int y, double delta_t){
    u_x_buffer_b[x][y] = u_x_buffer_a[x][y];
    u_y_buffer_b[x][y] = u_y_buffer_a[x][y];
}

/*
 * Update velocity due to density as described in page 71 - 5.1
 */
std::pair<double, double> Universe::update_velocity2(int x, int y, double delta_t){

    double scale = delta_t / 1.0; //<< 1.0 = density * dx
    double u_x_tmp = u_x_buffer_a[x][y];
    double u_y_tmp;

    u_x_buffer_b[x][y] = u_x_buffer_a[x][y];
    u_y_buffer_b[x][y] = u_y_buffer_a[x][y];

    if(contents[x-1][y] == 'F' || contents[x][y] == 'F'){
        if(contents[x-1][y] == 'S' || contents[x][y] == 'S'){
            // do calculation for solid interaction
            //u_x[x][y] =
            u_x_buffer_b[x][y] = 0;
        }else{
            //u_x_buffer_b[x][y] -= scale * (density[x][y] - density[x-1][y]);
        }
    }else{
        // mark actual velocity as unknown (here fixed zero so far)
        u_x_buffer_b[x][y] = 0;
    }
    if(contents[x][y-1] == 'F' || contents[x][y] == 'F'){
        if(contents[x][y-1] == 'S' || contents[x][y] == 'S'){
            // do calculation for solid interaction
            u_y_buffer_b[x][y] = 0;
        }else{
            //u_y_buffer_b[x][y] -= scale * (density[x][y] - density[x][y-1]);
        }
    }else{
        // mark actual velocity as unknown (here fixed zero so far)
        u_x_buffer_b[x][y] = 0;
    }



    return std::make_pair(u_x_tmp, u_y_tmp);
}

/*
 * Update velocity due to density as described in page 69 - 5.1
 * I have modified it slightly because my velocities seem to run from i-1/2 until i_max-1/2 instead of +1/2
 */
std::pair<double, double> Universe::update_velocity(int x, int y, double delta_t){

    //along the perimeter, we need to update velocity based on whatever we define the boundary as
    //here we will assume that it is empty
    //it would be more efficient to do this in a separate loop, rather than if statements

    // for both of these cases, the first 1/1 should be 1/rho (when density introduced)
    // the second /1 should be the /<distance between grid points> , once scaling is introduced
    // here we update u_{i+1/2,j}, then u_{i,j+1/2}
    double u_x_tmp = u_x[x+1][y] - delta_t * (1/1) * ( (density[x+1][y] - density[x][y]) / 1);
    double u_y_tmp = u_y[x][y+1] - delta_t * (1/1) * ( (density[x][y+1] - density[x][y]) / 1);

    return std::make_pair(u_x_tmp, u_y_tmp);
}

double Universe::update_velocity_edge(int x, int y, double delta_t){

    double u_tmp;
    if(x == 0){
        //update u_{i-1/2,j}
        //density[0][y] - 0 for empty
        u_tmp = u_x[0][y] - delta_t * (1/1) * ( (density[0][y] - 0) / 1);
    }else{
        //update v_{i,j-1/2}
        u_tmp = u_y[x][0] - delta_t * (1/1) * ( (density[x][0] - 0) / 1);
    }

    return u_tmp;
}

std::pair<double, double> Universe::advect_velocity(int x_g_x, int x_g_y, double delta_t) {

    //step one step of 'forward euler' backward in time to find x_p from x_g
    //we will start by using second order runge kutta and upgrade later
    //I will try this by separating x and y calculations while holding the other constant

    std::cout << "Advecting final position " << x_g_x << " " << x_g_y << std::endl;
    std::cout << "With fluid velocity " << u_x[x_g_x][x_g_y] << " " << u_y[x_g_x][x_g_y] << std::endl;


    //double x_mid_x = x_g_x - 0.5 * delta_t * u_x[x_g_x][x_g_y]; //midpoint
    //double x_mid_y = x_g_y - 0.5 * delta_t * u_y[x_g_x][x_g_y];

    //double vcx, vcy;
    //std::tie(vcx, vcy) = velocity_at_center(x_g_x, x_g_y);



//    double x_mid_x;
//    double x_mid_y;

//    if(stagger){
//        x_mid_x = (x_g_x + 0.5) - 0.5 * delta_t * Interpolators::linear_interpolate(x_g_x + 0.5, x_g_y, 'x', u_x); //midpoint
//        x_mid_y = (x_g_y + 0.5) - 0.5 * delta_t * Interpolators::linear_interpolate(x_g_y + 0.5, x_g_x, 'y', u_y);
//    }else{
//        x_mid_x = (x_g_x) - 0.5 * delta_t * Interpolators::linear_interpolate(x_g_x, x_g_y, 'x', u_x); //midpoint
//        x_mid_y = (x_g_y) - 0.5 * delta_t * Interpolators::linear_interpolate(x_g_y, x_g_x, 'y', u_y);
//    }

    //std::cout << "Found midpoints " << x_mid_x << " " << x_mid_y << std::endl;



//    double x_p_x = x_g_x - delta_t * Interpolators::bilinear_interpolate(x_mid_x, x_mid_y, u_x); //final result
//    double x_p_y = x_g_y - delta_t * Interpolators::bilinear_interpolate(x_mid_x, x_mid_y, u_y);

//    double x_p_x = (x_g_x) - delta_t * Interpolators::linear_interpolate(x_mid_x, x_g_y, 'x', u_x); //final result
//    double x_p_y = (x_g_y) - delta_t * Interpolators::linear_interpolate(x_mid_y, x_g_x, 'y', u_y);


    // get simple average of velocities to get a velocity vector at the bottom left MAC corner
    double vcx, vcy;

//    if(x_g_x == 0){ //simple boundary conditions so far
//        vcx = 0;
//    }else{
//        vcx = (u_x[x_g_x][x_g_y - 1] + u_x[x_g_x][x_g_y]) / 2.0;
//    }
//    if(x_g_y == 0){
//        vcy = 0;
//    }else{
//        vcy = (u_y[x_g_x - 1][x_g_y] + u_y[x_g_x][x_g_y]) / 2.0;
//    }
    vcx = x_g_x;
    vcy = x_g_y;


    std::cout << "Determined velocity at bottom left MAC to be " << vcx << " " << vcy << std::endl;

    // find the point where the new velocity should be coming from is
    double x_p_x = (x_g_x) - delta_t * u_x[vcx][vcy];
    double x_p_y = (x_g_y) - delta_t * u_y[vcx][vcy];

    std::cout << "Found initial position " << x_p_x << " " << x_p_y << std::endl;

    if(x_p_x < 0 || x_p_y < 0 || x_p_x >= NUM_GRID_CELLS_X || x_p_y >= NUM_GRID_CELLS_Y){
        std::cout << "Initial position off grid, assuming zero" << std::endl;
        return std::make_pair(0.0, 0.0);
    }

    // the first interpolation above gives us a good location of the particle at the last time step
    // we now need to interpolate again in order to find the quantity q at the last time step, as it is likely off-grid

    double velocity_new_x = Interpolators::bilinear_interpolate(x_p_x, x_p_y, u_x);
    double velocity_new_y = Interpolators::bilinear_interpolate(x_p_x, x_p_y, u_y);

    std::cout << "Which contains last frame density " << velocity_new_x << " " << velocity_new_y << std::endl;

    // undo the average value we took earlier to get the new velocity value back on the
    // correct staggered position. Again we must take into account the boundaries
//    if(x_g_x == 0){
//        velocity_new_x = 2 * velocity_new_x;
//    }else{
//        velocity_new_x = (2 * velocity_new_x) - u_x[x_g_x][x_g_y - 1];
//    }
//    if(x_g_y == 0){
//        velocity_new_y = 2 * velocity_new_y;
//    }else{
//        velocity_new_y = (2 * velocity_new_y) - u_y[x_g_x - 1][x_g_y];
//    }

    std::cout << "reverse average " << velocity_new_x << " " << velocity_new_y << std::endl;

    return std::make_pair(velocity_new_x, velocity_new_y);
}

/*
 * location in space of grid point we are looking for: x_g
 * new value of q at that point (return): q_g^{n+1} or q_g_new = q_g_old also
 * location in space of gridpoint of particle one timestep ago: x_p
 * x_g is a exact known value at a grid point (not sure to use on the grid lines or middle of square ;
 * in the book, it is described that values which are advected (ex density) are in the middle of the squares,
 * but in the diagram for advection 3.1 it is shown that the velocity is determined on the grid lines...)
 * x_p is calculated, and will not generally be at an exact grid point
 *
 * this function will advect the quantity in gridvals (density, for example)
 * of a single virtual particle at location x, y
 */
double Universe::advect_q(int x_g_x, int x_g_y, gridArray& gridvals, double delta_t) {

    //step one step of 'forward euler' backward in time to find x_p from x_g
    //we will start by using second order runge kutta and upgrade later
    //I will try this by separating x and y calculations while holding the other constant

    std::cout << "Advecting final position " << x_g_x << " " << x_g_y << std::endl;
    std::cout << "With fluid velocity " << u_x[x_g_x][x_g_y] << " " << u_y[x_g_x][x_g_y] << std::endl;


    //double x_mid_x = x_g_x - 0.5 * delta_t * u_x[x_g_x][x_g_y]; //midpoint
    //double x_mid_y = x_g_y - 0.5 * delta_t * u_y[x_g_x][x_g_y];

    //double vcx, vcy;
    //std::tie(vcx, vcy) = velocity_at_center(x_g_x, x_g_y);

    if(x_g_x + 0.5 >= NUM_GRID_CELLS_X-1 || x_g_y + 0.5 >= NUM_GRID_CELLS_Y-1){
        std::cout << "Averaging would take place off grid, ignoring" << std::endl;
        return 0.0;
    }

    double x_mid_x;
    double x_mid_y;


    x_mid_x = (x_g_x) - 0.5 * delta_t * Interpolators::linear_interpolate(x_g_x, x_g_y, 'x', u_x); //midpoint
    x_mid_y = (x_g_y) - 0.5 * delta_t * Interpolators::linear_interpolate(x_g_y, x_g_x, 'y', u_y);




    std::cout << "Found midpoints " << x_mid_x << " " << x_mid_y << std::endl;



//    double x_p_x = x_g_x - delta_t * Interpolators::bilinear_interpolate(x_mid_x, x_mid_y, u_x); //final result
//    double x_p_y = x_g_y - delta_t * Interpolators::bilinear_interpolate(x_mid_x, x_mid_y, u_y);

    double x_p_x = (x_g_x) - delta_t * Interpolators::linear_interpolate(x_mid_x, x_g_y, 'x', u_x); //final result
    double x_p_y = (x_g_y) - delta_t * Interpolators::linear_interpolate(x_mid_y, x_g_x, 'y', u_y);



    std::cout << "Found initial position " << x_p_x << " " << x_p_y << std::endl;

    if(x_p_x < 0 || x_p_y < 0 || x_p_x >= NUM_GRID_CELLS_X || x_p_y >= NUM_GRID_CELLS_Y){
        std::cout << "Initial position off grid, assuming zero" << std::endl;
        return 0.0;
    }



    // the first interpolation above gives us a good location of the particle at the last time step
    // we now need to interpolate again in order to find the quantity q at the last time step, as it is likely off-grid

    double q_n = Interpolators::bilinear_interpolate(x_p_x, x_p_y, gridvals); //Pressure update if gridvals not staggered?

    std::cout << "Which contains last frame density " << q_n << std::endl;

    // relative to the density grid, the velocity grid is stepped 1/2 unit down and 1/2 unit left
    // so when doing the interpolation for density we use a position that is moved +1/2, +1/2 (no grid scaling yet)
    //double q_n = Interpolators::bilinear_interpolate(x_p_x + 0.5, x_p_y + 0.5, gridvals);



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
