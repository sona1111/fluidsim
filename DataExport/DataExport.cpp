//
// Created by paul on 12/15/17.
//

#include "DataExport.h"


void DataExport::writeFrames(dataFrames &frames) {

//    for(auto&& e:frames){
//        std::cout << e << std::endl;
//    }


    std::ofstream ofs;
    ofs.open ("/home/paul/dev/fluidsim/py/frames.dat", std::ofstream::out);

    for(int frame=0; frame<NUM_FRAMES; frame++){
        for(int cell=0; cell<NUM_GRID_CELLS; cell++){

            ofs << std::to_string(frames.u_x[frame*NUM_GRID_CELLS+cell]);
            ofs << ",";
            ofs << std::to_string(frames.u_y[frame*NUM_GRID_CELLS+cell]);
            ofs << ",";
            ofs << std::to_string(frames.density[frame*NUM_GRID_CELLS+cell]);
            ofs << ",";
            ofs << frames.contents[frame*NUM_GRID_CELLS+cell];


//            for(int data=0; data<3; data++){
//                ofs << std::to_string(frames[frame*NUM_GRID_CELLS*3+cell*3+data]);
//                if(data != 2){
//                    ofs << ",";
//                }
//            }
            if(cell != NUM_GRID_CELLS-1){
                ofs << ";";
            }
        }
        ofs << "\n";
    }

    ofs.close();

    std::cout << "Wrote File" << std::endl;


}