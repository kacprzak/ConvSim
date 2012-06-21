#ifndef ATOMIC_H
#define ATOMIC_H

/**
 * Klasa bazowa dla dla modeli obiektów niepodzielnych.
 */
template <class T>
class Atomic
{
public:
    virtual void updateState(const T& x, unsigned int dt = 1) = 0;
    virtual void updateOutput(T& y) = 0;

private:
    T m_output;
};

#endif // ATOMIC_H
