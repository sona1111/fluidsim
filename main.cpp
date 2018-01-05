#include <iostream>
#include <array>
#include "DataExport/DataExport.h"
#include "DataImport/DataImport.h"
#include "Constants.h"
#include "Core/Universe.h"



int main() {





    DataExport writer;
    DataImport reader;

    initData init_Data = reader.readState();


    Universe Uni{};


    Uni.setState(init_Data.u_x, init_Data.u_y, init_Data.pressure);

    dataFrames frames = Uni.run(1.0);


    //[NumFrames][NumGridCells][3]
    //std::array<double, NUM_FRAMES*NUM_GRID_CELLS*3> frames;


    //double testval = 1.0;
    //frames.pressure[0*NUM_GRID_CELLS+1] = 5.0;
//    for(auto&& e:frames){
//        e = testval;
//        testval++;
//    }
//    for(auto&& e:frames){
//        std::cout << e << std::endl;
//    }

    writer.writeFrames(frames);

    //Universe Uni{};

    //std::cout << Uni.test() << std::endl;


    std::cout << "Done" << std::endl;
    return 0;
}