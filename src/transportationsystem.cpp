// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "transportationsystem.h"

#include "utils.h"

TransportationSystem::~TransportationSystem()
{
    freeClear(m_connections);
}

void TransportationSystem::getComponents(std::set<dtss::Model<IO_type>*>& c) const
{
    c.insert(m_conveyors.begin(), m_conveyors.end());
}

//------------------------------------------------------------------------------

void TransportationSystem::route(const IO_type &value, dtss::Model<IO_type> *source,
                                 std::set<dtss::Event<IO_type> >& r)
{
    // Źródło
    Conveyor *outConv = static_cast<Conveyor *>(source);
    // Numer wyjścia źródła
    int outNum = value.first;
    // Obiekt połączenia
    const ConvConvConnection* conn = findConnection(outConv, outNum);
    // Model docelowy
    if (conn) {
        Conveyor *inConv = conn->destination();
        int inputNumber = conn->destIn();
        dtss::Event<IO_type> e(inConv, IO_type(inputNumber, value.second));
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

void TransportationSystem::connect(Conveyor *outConv, Conveyor *inConv)
{
    connect(outConv, 1, inConv, 1);
}

//------------------------------------------------------------------------------

void TransportationSystem::connect(Conveyor *outConv, int outNum, Conveyor *inConv, int inNum)
{
    Connection *conn = new ConvConvConnection(outConv, outNum, inConv, inNum);
    m_connections.insert(conn);
}

//------------------------------------------------------------------------------

const ConvConvConnection *TransportationSystem::findConnection(Conveyor *outConv, int outNum)
{
    for (auto it = m_connections.cbegin(); it != m_connections.cend(); ++it)
    {
        Connection *conn = *it;
        if (conn->connType() == CONV_CONV) {
            ConvConvConnection *c = static_cast<ConvConvConnection *>(conn);
            if (c->source() == outConv && c->srcOut() == outNum)
                return c;
        }
    }
    //assert(false);
    return nullptr;
}
