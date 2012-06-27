#ifndef DTSS_ATOMIC_H
#define DTSS_ATOMIC_H

#include "model.h"

#include <set>

namespace dtss {

// forward declaration
template <typename T> class Simulator;

/**
 * Klasa bazowa dla modeli obiektów, które można przedsatwić jako jeden blok
 * z wejściami i wyjściami.
 *
 * Tworząc własny model należy dziedziczyć po tej klasie i zaimplmentować
 * metody delta oraz outputFuction.
 */
template <typename T>
class Atomic : public Model<T>
{
    template <typename TT> friend class Simulator;

public:
    /**
     * Aktualizuje stan modelu na podstawie zbioru wejść.
     *
     * @param xb    zbiór wejść
     */
    virtual void delta(const std::set<T>& xb) = 0;

    /**
     * Oblicza wyjścia i dodaje je do zbioru przekazanego jako argument.
     *
     * @param yb    zbiór wyjść
     */
    virtual void outputFunction(std::set<T>& yb) const = 0;

    /**
     * Odpowiada za zwolnienie pamięci przydzielonej obiektom ze zbioru gb.
     * Jeśli obiekty wyjściowe nie są tworzone na stercie to ta funkcja nic
     * nie powinna robić.
     */
    virtual void gcOutput(std::set<T>& /*gb*/) {}

    Atomic<T> *typeIsAtomic() { return this; }

private:
    std::set<T> m_input;    ///< Stany na wejściach
    std::set<T> m_output;   ///< Stany na wyjściach
};

} // namespace dtss
#endif // DTSS_ATOMIC_H
