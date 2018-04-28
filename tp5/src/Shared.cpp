#pragma once

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>
#include <string>

#include "../lib/bitmap_gen.hpp"

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

    while (std::getline(infilestream, line))
    {
        std::istringstream iss(line);
        int i, j;
        double v;
        if (!(iss >> i >> j >> v)) { break; } // error

        array2D[i][j] = v;
    }

    infilestream.seekg(0, ios::beg);
    infilestream.close();
    infilestream.clear();

    return array2D;
}

// TODO: change to create an image
void print_output(double **u, int d, char *filename){

    for (int i = 0; i < d; i++) {
        for (int j = 0; j < d; j++) {
            cout << u[i][j] << " ";
        }
        cout << "\n";
    }

    cout << "\n";

    cout << "****** \n\n";

    int i,j;

    RGB_data buffer[d*2][d*2];

    memset(buffer, 0, sizeof(buffer));

    for (i = 0; i < d; i++)
    {
        for (j = 0; j < d; j++)
        {
            buffer[i][j].g = buffer[i][j].b = 0; // 0x07;
            buffer[i][j].r = u[i][j] * 10;
        }
    }

    bmp_generator(filename, d*2, d*2, (BYTE*)buffer);

    //return EXIT_SUCCESS;
}