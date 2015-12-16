#ifndef BEE_H
#define BEE_H

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
 #include <time.h>
#include <stdint.h>


#include <QThread>


#define NP 40 /* The number of colony size (employed bees+onlooker bees)*/
#define FoodNumber NP/2 /*The number of food sources equals the half of the colony size*/
#define limit 100  /*A food source which could not be improved through "limit" trials is abandoned by its employed bee*/
#define maxCycle 3000 /*The number of cycles for foraging {a stopping criteria}*/

/* Problem specific variables*/
#define D 10 /*The number of parameters of the problem to be optimized*/
#define lb 0 /*lower bound of the parameters. */
#define ub 255 /*upper bound of the parameters. lb and ub can be defined as arrays for the problems of which parameters have different bounds*/

#define runtime 9  /*Algorithm can be run many times in order to see its robustness*/
class bee: public QThread
{
    // http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
    // http://www.possibility.com/Cpp/CppCodingStandard.html
    // http://geosoft.no/development/cppstyle.html

    //#include <QApplication>
    //#include <signal.h>
  // #include <stdlib.h>
    //#include <assert.h>
    //#include <iostream>
    //#include "mainwindow.h"
    //#include "soccer.h"


             // std::chrono::seconds
    //using namespace std;

    /* ABC algorithm coded using C programming language */




    /* Control Parameters of ABC algorithm*/

public:

    double Foods[FoodNumber][D]; /*Foods is the population of food sources. Each row of Foods matrix is a vector holding D parameters to be optimized. The number of rows of Foods matrix equals to the FoodNumber*/
    double f[FoodNumber];  /*f is a vector holding objective function values associated with food sources */
    double fitness[FoodNumber]; /*fitness is a vector holding fitness (quality) values associated with food sources*/
    double trial[FoodNumber]; /*trial is a vector holding trial numbers through which solutions can not be improved*/
    double prob[FoodNumber]; /*prob is a vector holding probabilities of food sources (solutions) to be chosen*/
    double solution [D]; /*New solution (neighbour) produced by v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij}) j is a randomly chosen parameter and k is a randomlu chosen solution different from i*/
    double ObjValSol; /*Objective function value of new solution*/
    double FitnessSol; /*Fitness value of new solution*/
    int neighbour, param2change; /*param2change corrresponds to j, neighbour corresponds to k in equation v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij})*/
    double GlobalMin; /*Optimum solution obtained by ABC algorithm*/
    double GlobalParams[D]; /*Parameters of the optimum solution*/
    double GlobalMins[runtime]; /*GlobalMins holds the GlobalMin of each run in multiple runs*/
    double r; /*a random number in the range [0,1)*/

    /*a function pointer returning double and taking a D-dimensional array as argument */
    /*If your function takes additional arguments then change function pointer definition and lines calling "...=function(solution);" in the code*/


    /*benchmark functions */
    double sphere(double sol[D]);
    double Rosenbrock(double sol[D]);
    double Griewank(double sol[D]);
    double Rastrigin(double sol[D]);

    /*Write your own objective function name instead of sphere*/

//FunctionCallback function = &Griewank;/*Fitness function*/
    double CalculateFitness(double fun);

    /*The best food source is memorized*/
    void MemorizeBestSource();

    /*Variables are initialized in the range [lb,ub]. If each parameter has different range, use arrays lb[j], ub[j] instead of lb and ub */
    /* Counters of food sources are also initialized in this function*/
    void init(int index);

    /*All food sources are initialized */
    void initial() ;

    void SendEmployedBees();
            /*end of employed bee phase*/
    //typedef double (*FunctionCallback)(double sol[D]);
    //    FunctionCallback function;// = &bee::Griewank;


    /* A food source is chosen with the probability which is proportioal to its quality*/
    /*Different schemes can be used to calculate the probability values*/
    /*For example prob(i)=fitness(i)/sum(fitness)*/
    /*or in a way used in the metot below prob(i)=a*fitness(i)/max(fitness)+b*/
    /*probability values are calculated by using fitness values and normalized by dividing maximum fitness value*/
    void CalculateProbabilities();

    void SendOnlookerBees();

    /*determine the food sources whose trial counter exceeds the "limit" value. In Basic ABC, only one scout is allowed to occur in each cycle*/
    void SendScoutBees();


    /*Main program of the ABC algorithm*/
    //int main()
    //{

    //}







    bee();
    void run();
};

#endif // BEE_H
