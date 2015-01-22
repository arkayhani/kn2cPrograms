#include "playgameon.h"

PlayGameOn::PlayGameOn(WorldModel *worldmodel, QObject *parent) :
    Play("PlayGameOn", worldmodel, parent)
{
//    firstTimeInit = false; // Should Checked!!!!!

//    freeKickStart = false;// Should Checked!!!!!

//    numberOfPlayer = 0;// Should Checked!!!!!

    tGolie = new TacticGoalie(wm);

//    numberOfDef = NUMOFDEFENDERS; // Should Checked!!!!!
    tDefenderLeft = new TacticDefender(wm);
    tDefenderRight = new TacticDefender(wm);
    tDefenderMid = new TacticDefender(wm);

    tAttackerLeft = new TacticAttacker(wm);
    tAttackerMid = new TacticAttacker(wm);
    tAttackerRight = new TacticAttacker(wm);
}

int PlayGameOn::enterCondition()
{
    if(wm->cmgs.gameOn() && wm->kn->ActiveAgents().size() > 3)
        return 100;
    else
        return 0;

}

void PlayGameOn::setTactics(int index)
{
    switch (wm->ourRobot[index].Role) {
    case AgentRole::Golie:
        tactics[index] = tGolie;
        break;
    case AgentRole::DefenderMid:
        tactics[index] = tDefenderMid;
        break;
    case AgentRole::DefenderLeft:
        tactics[index] = tDefenderLeft;
        break;
    case AgentRole::DefenderRight:
        tactics[index] = tDefenderRight;
        break;
    case AgentRole::AttackerMid:
        tactics[index] = tAttackerMid;
        break;
    case AgentRole::AttackerRight:
        tactics[index] = tAttackerRight;
        break;
    case AgentRole::AttackerLeft:
        tactics[index] = tAttackerLeft;
        break;
    }

    if(wm->ourRobot[index].Status == AgentStatus::RecievingPass)
    {
        wm->ourRobot[index].Status = AgentStatus::FollowingBall;
    }
    else if(wm->ourRobot[index].Status == AgentStatus::Kicking)
    {
        wm->ourRobot[index].Status = AgentStatus::Idle;
    }
//    {
//        wm->ourRobot[index].Status = AgentStatus::Idle;
//    }
}

void PlayGameOn::initRole()
{
    if( wm->gs_last == STATE_Free_kick_Our)
        return ;
}

void PlayGameOn::execute()
{
    QList<int> activeAgents = wm->kn->ActiveAgents();

    initRole();

    while( activeAgents.size() > 0 )
    {
        setTactics(activeAgents.takeFirst());
    }
}
