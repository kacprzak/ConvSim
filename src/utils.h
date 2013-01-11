// -*- c-basic-offset: 4; indent-tabs-mode: nil; -*-
#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>
#include <vector>

// Jeśli kompilujemy MSVC to na pewno jesteśmy na Windowsie
#ifdef _MSC_VER
  #define WINDOWS
#endif

#ifdef WINDOWS
  #include <windows.h> // Sleep
#else
  #include <unistd.h> // usleep
#endif


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

/**
 * Przyjmuje listę wskaźników. Zwraca wskaźnik do pierwszego obiektu o
 * nazwie przekazanej w drugim argumencie, lub null jeśli nie ma
 * takiego obiektu.
 */
template <class T>
T *findByName(const std::vector<T *>& v, const std::string& name)
{
    for (auto it = v.begin(); it != v.end(); ++it) {
        if ((*it)->name() == name)
            return (*it);
    }
    return nullptr;
}

/**
 * Czeka na wciśnięcie klawisza.
 */
inline void pressAnyKey()
{
    std::cout << std::endl << "Press any key to continue...";
    std::cin.get();
}

/**
 * Czyści ekran.
 */
inline void clearScreen()
{
#ifdef WINDOWS
    system("cls");
#else
    system("clear");
#endif
}

#endif // UTILS_H
