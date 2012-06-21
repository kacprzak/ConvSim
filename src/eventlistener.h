#ifndef EVENTLISTENER_H
#define EVENTLISTENER_H

#include "atomic.h"

template <class T>
class EventListener
{
public:
    EventListener();

    virtual void stateChanged(Atomic<T> */*model*/, unsigned int /*t*/) {}
    virtual void outputEvent(Atomic<T> */*model*/, const T& /*output*/, unsigned int /*t*/) {}
};

#endif // EVENTLISTENER_H
