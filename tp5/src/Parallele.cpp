#pragma once

#include <omp.h>

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

Chrono executerParallele(int d, double seuil, int iterations, int coeur, const std::string& fichier, bool performanceTest) {

    double** u = initialize_matrix(fichier, d);

    bool stopSeuil = false;

    int iterationCount = 1;

    //omp_set_num_threads(coeur);

    int part_rows, th_id;
    part_rows = d/coeur;

    Chrono lChrono(true);

    //#pragma omp parallel
    while (!stopSeuil) {

        #pragma omp parallel //shared(stopSeuil)
        for (int i=1; i < d - 1; i++) {

            //#pragma omp barrier
            //#pragma omp single
            //{
                 int oddLineStart = i % 2 == 0 ? 1 : 2;
                 int evenLineStart = i % 2 == 0 ? 2 : 1;
            //}

            // Red sweep.
            //#pragma omp for  //shared( d, originalU, stopSeuil, i)
            #pragma omp parallel for //schedule(guided,part_rows)
            for (int j = oddLineStart; j < d-1; j+=2)
            {
                u[i][j] = 0.25*(u[i][j-1]+u[i-1][j]+u[i][j+1]+u[i+1][j]);

                if ( u[i][j] <= seuil)
                {
                    stopSeuil = true;
                }
            }

            // Black sweep.
            //#pragma omp for  //shared(u, d, originalU, stopSeuil, i)
            #pragma omp parallel for //schedule(guided,part_rows)
            for (int j = evenLineStart; j < d-1; j+=2)
            {
                u[i][j] = 0.25*(u[i][j-1]+u[i-1][j]+u[i][j+1]+u[i+1][j]);

                if (u[i][j]  <= seuil)
                {
                    stopSeuil = true;
                }
            }
        }

        lChrono.pause();

        if (iterations != 0 && iterationCount % iterations == 0 && !performanceTest) {
            string filename = "src/output/parallel/file_" + std::to_string(iterationCount) + ".bmp";
            print_output(u, d, &filename[0u]);
        }

        lChrono.resume();

        iterationCount++;
    }

    lChrono.pause();

    if (!performanceTest) {
        cout << "Temps d'execution parallele  = \033[1;31m" << lChrono.get() << " sec\033[0m" << endl;
    }

    if (iterations == 0 && !performanceTest) {
        string filename = "src/output/parallele/file_0.bmp";

        cout << "Resultat final parallele\n";
        print_output(u, d, &filename[0u]);
    }

    return lChrono;
}