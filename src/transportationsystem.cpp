// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "transportationsystem.h"

#include <cstdlib> // atoi
#include <iostream>
#include <fstream>
#include "utils.h"

TransportationSystem::~TransportationSystem()
{
    freeClear(m_connections);
}

void TransportationSystem::getComponents(std::set<dtss::Model<IO_type>*>& c) const
{
    c.insert(m_conveyors.begin(), m_conveyors.end());
    c.insert(m_tanks.begin(), m_tanks.end());
}

//------------------------------------------------------------------------------

void TransportationSystem::route(const IO_type &value, dtss::Model<IO_type> *source,
                                 std::set<dtss::Event<IO_type> >& r)
{
    // Numer wyjścia źródła
    int outNum = value.first;
    // Obiekt połączenia
    const TSConnection* conn = findConnection(source, outNum);
    // Model docelowy
    if (conn) {
        int inputNumber = conn->destIn();
        dtss::Event<IO_type> e(conn->destination(), IO_type(inputNumber, value.second));
        r.insert(e);
    } else {
        // Wysypany materiał na wyjściu nie jest przejmowany przez kolejne modele
        //std::cerr << "WARNING: empty Connection in TransportationSystem::route\n";
    }
}

//------------------------------------------------------------------------------

void TransportationSystem::addConveyor(Conveyor *conv)
{
    m_conveyors.insert(conv);
    conv->setParent(this);
}

//------------------------------------------------------------------------------

void TransportationSystem::addTank(Tank *tank)
{
    m_tanks.insert(tank);
    tank->setParent(this);
}

//------------------------------------------------------------------------------

void TransportationSystem::connect(dtss::Model<IO_type> *src, dtss::Model<IO_type> *dest)
{
    connect(src, 1, dest, 1);
}

//------------------------------------------------------------------------------

void TransportationSystem::connect(dtss::Model<IO_type> *src, int outNum,
                                   dtss::Model<IO_type> *dest, int inNum)
{
    Connection *conn = new TSConnection(src, outNum, dest, inNum);
    m_connections.insert(conn);
}

//------------------------------------------------------------------------------

const TSConnection *TransportationSystem::findConnection(dtss::Model<IO_type> *src, int outNum)
{
    for (auto it = m_connections.cbegin(); it != m_connections.cend(); ++it)
    {
        Connection *conn = *it;
        if (conn->connType() == MODEL_MODEL) {
            TSConnection *c = static_cast<TSConnection *>(conn);
            if (c->source() == src && c->srcOut() == outNum)
                return c;
        }
    }
    //assert(false);
    return nullptr;
}

//------------------------------------------------------------------------------

bool TransportationSystem::loadConnectionsFromFile(const std::string& filename)
{
    using namespace std;

    char sep = ';';
    ifstream f(filename);

    if (f.is_open() == true) {
        while (f.good()) {
            string conn_type;
            string src;
            string out;
            string dst;
            string in;

            std::getline(f, conn_type, sep);
            std::getline(f, src, sep);
            std::getline(f, out, sep);
            std::getline(f, dst, sep);
            std::getline(f, in);

            if (conn_type.empty() || src.empty())
                break;

            int so = atoi(out.c_str());
            int di = atoi(in.c_str());

#if 0
            cout << conn_type << ' '
                 << src << ' '
                 << so << ' '
                 << dst << ' '
                 << di << '\n';
#endif
            if (conn_type == "CC") {
                Conveyor *srcp = findByName<Conveyor *>(m_conveyors, src);
                Conveyor *dstp = findByName<Conveyor *>(m_conveyors, dst);
                connect(srcp, so, dstp, di);
            } else if (conn_type == "CT") {
                Conveyor *srcp = findByName<Conveyor *>(m_conveyors, src);
                Tank *dstp     = findByName<Tank *>(m_tanks, dst);
                connect(srcp, so, dstp, di);
            } else if (conn_type == "TC") {
                Tank *srcp     = findByName<Tank *>(m_tanks, src);
                Conveyor *dstp = findByName<Conveyor *>(m_conveyors, dst);
                connect(srcp, so, dstp, di);
            } else {
                cerr << "Error: unsupported connection type " 
                     << conn_type << " in " << filename << endl;
            }
        }
        f.close();
        return true;
    } else {
        cerr << "Error: unable to open " << filename << endl;
        return false;
    }
}
