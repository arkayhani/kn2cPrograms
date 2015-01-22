#include "playfreekickour.h"
PlayFreeKickOur::PlayFreeKickOur(WorldModel *worldmodel, QObject *parent) :
    Play("PlayFreeKickOur", worldmodel, parent)
{
    firstTimeInit = false;

    freeKickStart = false;

    numberOfPlayer = 0;

    tGolie = new TacticGoalie(wm);

    numberOfDef = NUMOFDEFENDERS;
    tDefenderLeft = new TacticDefender(wm);
    tDefenderRight = new TacticDefender(wm);
    tDefenderMid = new TacticDefender(wm);

    tAttackerLeft = new TacticAttacker(wm);
    tAttackerMid = new TacticAttacker(wm);
    tAttackerRight = new TacticAttacker(wm);
}
int PlayFreeKickOur::enterCondition()
{
    if(wm->cmgs.ourFreeKick() || wm->cmgs.ourIndirectKick())
    {
        if(wm->gs_last != wm->gs)
        {
            firstTimeInit = false;
            freeKickStart = false;
            tAttackerMid->kickedSucceccfully = false;
        }
        else
        {
            firstTimeInit = conditionChanged();
        }
        return 100;
    }
    else
        return 0;
    return 0;
           // return 2000000;
}

void PlayFreeKickOur::initRole()
{
    QList<int> activeAgents=wm->kn->ActiveAgents();
    numberOfPlayer = activeAgents.size();
    activeAgents.removeOne(wm->ref_goalie_our);
    wm->ourRobot[wm->ref_goalie_our].Role = AgentRole::Golie;
    switch (activeAgents.length()) {
    case 1:
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerMid;
        break;
    case 2:
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerMid;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderMid;

        //--------------for TAGHI test---------------------------------------
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerLeft;
        //-------------------------------------------------------------------
        break;
    case 3:
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderRight;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderLeft;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerMid;
        break;
    case 4:
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderRight;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderLeft;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerMid;
        wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerLeft;
        break;
    case 5:
        switch (numberOfDef) {
        case 2:
            wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderRight;
            wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderLeft;
            wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerMid;
            wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerRight;
            wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerLeft;
            break;
        case 3:
            wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderRight;
            wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderLeft;
            wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::DefenderMid;
            wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerMid;
            wm->ourRobot[activeAgents.takeFirst()].Role = AgentRole::AttackerRight;
            break;
        }
        break;
    }
    firstTimeInit = true;
}

bool PlayFreeKickOur::conditionChanged()
{
    bool out;
    QList<int> activeAgents=wm->kn->ActiveAgents();
    if(activeAgents.size() != numberOfPlayer)
    {
        numberOfPlayer = activeAgents.size();
        out = false;
    }
    return out;
}

void PlayFreeKickOur::setTactics(int index)
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
}

void PlayFreeKickOur::execute()
{
    QList<int> activeAgents=wm->kn->ActiveAgents();

    if(!firstTimeInit)
    {
        initRole();
    }

    for(int i=0;i<activeAgents.size();i++)
    {
        setTactics(activeAgents.at(i));
    }

//    if(!freeKickStart)
//    {
//        tAttackerMid->isKicker();
//        activeAgents.removeOne(tAttackerMid->getID());
//        int recieverID = tAttackerMid->findBestPlayerForPass();
//        if(recieverID != -1)
//        {
//            if(tAttackerMid->kickedSucceccfully)
//            {
//                qDebug()<<"KICKER";
//                wm->ourRobot[recieverID].Status = AgentStatus::FollowingBall;
//                freeKickStart = true;
//            }
//            activeAgents.removeOne(recieverID);
//        }
//        while(activeAgents.size() > 0)
//        {
//            wm->ourRobot[activeAgents.takeFirst()].Status = AgentStatus::Idle;
//        }
//    }
    tAttackerMid->isKicker();
    activeAgents.removeOne(tAttackerMid->getID());
    int recieverID = tAttackerMid->findBestPlayerForPass();
    if(recieverID != -1)
    {
//        if(tAttackerMid->kickedSucceccfully)
//        {
            wm->ourRobot[recieverID].Status = AgentStatus::RecievingPass;
//            freeKickStart = true;
//        }
        activeAgents.removeOne(recieverID);
    }
    while(activeAgents.size() > 0)
    {
        wm->ourRobot[activeAgents.takeFirst()].Status = AgentStatus::Idle;
    }
}
