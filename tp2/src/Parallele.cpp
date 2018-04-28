#pragma once

#include "../lib/lodepng.h"
#include <iostream>
#include <fstream>
#include <omp.h>

#include "../lib/Chrono.hpp"
#include "../PACC/Tokenizer.hpp"

#include "Shared.cpp"

using namespace std;

Chrono executerParallele(string iFilename, string iOutFilename, string noyau){

    // Lire le noyau.
    ifstream lConfig;

    lConfig.open(noyau);
    if (!lConfig.is_open()) {
        cerr << "Le fichier noyau fourni (" << noyau << ") est invalide." << endl;
        exit(1);
    }

    PACC::Tokenizer lTok(lConfig);
    lTok.setDelimiters(" \n", "");

    string lToken;
    lTok.getNextToken(lToken);

    int lK = atoi(lToken.c_str());
    int lHalfK = lK / 2;

    //Lecture du filtre
    double *lFilter;
    lFilter = new double[lK * lK];

    //Lecture de l'image
    //Variables à remplir
    unsigned int lWidth, lHeight;
    vector<unsigned char> lImage; //Les pixels bruts

    Chrono lChrono(true);

    // Ce premier pragma ne marche jamais, ca compile, mais ca s'execute pas
    //#pragma omp parallel num_threads(4) private(lWidth, lHeight, lImage, lFilter, lTok, lK)
    //{
        // Tout les options, sauf ordered brise l'image final des fois
        //#pragma omp parallel for // private(lToken) // schedule(auto) // collapse(2)
        #pragma omp for ordered //
        for (int i = 0; i < lK; i++) {
            for (int j = 0; j < lK; j++) {
                // ca ne marche pas, sauf pour les assignations de valeur tout de suite apres
                //#pragma omp atomic read
                lTok.getNextToken(lToken);
                lFilter[i * lK + j] = atof(lToken.c_str());
            }
        }

        // on compte pas l'appele a lodepng pour nous temps d'execution
        lChrono.pause();

        //Appeler lodepng
        decode(iFilename.c_str(), lImage, lWidth, lHeight);

        lChrono.resume();

        //Variables contenant des indices
        int fy, fx;
        //Variables temporaires pour les canaux de l'image
        double lR, lG, lB;

        int maxWidth = (int) lWidth - lHalfK;
        int maxHeight = (int) lHeight - lHalfK;

        //omp_set_num_threads(maxWidth);

        //#pragma omp parallel for schedule(dynamic) shared(lImage, lWidth, lHeight) private(lR, lG, lB, fy, fx)

        //#pragma omp parallel for schedule(dynamic) shared(lImage, lWidth, lHeight) private(lR, lG, lB, fy, fx)

        // collapse(2) breaks sometimes the image
        #pragma omp parallel for schedule(static, 2) shared(lImage, lWidth, lHeight) private(lR, lG, lB, fy, fx)
        for (int x = lHalfK; x < maxWidth; x++) {
            for (int y = lHalfK; y < maxHeight; y++) {
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
                    #pragma omp reduction(+:lR, lG, lB) private(fx)
                    for (int i = -lHalfK; i <= lHalfK; i++) {
                        fx = i + lHalfK;

                        //#pragma omp atomic update
                        lR += double(lImage[(y + j) * lWidth * 4 + (x + i) * 4]) * lFilter[fx + fy * lK];
                        //#pragma omp atomic update
                        lG += double(lImage[(y + j) * lWidth * 4 + (x + i) * 4 + 1]) * lFilter[fx + fy * lK];
                        //#pragma omp atomic update
                        lB += double(lImage[(y + j) * lWidth * 4 + (x + i) * 4 + 2]) * lFilter[fx + fy * lK];

                    }
                }
                //Placer le résultat dans l'image.
                lImage[y * lWidth * 4 + x * 4] = (unsigned char) lR;
                lImage[y * lWidth * 4 + x * 4 + 1] = (unsigned char) lG;
                lImage[y * lWidth * 4 + x * 4 + 2] = (unsigned char) lB;
            }
        }

        lChrono.pause();

        //Sauvegarde de l'image dans un fichier sortie
        encode(iOutFilename.c_str(), lImage, lWidth, lHeight);

        delete lFilter;

        //lChrono.pause();

        //cout << "L'image a été filtrée et enregistrée dans " << iOutFilename << " avec succès!" << endl;

        cout << "Temps d'execution parallele  = \033[1;31m" << lChrono.get() << " sec\033[0m" << endl;
    //}

    return lChrono;

}