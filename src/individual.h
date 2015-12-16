#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include "QString"
#include "fitnesscalc.h"
class Individual
{


private:
    int* genes;
    // Cache

public:
int fitness;

    static int defaultGeneLength ;
    // Create a random individual
     void generateIndividual() ;

    /* Getters and setters */
    // Use this if you want to create individuals with different gene lengths
     static void setDefaultGeneLength(int length) ;

     int getGene(int index) ;

     void setGene(int index, int value) ;

    /* Public methods */
     int size() ;

     int getFitness() ;


     QString toString();


    Individual();
};

#endif // INDIVIDUAL_H
