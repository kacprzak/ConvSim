// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef DTSS_EVENT_H
#define DTSS_EVENT_H

#include "model.h"

namespace dtss {

/**
 * Klasa reprezentująca zdarzenia zachodzące podczas symulacji.
 * Zdarzenia takie jak: wejścia do symulatora, wyjścia z modeli, wejścia do
 * modeli.
 */
template <typename T>
class Event
{
public:
    /**
     * Konstruktor. Puste zdarzenie.
     */
    Event() : m_model(0) {}

    /**
     * Konstruktor.
     *
     * @param model     model którego dotyczy zdarzenie
     * @param value     treść zdarzenia
     */
    Event(Model<T> *model, const T& value) : m_model(model), m_value(value) {}

    // Copy-constructors
    Event(const Event<T>& e) : m_model(e.model()), m_value(e.value()) {}
    const Event<T>& operator=(const Event<T>& e)
    {
        this->m_model = e.model();
        this->m_value = e.value();
        return *this;
    }

    // for STL
    bool operator<(const Event<T>& other) const
    {
        return m_value < other.value();
    }

    /**
     * Zwaraca model którego dotyczy zdarzenie.
     */
    Model<T> *model() const { return m_model; }

    /**
     * Zwraca treść zdarzenia.
     */
    T value() const { return m_value; }

private:
    Model<T> *m_model;  ///< Obiekt którego dotyczy zdarzenie
    T m_value;          ///< Treść zdarzenia

};

} // namespace dtss

#endif // DTSS_EVENT_H
