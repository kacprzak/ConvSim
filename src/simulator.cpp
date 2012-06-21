#include "simulator.h"

template <class T>
Simulator<T>::Simulator(Atomic<T> *model)
    : m_model(model)
    , m_t(0)
    , m_outputUpToDate(false)
{
}

template <class T>
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

    // TODO wyczyścić output modelu
    m_outputUpToDate = false; // Wyjścia nie są aktualne
}

template <class T>
void Simulator<T>::addEventListener(EventListener<T> *listener)
{
    listeners.push_back(listener);
}

template <class T>
void Simulator<T>::computeOutput()
{
    // Jeśli wyjącie aktualne to nic nie rób
    if (m_outputUpToDate) return;

    // Aktualizuje wyjścia
    m_outputUpToDate = true;

    // Rozesłanie zdarzeń
    // TODO
}
