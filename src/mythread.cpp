// mythread.cpp

#include "mythread.h"
#include <QDebug>

int MyThread::a=10;
int MyThread::gennum=10;
int MyThread::popnum=10;
Soccer* MyThread::sc;
bool MyThread::start2=false;
//Population Algorithm::evolvePopulation(Population pop);
MyThread::MyThread(Soccer* s)
{
    sc=s;
    //this->mn=mn;
}

// We overrides the QThread's run() method here
// run() will be called when a thread starts
// the code will be shared by all threads

void MyThread::run()
{
    while(!start2){QThread::sleep(15);}
    QFile file("result.txt");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file

    qDebug()<<"genarating";
   // QString s="11110000000000000000000000";
  //  FitnessCalc::setSolution(s);
Individual::setDefaultGeneLength(32);
       // Create an initial population
       Population* myPop = new Population(popnum, true);
qDebug()<<"Staring ... in 5 seconds";
       // Evolve our population until we reach an optimum solution
       int generationCount = 0;
QThread::sleep(5);
int max=gennum;
      while (true) {//myPop->getFittest().getFitness() < FitnessCalc::getMaxFitness()
           generationCount++;
           if(max==0)
           {break;}
           max--;
qDebug()<<"Generation: " + QString::number(generationCount)+" Fittest: " + QString::number(myPop->getFittest().getFitness())+"genes:"+myPop->getFittest().toString();
           //System.out.println("Generation: " + generationCount + " Fittest: " + myPop.getFittest().getFitness());
bool* f1=new bool();
bool* f2=new bool();
out << QString("Generation: " + QString::number(generationCount)+" Fittest: " + QString::number(myPop->getFittest().getFitness())+"genes:"+myPop->getFittest().toString()).toLatin1()<<'\n';
         //out<<QString("Generation:").toLatin1();
*myPop = Algorithm2::evolvePopulation(*myPop);
QThread::sleep(2);

       }

file.close();

}
