// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "transportationsystem.h"

void TransportationSystem::getComponents(std::set<dtss::Model<IO_type>*>& c) const
{
    c.insert(m_conveyors.begin(), m_conveyors.end());
}

void TransportationSystem::route(const IO_type &value, dtss::Model<IO_type> *source,
                                 std::set<dtss::Event<IO_type> >& r)
{
    // Źródło
    Conveyor *outConv = static_cast<Conveyor *>(source);
    // Numer wyjścia źródła
    int outNum = value.first;
    // Obiekt połączenia
    Connection conn = findConnection(outConv, outNum);
    // Model docelowy
    if (!conn.isNull()) {
        Conveyor *inConv = conn.inConv;
        int inputNumber = conn.inputNumber;
        dtss::Event<IO_type> e(inConv, IO_type(inputNumber, value.second));
        r.insert(e);
    } else {
        // Wysypany materiał na wyjściu nie jest przejmowany przez kolejne modele
        //std::cerr << "WARNING: empty Connection in TransportationSystem::route\n";
    }
}

void TransportationSystem::addConveyor(Conveyor *conv)
{
    m_conveyors.insert(conv);
    conv->setParent(this);
}

void TransportationSystem::connect(Conveyor *outConv, Conveyor *inConv)
{
    connect(outConv, 1, inConv, 1);
}

void TransportationSystem::connect(Conveyor *outConv, int outNum, Conveyor *inConv, int inNum)
{
    Connection conn;
    conn.outConv = outConv;
    conn.outputNumber = outNum;
    conn.inConv = inConv;
    conn.inputNumber = inNum;
    m_connections.insert(conn);
}

TransportationSystem::Connection TransportationSystem::findConnection(Conveyor *outConv,
                                                                      int outNum)
{
    for (std::set<Connection>::const_iterator it = m_connections.begin();
         it != m_connections.end(); ++it)
    {
        if (it->outConv == outConv && it->outputNumber == outNum)
            return *it;
    }

    return Connection();
}
