//
// Created by paul on 12/31/17.
//

#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <vector>
#include "../Constants.h"

#ifndef FLUIDSIM_DATAIMPORT_H
#define FLUIDSIM_DATAIMPORT_H


class DataImport {

public:

    initData readState();

    std::vector<std::string> split(const std::string &text, char sep);
};


#endif //FLUIDSIM_DATAIMPORT_H
