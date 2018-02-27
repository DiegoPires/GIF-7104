//
//  tp2.cpp
//  Exemple de convolution d'image avec lodepng
//
//  Créé par Julien-Charles Lévesque
//  Copyright 2015 Université Laval. Tous droits réservés.
//
#include "lib/lodepng.h"
#include <iostream>

#include "src/Sequentiel.cpp"
#include "src/Parallele.cpp"

#include "lib/Chrono.hpp"

using namespace std;

//Aide pour le programme
void usage(char* inName) {
    cout << endl << "Utilisation> " << inName << " fichier_image fichier_noyau [fichier_sortie=output.png]" << endl;
    exit(1);
}

int main(int inArgc, char *inArgv[]) {

    if (inArgc < 3 or inArgc > 4) usage(inArgv[0]);

    string lFilename = inArgv[1];
    string noyau = inArgv[2];
    string lOutFilename;

    Chrono c;

    if (inArgc == 4)
        lOutFilename = inArgv[3];
    else
        lOutFilename = "output.png";

    double tempsTotalParallele = 0;
    double tempsTotalSequentielle = 0;

    int iteration = 10;
    for(int i = 0; i<iteration; i++) {
        string lOutFile = lOutFilename; // to_string(i).append("_").append(lOutFilename);

        c = executerSequentiel(lFilename, lOutFile, noyau);
        tempsTotalSequentielle+= c.get();
        // cout << "Temps d'execution séquentiel = \033[1;31m3.35089 ec\033[0m " << endl;

        c = executerParallele(lFilename, lOutFile, noyau);
        tempsTotalParallele+= c.get();
    }

    double tempsMoyenSeq = (tempsTotalSequentielle / iteration);
    double tempsMoyenPar = (tempsTotalParallele / iteration);

    cout << "Temps d'execution séquentiel moyen = \033[1;31m" << tempsMoyenSeq << " sec\033[0m" << endl;
    cout << "Temps d'execution parallele moyen  = \033[1;31m" << tempsMoyenPar << " sec\033[0m" << endl;

}
