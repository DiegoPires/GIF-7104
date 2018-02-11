#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Chrono.hpp"

// déclaration des méthods
int executerSequentiale(int iMaxValue, bool iShowResult);
int executerSequentialeOptimize(int iMaxValue, bool iShowResult);

void executerParallele(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult);
void executerParallele2(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult);
void executerParallele3(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult);
void executerParallele4(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult);
void executerParallele5(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult);
void executerParallele6(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult);

int obtenirResultats(int iMaxValue, bool iShowResult, char *iFlags);

// variables globales
char *flagsParallel;
char *flagsParallel2;
char *flagsParallel3;
char *flagsParallel4;
char *flagsParallel5;
char *flagsParallel6;

pthread_mutex_t lock_x;
pthread_mutex_t lock_x2;

// structures de données utilise dans les Threads
struct ThreadInput{
    long id, begin, end, wheelFactor;
};

// Programme qui trouve à l'aide du Crible d'Ératosthène,
// tous les nombres premiers inférieurs à un certain seuil
// spécifié sur la ligne de commande.
int main(int argc, char *argv[]) {

    int lThreadCount = 4, lMaxValue = 100;
    int lQttExecution = argc <= 1 ? 4 : 1;
    int lQttThread = argc <= 1 ? 4 : 1;

    bool lShowResult = argc <= 1 ? false : true;

    if (argc > 1) lMaxValue = atoi(argv[1]);
    if (argc > 2) lThreadCount = atoi(argv[2]);
    if (argc > 3) lShowResult = static_cast<bool>(argv[3]);

    // Dans le cas qu'on recois aucun parametre, calculer le temps d'execution
    // de chaque algorithme plusieurs fois, avec une quantité max d'elements
    // grandissent.
    // On fait ca pour gerer les donnes de performance pour le rapport final
    for (int lExecutionsThread=1; lExecutionsThread <= lQttThread; lExecutionsThread++) {

        for (int lExecutions=1; lExecutions <= lQttExecution; lExecutions++) {

            lMaxValue = argc > 1 ? lMaxValue : pow(10, lExecutions + 4);
            lThreadCount = argc > 2 ? lThreadCount : lExecutionsThread;

            printf("\n\n** Éxecution %d sur %d items et %d threads\n", lExecutions, lMaxValue, lThreadCount);

            int lQtt = executerSequentiale(lMaxValue, lShowResult);
            int lQttO = executerSequentialeOptimize(lMaxValue, lShowResult);

            executerParallele(lMaxValue, lThreadCount, lQtt, lShowResult);
            executerParallele2(lMaxValue, lThreadCount, lQtt, lShowResult);
            executerParallele3(lMaxValue, lThreadCount, lQtt, lShowResult);
            executerParallele4(lMaxValue, lThreadCount, lQtt, lShowResult);
            executerParallele5(lMaxValue, lThreadCount, lQtt, lShowResult);
            executerParallele6(lMaxValue, lThreadCount, lQtt, lShowResult);

        }
    }
    return 0;
}

// Calcul du debut, jusqu"a la fin du morceau (NO LOCK)
void* executerEratosthene(void *iArgs) {

    struct ThreadInput *lThreadInput = (struct ThreadInput*)iArgs;

    unsigned int end = (*lThreadInput).end;

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
    unsigned int end = (*lThreadInput).end;

    for (unsigned long p=2; p < end; p+=wheelFactor) {
        if (flagsParallel2[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=2; i*p < end; i+=wheelFactor) {
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

    for (unsigned long p=begin; p < end; p++) {
        if (flagsParallel3[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=begin; i*p < end; i++) {
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

    for (unsigned long p=begin; p < end; p++) {
        if (flagsParallel4[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=begin; i*p < end; i++) {
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

    unsigned int end = (*lThreadInput).end;

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

void* executerEratosthene6(void *iArgs) {

    struct ThreadInput *lThreadInput = (struct ThreadInput*)iArgs;

    unsigned int begin = (*lThreadInput).begin;
    unsigned int end = (*lThreadInput).end;

    if (begin == 1) begin = 2;

    for (unsigned long p=begin; p*p <= end; p++) {

        if (flagsParallel6[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=p*p; i <= end; i+=p) {
                flagsParallel6[i]++;
            }
        }
    }

    pthread_exit(NULL);
}

// Méthodes d'executions du sieve
void executerParallele(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult) {

    long lItemPourThread = iMaxValue / iThreadCount;

    struct ThreadInput *lInput = nullptr;

    // Démarrer le chronomètre
    Chrono lChrono(true);

    flagsParallel = (char*) calloc(iMaxValue, sizeof(*flagsParallel));
    assert(flagsParallel != 0);

    // declaration des threads
    pthread_t eThread[iThreadCount];

    // creation des threads
    for(int i=0; i < iThreadCount; i++) {

        lInput = (struct ThreadInput*)malloc(sizeof *lInput);
        lInput->begin = i * lItemPourThread + 1 ;
        lInput->end = (i + 1) * lItemPourThread ;

        pthread_create(&eThread[i], NULL, executerEratosthene, lInput);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < iThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    int lQtt = obtenirResultats(iMaxValue, iShowResult, flagsParallel);

    printf("\nTemps d'execution parallèle 1 (%s)  = %f sec\n", (lQtt == iComparableQtt ? "BON" : "PAS"), lChrono.get());
}

void executerParallele2(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult) {

    long lItemPourThread = iMaxValue / iThreadCount;

    struct ThreadInput *lInput = nullptr;

    // Démarrer le chronomètre
    Chrono lChrono(true);

    flagsParallel2 = (char*) calloc(iMaxValue, sizeof(*flagsParallel2));
    assert(flagsParallel2 != 0);

    // declaration des threads
    pthread_t eThread[iThreadCount];


    lInput = (struct ThreadInput*)malloc(sizeof *lInput);
    /*lInput->wheelFactor = 2 ;

    executeEratosthene(lInput);

    lInput->wheelFactor = 3 ;
    executeEratosthene(lInput);

    lInput->wheelFactor = 5 ;
    executerEratosthene(lInput);
    */

    // creation des threads
    for(int i=0; i < iThreadCount; i++) {

        lInput->wheelFactor = lItemPourThread ;
        lInput->end = iMaxValue ;

        pthread_create(&eThread[i], NULL, executerEratosthene2, lInput);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < iThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    int lQtt = obtenirResultats(iMaxValue, iShowResult, flagsParallel2);

    printf("\nTemps d'execution parallèle 2 (%s)  = %f sec\n", (lQtt == iComparableQtt ? "BON" : "PAS"), lChrono.get());
}

void executerParallele3(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult) {

    long lItemPourThread = iMaxValue / iThreadCount;

    struct ThreadInput *lInput = nullptr;

    // Démarrer le chronomètre
    Chrono lChrono(true);

    flagsParallel3 = (char*) calloc(iMaxValue, sizeof(*flagsParallel3));
    assert(flagsParallel3 != 0);

    // declaration des threads
    pthread_t eThread[iThreadCount];

    // creation des threads
    for(int i=0; i < iThreadCount; i++) {

        lInput = (struct ThreadInput*)malloc(sizeof *lInput);
        lInput->begin = i * lItemPourThread + 1 ;
        lInput->end = iMaxValue ;
        lInput->wheelFactor = lItemPourThread ;

        pthread_create(&eThread[i], NULL, executerEratosthene3, lInput);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < iThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    int lQtt = obtenirResultats(iMaxValue, iShowResult, flagsParallel3);

    printf("\nTemps d'execution parallèle 3 (%s)  = %f sec\n", (lQtt == iComparableQtt ? "BON" : "PAS"), lChrono.get());
}

void executerParallele4(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult) {

    long lItemPourThread = iMaxValue / iThreadCount;

    struct ThreadInput *lInput = nullptr;

    // Démarrer le chronomètre
    Chrono lChrono(true);

    flagsParallel4 = (char*) calloc(iMaxValue, sizeof(*flagsParallel4));
    assert(flagsParallel4 != 0);

    // declaration des threads
    pthread_t eThread[iThreadCount];

    // creation des threads
    for(int i=0; i < iThreadCount; i++) {

        lInput = (struct ThreadInput*)malloc(sizeof *lInput);
        lInput->begin = i * lItemPourThread + 1 ;
        lInput->end = iMaxValue ;
        lInput->wheelFactor = lItemPourThread ;

        pthread_create(&eThread[i], NULL, executerEratosthene4, lInput);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < iThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    int lQtt = obtenirResultats(iMaxValue, iShowResult, flagsParallel4);

    printf("\nTemps d'execution parallèle 4 (%s)  = %f sec\n", (lQtt == iComparableQtt ? "BON" : "PAS"), lChrono.get());
}

void executerParallele5(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult) {

    long lItemPourThread = iMaxValue / iThreadCount;

    struct ThreadInput *lInput = nullptr;

    // Démarrer le chronomètre
    Chrono lChrono(true);

    flagsParallel5 = (char*) calloc(iMaxValue, sizeof(*flagsParallel5));
    assert(flagsParallel5 != 0);

    // declaration des threads
    pthread_t eThread[iThreadCount];

    // creation des threads
    for(int i=0; i < iThreadCount; i++) {

        lInput = (struct ThreadInput*)malloc(sizeof *lInput);
        lInput->begin = i * lItemPourThread + 1 ;
        lInput->end = (i + 1) * lItemPourThread ;
        lInput->wheelFactor = lItemPourThread ;
        lInput->id = i ;

        pthread_create(&eThread[i], NULL, executerEratosthene5, lInput);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < iThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    int lQtt = obtenirResultats(iMaxValue, iShowResult, flagsParallel5);

    printf("\nTemps d'execution parallèle 5 (%s)  = %f sec\n", (lQtt == iComparableQtt ? "BON" : "PAS"), lChrono.get());
}

void executerParallele6(int iMaxValue, int iThreadCount, int iComparableQtt, bool iShowResult) {

    int lItemPourThread = sqrt(iMaxValue) / iThreadCount;

    struct ThreadInput *lInputThread = nullptr;

    // Démarrer le chronomètre
    Chrono lChrono(true);

    flagsParallel6 = (char*) calloc(iMaxValue, sizeof(*flagsParallel6));
    assert(flagsParallel6 != 0);

    // declaration des threads
    pthread_t eThread[iThreadCount];

    // creation des threads
    for(int i=0; i < iThreadCount; i++) {

        //weight = i+1;

        lInputThread = (struct ThreadInput*)malloc(sizeof *lInputThread);
        //lInputThread->begin = begin ;
        //lInputThread->end = begin*weight ;

        lInputThread->begin = i * lItemPourThread + 1 ;
        lInputThread->end = iMaxValue ;
        lInputThread->id = i ;

        pthread_create(&eThread[i], NULL, executerEratosthene6, lInputThread);
    }

    // attends jusqu'a que les threads s'executent
    for(int i=0; i < iThreadCount; i++){
        pthread_join(eThread[i], NULL);
    }

    lChrono.pause();

    // Afficher les nombres trouvés à la console
    int lQtt = obtenirResultats(iMaxValue, iShowResult, flagsParallel6);

    printf("\nTemps d'execution parallèle 6 (%s)  = %f sec\n", (lQtt == iComparableQtt ? "BON" : "PAS"), lChrono.get());
}

int executerSequentiale(int iMaxValue, bool iShowResult) {

    // Déterminer la limite supérieure pour la recherche;
    // par défaut, prendre 1000
    long lMax = iMaxValue;

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
    int lQtt = obtenirResultats(iMaxValue, iShowResult, lFlags);

    // Afficher le temps d'exécution dans le stderr
    printf("\nTemps d'execution séquentielle       = %f sec\n", lChrono.get());

    return lQtt;
}

int executerSequentialeOptimize(int iMaxValue, bool iShowResult) {

    // Déterminer la limite supérieure pour la recherche;
    // par défaut, prendre 1000
    long lMax = iMaxValue;

    // Démarrer le chronomètre
    Chrono lChrono(true);

    // Allouer le tableau des drapeaux (flags) d'invalidation
    char *lFlags2 = (char*) calloc(lMax, sizeof(*lFlags2));
    assert(lFlags2 != 0);

    for (unsigned long p=2; p*p <= lMax; p++) {

        if (lFlags2[p] == 0) {
            // invalider tous les multiples
            for (unsigned long i=p*p; i <= lMax; i+=p) {
                lFlags2[i]++;
            }
        }
    }

    // Arrêter le chronomètre
    lChrono.pause();

    // Afficher les nombres trouvés à la console
    int lQtt = obtenirResultats(iMaxValue, iShowResult, lFlags2);

    // Afficher le temps d'exécution dans le stderr
    printf("\nTemps d'execution séquentielle 2     = %f sec\n", lChrono.get());

    return lQtt;
}

// Méthode utilise pour afficher les resultats calcules du sieve
int obtenirResultats(int iMaxValue, bool iShowResult, char *iFlags){

    int lQuantite = 0;
    int lQuantiteAfficher = iMaxValue > 10000 ? 10000 : iMaxValue;

    if (iShowResult) printf("\nChiffres: ");

    for (unsigned long p=2; p<lQuantiteAfficher; p++) {
        if (iFlags[p] == 0) {
            lQuantite += 1;
            if (iShowResult) printf("%lu ", p);
        }
    }

    return lQuantite;
}