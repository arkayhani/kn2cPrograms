#ifndef ALGORITHM2_H
#define ALGORITHM2_H

#include "population.h"
#include "cmath"
class Algorithm2
{
private: static constexpr double uniformRate = 0.5;
       static constexpr double mutationRate = 0.02;
        static const int tournamentSize = 15;
        static const bool elitism = false;
         static Individual crossover(Individual indiv1, Individual indiv2);
         static void mutate(Individual indiv);
        static Individual tournamentSelection(Population pop);
public:
    Algorithm2();
 static Population evolvePopulation(Population pop);

};

#endif // ALGORITHM_H
