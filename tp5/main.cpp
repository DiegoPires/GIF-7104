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

#include <unistd.h>
#include <math.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>

using namespace std;

void showHelp(){
    cout << "main <command>\n\n";
    cout << "-h       help\n";
    cout << "-d       size of the matrix\n";
    cout << "-f       path of the initialization matrix - Ex: 'src/input/init100.txt'\n";
    cout << "-s       threshold indicating when to stop the process\n";
    cout << "-i       between how many iterations we should generate an output\n";
    cout << "-c       how many process to use in the parallelisation\n";
    cout << "-t       creates a new file with the dimension provided and overwrite the -f parameter with the file created\n";
    cout << "-p       ignores all the other parameters and run a performance test\n";
}

string createInitializationFile(int dimension){

    string fileName = "src/input/init" + std::to_string(dimension) + ".txt";

    if (!( access( fileName.c_str(), F_OK ) != -1 ))
    {
        std::ofstream outfile (fileName);

        for(int i=1; i<dimension-1;i++){
            for (int j=1; j<dimension-1; j++){
                double randomValue = rand()%((100 - 20) + 1) + 10;
                outfile << std::to_string(i) << " " << std::to_string(j) << " " << randomValue << std::endl;
            }
        }

        outfile.close();
    }

    return fileName;
}

int main(int argc, char *argv[]) {

    Chrono c;

    const char *fichier = "";

    int opt = 0;

    int dimension = 100;
    double seuil = 1.35335283237; //pow(10, -2);
    int iterations = 1;
    int coeur = 1;

    bool createFile = false;
    bool performanceTest = false;

    while ((opt = getopt(argc, argv, "d:s:i:c:f:t:h:p:")) != -1) {
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
            case 'p':
                if(optarg) performanceTest = true;
                break;
            case '?':
                cout << "\nParametre incorrect";
                exit(1);
            case 'h':
                showHelp();
                exit(1);
        }
    }

    if (createFile) {
        string fileName = createInitializationFile(dimension);
        fichier = &fileName[0u];
    }

    if (fichier == "" && performanceTest == false) {
        cout << "\nParametre -f est obligatoire";
        exit(1);
    }

    if (!performanceTest) {
        cout << "** Parametres d'entre";
        cout << "\nDimension=" << dimension;
        cout << "\nSeuil=" << seuil;
        cout << "\nIterations=" << iterations;
        cout << "\nCoeur=" << coeur;
        cout << "\nFichier=" << fichier;

        cout << "\n\n";

        executerSequentiel(dimension, seuil, iterations, coeur, fichier, performanceTest);

        // std::this_thread::sleep_for (std::chrono::seconds(2));

        executerParallele(dimension, seuil, iterations, coeur, fichier, performanceTest);
    }
    else {

        cout << "**** Test de performance" << endl;

        cout << "Dimension ; Séquentiel ";
        for (int i=1; i<=10; i++) {
            cout << " ; Parallele " << i;
        }
        cout << endl;

        for (int i=10, j=1; i<=10000; i*=5, j++){
            dimension = i;
            string fileName = createInitializationFile(dimension);
            fichier = &fileName[0u];

            cout << dimension << "x" << dimension << " ; ";

            c = executerSequentiel(dimension, seuil, iterations, coeur, fichier, true);

            double oldTime = c.get();
            cout << " \033[1;31m" << oldTime << " \033[0m ; ";

            for (int i=1; i<=10; i++) {
                c = executerSequentiel(dimension, seuil, iterations, coeur, fichier, true);
                double newTime = c.get();
                double speedyUp = oldTime / newTime;
                double efficacity = speedyUp / i;
                cout << " \033[1;31m" << newTime << " \033[0m ; ";

                // cout << " ; " << speedyUp;
                // cout << " ; " << efficacity;
                // cout << endl;
            }

            cout << endl;
        }
    }
}
