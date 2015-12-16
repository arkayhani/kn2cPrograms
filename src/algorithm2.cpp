
#include "algorithm2.h"



 Individual Algorithm2::crossover(Individual indiv1, Individual indiv2)
{
    Individual* newSol = new Individual();
    // Loop through genes
    for (int i = 0; i < indiv1.size(); i++) {
        // Crossover
        if ((((double)random())/RAND_MAX) <= uniformRate) {
            newSol->setGene(i, indiv1.getGene(i));
        } else {
            newSol->setGene(i, indiv2.getGene(i));
        }
    }
    return *newSol;


}

 void Algorithm2::mutate(Individual indiv)
{
    // Loop through genes
    for (int i = 0; i < indiv.size(); i++) {
        if ((double)random() /2147483647<= mutationRate) {
            // Create random gene
            int gene = (int) round((double)random()/2147483647);
            indiv.setGene(i, gene);
        }
    }


}

 Individual Algorithm2::tournamentSelection(Population pop)
{
    // Create a tournament population
    Population* tournament = new Population(tournamentSize, false);
    // For each place in the tournament get a random individual

   for (int i = 0; i < tournamentSize; i++) {
        int randomId = (int) (((double)random()/2147483647) * pop.size());
        tournament->saveIndividual(i, *(pop.getIndividual(randomId)));
    }
    // Get the fittest
    Individual fittest = tournament->getFittest();
    return fittest;


}

 Algorithm2::Algorithm2()
{

}

 Population Algorithm2::evolvePopulation(Population pop)
{
qDebug()<<QString::number(pop.size());
    Population* newPopulation = new Population(pop.size(), false);

    // Keep our best individual
    if (elitism) {
        newPopulation->saveIndividual(0, pop.getFittest());
    }

    // Crossover population
    int elitismOffset;
    if (elitism) {
        elitismOffset = 1;
    } else {
        elitismOffset = 0;
    }

  //  Individual indiv1 = tournamentSelection(pop);


    // Loop over the population size and create new individuals with
    // crossover
    for (int i = elitismOffset; i < pop.size(); i++) {
        Individual indiv1 = tournamentSelection(pop);
        Individual indiv2 = tournamentSelection(pop);
        Individual newIndiv = crossover(indiv1, indiv2);
        newPopulation->saveIndividual(i, newIndiv);
    }

    // Mutate population
    for (int i = elitismOffset; i < newPopulation->size(); i++) {
        mutate(*(newPopulation->getIndividual(i)));
    }

    return *newPopulation;

}
