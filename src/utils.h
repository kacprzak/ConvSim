// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#include <iostream>
#include <fstream>
#include <vector>

/**
 * Czyta plik linijka po linijce. Każdą przeczytaną linijkę tekstu
 * przekazuje do metody create, która tworzy obiekt typu T i zwraca
 * wskaźnik na niego. Funkcja zwaraca listę wskaźników na utworzone
 * obiekty.
 */
template <class T>
std::vector<T *> loadFromFile(const char *file)
{
    using namespace std;

    string tmp;
    vector<T *> v;
    ifstream f(file);

    if (f.is_open() == true) {
        while (f.good()) {
            getline(f, tmp);
            if (!tmp.empty())
                v.push_back(T::create(tmp));
        }
        f.close();
    } else {
        cerr << "Error: unable to open " << file << endl;
    }

    return v;
}

/**
 * Przyjmuje listę wskaźników. Następnie kasuje wszystkie
 * obiekty na które wskazują wskaźniki oraz czyści listę.
 */
template <class T>
void freeClear(T& cntr)
{
    for (typename T::iterator it = cntr.begin();
         it != cntr.end(); ++it )
    {
        delete *it;
    }
    cntr.clear();
}
