#pragma once

#include <omp.h>

#include "../lib/Chrono.hpp"
#include "Shared.cpp"

using namespace std;

Chrono executerParallele(){


    Chrono lChrono(true);

    //lChrono.pause();

    //lChrono.resume();

    lChrono.pause();

    cout << "Temps d'execution parallele = \033[1;31m" << lChrono.get() << " sec\033[0m" << endl;

    return lChrono;

}