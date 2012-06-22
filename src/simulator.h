#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "atomic.h"
#include "eventlistener.h"
#include <list>

namespace dtss {

/**
 * Klasa zarządzająca postępem symulacji.
 */
template <typename T>
class Simulator
{
public:
    Simulator(Atomic<T> *model)
        : m_model(model)
        , m_t(0)
        , m_outputUpToDate(false)
    {}

    void computeNextState(T input);
    void computeOutput();
    /** Zwraca aktualny czas symulacji */
    unsigned int getTime() { return m_t; }

    void addEventListener(EventListener<T> *listener);

private:
    Atomic<T> *m_model;     ///< Symulowany model
    unsigned int m_t;       ///< Zegar symulacji
    bool m_outputUpToDate;  ///< Czy wyjścia są aktualne?

    typedef std::list<EventListener<T> *> ListenerList;
    ListenerList listeners; ///< Obiekty nasłuchujące
};


/**
 * Funkcja nakazująca modelowi uaktualnić swój stan.
 *
 * @param input     stan wejścia do modelu
 */
template <typename T>
void Simulator<T>::computeNextState(T input)
{
    computeOutput(); // Aktualizuje stan wyjść w czasie t

    ++m_t; // Przejście do następnej chwili w czasie
    m_model->updateState(input);

    // Rozgłoszenie zmiany stanu
    for (typename ListenerList::iterator it = listeners.begin();
         it != listeners.end(); ++it)
    {
        (*it)->stateChanged(m_model, m_t);
    }

    m_outputUpToDate = false; // Wyjścia nie są aktualne
}


template <typename T>
void Simulator<T>::addEventListener(EventListener<T> *listener)
{
    listeners.push_back(listener);
}


/**
 * Uzyskuje stan wyjść modelu i rozgłasza do obiektów nasłuchujących.
 */
template <typename T>
void Simulator<T>::computeOutput()
{
    // Jeśli wyjącie aktualne to nic nie rób
    if (m_outputUpToDate) return;

    // Aktualizuje wyjścia
    m_outputUpToDate = true;

    T output;
    m_model->updateOutput(output);
    // Rozesłanie zdarzeń
    for (typename ListenerList::iterator it = listeners.begin();
         it != listeners.end(); ++it)
    {
        (*it)->outputEvent(m_model, output, m_t);
    }
}

} // namespace dtss

#endif // SIMULATOR_H
