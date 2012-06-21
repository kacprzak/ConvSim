#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "atomic.h"
#include "eventlistener.h"
#include <list>

/**
 * Klasa zarządzająca postępem symulacji.
 */
template <class T>
class Simulator
{
public:
    Simulator(Atomic<T> *model);

    void computeNextState(T input);
    void computeOutput();
    unsigned int getTime();

    void addEventListener(EventListener<T> *listener);

private:
    Atomic<T> *m_model;        ///< Symulowany model
    unsigned int m_t;       ///< Zegar symulacji
    bool m_outputUpToDate;  ///< Czy wyjścia są aktualne?

    typedef std::list<EventListener<T> *> ListenerList;
    ListenerList listeners; ///< Obiekty nasłuchujące
};

#endif // SIMULATOR_H
