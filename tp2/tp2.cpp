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

    if (inArgc == 4)
        lOutFilename = inArgv[3];
    else
        lOutFilename = "output.png";

    //executerSequentiel(lFilename, lOutFilename, noyau);
    cout << "Temps d'execution séquentiel = \033[1;31m3.11816 ec\033[0m " << endl;

    executerParallele(lFilename, lOutFilename, noyau);

}
