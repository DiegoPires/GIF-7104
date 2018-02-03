#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Chrono.hpp"

// methods declaration
void executeSequential(int argc, char **argv);
void executeParallel(int argc, char **argv);
void executeParallel2(int argc, char **argv);
void executeParallel3(int argc, char **argv);
void showResults(long iMax, char* iFlags);

// global variables
char *flagsParallel;
char *flagsParallel2;
char *flagsParallel3;

int maxValue = 1000;

// structures de données
struct ThreadInput{
    long begin, end, wheelFactor;
};

// Programme qui trouve à l'aide du Crible d'Ératosthène,
// tous les nombres premiers inférieurs à un certain seuil
// spécifié sur la ligne de commande.
int main(int argc, char *argv[]) {

    executeSequential(argc, argv);
    executeParallel(argc, argv);
    executeParallel2(argc, argv);
    executeParallel3(argc, argv);

    return 0;
}

void* executeEratosthene(void* iArgs) {

    struct ThreadInput *lThreadInput = (struct ThreadInput*)iArgs;

    unsigned int begin = (*lThreadInput).begin;
    unsigned int end = (*lThreadInput).end;
    unsigned int wheelFactor = (*lThreadInput).wheelFactor;

    for (unsigned long p=2; p < end; p++) {
        if (flagsParallel[p] == 0) {
            for (unsigned long i=2; i*p < end; i++) {
                flagsParallel[i*p]++;
            }
        }
    }

    pthread_exit(NULL);
}

void* executeEratosthene2(void* iArgs) {

    struct ThreadInput *lThreadInput = (struct ThreadInput*)iArgs;

    unsigned int wheelFactor = (*lThreadInput).wheelFactor;

    for (unsigned long p=2; p < maxValue; p+=wheelFactor-2) {
        if (flagsParallel2[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=2; i*p < maxValue; i+=wheelFactor-2) {
                flagsParallel2[i*p]++;
            }
        }
    }

    pthread_exit(NULL);
}

void* executeEratosthene3(void* iArgs) {

    struct ThreadInput *lThreadInput = (struct ThreadInput*)iArgs;

    unsigned int begin = (*lThreadInput).begin;
    unsigned int end = (*lThreadInput).end;

    if (begin == 1) begin = 2;

    for (unsigned long p=begin; p < maxValue; p++) {
        if (flagsParallel3[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=begin; i*p < maxValue; i++) {
                flagsParallel3[i*p]++;
            }
        }
    }

    pthread_exit(NULL);
}

void executeParallel(int argc, char *argv[]) {

    int lThreadCount = 1;
    long lItemPourThread;

    if (argc > 1) maxValue = atoi(argv[1]);
    if (argc > 2) lThreadCount = atoi(argv[2]);

    lItemPourThread = maxValue / lThreadCount;

    struct ThreadInput *lInput = nullptr;


    // Démarrer le chronomètre
    Chrono lChrono(true);

    flagsParallel = (char*) calloc(maxValue, sizeof(*flagsParallel));
    assert(flagsParallel != 0);

    // declaration des threads
    pthread_t eThread[lThreadCount];

    // creation des threads
    for(int i=0; i < lThreadCount; i++) {

        lInput = (struct ThreadInput*)malloc(sizeof *lInput);
        lInput->begin = i * lItemPourThread + 1 ;
        lInput->end = (i + 1) * lItemPourThread ;

        pthread_create(&eThread[i], NULL, executeEratosthene, lInput);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < lThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    showResults(maxValue, flagsParallel);

    printf("\nTemps d'execution parallèle    = %f sec\n", lChrono.get());
}

void executeParallel2(int argc, char *argv[]) {


    int lThreadCount = 1;
    long lItemPourThread;

    if (argc > 1) maxValue = atoi(argv[1]);
    if (argc > 2) lThreadCount = atoi(argv[2]);

    lItemPourThread = maxValue / lThreadCount;

    struct ThreadInput *lInput = nullptr;

    // Démarrer le chronomètre
    Chrono lChrono(true);

    flagsParallel2 = (char*) calloc(maxValue, sizeof(*flagsParallel2));
    assert(flagsParallel2 != 0);

    // declaration des threads
    pthread_t eThread[lThreadCount];


    lInput = (struct ThreadInput*)malloc(sizeof *lInput);
    /*lInput->wheelFactor = 2 ;

    executeEratosthene(lInput);

    lInput->wheelFactor = 3 ;
    executeEratosthene(lInput);

    lInput->wheelFactor = 5 ;
    executeEratosthene(lInput);
    */

    // creation des threads
    for(int i=0; i < lThreadCount; i++) {

        lInput->wheelFactor = lItemPourThread ;

        pthread_create(&eThread[i], NULL, executeEratosthene2, lInput);
    }

    for (unsigned long p=2; p < maxValue; p+=1) {
        if (flagsParallel2[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=2; i*p < maxValue; i+=1) {
                flagsParallel2[i*p]++;
            }
        }
    }

    for (unsigned long p=2; p < maxValue; p+=3) {
        if (flagsParallel2[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=2; i*p < maxValue; i+=3) {
                flagsParallel2[i*p]++;
            }
        }
    }

    for (unsigned long p=2; p < maxValue; p+=5) {
        if (flagsParallel2[p]== 0) {
            // invalider tous les multiples
            for (unsigned long i=2; i*p < maxValue; i+=5) {
                flagsParallel2[i*p]++;
            }
        }
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < lThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    showResults(maxValue, flagsParallel2);
    printf("\nTemps d'execution parallèle 2  = %f sec\n", lChrono.get());
}

void executeParallel3(int argc, char *argv[]) {

    int lThreadCount = 1;
    long lItemPourThread;

    if (argc > 1) maxValue = atoi(argv[1]);
    if (argc > 2) lThreadCount = atoi(argv[2]);

    lItemPourThread = maxValue / lThreadCount;

    struct ThreadInput *lInput = nullptr;

    // Démarrer le chronomètre
    Chrono lChrono(true);

    flagsParallel3 = (char*) calloc(maxValue, sizeof(*flagsParallel3));
    assert(flagsParallel3 != 0);

    // declaration des threads
    pthread_t eThread[lThreadCount];

    // creation des threads
    for(int i=0; i < lThreadCount; i++) {

        lInput = (struct ThreadInput*)malloc(sizeof *lInput);
        lInput->begin = i * lItemPourThread + 1 ;
        lInput->end = (i + 1) * lItemPourThread ;
        lInput->wheelFactor = lItemPourThread ;

        pthread_create(&eThread[i], NULL, executeEratosthene3, lInput);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < lThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    showResults(maxValue, flagsParallel3);

    printf("\nTemps d'execution parallèle 3  = %f sec\n", lChrono.get());
}

void executeSequential(int argc, char *argv[]) {

    // Déterminer la limite supérieure pour la recherche;
    // par défaut, prendre 1000
    long lMax = 1000;
    if (argc >= 2) {
        lMax = atol(argv[1]);
    }

    // Démarrer le chronomètre
    Chrono lChrono(true);

    // Allouer le tableau des drapeaux (flags) d'invalidation
    char *lFlags = (char*) calloc(lMax, sizeof(*lFlags));
    assert(lFlags != 0);

    // Appliquer le crible d'Ératosthène
    for (unsigned long p=2; p < lMax; p++) {
        if (lFlags[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=2; i*p < lMax; i++) {

                lFlags[i*p]++;
            }
        }
    }

    // Arrêter le chronomètre
    lChrono.pause();

    // Afficher les nombres trouvés à la console
    showResults(lMax, lFlags);

    // Afficher le temps d'exécution dans le stderr
    printf("\nTemps d'execution séquentielle = %f sec\n", lChrono.get());

}

void showResults(long iMax, char* iFlags){
    printf("\nChiffres: ");

    //for (unsigned long p=2; p<iMax; p++) {
        //if (iFlags[p] == 0) printf("%ld ", p);
    //}

}