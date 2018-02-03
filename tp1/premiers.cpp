#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Chrono.hpp"

// déclaration des méthods
void executerSequentiale(int argc, char **argv);
void executerParallele(int argc, char **argv);
void executerParallele2(int argc, char **argv);
void executerParallele3(int argc, char **argv);
void executerParallele4(int argc, char **argv);
void executerParallele5(int argc, char **argv);
void afficherResultats(long iMax, char *iFlags);

// variables globales
char *flagsParallel;
char *flagsParallel2;
char *flagsParallel3;
char *flagsParallel4;
char *flagsParallel5;

pthread_mutex_t lock_x;
pthread_mutex_t lock_x2;

int maxValue = 1000;

// structures de données
struct ThreadInput{
    long id, begin, end, wheelFactor;
};

// Programme qui trouve à l'aide du Crible d'Ératosthène,
// tous les nombres premiers inférieurs à un certain seuil
// spécifié sur la ligne de commande.
int main(int argc, char *argv[]) {

    executerSequentiale(argc, argv);
    executerParallele(argc, argv);
    executerParallele2(argc, argv);
    executerParallele3(argc, argv);
    executerParallele4(argc, argv);
    executerParallele5(argc, argv);

    return 0;
}

// Calcul du debut, jusqu"a la fin du morceau (NO LOCK)
void* executerEratosthene(void *iArgs) {

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

// Calcul tous les valeurs, avec l'increment de la quantite pour morceau
void* executerEratosthene2(void *iArgs) {

    struct ThreadInput *lThreadInput = (struct ThreadInput*)iArgs;

    unsigned int wheelFactor = (*lThreadInput).wheelFactor;

    for (unsigned long p=2; p < maxValue; p+=wheelFactor) {
        if (flagsParallel2[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=2; i*p < maxValue; i+=wheelFactor) {
                flagsParallel2[i*p]++;
            }
        }
    }

    pthread_exit(NULL);
}

// Calcul du debut du morceau, jusqu'a la fin de tout (NO LOCK)
void* executerEratosthene3(void *iArgs) {

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

// Calcul du debut du morceau, jusqu'a la fin de tout (WITH LOCK)
void* executerEratosthene4(void *iArgs) {

    struct ThreadInput *lThreadInput = (struct ThreadInput*)iArgs;

    unsigned int begin = (*lThreadInput).begin;
    unsigned int end = (*lThreadInput).end;

    if (begin == 1) begin = 2;

    for (unsigned long p=begin; p < maxValue; p++) {
        if (flagsParallel4[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=begin; i*p < maxValue; i++) {
                pthread_mutex_lock(&lock_x);
                flagsParallel4[i*p]++;
                pthread_mutex_unlock(&lock_x);
            }
        }
    }

    pthread_exit(NULL);
}

// Calcul du debut, jusqu'au morceau desire (WITH LOCK)
void* executerEratosthene5(void *iArgs) {

    struct ThreadInput *lThreadInput = (struct ThreadInput*)iArgs;

    unsigned int begin = (*lThreadInput).begin;
    unsigned int end = (*lThreadInput).end;
    unsigned int id = (*lThreadInput).id;
    unsigned int wheelFactor = (*lThreadInput).wheelFactor;

    if (begin == 1) begin = 2;

    for (unsigned long p=2; p < sqrt(end); p++) {

        if (flagsParallel5[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=2; i*p < end; i++) {
                // printf("\n%d - %d * %d = %d",id, i, p, i*p);

                if (flagsParallel5[i*p] == 0){
                    pthread_mutex_lock(&lock_x2);
                    flagsParallel5[i*p]++;
                    pthread_mutex_unlock(&lock_x2);
                }
            }
        }
    }

    pthread_exit(NULL);
}

void executerParallele(int argc, char **argv) {

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

        pthread_create(&eThread[i], NULL, executerEratosthene, lInput);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < lThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    afficherResultats(maxValue, flagsParallel);

    printf("\nTemps d'execution parallèle    = %f sec\n", lChrono.get());
}

void executerParallele2(int argc, char **argv) {


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
    executerEratosthene(lInput);
    */

    // creation des threads
    for(int i=0; i < lThreadCount; i++) {

        lInput->wheelFactor = lItemPourThread ;

        pthread_create(&eThread[i], NULL, executerEratosthene2, lInput);
    }

    for (unsigned long p=7; p < maxValue; p+=1) {
        if (flagsParallel2[p] == 0 && ((p % 2 == 0) || (p % 3 == 0) || (p % 5 == 0) || (p % 7 == 0))){
            flagsParallel2[p]++;
        }
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < lThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    afficherResultats(maxValue, flagsParallel2);
    printf("\nTemps d'execution parallèle 2  = %f sec\n", lChrono.get());
}

void executerParallele3(int argc, char **argv) {

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

        pthread_create(&eThread[i], NULL, executerEratosthene3, lInput);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < lThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    afficherResultats(maxValue, flagsParallel3);

    printf("\nTemps d'execution parallèle 3  = %f sec\n", lChrono.get());
}

void executerParallele4(int argc, char **argv) {

    int lThreadCount = 1;
    long lItemPourThread;

    if (argc > 1) maxValue = atoi(argv[1]);
    if (argc > 2) lThreadCount = atoi(argv[2]);

    lItemPourThread = maxValue / lThreadCount;

    struct ThreadInput *lInput = nullptr;

    // Démarrer le chronomètre
    Chrono lChrono(true);

    flagsParallel4 = (char*) calloc(maxValue, sizeof(*flagsParallel4));
    assert(flagsParallel4 != 0);

    // declaration des threads
    pthread_t eThread[lThreadCount];

    // creation des threads
    for(int i=0; i < lThreadCount; i++) {

        lInput = (struct ThreadInput*)malloc(sizeof *lInput);
        lInput->begin = i * lItemPourThread + 1 ;
        lInput->end = (i + 1) * lItemPourThread ;
        lInput->wheelFactor = lItemPourThread ;

        pthread_create(&eThread[i], NULL, executerEratosthene4, lInput);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < lThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    afficherResultats(maxValue, flagsParallel4);

    printf("\nTemps d'execution parallèle 4  = %f sec\n", lChrono.get());
}

void executerParallele5(int argc, char **argv) {

    int lThreadCount = 1;
    long lItemPourThread;

    if (argc > 1) maxValue = atoi(argv[1]);
    if (argc > 2) lThreadCount = atoi(argv[2]);

    lItemPourThread = maxValue / lThreadCount;

    struct ThreadInput *lInput = nullptr;

    // Démarrer le chronomètre
    Chrono lChrono(true);

    flagsParallel5 = (char*) calloc(maxValue, sizeof(*flagsParallel5));
    assert(flagsParallel5 != 0);

    // declaration des threads
    pthread_t eThread[lThreadCount];

    // creation des threads
    for(int i=0; i < lThreadCount; i++) {

        lInput = (struct ThreadInput*)malloc(sizeof *lInput);
        lInput->begin = i * lItemPourThread + 1 ;
        lInput->end = (i + 1) * lItemPourThread ;
        lInput->wheelFactor = lItemPourThread ;
        lInput->id = i ;

        pthread_create(&eThread[i], NULL, executerEratosthene5, lInput);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < lThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    afficherResultats(maxValue, flagsParallel5);

    printf("\nTemps d'execution parallèle 5  = %f sec\n", lChrono.get());
}

void executerSequentiale(int argc, char **argv) {

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
    afficherResultats(lMax, lFlags);

    // Afficher le temps d'exécution dans le stderr
    printf("\nTemps d'execution séquentielle = %f sec\n", lChrono.get());

}

void afficherResultats(long iMax, char *iFlags){

    if (iMax < 1000){
        printf("\nChiffres: ");

        for (unsigned long p=2; p<iMax; p++) {
            if (iFlags[p] == 0) printf("%lu ", p);
        }
    }
}