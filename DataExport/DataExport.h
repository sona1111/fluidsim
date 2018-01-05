//
// Created by paul on 12/15/17.
//

#include <iostream>
#include <fstream>
#include <array>
#include "../Constants.h"

#ifndef FLUIDSIM_DATAEXPORT_H
#define FLUIDSIM_DATAEXPORT_H




class DataExport {

    //int width, height;

public:
    /// Frames are stored in the following manner:
    /// -each line is one frame
    /// -each frame contains a number of call data separated by ';'
    /// -each cell data is a comma separated list of vx,vy,p , where
    /// vx is the velocity out of the left face, pointing right and
    /// vy is the velocity out of the top face, pointing down
    void writeFrames (dataFrames &);

};


#endif //FLUIDSIM_DATAEXPORT_H
