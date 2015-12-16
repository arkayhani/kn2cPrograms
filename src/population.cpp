#include "population.h"

Population::Population(int populationSize, bool initialise)
{
ps=populationSize;
    individuals = new Individual[populationSize];
    // Initialise population
    if (initialise) {
        // Loop and create individuals
        for (int i = 0; i < size(); i++) {
            Individual* newIndividual = new Individual();
            newIndividual->generateIndividual();
            saveIndividual(i, *newIndividual);
        }
    }

}

Individual* Population::getIndividual(int index)
{

    return &(individuals[index]);

}

Individual Population::getFittest()
{

        Individual* fittest=getIndividual(0);

    for (int i = 0; i < size(); i++) {

        int f=fittest->getFitness();

        Individual* s=getIndividual(i);

        int f2=s->getFitness();


        if (f <= f2) {
            fittest = getIndividual(i);
        }
    }
    return *fittest;

}

int Population::size()
{

    return ps;

}

void Population::saveIndividual(int index, Individual indiv)
{

    individuals[index] = indiv;

}

Population::Population()
{
    ps=10;
}
