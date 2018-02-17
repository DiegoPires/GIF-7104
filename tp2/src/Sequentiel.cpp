#pragma once

#include "../lib/lodepng.h"
#include <iostream>
#include <fstream>

#include "../lib/Chrono.hpp"
#include "../PACC/Tokenizer.hpp"

#include "Shared.cpp"

using namespace std;

void executerSequentiel(string iFilename, string iOutFilename, string noyau){

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

    // cout << "Taille du noyau: " <<  lK << endl;

    Chrono lChrono(true);

    //Lecture du filtre
    double* lFilter;
    lFilter = new double[lK * lK];

    for (int i = 0; i < lK; i++) {
        for (int j = 0; j < lK; j++) {
            lTok.getNextToken(lToken);
            lFilter[i*lK+j] = atof(lToken.c_str());
        }
    }

    //Lecture de l'image
    //Variables à remplir
    unsigned int lWidth, lHeight;
    vector<unsigned char> lImage; //Les pixels bruts
    //Appeler lodepng
    decode(iFilename.c_str(), lImage, lWidth, lHeight);

    //Variables contenant des indices
    int fy, fx;
    //Variables temporaires pour les canaux de l'image
    double lR, lG, lB;
    for(int x = lHalfK; x < (int)lWidth - lHalfK; x++)
    {
        for (int y = lHalfK; y < (int)lHeight - lHalfK; y++)
        {
            lR = 0.;
            lG = 0.;
            lB = 0.;
            for (int j = -lHalfK; j <= lHalfK; j++) {
                fy = j + lHalfK;
                for (int i = -lHalfK; i <= lHalfK; i++) {
                    fx = i + lHalfK;
                    //R[x + i, y + j] = Im[x + i, y + j].R * Filter[i, j]
                    lR += double(lImage[(y + j)*lWidth*4 + (x + i)*4]) * lFilter[fx + fy*lK];
                    lG += double(lImage[(y + j)*lWidth*4 + (x + i)*4 + 1]) * lFilter[fx + fy*lK];
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

    lChrono.pause();

    cout << "L'image a été filtrée et enregistrée dans " << iOutFilename << " avec succès!" << endl;

    cout << "Temps d'execution séquentiel = \033[1;31m" << lChrono.get() << " sec\033[0m" << endl;

    delete lFilter;

}