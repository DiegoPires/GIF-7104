#pragma once

#include "../lib/Chrono.hpp"
#include "Shared.cpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

// FONT: https://math.berkeley.edu/~wilken/228A.F07/chr_lecture.pdf

// Set grid size and number of iterations
const int saveiters=20;
const int totaliters=200;
const int errorevery=2;
const int m=33,n=33;
const double xmin=-1,xmax=1;
const double ymin=-1,ymax=1;

// Compute useful constants
// TODO: comprendre mieux ca
const double pi=3.1415926535897932384626433832795;
const double omega=2/(1+sin(2*pi/n));
const double dx=(xmax-xmin)/(m-1);
const double dy=(ymax-ymin)/(n-1);
const double dxxinv=1/(dx*dx);
const double dyyinv=1/(dy*dy);
const double dcent=1/(2*(dxxinv+dyyinv));

// TODO: deplacer ces deux methodes dans Shared.cpp... peut-etre qqs constants en haut aussi
inline double f(int i,int j) {
    double x = xmin + i * dx, y = ymin + j * dy;
    return abs(x) > 0.5 || abs(y) > 0.5 ? 0 : 1;
}

// TODO: remplacer ca pour le methode que vas generer les images
void outputanderror(char* filename,double *a,const int sn) {
    // Computes the error if sn%error every==0
    if(sn%errorevery==0) {

        double z,error=0;int ij;
        for(int j=1;j<n-1;j++) {
            for(int i=1;i<m-1;i++) {
                ij=i+m*j;
                z=f(i,j)-a[ij]*(2*dxxinv+2*dyyinv)
                +dxxinv*(a[ij-1]+a[ij+1])
                +dyyinv*(a[ij-m]+a[ij+m]);
                error+=z*z;
            }
        }
        cout << sn << " " << error*dx*dy << endl;
    }

    // Saves the matrix if sn<=saveiters
    if(sn<=saveiters) {
        int i,j,ij=0,ds=sizeof(float);
        float x,y,datafloat;const char *pfloat;
        pfloat=(const char*)&datafloat;
        ofstream outfile;
        static char fname[256];
        sprintf(fname,"%s.%d",filename,sn);
        outfile.open(fname,fstream::out | fstream::trunc | fstream::binary);
        datafloat=m;
        outfile.write(pfloat,ds);
        for(i=0;i<m;i++) {
            x=xmin+i*dx;
            datafloat=x;
            outfile.write(pfloat,ds);
        }
        for(j=0;j<n;j++) {
            y=ymin+j*dy;
            datafloat=y;
            outfile.write(pfloat,ds);
            for(i=0;i<m;i++) {
                datafloat=a[ij++];
                outfile.write(pfloat,ds);
            }
        }
        outfile.close();
    }
}

Chrono executerSequentiel(int dimension, double seuil, int iterations, int coeur, string fichier) {

    int i, j, ij, k;
    // TODO: c'est pas m*n, cest la dimension recu en parametre (?)
    double error, u[m * n], z;

    Chrono lChrono(true);

    //lChrono.pause();

    //lChrono.resume();

    // TODO: remplacer l'initialisation toujours a 0 pour la lecture du fichier
    for (ij = 0; ij < m * n; ij++) u[ij] = 0;
    outputanderror("gsrb_out",u,0);
    //output and error("gsrb out",u,0);

    // Compute Red-Black Gauss-Seidel iteration
    // TODO: replancer le for pour un while par rapport au seuil
    // TODO: N<est pas oublier de pas calculer les bordes
    for (k = 1; k <= totaliters; k++) {
        for (j = 1; j < n - 1; j++) {
            for (i = 1 + (j & 1); i < m - 1; i += 2) {
                ij = i + m * j;
                u[ij] = (f(i, j) + dxxinv * (u[ij - 1] + u[ij + 1])
                         + dyyinv * (u[ij - m] + u[ij + m])) * dcent;
            }
        }
        for (j = 1; j < n - 1; j++) {
            for (i = 2 - (j & 1); i < m - 1; i += 2) {
                ij = i + m * j;
                u[ij] = (f(i, j) + dxxinv * (u[ij - 1] + u[ij + 1])
                         + dyyinv * (u[ij - m] + u[ij + m])) * dcent;
            }
        }

        // Save the result and compute error if necessary
        outputanderror("gsrb_out",u,k);
    }

    lChrono.pause();

    cout << "Temps d'execution séquentiel = \033[1;31m" << lChrono.get() << " sec\033[0m" << endl;

    return lChrono;
}