#include "bee.h"
//typedef double (*FunctionCallback)(double sol[D]);
 //bee::FunctionCallback bee::function = &Griewank;



    // http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
    // http://www.possibility.com/Cpp/CppCodingStandard.html
    // http://geosoft.no/development/cppstyle.html

    //#include <QApplication>
    //#include <signal.h>
    //#include <stdlib.h>
    //#include <assert.h>
    //#include <iostream>
    //#include "mainwindow.h"
    //#include "soccer.h"
    //#include <stdio.h>
    //#include <math.h>

    //#include <time.h>

             // std::chrono::seconds
    //using namespace std;

    /* ABC algorithm coded using C programming language */

    /* Artificial Bee Colony (ABC) is one of the most recently defined algorithms by Dervis Karaboga in 2005,
    motivated by the intelligent behavior of honey bees. */

    /* Referance Papers*/

    /*D. Karaboga, AN IDEA BASED ON HONEY BEE SWARM FOR NUMERICAL OPTIMIZATION,TECHNICAL REPORT-TR06, Erciyes University, Engineering Faculty, Computer Engineering Department 2005.*/

    /*D. Karaboga, B. Basturk, A powerful and Efficient Algorithm for Numerical Function Optimization: Artificial Bee Colony (ABC) Algorithm, Journal of Global Optimization, Volume:39, Issue:3,pp:459-171, November 2007,ISSN:0925-5001 , doi: 10.1007/s10898-007-9149-x */

    /*D. Karaboga, B. Basturk, On The Performance Of Artificial Bee Colony (ABC) Algorithm, Applied Soft Computing,Volume 8, Issue 1, January 2008, Pages 687-697. */

    /*D. Karaboga, B. Akay, A Comparative Study of Artificial Bee Colony Algorithm,  Applied Mathematics and Computation, 214, 108-132, 2009. */

    /*Copyright © 2009 Erciyes University, Intelligent Systems Research Group, The Dept. of Computer Engineering*/

    /*Contact:
    Dervis Karaboga (karaboga@erciyes.edu.tr )
    Bahriye Basturk Akay (bahriye@erciyes.edu.tr)
    */





    /* Control Parameters of ABC algorithm*/
  \

    /*Write your own objective function name instead of sphere*/


    /*Fitness function*/
    double bee::CalculateFitness(double fun)
     {
         double result=0;
         if(fun>=0)
         {
             result=1/(fun+1);
         }
         else
         {
             result=1+fabs(fun);
         }
         return result;
     }

    /*The best food source is memorized*/
    void bee::MemorizeBestSource()
    {
       int i,j;

        for(i=0;i<FoodNumber;i++)
        {
        if (f[i]<GlobalMin)
            {
            GlobalMin=f[i];
            for(j=0;j<D;j++)
               GlobalParams[j]=Foods[i][j];
            }
        }
     }

    /*Variables are initialized in the range [lb,ub]. If each parameter has different range, use arrays lb[j], ub[j] instead of lb and ub */
    /* Counters of food sources are also initialized in this function*/
    void bee::init(int index)
    {
       int j;
       for (j=0;j<D;j++)
            {
            r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
            Foods[index][j]=r*(ub-lb)+lb;
            solution[j]=Foods[index][j];
            }
        f[index]=Griewank(solution);
        fitness[index]=CalculateFitness(f[index]);
        trial[index]=0;
    }

    /*All food sources are initialized */
    void bee::initial()
    {
        int i;
        for(i=0;i<FoodNumber;i++)
        {
        init(i);
        }
        GlobalMin=f[0];
        for(i=0;i<D;i++)
        GlobalParams[i]=Foods[0][i];


    }

    void bee::SendEmployedBees()
    {
      int i,j;
      /*Employed Bee Phase*/
       for (i=0;i<FoodNumber;i++)
            {
            /*The parameter to be changed is determined randomly*/
            r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)) );
            param2change=(int)(r*D);

            /*A randomly chosen solution is used in producing a mutant solution of the solution i*/
            r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
            neighbour=(int)(r*FoodNumber);

            /*Randomly selected solution must be different from the solution i*/
            while(neighbour==i)
            {
            r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
            neighbour=(int)(r*FoodNumber);
            }
            for(j=0;j<D;j++)
            solution[j]=Foods[i][j];

            /*v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij}) */
            r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
            solution[param2change]=Foods[i][param2change]+(Foods[i][param2change]-Foods[neighbour][param2change])*(r-0.5)*2;

            /*if generated parameter value is out of boundaries, it is shifted onto the boundaries*/
            if (solution[param2change]<lb)
               solution[param2change]=lb;
            if (solution[param2change]>ub)
               solution[param2change]=ub;
            ObjValSol=Griewank(solution);
            FitnessSol=CalculateFitness(ObjValSol);

            /*a greedy selection is applied between the current solution i and its mutant*/
            if (FitnessSol>fitness[i])
            {
            /*If the mutant solution is better than the current solution i, replace the solution with the mutant and reset the trial counter of solution i*/
            trial[i]=0;
            for(j=0;j<D;j++)
            Foods[i][j]=solution[j];
            f[i]=ObjValSol;
            fitness[i]=FitnessSol;
            }
            else
            {   /*if the solution i can not be improved, increase its trial counter*/
                trial[i]=trial[i]+1;
            }


            }

            /*end of employed bee phase*/

    }

    /* A food source is chosen with the probability which is proportioal to its quality*/
    /*Different schemes can be used to calculate the probability values*/
    /*For example prob(i)=fitness(i)/sum(fitness)*/
    /*or in a way used in the metot below prob(i)=a*fitness(i)/max(fitness)+b*/
    /*probability values are calculated by using fitness values and normalized by dividing maximum fitness value*/
    void bee::CalculateProbabilities()
    {
         int i;
         double maxfit;
         maxfit=fitness[0];
      for (i=1;i<FoodNumber;i++)
            {
               if (fitness[i]>maxfit)
               maxfit=fitness[i];
            }

     for (i=0;i<FoodNumber;i++)
            {
             prob[i]=(0.9*(fitness[i]/maxfit))+0.1;
            }

    }

    void bee::SendOnlookerBees()
    {

      int i,j,t;
      i=0;
      t=0;
      /*onlooker Bee Phase*/
      while(t<FoodNumber)
            {

            r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
            if(r<prob[i]) /*choose a food source depending on its probability to be chosen*/
            {
            t++;

            /*The parameter to be changed is determined randomly*/
            r = ((double)rand() / ((double)(RAND_MAX)+(double)(1)) );
            param2change=(int)(r*D);

            /*A randomly chosen solution is used in producing a mutant solution of the solution i*/
            r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
            neighbour=(int)(r*FoodNumber);

            /*Randomly selected solution must be different from the solution i*/
            while(neighbour==i)
            {
            r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
            neighbour=(int)(r*FoodNumber);
            }
            for(j=0;j<D;j++)
            solution[j]=Foods[i][j];

            /*v_{ij}=x_{ij}+\phi_{ij}*(x_{kj}-x_{ij}) */
            r = (   (double)rand() / ((double)(RAND_MAX)+(double)(1)) );
            solution[param2change]=Foods[i][param2change]+(Foods[i][param2change]-Foods[neighbour][param2change])*(r-0.5)*2;

            /*if generated parameter value is out of boundaries, it is shifted onto the boundaries*/
            if (solution[param2change]<lb)
               solution[param2change]=lb;
            if (solution[param2change]>ub)
               solution[param2change]=ub;
            ObjValSol=Griewank(solution);
            FitnessSol=CalculateFitness(ObjValSol);

            /*a greedy selection is applied between the current solution i and its mutant*/
            if (FitnessSol>fitness[i])
            {
            /*If the mutant solution is better than the current solution i, replace the solution with the mutant and reset the trial counter of solution i*/
            trial[i]=0;
            for(j=0;j<D;j++)
            Foods[i][j]=solution[j];
            f[i]=ObjValSol;
            fitness[i]=FitnessSol;
            }
            else
            {   /*if the solution i can not be improved, increase its trial counter*/
                trial[i]=trial[i]+1;
            }
            } /*if */
            i++;
            if (i==FoodNumber)
            i=0;
            }/*while*/

            /*end of onlooker bee phase     */
    }

    /*determine the food sources whose trial counter exceeds the "limit" value. In Basic ABC, only one scout is allowed to occur in each cycle*/
    void bee::SendScoutBees()
    {
    int maxtrialindex,i;
    maxtrialindex=0;
    for (i=1;i<FoodNumber;i++)
            {
             if (trial[i]>trial[maxtrialindex])
             maxtrialindex=i;
            }
    if(trial[maxtrialindex]>=limit)
    {
        init(maxtrialindex);
    }
    }


    /*Main program of the ABC algorithm*/
    //int main()
    //{

    //}


    double bee::sphere(double sol[D])
    {
    int j;
    double top=0;
    for(j=0;j<D;j++)
    {
    top=top+sol[j]*sol[j];
    }
    return top;
    }

    double bee::Rosenbrock(double sol[D])
    {
    int j;
    double top=0;
    for(j=0;j<D-1;j++)
    {
    top=top+100*pow((sol[j+1]-pow((sol[j]),(double)2)),(double)2)+pow((sol[j]-1),(double)2);
    }
    return top;
    }

     double bee::Griewank(double sol[D])
     {
         /*int j;
         double top1,top2,top;
         top=0;
         top1=0;
         top2=1;
         for(j=0;j<D;j++)
         {
             top1=top1+pow((sol[j]),(double)2);
             top2=top2*cos((((sol[j])/sqrt((double)(j+1)))*M_PI)/180);

         }
         top=(1/(double)4000)*top1-top2+1;*/
         int j;
         double top1,top2,top;
         top=0;
         top1=0;
         top2=1;
         for(j=0;j<D;j++)
         {
             top1=250-sol[j];
             //top2=top2*cos((((sol[j])/sqrt((double)(j+1)))*M_PI)/180);
top+=top1;
         }

         return top;
     }

     double bee::Rastrigin(double sol[D])
     {
         int j;
         double top=0;

         for(j=0;j<D;j++)
         {
             top=top+(pow(sol[j],(double)2)-10*cos(2*M_PI*sol[j])+10);
         }
         return top;
     }

     //////////////////////////////////////////////
     /////////////////////////////////////////////////



     bee::bee()
    {


     }

     void bee::run()
     {
         //fun//ction = &(this->bee::Griewank);
             ////////////////////////////////
                int iter,run,j;
                double mean;
                mean=0;
                srand(time(NULL));

                for(run=0;run<runtime;run++)
                {

                initial();
                MemorizeBestSource();
                for (iter=0;iter<maxCycle;iter++)
                    {
                    SendEmployedBees();
                    CalculateProbabilities();
                    SendOnlookerBees();
                    MemorizeBestSource();
                    SendScoutBees();
                    }
                for(j=0;j<D;j++)
                        {
                            printf("GlobalParam[%d]: %f\n",j+1,GlobalParams[j]);
                        }
                printf("%d. run: %e \n",run+1,GlobalMin);
                GlobalMins[run]=GlobalMin;
                mean=mean+GlobalMin;
                }
                mean=mean/runtime;
                printf("Means of %d runs: %e\n",runtime,mean);
               // getchar();
     }


