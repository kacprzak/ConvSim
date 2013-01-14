// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef IO_TYPE_H
#define IO_TYPE_H

#include <vector>
#include <cassert>

enum Rejon { RUDNA_WEGLANOWA, RUDNA_PIASKOWCOWA, POLKOWICE };

struct Material
{
    // Skład masowo
    Material(double w, double p, double l)
        : weglanowej(w)
        , piaskowcowej(p)
        , lupkowej(l)
    {}

    Material()
        : weglanowej(0.0)
        , piaskowcowej(0.0)
        , lupkowej(0.0)
    {}

    // Masa i skład procentowo
    Material(double m, double w, double p, double l)
        : weglanowej(w * m)
        , piaskowcowej(p * m)
        , lupkowej(l * m)
    {}

    double mass() const
    {
        return weglanowej + piaskowcowej + lupkowej;
    }

    static Material build(double m, Rejon rejon)
    {
        switch (rejon) {
        case RUDNA_WEGLANOWA:
            return Material(m, 0.43, 0.3735, 0.1895);
        case RUDNA_PIASKOWCOWA:
            return Material(m, 0.2098, 0.6256, 0.1647);
        default: // POLKOWICE
            return Material(m, 0.7676, 0.1050, 0.2174);
        }
    }

    static Material combine(const std::vector<Material>& materials)
    {
        int count = materials.size();
        double w = 0.0;
        double p = 0.0;
        double l = 0.0;
        
        for (int i = 0; i < count; ++i) {
            w += materials[i].weglanowej;
            p += materials[i].piaskowcowej;
            l += materials[i].lupkowej;
        }

        return Material(w, p, l);
    }

    // Porównuje pointery
    bool operator<(const Material& other) const
    {
        return this < &other;
    }

    const Material operator+(const Material& other) const
    {
        return Material(weglanowej + other.weglanowej,
                        piaskowcowej + other.piaskowcowej,
                        lupkowej + other.lupkowej);
    }

    const Material& operator+=(const Material& other)
    {
        weglanowej += other.weglanowej;
        piaskowcowej += other.piaskowcowej;
        lupkowej += other.lupkowej;
        return *this;
    }

    // Masowo
    const Material operator*(double ratio) const
    {
        return Material(ratio * weglanowej,
                        ratio * piaskowcowej,
                        ratio * lupkowej);
    }


    double weglanowej; // masa r. węglanowej
    double piaskowcowej; // masa r. piaskowcowej
    double lupkowej; // masa r. łupkowej
};

// Typ wejścia wyjścia
typedef std::pair<int, Material> IO_type;

#endif // IO_TYPE_H
