#ifndef DTSS_EVENTLISTENER_H
#define DTSS_EVENTLISTENER_H

#include "atomic.h"

namespace dtss {

template <class T>
class EventListener
{
public:
    EventListener();

    /**
     * Wywoływana przez symulator w razie zmiany stanu symulowanego modelu.
     *
     * @param model     model który zmienił stan
     * @param t         czas w którym nastąpiło zdarzenie
     */
    virtual void stateChanged(Atomic<T> */*model*/, unsigned int /*t*/) {}

    /**
     * Wywoływana przez symulator w momencie obliczanie stanu wyjść modelu.
     *
     * @param model     model dla którego wyjścia zostały obliczone
     * @param outpu     wartość wyjścia tego modelu
     * @param t         czas w którym nastąpiło zdarzenie
     */
    virtual void outputEvent(Atomic<T> */*model*/, const T& /*output*/, unsigned int /*t*/) {}
};

template <class T>
EventListener<T>::EventListener()
{
}

} // namespace dtss

#endif // DTSS_EVENTLISTENER_H
