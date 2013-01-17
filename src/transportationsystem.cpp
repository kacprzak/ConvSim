// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include "transportationsystem.h"

#include "utils.h"
#include <boost/lexical_cast.hpp>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"

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
    TSConnection *conn = new TSConnection(src, outNum, dest, inNum);
    m_connections.insert(conn);
}

//------------------------------------------------------------------------------

void TransportationSystem::connect(const std::string& conn_type,
                                   const std::string& src, int so,
                                   const std::string& dst, int di)
{
    if (conn_type == "CONVEYOR_CONVEYOR") {
        Conveyor *srcp = findByName<Conveyor *>(m_conveyors, src);
        Conveyor *dstp = findByName<Conveyor *>(m_conveyors, dst);
        connect(srcp, so, dstp, di);
    } else if (conn_type == "CONVEYOR_TANK") {
        Conveyor *srcp = findByName<Conveyor *>(m_conveyors, src);
        Tank *dstp     = findByName<Tank *>(m_tanks, dst);
        connect(srcp, so, dstp, di);
    } else if (conn_type == "TANK_CONVEYOR") {
        Tank *srcp     = findByName<Tank *>(m_tanks, src);
        Conveyor *dstp = findByName<Conveyor *>(m_conveyors, dst);
        connect(srcp, so, dstp, di);
    } else {
        std::cerr << "Error: unsupported connection type " 
                  << conn_type << std::endl;
    }
}

//------------------------------------------------------------------------------

const TSConnection *TransportationSystem::findConnection(dtss::Model<IO_type> *src, int outNum)
{
    for (auto it = m_connections.cbegin(); it != m_connections.cend(); ++it)
    {
        TSConnection *c = *it;
        if (c->source() == src && c->srcOut() == outNum)
            return c;
    }
    //assert(false);
    return nullptr;
}

//------------------------------------------------------------------------------

bool TransportationSystem::loadConnectionsFromFile(const std::string& filename)
{
    // File has to exist while working with XML
    rapidxml::file<> xmlfile(filename.c_str());
    
    using namespace rapidxml;
    
    xml_document<> doc;
    doc.parse<0>(xmlfile.data());
    
    xml_node<> *ts_node = doc.first_node("TransportationSystem");
    
    try {
        xml_node<> *conn_node = ts_node->first_node("Connection");
        while (conn_node) {
            std::string conn_type = conn_node->first_attribute("type")->value();
            std::string src       = conn_node->first_attribute("src")->value();
            int so = boost::lexical_cast<int>(conn_node->first_attribute("srcOutput")->value());
            std::string dst       = conn_node->first_attribute("dest")->value();
            int di = boost::lexical_cast<int>(conn_node->first_attribute("destInput")->value());

            connect(conn_type, src, so, dst, di);

            // Next node
            conn_node = conn_node->next_sibling("Connection");
        }
        return true;
        
    } catch (const boost::bad_lexical_cast& e) {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}
