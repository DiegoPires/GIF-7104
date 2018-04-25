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

double** initialize_matrix(string fichier, int d){

    double** array2D = 0;
    array2D = new double*[d];

    // Initialize all to 0
    for (int h = 0; h < d; h++)
    {
        array2D[h] = new double[d];

        for (int w = 0; w < d; w++)
        {
            array2D[h][w] = 0;
        }
    }

    // Initialize all to file value
    std::ifstream infilestream;
    std::string line;

    infilestream.open(fichier);

    while (infilestream)
    {
        std::getline(infilestream, line);
        std::istringstream iss(line);
        int i, j;
        double v;
        if (!(iss >> i >> j >> v)) { break; } // error

        array2D[i][j] = v;
    }

    return array2D;
}

// TODO: change to create an image
void print_output(double **u, int d){

    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            cout << u[i][j] << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

Chrono executerSequentiel(int d, double seuil, int iterations, int coeur, const std::string& fichier) {

    double** u = initialize_matrix(fichier, d);
    double** originalU = initialize_matrix(fichier, d);

    Chrono lChrono(true);

    double oldData = 0;
    double newData = 0;
    bool stopSeuil = false;

    int iteration = 1;

    while (!stopSeuil) {
    // while (iteration <= 10) {

        // Pas calculer les border
        for (int i=1; i < d - 1; i++) {

            int oddLineStart = i % 2 == 0 ? 1 : 2;
            int evenLineStart = i % 2 == 0 ? 2 : 1;

            // Red sweep.
            //#pragma omp parallel for shared(u, d, original, stopSeuil, i) private(j)
            for (int j = oddLineStart; j < d-1; j+=2)
            {
                // cout << "--- Original: " << i << j << " - " << u[i][j] << "\n";
                // cout << "--- Valeurs: "<<  u[i][j-1] << " + " << u[i-1][j] << " + " << u[i][j+1] << " + " << u[i+1][j] << "\n";
                // cout << "--- Somme: "<< u[i][j-1]+u[i-1][j]+u[i][j+1]+u[i+1][j] << "\n";
                // cout << "--- Final: "<< 0.25*(u[i][j-1]+u[i-1][j]+u[i][j+1]+u[i+1][j]) << "\n\n";

                u[i][j] = 0.25*(u[i][j-1]+u[i-1][j]+u[i][j+1]+u[i+1][j]);

                // cout << "---" << u[i][j] << "\n\n";

                if ( u[i][j]  <= seuil) // originalU[i][j] -
                {

                    //cout << "+++" << u[i][j] << "\n";
                    //cout << "+++" << originalU[i][j] << "\n";
                    //cout << "++" << originalU[i][j] - u[i][j] << "\n";
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
                    //cout << "---" << u[i][j] << "\n";
                    //cout << "---" << originalU[i][j] << "\n";
                    //cout << u[i][j] - originalU[i][j] << "\n";
                    //cout << "--" << originalU[i][j] - u[i][j] << "\n";
                    stopSeuil = true;
                }
            }
        }


        if (iteration % iterations == 0) {
            print_output(u, d);
        }

        iteration++;

        cout << "****** \n\n";
    }

    // print between each "iterations"

    // end while


    lChrono.pause();

    cout << "Temps d'execution séquentiel = \033[1;31m" << lChrono.get() << " sec\033[0m" << endl;

    return lChrono;
}