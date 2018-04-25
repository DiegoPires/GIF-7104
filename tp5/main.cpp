//
//  main.cpp
//  Simulation thermique 2D - Résolution de l'équation de Laplace avec la méthode
// "Red Black" de Gauss-Seidel
//
//  Créé par Diego Silva Pires et Hervé Simard
//  Copyright 2018 Université Laval. Tous droits réservés.
//

#include "src/Sequentiel.cpp"
#include "src/Parallele.cpp"

#include "lib/Chrono.hpp"

#include <math.h>
#include <unistd.h>

using namespace std;

// Une petite refactoration du code a été fait juste pour facilier sa lecture
int main(int argc, char *argv[]) {

    Chrono c;

    const char *fichier = "";

    int opt = 0;

    int dimension = 100;
    double seuil = 1.35335283237; //pow(10, -2);
    int iterations = 1;
    int coeur = 1;

    bool createFile = false;

    while ((opt = getopt(argc, argv, "d:s:i:c:f:t:")) != -1) {
        switch(opt) {
            case 'd':
                if(optarg) dimension = atoi(optarg);
                break;
            case 's':
                if(optarg) seuil = atof(optarg);
                break;
            case 'i':
                if(optarg) iterations = atoi(optarg);
                break;
            case 'c':
                if(optarg) coeur = atoi(optarg);
                break;
            case 'f':
                if(optarg) fichier = optarg;
                break;
            case 't':
                if(optarg) createFile = true;
                break;
            case '?':
                cout << "\nParametre incorrect";
                exit(1);
        }
    }

    if (createFile) {
        string fileName = "src/input/init" + std::to_string(dimension) + ".txt";
        std::ofstream outfile (fileName);

        for(int i=1; i<dimension-1;i++){
            for (int j=1; j<dimension-1; j++){
                double randomValue = rand()%((100 - 20) + 1) + 10;
                outfile << std::to_string(i) << " " << std::to_string(j) << " " << randomValue << std::endl;
            }
        }

        outfile.close();
    }

    if (fichier == "") {
        cout << "\nParametre -f est obligatoire";
        exit(1);
    }

    cout << "\nDimension=" << dimension;
    cout << "\nSeuil=" << seuil;
    cout << "\nIterations=" << iterations;
    cout << "\nCoeur=" << coeur;
    cout << "\nFichier=" << fichier;

    cout << "\n\n";

    //double tempsTotalParallele = 0;
    //double tempsTotalSequentielle = 0;

    // La boucle FOR est commenté, parce que ca sers juste pour le test de performance
    //int iteration = 1;
    //for(int i = 0; i<iteration; i++) {

    // Si vous voulez generer des fichier avec des noms differents dans le cas de la boucle

    c = executerSequentiel(dimension, seuil, iterations, coeur, fichier);

    //tempsTotalSequentielle+= c.get();

    c = executerParallele(dimension, seuil, iterations, coeur, fichier);
    //tempsTotalParallele+= c.get();
    //}

    //double tempsMoyenSeq = (tempsTotalSequentielle / iteration);
    //double tempsMoyenPar = (tempsTotalParallele / iteration);

    //cout << "Temps d'execution séquentiel moyen = \033[1;31m" << tempsMoyenSeq << " sec\033[0m" << endl;
    //cout << "Temps d'execution parallele moyen  = \033[1;31m" << tempsMoyenPar << " sec\033[0m" << endl;

}
