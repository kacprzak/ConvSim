#include "simulator.h"

Simulator::Simulator(Atomic *model)
    : m_model(model)
    , m_t(0)
    , m_outputUpToDate(false)
{
}

void Simulator::computeNextState(double input)
{
    computeOutput(); // Aktualizuje stan wyjść w czasie t

    ++m_t; // Przejście do następnej chwili w czasie
    m_model->updateState(input);

    // Rozgłoszenie zmiany stanu
    for (ListenerList::iterator it = listeners.begin();
         it != listeners.end(); ++it)
    {
        (*it)->stateChanged(m_model, m_t);
    }

    m_outputUpToDate = false; // Wyjścia nie są aktualne
}

void Simulator::addEventListener(EventListener *listener)
{
    listeners.push_back(listener);
}

void Simulator::computeOutput()
{
    // Jeśli wyjącie aktualne to nic nie rób
    if (m_outputUpToDate) return;

    // Aktualizuje wyjścia
    m_outputUpToDate = true;

    // Rozesłanie zdarzeń
    // TODO
}
