#pragma once

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
    cout << "****** \n\n";
}