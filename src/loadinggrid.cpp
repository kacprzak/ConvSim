// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "loadinggrid.h"

#include <fstream>

LoadingGrid::LoadingGrid(const std::string &file)
    : m_last_n(0)
{
    loadData(file);
}

void LoadingGrid::loadData(const std::string &file)
{
    std::string tmp;
    double input;
    std::ifstream ifs(file);

    if (ifs.is_open() == true) {
        getline(ifs, tmp); // ignore first line
        while (ifs.good()) {
          // getline(f, tmp);
          // if (!tmp.empty()) {
          //    std::string ignore;
          //    tmp
          //  }
          ifs >> tmp;
          ifs >> tmp;
          ifs >> input;
          m_nadawa.push_back(input);        
        }
        ifs.close();
    } else {
        std::cerr << "Error: unable to open " << file << std::endl;
    }
}

