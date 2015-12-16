#ifndef FITNESSCALC_H
#define FITNESSCALC_H
#include "qstring.h"
#include "qthread.h"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include "spline.h"
#include "soccer.h"
class prepoint{
public:
    int x;
    int y;
    int z;
    int fx;
};
class FitnessCalc
{
    public:
static std::list<prepoint> prepoints;
    static int* solution;
static bool flag1;
static bool flag2;
static bool aprox;
static int def1;
static int def2;
static int def3;
static int def4;
static int def5;
static int att1;
static int att2;
static int att3;
static int att4;
static int att5;
static int aproxrange;
static double aproxerr;
    /* Public methods */
    // Set a candidate solution as a byte array
     static void setSolution(int newSolution[]) ;

    // To make it easier we can use this method to set our candidate solution
    // with string of 0s and 1s
    static void setSolution(QString newSolution) ;

    // Calculate inidividuals fittness by comparing it to our candidate solution
    static int getFitness(int indiv[]) ;

    // Get optimum fitness
    static int getMaxFitness() ;


    FitnessCalc();
};

#endif // FITNESSCALC_H
