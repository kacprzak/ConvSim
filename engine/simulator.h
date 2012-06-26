#ifndef DTSS_SIMULATOR_H
#define DTSS_SIMULATOR_H

#include "atomic.h"
#include "network.h"
#include "event.h"
#include "eventlistener.h"
#include <list>
#include <set>

namespace dtss {

/**
 * Klasa zarządzająca postępem symulacji.
 */
template <typename T>
class Simulator
{

public:
    Simulator(Model<T> *model);

    void computeNextState(const std::set<Event<T> > &input);
    void computeOutput();

    /** Zwraca aktualny czas symulacji */
    unsigned int getTime() { return m_t; }

    void addEventListener(EventListener<T> *listener);

private:
    /// Zwraca wszystkie komponenty sieci
    void getAllChildren(Network<T> *network, std::set<Atomic<T> *> s);
    /// Przesuwa wartośi z wyjść na odpowiednie wejścia
    void route(Network<T> *parent, Model<T> *source, const T& x);
    /// Zawiadom obiekty nasłuchujące o zmianie wyjścia
    void notifyOutputListeners(Model<T> *model, const T& value, unsigned int t);


    unsigned int m_t;                   ///< Zegar symulacji
    bool m_outputUpToDate;              ///< Czy wyjścia są aktualne?

    typedef std::set<Atomic<T> *> AtomicsSet;
    std::set<Atomic<T> *> m_atomics;    ///< Wszystkie symulowane komponenty

    typedef std::list<EventListener<T> *> ListenerList;
    ListenerList m_listeners;             ///< Obiekty nasłuchujące
};


template <typename T>
Simulator<T>::Simulator(Model<T> *model)
    : m_t(0)
    , m_outputUpToDate(false)
{
    // Jeśli model nie jest siecią
    if (model->typeIsAtomic() != 0) {
        m_atomics.insert(model->typeIsAtomic());
    } else {
        // Jeśli jest siecią to pozyskaj wszystkie komponenty
        getAllChildren(model->typeIsNetwork(), m_atomics);
    }
}

/**
 * Funkcja nakazująca modelowi uaktualnić swój stan.
 *
 * @param input     stan wejścia do modelu
 */
template <typename T>
void Simulator<T>::computeNextState(const std::set<Event<T> >& input)
{
    computeOutput(); // Aktualizuje stan wyjść w czasie t

    // Przekazanie wejść symulatora do odpowiednich komponentów
    for (typename std::set<Event<T> >::const_iterator it = input.begin();
         it != input.end(); ++it)
    {
        const Event<T>& evt = *it;
        if (evt.model()->typeIsAtomic()) {
            evt.model()->m_input.insert(evt.value());
        } else {
            route(evt.model()->typeIsNetwork(), evt.model(), evt.value());
        }
    }

    ++m_t; // Przejście do następnej chwili w czasie

    // Aktualizacja stanu każdego komponentu
    for (typename std::set<Atomic<T> *>::iterator it = m_atomics.begin();
         it != m_atomics.end(); ++it)
    {
        Atomic<T> *atomic = *it;
        atomic->delta(atomic->m_input);

        // Rozgłoszenie zmiany stanu
        for (typename ListenerList::iterator liter = m_listeners.begin();
             liter != m_listeners.end(); ++liter)
        {
            (*liter)->stateChanged(atomic, m_t);
        }
    }

    // Sprzątanie
    for (typename std::set<Atomic<T> *>::iterator it = m_atomics.begin();
         it != m_atomics.end(); ++it)
    {
        Atomic<T> *atomic = *it;
        atomic->gcOutput(atomic->m_output);
        atomic->m_output.clear();
        atomic->m_input.clear();
    }

    m_outputUpToDate = false; // Wyjścia nie są aktualne
}


template <typename T>
void Simulator<T>::addEventListener(EventListener<T> *listener)
{
    m_listeners.push_back(listener);
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

    for (typename std::set<Atomic<T> *>::iterator it = m_atomics.begin();
         it != m_atomics.end(); ++it)
    {
        Atomic<T> *atomic = *it;
        // Obliczenie stanu wyjść komponentu
        atomic->outputFunction(atomic->m_output);

        // Przekazanie stanu z wyjść na wejścia innego komponentu lub sieci
        std::set<T>& y = atomic->m_output;

        for (typename std::set<T>::iterator yIt = y.begin();
             yIt != y.end(); ++yIt)
        {
            route(atomic->parent(), atomic, *yIt);
        }
    }
}


template <typename T>
void Simulator<T>::getAllChildren(Network<T> *network, std::set<Atomic<T> *> s)
{
    std::set<Model<T> *> c;
    network->getComponents(c);

    // Dodaje komponenty ze zbioru c do s, a sieci
    // przeszukuje w głąb rekurencyjnie
    for (typename std::set<Model<T> *>::iterator it = c.begin();
         it != c.end(); ++it)
    {
        Model<T> *m = *it;
        if (m->typeIsAtomic()) {
            s.insert(m->typeIsAtomic());
        } else {
            getAllChildren(m->typeIsNetwork(), s);
        }
    }
}


template <typename T>
void Simulator<T>::route(Network<T> *parent, Model<T> *source, const T& x)
{
    // TODO
}

} // namespace dtss

#endif // DTSS_SIMULATOR_H
