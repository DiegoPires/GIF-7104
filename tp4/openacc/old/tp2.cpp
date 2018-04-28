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

// Une petite refactoration du code a été fait juste pour facilier sa lecture
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

    //double tempsTotalParallele = 0;
    //double tempsTotalSequentielle = 0;

    // La boucle FOR est commenté, parce que ca sers juste pour le test de performance
    //int iteration = 1;
    //for(int i = 0; i<iteration; i++) {

    // Si vous voulez generer des fichier avec des noms differents dans le cas de la boucle
    string lOutFile = lOutFilename; // to_string(i).append("_").append(lOutFilename);

    c = executerSequentiel(lFilename, lOutFile, noyau);

    //tempsTotalSequentielle+= c.get();

    c = executerParallele(lFilename, lOutFile, noyau);
    //tempsTotalParallele+= c.get();
    //}

    //double tempsMoyenSeq = (tempsTotalSequentielle / iteration);
    //double tempsMoyenPar = (tempsTotalParallele / iteration);

    //cout << "Temps d'execution séquentiel moyen = \033[1;31m" << tempsMoyenSeq << " sec\033[0m" << endl;
    //cout << "Temps d'execution parallele moyen  = \033[1;31m" << tempsMoyenPar << " sec\033[0m" << endl;

}
