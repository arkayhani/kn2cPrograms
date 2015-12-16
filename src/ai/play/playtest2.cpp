#include "playtest2.h"

PlayTest2::PlayTest2(WorldModel *worldmodel, QObject *parent) :
    Play("PlayTest2", worldmodel, parent)
{
    rolesIsInit = false;

    tGolie = new TacticGoalie(wm);

    tDefenderLeft = new TacticDefender(wm);
    tDefenderRight = new TacticDefender(wm);

    tAttackerLeft = new TacticAttacker(wm);
    tAttackerMid = new TacticAttacker(wm);
    tAttackerRight = new TacticAttacker(wm);

    game_status = "Attacking";
}

int PlayTest2::enterCondition()
{
    return 0;  // Make it larger for attacking
}

void PlayTest2::setTactics(int index)
{
    switch (wm->ourRobot[index].Role) {
    case AgentRole::Golie:
        tactics[index] = tGolie;
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
    default:
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
    else if(wm->ourRobot[index].Status == AgentStatus::BlockingBall)
    {
        wm->ourRobot[index].Status = AgentStatus::BlockingBall;
    }
}

void PlayTest2::pressing(int ballOwner)
{
    QList<int> oppPlayers = wm->kn->findNearestOppositeTo(wm->ball.pos.loc);
    int oppBallTracker = findOppReciever();
    if( oppBallTracker != -1 && !isDefender(ballOwner))
        oppPlayers.removeOne(oppBallTracker);
    oppPlayers.removeOne(wm->ref_goalie_opp);

    QList<int> ourPlayers = wm->kn->findAttackers();
    ourPlayers.removeOne(ballOwner);
    ourPlayers.removeOne(wm->ref_goalie_our);

    Marking defence;
    defence.setWorldModel(wm);
    bool isMatched;
    QList<Marking_Struct> m2m = defence.findMarking(ourPlayers,oppPlayers,isMatched);
    if( isMatched )
    {
        for(int i=0;i<m2m.size();i++)
            setPlayer2Keep(m2m.at(i).ourI,m2m.at(i).oppI);
    }

    wm->marking = m2m;
}

int PlayTest2::findBallOwner()
{
    int ownerIndex = -1;
    QList<int> candidates , ours = wm->kn->ActiveAgents();
    QList<double> distance2Prediction;

    if( wm->ball.isValid && !wm->kn->IsInsideGolieArea(wm->ball.pos.loc) )
    {
        if( wm->defenceMode )
            candidates = wm->kn->findAttackers();
        else
            candidates = wm->kn->ActiveAgents();

        ours.removeOne(wm->ref_goalie_our);
        candidates.removeOne(wm->ref_goalie_our);

        for(int i=0;i<candidates.size();i++)
        {
            Vector2D predictedPos;
            if( (wm->ourRobot[candidates.at(i)].Role == AgentRole::AttackerMid)
                    ||
                    (wm->ourRobot[candidates.at(i)].Role == AgentRole::AttackerLeft)
                    ||
                    (wm->ourRobot[candidates.at(i)].Role == AgentRole::AttackerRight)  )
            {
                predictedPos = wm->kn->PredictDestination(wm->ourRobot[candidates.at(i)].pos.loc, wm->ball.pos.loc,2,wm->ball.vel.loc);
                double distance = (predictedPos - wm->ourRobot[candidates.at(i)].pos.loc).length();
                distance2Prediction.append(distance);
            }
            else
            {
                predictedPos = wm->kn->PredictDestination(wm->ourRobot[candidates.at(i)].pos.loc, wm->ball.pos.loc,1,wm->ball.vel.loc);
                double distance = (predictedPos - wm->ourRobot[candidates.at(i)].pos.loc).length();
                distance2Prediction.append(distance);
            }
        }

        int min_i = -1;
        double min_d = 35000000;
        QList<int> sameDistances;

        for(int i=0;i<distance2Prediction.size();i++)
        {
            if( min_d - distance2Prediction.at(i) > ROBOT_RADIUS )
            {
                min_d = distance2Prediction.at(i);
                min_i = candidates.at(i);
                sameDistances.clear();
                sameDistances.append(candidates.at(i));
            }
            else if( fabs(distance2Prediction.at(i)-min_d) < ROBOT_RADIUS )
            {
                sameDistances.append(candidates.at(i));
            }
        }

        if( sameDistances.size() > 1 )
        {
            int min_i_s = -1;
            double min_degree = 35000000;

            for(int i=0;i<sameDistances.size();i++)
            {
                double r2b = fabs((wm->ball.pos.loc - wm->ourRobot[sameDistances.at(i)].pos.loc).dir().radian());
                if( r2b < min_degree )
                {
                    min_degree = r2b;
                    min_i_s = sameDistances.at(i);
                }
            }

            min_i = min_i_s;
        }

        if(min_i != -1)
        {
            wm->ourRobot[min_i].Status = AgentStatus::FollowingBall;
            ownerIndex = min_i;
            ours.removeOne(min_i);
        }
        while( !ours.isEmpty() )
            wm->ourRobot[ours.takeFirst()].Status = AgentStatus::Idle;

    }

    return ownerIndex;
}

int PlayTest2::findOppReciever()
{
    int ownerIndex = -1;

    QList<int> opps = wm->kn->ActiveOppAgents();
    opps.removeOne(wm->ref_goalie_opp);

    QList<double> distance2Prediction;

    if( wm->ball.isValid )
    {
        for(int i=0;i<opps.size();i++)
        {
            Vector2D predictedPos = wm->kn->PredictDestination(wm->oppRobot[opps.at(i)].pos.loc, wm->ball.pos.loc,wm->opp_vel,wm->ball.vel.loc);
            double distance = (predictedPos - wm->oppRobot[opps.at(i)].pos.loc).length();
            distance2Prediction.append(distance);
        }

        int min_i = -1;
        double min_d = 35000000;

        for(int i=0;i<distance2Prediction.size();i++)
        {
            if( distance2Prediction.at(i) <= min_d )
            {
                min_d = distance2Prediction.at(i);
                min_i = opps.at(i);
            }
        }

        ownerIndex = min_i;
    }

    return ownerIndex;
}

void PlayTest2::setPlayer2Keep(int ourR, int oppR)
{
    wm->ourRobot[ourR].Status = AgentStatus::BlockingRobot;

    switch (wm->ourRobot[ourR].Role)
    {
    case AgentRole::AttackerMid:
        tAttackerMid->setPlayerToKeep(oppR);
        break;
    case AgentRole::AttackerRight:
        tAttackerRight->setPlayerToKeep(oppR);
        break;
    case AgentRole::AttackerLeft:
        tAttackerLeft->setPlayerToKeep(oppR);
        break;
    default:
        break;
    }
}

QList<AgentRegion> PlayTest2::freeRegions()
{
    QList<AgentRegion> regions;
    regions.append(AgentRegion::Center);
    regions.append(AgentRegion::Left);
    regions.append(AgentRegion::Right);

    if( !wm->kn->IsInsideOurField(wm->ball.pos.loc ) )
    {
        if( wm->kn->IsInsideRect(wm->ball.pos.loc,Vector2D(Field::MinX,Field::MaxY),Vector2D(Field::MaxX,0.334*Field::MaxY)) )
            regions.removeOne(AgentRegion::Left);
        else if( wm->kn->IsInsideRect(wm->ball.pos.loc,Vector2D(Field::MinX,0.334*Field::MaxY),Vector2D(Field::MaxX,0.334*Field::MinY)) )
            regions.removeOne(AgentRegion::Center);
        else if( wm->kn->IsInsideRect(wm->ball.pos.loc,Vector2D(Field::MinX,0.334*Field::MinY),Vector2D(Field::MaxX,Field::MinY)) )
            regions.removeOne(AgentRegion::Right);
    }

    return regions;
}

void PlayTest2::initRole()
{
    wm->marking.clear();

    QList<int> activeAgents=wm->kn->ActiveAgents();
    QList<int> attackers;
    numberOfPlayers = activeAgents.size();
    activeAgents.removeOne(wm->ref_goalie_our);
    wm->ourRobot[wm->ref_goalie_our].Role = AgentRole::Golie;

    QList<AgentRole> roles;
    roles.append(AgentRole::DefenderLeft);
    roles.append(AgentRole::DefenderRight);
    roles.append(AgentRole::AttackerLeft);
    roles.append(AgentRole::AttackerRight);
    roles.append(AgentRole::AttackerMid);

    for(int i=0;i<activeAgents.size();i++)
    {
        if( roleIsValid(wm->ourRobot[activeAgents.at(i)].Role) )
        {
            roles.removeOne(wm->ourRobot[activeAgents.at(i)].Role);
            activeAgents.removeAt(i);
        }
    }
    int counter = 0;
    while( counter < activeAgents.size() )
    {
        if( roleIsValid(wm->ourRobot[activeAgents.at(counter)].Role) )
        {
            roles.removeOne(wm->ourRobot[activeAgents.at(counter)].Role);
            activeAgents.removeAt(counter);
        }
        else
            counter++;
    }

    attackers = wm->kn->findAttackers();

    for(int i=0;i<activeAgents.size();i++)
    {
        if( !roles.isEmpty() )
            wm->ourRobot[activeAgents.at(i)].Role = roles.takeFirst();
    }

    if( roles.contains(AgentRole::DefenderLeft) )
    {
        for(int i=0;i<attackers.size();i++)
        {
            if(wm->ourRobot[attackers.at(i)].Status != AgentStatus::FollowingBall )
            {
                wm->ourRobot[attackers.at(i)].Role = AgentRole::DefenderLeft;
                attackers.removeAt(i);
                break;
            }
        }
    }

    if( roles.contains(AgentRole::DefenderRight) )
    {
        for(int i=0;i<attackers.size();i++)
        {
            if(wm->ourRobot[attackers.at(i)].Status != AgentStatus::FollowingBall )
            {
                wm->ourRobot[attackers.at(i)].Role = AgentRole::DefenderRight;
                attackers.removeAt(i);
                break;
            }
        }
    }

    rolesIsInit = true;
}

void PlayTest2::coach()
{
    game_status = wm->kn->gameStatus(game_status);

//    int ballOwner = findBallOwner();

    Position leftDefPos,rightDefPos,goaliePos;
    int leftID = -1, rightID = -1 , midID = -1;
    bool leftNav, rightNav;

    if( wm->ourRobot[tDefenderLeft->getID()].Role == AgentRole::DefenderLeft )
        leftID = tDefenderLeft->getID();

    if( wm->ourRobot[tDefenderRight->getID()].Role == AgentRole::DefenderRight )
        rightID = tDefenderRight->getID();

    if( leftChecker > 100 || leftID == -1 )
        midID = rightID;

    if( rightChecker > 100  || rightID == -1)
        midID = leftID;

    zonePositions(leftID,rightID,midID,goaliePos,leftDefPos,leftNav,rightDefPos,rightNav);

    tGolie->setIdlePosition(goaliePos);
    tDefenderLeft->setIdlePosition(leftDefPos);
    tDefenderLeft->setUseNav(leftNav);
    tDefenderRight->setIdlePosition(rightDefPos);
    tDefenderRight->setUseNav(rightNav);

    if( leftID != -1)
    {
        if( (wm->ourRobot[leftID].pos.loc - leftDefPos.loc).length() > 250 )
            leftChecker++;
        else
            leftChecker = 0;
    }

    if( rightID != -1)
    {
        if( (wm->ourRobot[rightID].pos.loc - rightDefPos.loc).length() > 250 )
            rightChecker++;
        else
            rightChecker = 0;
    }

    Positioning pos_calc;
    pos_calc.setWorldModel(wm);

    bool isMatched;

    QList<int> ourAttackers = wm->kn->findAttackers();
    //ourAttackers.removeOne(ballOwner);
    QList<Positioning_Struct> positions = pos_calc.find_positions(ourAttackers,isMatched);

    for(int i=0;i<positions.size();i++)
    {
        setGameOnPos(positions.at(i).ourI,positions.at(i).loc);
    }
}

void PlayTest2::setGameOnPos(int ourR, Vector2D loc)
{
    Vector2D filteredPos;

    switch (wm->ourRobot[ourR].Role)
    {
    case AgentRole::AttackerMid:
        filteredPos = midPrevious + (loc - midPrevious) * (wm->var[7]/1000);
        tAttackerMid->setGameOnPositions(filteredPos);
        midPrevious = filteredPos;
        break;
    case AgentRole::AttackerRight:
        filteredPos = rightPrevious + (loc - rightPrevious) * (wm->var[7]/1000);
        tAttackerRight->setGameOnPositions(filteredPos);
        rightPrevious = filteredPos;
        break;
    case AgentRole::AttackerLeft:
        filteredPos = leftPrevious + (loc - leftPrevious) * (wm->var[7]/1000);
        tAttackerLeft->setGameOnPositions(filteredPos);
        leftPrevious = filteredPos;
        break;
    default:
        break;
    }
}

bool PlayTest2::roleIsValid(AgentRole role)
{
    if( role == AgentRole::NoRole)
        return false;

    return true;
}

bool PlayTest2::isDefender(int index)
{
    bool out = false;
    switch (wm->ourRobot[index].Role)
    {
    case AgentRole::DefenderLeft:
        out = true;
        break;
    case AgentRole::DefenderRight:
        out = true;
        break;
    case AgentRole::DefenderMid:
        out = true;
        break;
    default:
        break;
    }

    return out;
}

void PlayTest2::execute()
{
    wm->passPoints.clear();

    QList<int> activeAgents = wm->kn->ActiveAgents();

    initRole();

    coach();

    while( activeAgents.size() > 0 )
    {
        setTactics(activeAgents.takeFirst());
    }
}
