//
// Created by paul on 12/15/17.
//

#ifndef FLUIDSIM_CONSTANTS_H
#define FLUIDSIM_CONSTANTS_H

const int NUM_GRID_CELLS_X = 7;
const int NUM_GRID_CELLS_Y = 7;
const int NUM_GRID_CELLS = NUM_GRID_CELLS_X * NUM_GRID_CELLS_Y;
const int NUM_FRAMES = 10;

const char OFFGRID_CONTENTS = 'e';

//typedef std::array<double, NUM_FRAMES*NUM_GRID_CELLS*3> dataFrames;
typedef std::array<std::array<double, NUM_GRID_CELLS_X>, NUM_GRID_CELLS_Y> gridArray;
typedef std::array<std::array<char, NUM_GRID_CELLS_X>, NUM_GRID_CELLS_Y> gridArray_char;

//struct of live data
struct initData {
    gridArray u_x;
    gridArray u_y;
    gridArray pressure;
    gridArray_char contents;
};

//struct of data to write out to file
struct dataFrames {
    std::array<double, NUM_FRAMES*NUM_GRID_CELLS> u_x;
    std::array<double, NUM_FRAMES*NUM_GRID_CELLS> u_y;
    std::array<double, NUM_FRAMES*NUM_GRID_CELLS> pressure;
    std::array<char, NUM_FRAMES*NUM_GRID_CELLS> contents;
};

#endif //FLUIDSIM_CONSTANTS_H
