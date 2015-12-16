#include "individual.h"
int Individual::defaultGeneLength=32;
void Individual::generateIndividual()
{
    for (int i = 0; i < size(); i++) {
        int gene = (int) qRound(((double)rand()/2147483647));
        genes[i] = gene;
    }


}

void Individual::setDefaultGeneLength(int length)
{
    defaultGeneLength = length;


}

int Individual::getGene(int index)
{
    return genes[index];


}

void Individual::setGene(int index, int value)
{
    genes[index] = value;
    fitness = 0;


}

int Individual::size()
{
    return defaultGeneLength;


}

int Individual::getFitness()
{

    if (fitness == 0) {
        fitness = FitnessCalc::getFitness(genes);
    }
    return fitness;

}

QString Individual::toString()
{
    QString geneString = "";
    for (int i = 0; i < size()/8; i++) {
        geneString +="coef["+QString::number(i+1)+"]: ";
        for (int j = 0; j < 8; j++) {
            geneString += QString::number(getGene(i*8+j));
        }
      //  geneString += QString::number(getGene(i));
    }
    return geneString;


}

Individual::Individual()
{
    genes = new int[defaultGeneLength];
    //defaultGeneLength ;
    fitness = 0;
}
