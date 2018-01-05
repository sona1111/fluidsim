//
// Created by paul on 12/15/17.
//

#ifndef FLUIDSIM_CONSTANTS_H
#define FLUIDSIM_CONSTANTS_H

const int NUM_GRID_CELLS_X = 4;
const int NUM_GRID_CELLS_Y = 4;
const int NUM_GRID_CELLS = NUM_GRID_CELLS_X * NUM_GRID_CELLS_Y;
const int NUM_FRAMES = 15;

//typedef std::array<double, NUM_FRAMES*NUM_GRID_CELLS*3> dataFrames;
typedef std::array<std::array<double, NUM_GRID_CELLS_X>, NUM_GRID_CELLS_Y> gridArray;

//struct of live data
struct initData {
    gridArray u_x;
    gridArray u_y;
    gridArray pressure;
};

//struct of data to write out to file
struct dataFrames {
    std::array<double, NUM_FRAMES*NUM_GRID_CELLS> u_x;
    std::array<double, NUM_FRAMES*NUM_GRID_CELLS> u_y;
    std::array<double, NUM_FRAMES*NUM_GRID_CELLS> pressure;
};

#endif //FLUIDSIM_CONSTANTS_H