#ifndef POPULATION_H
#define POPULATION_H
#include "individual.h"
class Population
{
    public:
    Individual* individuals;

int ps;
    /*
     * Constructors
     */
    // Create a population
     Population(int populationSize, bool initialise) ;

    /* Getters */
     Individual* getIndividual(int index) ;

     Individual getFittest() ;

    /* Public methods */
    // Get population size
     int size() ;

    // Save individual
     void saveIndividual(int index, Individual indiv) ;


    Population();
};

#endif // POPULATION_H
