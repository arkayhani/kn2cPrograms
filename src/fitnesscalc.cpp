#include "fitnesscalc.h"
#include "mythread.h"
int* FitnessCalc::solution;
bool FitnessCalc::flag1=false;
bool FitnessCalc::flag2=false;
bool FitnessCalc::aprox=false;
int FitnessCalc::def1=10;
int FitnessCalc::def2=10;
int FitnessCalc::def3=10;
int FitnessCalc::def4=10;
int FitnessCalc::def5=10;
int FitnessCalc::att1=10;
int FitnessCalc::att2=10;
int FitnessCalc::att3=10;
int FitnessCalc::att4=10;
int FitnessCalc::att5=10;
int FitnessCalc::aproxrange=10;
double FitnessCalc::aproxerr=0.1;
std::list<prepoint> FitnessCalc::prepoints;
void FitnessCalc::setSolution(int newSolution[])
{

    solution = newSolution;


}

void FitnessCalc::setSolution(QString newSolution)
{

    solution = new int[newSolution.length()];
    // Loop through each character of our string and save it in our byte
    // array
    for (int i = 0; i < newSolution.length(); i++) {
        QString character = newSolution.mid(i,1);//.substring(i, i + 1);
        if (character.contains("0") || character.contains("1")) {
            solution[i] = character.toInt();
        } else {
            solution[i] = 0;
        }
    }

}

int FitnessCalc::getFitness(int indiv[])
{
Soccer* sc=MyThread::sc;
    int fitness = 0;

    int answer;


    QString geneString = "";
    for (int i = 0; i<32; i++) {
        geneString += QString::number(indiv[i]);
    }
    qDebug()<<geneString;
   // geneString="1101";
    bool* ffff=new bool();
    bool* ffff1=new bool();
    bool* ffff2=new bool();
    bool* ffff3=new bool();
    QString s1=geneString.mid(0,8);
    QString s2=geneString.mid(8,8);
    QString s3=geneString.mid(16,8);
    QString s4=geneString.mid(24,8);


    if(flag1){
std::list<prepoint> po1;


        sc->wm->pos_coef[1]=s1.toInt(ffff,2);
        sc->wm->pos_coef[2]=s2.toInt(ffff1,2);
        sc->wm->pos_coef[3]=s3.toInt(ffff2,2);
        sc->wm->pos_coef[4]=s4.toInt(ffff3,2);
        int c=prepoints.size();

        qDebug()<<"individual count:"<< QString::number(c);
       if(aprox){

        foreach (prepoint item, prepoints){

         int xdif=abs(qRound(item.x-sc->wm->pos_coef[3]));
         int ydif=abs(qRound(item.y-sc->wm->pos_coef[4]));
         int zdif=abs(qRound(item.z-sc->wm->pos_coef[2]));
         if(xdif<aproxrange&&ydif<aproxrange&&zdif<aproxrange)
         {

             if(po1.size()>4)
             {
                 foreach (prepoint p, po1) {
                     int xdif2=abs(qRound(p.x-sc->wm->pos_coef[3]));
                     int ydif2=abs(qRound(p.y-sc->wm->pos_coef[4]));
                     int zdif2=abs(qRound(p.z-sc->wm->pos_coef[2]));
                     if(xdif+ydif+zdif<xdif2+ydif2+zdif2)
                     {
                         p.x=item.x;
                         p.y=item.y;
                         p.z=item.z;
                         p.fx=item.fx;
                         break;

                     }
                 }
             }else{prepoint *temp=new prepoint;
                 temp->x=item.x;
                 temp->y=item.y;
                 temp->z=item.z;
                 temp->fx=item.fx;
             po1.push_back(*temp);
             }
         }
        }
        int c=po1.size();
qDebug()<<"neighbors:"<<QString::number(c);
      if(po1.size()>4){


      /*  std::vector<double> X(5), Y(5);
        int i=0;
        foreach (prepoint p, po1)
        {
            X[i]=p.x;
            Y[i]=p.fx;
             qDebug()<<"fx atx is"<<QString::number(p.x);

            i++;
        }
        tk::spline s;
        s.set_points(X,Y);    // currently it is required that X is already sorted

        double x=sc->wm->pos_coef[3];


        qreal ans1=s(x);
       i=0;
        foreach (prepoint p, po1)
        {
            X[i]=p.y;
            Y[i]=p.fx;

            qDebug()<<"fx aty is"<<QString::number(p.y);
            i++;
        }
        tk::spline s1;
        s1.set_points(X,Y);    // currently it is required that X is already sorted

        x=sc->wm->pos_coef[4];


        qreal ans2=s1(x);
        i=0;
        foreach (prepoint p, po1)
        {
            X[i]=p.z;
            Y[i]=p.fx;

            qDebug()<<"fx atz is"<<QString::number(p.z);
            i++;
        }
        tk::spline s2;
        s2.set_points(X,Y);    // currently it is required that X is already sorted

         x=sc->wm->pos_coef[2];


        qreal ans3=s2(x);*/
          qreal ans=0;
          foreach (prepoint p, po1)
          {


              ans+=p.fx;
          }
      int re=  qRound((ans)/5);
      qreal difans=0;
      foreach (prepoint p, po1)
      {


          difans+=p.fx;
      }


       qreal error_ans=((qreal)difans/5)/ans;

       qDebug()<<"error"<<QString::number(round(error_ans));
       if(error_ans<aproxerr)
       return ans;
      }
        }


        qreal* result=new qreal[4];
          result[0]=0;
          result[1]=0;
          result[2]=0;
          result[3]=0;
          //int sco=
          for (int var = 0; var < 100; var++) {



       QThread::msleep(100);

           QList<int> oppPlayers = sc->wm->kn->findNearestOppositeTo(sc->wm->ball.pos.loc);

           oppPlayers.removeOne(sc->wm->ref_goalie_opp);

           QList<int> ourPlayers = sc->wm->kn->findAttackers();

           ourPlayers.removeOne(sc->wm->ref_goalie_our);
           double *sum=new double[4];
           sum[0]=0;
           sum[1]=0;
           sum[2]=0;
           sum[3]=0;
           for (int i = 0; i <ourPlayers.size() ; i++) {
               double *near=new double[4];
               near[0]=100000;

               near[2]=100000;
               near[3]=100000;
               for (int j= 0;  j< oppPlayers.size(); j++) {



                double dist =(((sc->wm->ourRobot[ourPlayers.at(i)].pos.loc -
                                 sc->wm->oppRobot[oppPlayers.at(j)].pos.loc).length() )/100);

               if(near[0]>dist)
               near[0]=dist;




               }
               near[1] =-(qRound((double)((Field::oppGoalCenter- sc->wm->ourRobot[ourPlayers.at(i)].pos.loc).length() / 100)));
       near[2] = sc->wm->kn->scoringChance(sc->wm->ourRobot[ourPlayers.at(i)].pos.loc) / 100.0;

               sum[0]+=near[0];
               sum[1]+=near[1];
               sum[2]+=near[2];
               sum[3]+=near[3];


           }

           if(sum[0]==0)
               var--;
           else
           {result[0]+=sum[0];
           result[1]+=sum[1];
           result[2]+=sum[2];
           result[3]+=sum[3];}
          }

          result[0]=result[0]/100;
          result[1]=(10000+result[1])/100;
          result[2]=result[2]/10;

           qDebug()<<"result:"+QString::number(result[0])+" pos_coef[1]:"+QString::number(sc->wm->pos_coef[1]);
       qDebug()<<"result:"+QString::number(result[1])+" pos_coef[2]:"+QString::number(sc->wm->pos_coef[2]);
       qDebug()<<"result:"+QString::number(result[2])+" pos_coef[3]:"+QString::number(sc->wm->pos_coef[3]);

       answer=qRound(result[1])*att2;
       answer+=qRound(result[2])*att1;
       answer+=qRound(result[0])*att3;
       answer+=qRound(result[4])*att4;
           qDebug()<<"result:"+QString::number(answer);
         prepoint *temp=new prepoint;
         temp->x=sc->wm->pos_coef[3];
        temp->y= sc->wm->pos_coef[4];
        temp->z=sc->wm->pos_coef[2];
        temp->fx=answer;
        prepoints.push_back(*temp);


    }
 if(flag2){

        sc->wm->mark_coef[1]=s1.toInt(ffff,2);
        sc->wm->mark_coef[2]=s2.toInt(ffff1,2);
        sc->wm->mark_coef[3]=s3.toInt(ffff2,2);
        sc->wm->mark_coef[4]=s4.toInt(ffff3,2);

        qreal* result=new qreal[4];
          result[0]=0;
          result[1]=0;
          result[2]=0;
          result[3]=0;
          for (int var = 0; var < 100; var++) {



       QThread::msleep(100);
           QList<int> oppPlayers = sc->wm->kn->findNearestOppositeTo(sc->wm->ball.pos.loc);

           oppPlayers.removeOne(sc->wm->ref_goalie_opp);

           QList<int> ourPlayers = sc->wm->kn->findAttackers();
           ourPlayers.removeOne(sc->wm->ref_goalie_our);
           double *sum=new double[4];
           sum[0]=0;
           sum[1]=0;
           sum[2]=0;
           sum[3]=0;
           for (int i = 0; i <oppPlayers.size() ; i++) {
               double *near=new double[4];
               near[0]=100000;

               near[2]=100000;
               near[3]=100000;
               for (int j= 0;  j< ourPlayers.size(); j++) {



                double dist =(((sc->wm->ourRobot[ourPlayers.at(j)].pos.loc -
                                 sc->wm->oppRobot[oppPlayers.at(i)].pos.loc).length() )/100);

               if(near[0]>dist)
               near[0]=dist;




               }
               near[1] =(qRound((double)((Field::ourGoalCenter- sc->wm->oppRobot[oppPlayers.at(i)].pos.loc).length() / 100)));
                near[2] = sc->wm->kn->scoringChance(sc->wm->oppRobot[oppPlayers.at(i)].pos.loc) / 100.0;
               sum[0]+=near[0];
               sum[1]+=near[1];
               sum[2]+=near[2];
               sum[3]+=near[3];


           }

           if(sum[0]==0)
               var--;
           else
           {
               result[0]+=sum[0];
           result[1]+=sum[1];
           result[2]+=sum[2];
           result[3]+=sum[3];}
          }

          result[0]=200-(result[0]/100);
          result[1]=100-((result[1])/1000);
          result[2]=100-result[2]/10;
          result[3]=(10000+result[3])/100;

           qDebug()<<"result:"+QString::number(result[0])+" mark_coef[1]:"+QString::number(sc->wm->mark_coef[1]);
       qDebug()<<"result:"+QString::number(result[1])+" mark_coef[2]:"+QString::number(sc->wm->mark_coef[2]);
       qDebug()<<"result:"+QString::number(result[2])+" mark_coef[3]:"+QString::number(sc->wm->mark_coef[3]);
       qDebug()<<"result:"+QString::number(result[3])+" mark_coef[4]:"+QString::number(sc->wm->mark_coef[3]);

       answer=qRound(result[1])*def2;
       answer+=qRound(result[2])*def1;
       answer+=qRound(result[0])*def3;
       answer+=qRound(result[4])*def4;
           qDebug()<<"result:"+QString::number(answer);
    }
    return answer;


}

int FitnessCalc::getMaxFitness()
{

    int maxFitness = 1000000;
    return maxFitness;

}

FitnessCalc::FitnessCalc()
{ solution = new int[32];

}
