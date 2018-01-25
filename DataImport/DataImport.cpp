//
// Created by paul on 12/31/17.
//



#include "DataImport.h"

initData DataImport::readState() {


    std::ifstream infile("/home/paul/dev/fluidsim/py/initial.dat");

    std::string line;

    initData initialFrame = {0};

    int x = 0;
    int y = 0;


    /*
     * For each line read, we have a,b,c,d , where each line is one grid element
     * a: u_x , b: u_y , c: density ,
     * d: type of voxel where 'e' is empty, 'f' is fluid, 's' is solid
     */

    while (std::getline(infile, line))
    {
        std::istringstream iss(line);
        std::vector<std::string> dataStr = split(line, ',');

        if(x > NUM_GRID_CELLS_X-1){
            x = 0;
            y++;
        }

        initialFrame.u_x[x][y] = std::stod(dataStr[0]);
        initialFrame.u_y[x][y] = std::stod(dataStr[1]);
        initialFrame.density[x][y] = std::stod(dataStr[2]);
        initialFrame.contents[x][y] = dataStr[3][0];

        x++;

    }

    std::cout << "Finished reading File" << std::endl;

    return initialFrame;

}

std::vector<std::string> DataImport::split(const std::string &text, char sep) {
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = text.find(sep, start)) != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }
    tokens.push_back(text.substr(start));
    return tokens;
}