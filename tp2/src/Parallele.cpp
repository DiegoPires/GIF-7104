#pragma once

#include "../lib/lodepng.h"
#include <iostream>
#include <fstream>
#include <omp.h>

#include "../lib/Chrono.hpp"
#include "../PACC/Tokenizer.hpp"

#include "Shared.cpp"

using namespace std;

void executerParallele(string iFilename, string iOutFilename, string noyau){

    // Lire le noyau.
    ifstream lConfig;

    lConfig.open(noyau);
    if (!lConfig.is_open()) {
        cerr << "Le fichier noyau fourni (" << noyau << ") est invalide." << endl;
        exit(1);
    }

    PACC::Tokenizer lTok(lConfig);
    lTok.setDelimiters(" \n","");

    string lToken;
    lTok.getNextToken(lToken);

    int lK = atoi(lToken.c_str());
    int lHalfK = lK/2;

    Chrono lChrono(true);

    //Lecture du filtre
    double* lFilter;
    lFilter = new double[lK * lK];

    //Lecture de l'image
    //Variables à remplir
    unsigned int lWidth, lHeight;
    vector<unsigned char> lImage; //Les pixels bruts

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < lK; i++) {
        for (int j = 0; j < lK; j++) {
            lTok.getNextToken(lToken);
            lFilter[i * lK + j] = atof(lToken.c_str());
        }
    }

   //Appeler lodepng
    decode(iFilename.c_str(), lImage, lWidth, lHeight);

    //Variables contenant des indices
    int fy, fx;
    //Variables temporaires pour les canaux de l'image
    double lR, lG, lB;

    int maxWidth = (int)lWidth - lHalfK;
    int maxHeight = (int)lHeight - lHalfK;

    //omp_set_num_threads(maxWidth);

    //#pragma omp parallel for schedule(dynamic) shared(lImage, lWidth, lHeight) private(lR, lG, lB, fy, fx)

    //#pragma omp parallel for schedule(dynamic) shared(lImage, lWidth, lHeight) private(lR, lG, lB, fy, fx)

    // Dynamic sees better?
    //#pragma omp parallel for schedule(dynamic) collapse(2) shared(lImage, lWidth, lHeight) private(lR, lG, lB, fy, fx)
    for(int x = lHalfK; x < maxWidth; x++)
    {
        for (int y = lHalfK; y < maxHeight; y++)
        {
//#pragma omp atomic write
            lR = 0.;
//#pragma omp atomic write
            lG = 0.;
//#pragma omp atomic write
            lB = 0.;

            //This does not work at all - 40 sec +/-
            //#pragma omp parallel for schedule(dynamic) shared(lImage, lWidth, lHeight)
            for (int j = -lHalfK; j <= lHalfK; j++) {
                fy = j + lHalfK;
                for (int i = -lHalfK; i <= lHalfK; i++) {
                    fx = i + lHalfK;

                        //#pragma omp atomic update
                        lR += double(lImage[(y + j)*lWidth*4 + (x + i)*4]) * lFilter[fx + fy*lK];
                        //#pragma omp atomic update
                        lG += double(lImage[(y + j)*lWidth*4 + (x + i)*4 + 1]) * lFilter[fx + fy*lK];
                        //#pragma omp atomic update
                        lB += double(lImage[(y + j)*lWidth*4 + (x + i)*4 + 2]) * lFilter[fx + fy*lK];

                }
            }
            //Placer le résultat dans l'image.
            lImage[y*lWidth*4 + x*4] = (unsigned char)lR;
            lImage[y*lWidth*4 + x*4 + 1] = (unsigned char)lG;
            lImage[y*lWidth*4 + x*4 + 2] = (unsigned char)lB;
        }
    }

    //Sauvegarde de l'image dans un fichier sortie
    encode(iOutFilename.c_str(),  lImage, lWidth, lHeight);

    delete lFilter;

    lChrono.pause();

    cout << "L'image a été filtrée et enregistrée dans " << iOutFilename << " avec succès!" << endl;

    cout << "Temps d'execution parallele  = \033[1;31m" << lChrono.get() << " sec\033[0m" << endl;

}