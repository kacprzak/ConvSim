// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "loadinggrid.h"

#include <fstream>

LoadingGrid::LoadingGrid()
    : m_dataDelta(1)
{
}

double LoadingGrid::getNextValue(unsigned int t)
{
    if (m_nadawa.empty())
        return 0.0;

#if 1
    unsigned int current_n = (t / m_dataDelta) % m_nadawa.size();
    return m_nadawa[current_n] / 1000.0; // kg/1000
#else
    return 160.0/3600.0*4;
#endif
}


void LoadingGrid::loadData(const std::string& file, int dataTimeDelta)
{
    m_dataDelta = dataTimeDelta;

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

