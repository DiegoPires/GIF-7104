#pragma once

#include "../lib/Chrono.hpp"
#include "Shared.cpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

#include <sstream>
#include <string>

using namespace std;

Chrono executerSequentiel(int d, double seuil, int iterations, int coeur, const std::string& fichier) {

    double** u = initialize_matrix(fichier, d);
    double** originalU = initialize_matrix(fichier, d);

    Chrono lChrono(true);

    double oldData = 0;
    double newData = 0;
    bool stopSeuil = false;

    int iterationCount = 1;

    while (!stopSeuil) {

        for (int i=1; i < d - 1; i++) {

            int oddLineStart = i % 2 == 0 ? 1 : 2;
            int evenLineStart = i % 2 == 0 ? 2 : 1;

            // Red sweep.
            //#pragma omp parallel for shared(u, d, original, stopSeuil, i) private(j)
            for (int j = oddLineStart; j < d-1; j+=2)
            {
                u[i][j] = 0.25*(u[i][j-1]+u[i-1][j]+u[i][j+1]+u[i+1][j]);

                if ( u[i][j]  <= seuil) // originalU[i][j] -
                {
                   stopSeuil = true;
                }
            }

            // Black sweep.
            //#pragma omp parallel for shared(u, d, original, stopSeuil, i) private(j)
            for (int j = evenLineStart; j < d-1; j+=2)
            {
                u[i][j] = 0.25*(u[i][j-1]+u[i-1][j]+u[i][j+1]+u[i+1][j]);

                if (u[i][j]  <= seuil) // originalU[i][j] -
                {
                    stopSeuil = true;
                }
            }
        }

        if (iterations != 0 && iterationCount % iterations == 0) {
            print_output(u, d);
        }

        iterationCount++;
    }

    lChrono.pause();

    if (coeur == 1){
        cout << "Temps d'execution séquentiel = \033[1;31m" << lChrono.get() << " sec\033[0m" << endl;
    }

    return lChrono;
}