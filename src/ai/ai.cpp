#include "ai.h"
#include "play/playcalibration.h"
#include "play/playfreekickopp.h"
#include "play/playfreekickour.h"
#include "play/playgameon.h"
#include "play/playgameondefensive.h"
#include "play/playhalt.h"
#include "play/playkickoffopp.h"
#include "play/playkickoffour.h"
#include "play/playpenaltyopp.h"
#include "play/playpenaltyour.h"
#include "play/playstop.h"
#include "play/playtest.h"
#include "play/playtest2.h"

AI::AI(WorldModel *worldmodel, OutputBuffer *outputbuffer, QObject *parent) :
    QObject(parent),
    wm(worldmodel),
    outputbuffer(outputbuffer)
{
    qDebug() << "AI Initialization...";
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));

    udp = new QUdpSocket();
    ip.setAddress("192.168.4.121");

    current_play = 0;
    for(int i=0; i<PLAYERS_MAX_NUM; i++)
        current_tactic[i] = 0;


    plays.append(new PlayCalibration(wm));
    plays.append(new PlayFreeKickOpp(wm));
    plays.append(new PlayFreeKickOur(wm));
    plays.append(new PlayGameOn(wm));
    plays.append(new PlayGameOnDefensive(wm));
    plays.append(new PlayHalt(wm));
    plays.append(new PlayKickoffOpp(wm));
    plays.append(new PlayKickoffOur(wm));
    plays.append(new PlayPenaltyOpp(wm));
    plays.append(new PlayPenaltyOur(wm));
    plays.append(new PlayStop(wm));
    plays.append(new PlayTest(wm));
    plays.append(new PlayTest2(wm));
}

void AI::Start()
{
    qDebug() << "AI::Start";
    timer.start(AI_TIMER);
}

void AI::Stop()
{
    qDebug() << "AI::Stop";
    timer.stop();
}

Play* AI::getCurrentPlay()
{
    return current_play;
}

Tactic* AI::getCurrentTactic(int i)
{
    return current_tactic[i];
}

void AI::timer_timeout()
{
    //------------------
    double out_x = wm->ourRobot[2].pos.loc.x;
    double out_y = wm->ourRobot[2].pos.loc.y;
    double out_r = wm->ourRobot[2].pos.dir * 100;

    char* cp = (char*)&out_x ;
    char* cp2 = (char*)&out_y;
    char* cp3 = (char*)&out_r;

    udp->writeDatagram(cp,sizeof(double),ip,33433);
    udp->writeDatagram(cp2,sizeof(double),ip,33433);
    udp->writeDatagram(cp3,sizeof(double),ip,33433);
    //--------------------

    for(int i=0; i<PLAYERS_MAX_NUM; i++)
    {
        if(!wm->ourRobot[i].isValid)
            wm->ourRobot[i].Role = NoRole;
    }

    int max_i = 0;
    int max_p = 0;
    for(int i=0; i<plays.size(); i++)
    {
        int p = plays[i]->enterCondition();
        if(p > max_p)
        {
            max_i = i;
            max_p = p;
        }
    }

    Play *play = plays[max_i];
    current_play = play;
    play->execute();

    for(int i=0; i<PLAYERS_MAX_NUM; i++)
    {
        Tactic *tactic = play->getTactic(i);
        current_tactic[i] = tactic;
        if(tactic == NULL) continue;
        tactic->setID(i);
        RobotCommand rc = tactic->getCommand();
        wm->ourRobot[i].SendCommand(rc);
    }

    fps.Pulse();
}
