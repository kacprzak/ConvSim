#ifndef DTSS_EVENT_H
#define DTSS_EVENT_H

#include "model.h"

namespace dtss {

template <typename T>
class Event
{
public:
    Event() : m_model(0) {}

    Event(Model<T> *model, const T& value) : m_model(model), m_value(value) {}

    Event(const Event<T>& e) : m_model(e.model()), m_value(e.value()) {}

    const Event<T>& operator=(const Event<T>& e)
    {
        this->m_model = e.model();
        this->m_value = e.value();
        return *this;
    }

    bool operator<(const Event<T>& other) const
    {
        return m_value < other.value();
    }

    Model<T> *model() const { return m_model; }
    T value() const { return m_value; }

private:
    Model<T> *m_model; ///< Obiekt który wygenerował zdarzenie
    T m_value;          ///< Treść zdarzenia

};

} // namespace dtss

#endif // DTSS_EVENT_H
