#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "atomic.h"
#include "eventlistener.h"
#include <list>

/**
 * Klasa zarządzająca postępem symulacji.
 */
class Simulator
{
public:
    Simulator(Atomic *model);

    void computeNextState(double input);
    void computeOutput();
    unsigned int getTime();

    void addEventListener(EventListener *listener);

private:
    Atomic *m_model;        ///< Symulowany model
    unsigned int m_t;       ///< Zegar symulacji
    bool m_outputUpToDate;  ///< Czy wyjścia są aktualne?

    typedef std::list<EventListener*> ListenerList;
    ListenerList listeners; ///< Obiekty nasłuchujące
};

#endif // SIMULATOR_H
