// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
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
    Simulator(Model<T> *model, unsigned long dt = 1);

    void computeNextState(const std::set<Event<T> > &input);
    void computeOutput();

    /** Zwraca aktualny czas symulacji */
    unsigned int getTime() { return m_t; }

    void addEventListener(EventListener<T> *listener);

private:
    void getAllChildren(Network<T> *network, std::set<Atomic<T> *>& s);
    void route(Network<T> *parent, Model<T> *source, const T& x);
    void notifyOutputListeners(Model<T> *model, const T& value, unsigned int t);


    unsigned long m_t;                  ///< Zegar symulacji
    unsigned long m_dt;                 ///< Czas jednego kroku symulacji
    bool m_outputUpToDate;              ///< Czy wyjścia są aktualne?

    typedef std::set<Atomic<T> *> AtomicsSet;
    AtomicsSet m_atomics;               ///< Wszystkie symulowane komponenty

    typedef std::list<EventListener<T> *> ListenerList;
    ListenerList m_listeners;           ///< Obiekty nasłuchujące
};

/**
 * Konstruktor.
 *
 * @param model     model do symulacji. Sieć lub Atomic.
 */
template <typename T>
    Simulator<T>::Simulator(Model<T> *model, unsigned long dt)
    : m_t(0)
    , m_dt(dt)
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
    for (typename std::set<Event<T> >::const_iterator it = input.cbegin();
         it != input.cend(); ++it)
    {
        const Event<T>& evt = *it;
        if (evt.model()->typeIsAtomic()) {
            evt.model()->typeIsAtomic()->m_input.insert(evt.value());
        } else {
            route(evt.model()->typeIsNetwork(), evt.model(), evt.value());
        }
    }

    m_t += m_dt; // Przejście do następnej chwili w czasie

    // Aktualizacja stanu każdego komponentu
    for (typename AtomicsSet::iterator it = m_atomics.begin();
         it != m_atomics.end(); ++it)
    {
        Atomic<T> *atomic = *it;
        atomic->delta(m_dt, atomic->m_input);

        // Rozgłoszenie zmiany stanu
        for (typename ListenerList::iterator liter = m_listeners.begin();
             liter != m_listeners.end(); ++liter)
        {
            (*liter)->stateChanged(atomic, m_t);
        }
    }

    // Sprzątanie
    for (typename AtomicsSet::iterator it = m_atomics.begin();
         it != m_atomics.end(); ++it)
    {
        Atomic<T> *atomic = *it;
        atomic->gcOutput(atomic->m_output);
        atomic->m_output.clear();
        atomic->m_input.clear();
    }

    m_outputUpToDate = false; // Wyjścia nie są aktualne
}

/**
 * Rejestruje obiekt nasłuchujący.
 */
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

    for (typename AtomicsSet::iterator it = m_atomics.begin();
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

/** Zwraca wszystkie komponenty sieci.
 *
 *  @param network  sieć do inspekcji
 *  @param s        zbiór do którego zostaną zapisane modele typu Atomic
 *                  należące do sieci network oraz jej podsieci
 */
template <typename T>
void Simulator<T>::getAllChildren(Network<T> *network, std::set<Atomic<T> *>& s)
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

/**
 * Przesuwa wartości z wyjść na odpowiednie wejścia.
 */
template <typename T>
void Simulator<T>::route(Network<T> *parent, Model<T> *source, const T& x)
{
    // Powiadamia obiekty nasłuchujące o wyjściu. Uwaga: jeśli parent = source
    // to x jest wejściem do sieci. We wszystkich innych przypadkach x jest
    // wyściem z modelu.

    if (parent != source) // x jest wyjściem
        notifyOutputListeners(source, x, m_t);

    // Jeśli parent = 0 symulacji podlega pojedynczy prosty model. Czyli
    // przy braku sieci nie ma czego przekazywać.
    if (parent == 0) return;

    // Zbiór komponentów na które wpływa x
    std::set<Event<T> > r;
    parent->route(x, source, r);

    for (typename std::set<Event<T> >::iterator it = r.begin();
         it != r.end(); ++it)
    {
        const Event<T>& e = *it;
        // Jeśli obiektem docelowym zdarzenia jest komponent to ustaw wejście
        if (e.model()->typeIsAtomic() != 0) {
            e.model()->typeIsAtomic()->m_input.insert(e.value());
        }
        // Jeśli obiektem docelowym jest sieć (nie nadrzędna)
        else if (parent != e.model()) {
            route(e.model()->typeIsNetwork(), e.model(), e.value());
        }
        // Jeśli obiektem docelowym jest rodzic (sieć nadrzędna)
        else { // if e.model() == parent
            route(parent->parent(), parent, e.value());
        }
    }
}

/**
 * Zawiadom obiekty nasłuchujące o zmianie wyjścia.
 *
 * @param model     model który zmienił wyjście
 * @param value     stan wyjścia
 * @param t         czas w którym nastąpiła zmiana
 */
template <typename T>
void Simulator<T>::notifyOutputListeners(Model<T> *model,
                                         const T& value,
                                         unsigned int t)
{
    Event<T> e(model, value);
    for (typename ListenerList::iterator it = m_listeners.begin();
         it != m_listeners.end(); ++it)
    {
        (*it)->outputEvent(e, t);
    }
}

} // namespace dtss

#endif // DTSS_SIMULATOR_H
