#include "tacticattacker.h"


TacticAttacker::TacticAttacker(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticAttacker", worldmodel, parent)
{
    numberOfInvalidRanges=0;
    numberOfValidRanges=0;

    everyOneInTheirPos = false;

    waitTimer = new QTimer();
    connect(waitTimer,SIGNAL(timeout()),this,SLOT(dontWait()));
}

RobotCommand TacticAttacker::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    if(wm->ourRobot[id].Status == AgentStatus::FollowingBall)
    {
        Vector2D v;
        v = wm->kn->PredictDestination(wm->ourRobot[this->id].pos.loc,
                wm->ball.pos.loc,rc.maxSpeed,wm->ball.vel.loc);
        Position p = wm->kn->AdjustKickPoint(v, findTarget());

        rc.fin_pos = p;

        if(wm->kn->IsReadyForKick(wm->ourRobot[id].pos, p, wm->ball.pos.loc))
        {
            rc.kickspeedx = detectKickSpeed();
        }

        rc.maxSpeed = 2;
        rc.useNav = false;
        rc.isBallObs = true;
        rc.isKickObs = true;
    }
    else if(wm->ourRobot[id].Status == AgentStatus::Kicking)
    {
        if(wm->gs == STATE_Indirect_Free_kick_Our)
        {
            rc = KickTheBallIndirect();
        }
        else if(wm->gs == STATE_Free_kick_Our)
        {
            //Check For direct Goal
            rc = KickTheBallDirect();
        }
        else if(wm->gs == STATE_Start)
        {
            rc = StartTheGame();
        }

        rc.isBallObs = true;
        rc.isKickObs = true;
    }
    else if(wm->ourRobot[id].Status == AgentStatus::RecievingPass)
    {
        if(wm->gs == GameStateType::STATE_Free_kick_Our || wm->gs == GameStateType::STATE_Indirect_Free_kick_Our)
        {
            if(wm->ourRobot[id].Role == AgentRole::AttackerLeft)
            {
                rc.fin_pos.loc = Vector2D(Field::MaxX/2,Field::oppGoalPost_L.y+200);
            }
            else if(wm->ourRobot[id].Role == AgentRole::AttackerRight)
            {
                rc.fin_pos.loc = Vector2D(Field::MaxX/2,Field::oppGoalPost_R.y-200);
            }
            rc.maxSpeed = 1;
            rc.useNav = true;
            rc.isBallObs = true;
            rc.isKickObs = true;
        }
    }
    else if(wm->ourRobot[id].Status == AgentStatus::BlockingRobot)
    {
        AngleDeg desiredDeg =  (wm->oppRobot[playerToKeep].pos.loc-Field::ourGoalCenter).dir();
        Position final;
        final.loc.x = wm->oppRobot[playerToKeep].pos.loc.x - (300*cos(desiredDeg.radian()));
        final.loc.y = wm->oppRobot[playerToKeep].pos.loc.y - (300*sin(desiredDeg.radian()));
        final.dir = desiredDeg.radian();

        if( wm->gs == GameStateType::STATE_Free_kick_Opp || wm->gs == GameStateType::STATE_Indirect_Free_kick_Opp)
        {
            if( wm->kn->IsInsideSecureArea(final.loc,wm->ball.pos.loc) )
            {
                Vector2D fstInt,secInt;
                Circle2D secArea(wm->ball.pos.loc,ALLOW_NEAR_BALL_RANGE);
                Line2D connectedLine(wm->ball.pos.loc,final.loc);
                int numberOfIntersections = secArea.intersection(connectedLine,&fstInt,&secInt);

                if( numberOfIntersections == 2 )
                {
                    if( (fstInt-final.loc).length() > (secInt-final.loc).length() )
                        rc.fin_pos.loc = secInt;
                    else
                        rc.fin_pos.loc = fstInt;
                }
                else if( numberOfIntersections == 1 )
                {
                    rc.fin_pos.loc = fstInt;
                }
                else
                    rc.fin_pos = wm->ourRobot[this->id].pos;
            }
            else
            {
                rc.fin_pos = final;
            }
        }

        rc.maxSpeed = 2;
        rc.useNav = true;
        rc.isBallObs = true;
        rc.isKickObs = true;
    }
    else if(wm->ourRobot[id].Status == AgentStatus::Idle)
    {
        if(wm->gs == GameStateType::STATE_Free_kick_Our || wm->gs == GameStateType::STATE_Indirect_Free_kick_Our)
        {
            if(wm->ourRobot[id].Role == AgentRole::AttackerLeft)
            {
                rc.fin_pos.loc = Vector2D(Field::MaxX/2,Field::oppGoalPost_L.y+200);
            }
            else if(wm->ourRobot[id].Role == AgentRole::AttackerRight)
            {
                rc.fin_pos.loc = Vector2D(Field::MaxX/2,Field::oppGoalPost_R.y-200);
            }
            else if(wm->ourRobot[id].Role == AgentRole::AttackerMid)
            {
                rc.fin_pos = wm->ourRobot[id].pos;
            }
            rc.useNav = true;
            rc.isBallObs = true;
            rc.isKickObs = true;
        }
        else if(wm->gs == GameStateType::STATE_Free_kick_Opp || wm->gs == GameStateType::STATE_Indirect_Free_kick_Opp)
        {
            Vector2D finalPos;
            double m;
            double alfa;
            m=-(Field::ourGoalCenter.y-wm->ball.pos.loc.y)/(Field::ourGoalCenter.x-wm->ball.pos.loc.x);
            alfa=atan(m);

            if(alfa>75.0*3.14/180)
            {
                alfa=120.0*3.14/180;
            }

            if(alfa<-75.0*3.14/180)
            {
                alfa=-120.0*3.14/180;
            }

            switch (wm->ourRobot[id].Role) {
            case AgentRole::AttackerMid:

                break;
            case AgentRole::AttackerRight:
                alfa-=AngleDeg::PI/10;
                break;
            case AgentRole::AttackerLeft:
                alfa+=AngleDeg::PI/10;
                break;
            }
            finalPos.x=wm->ball.pos.loc.x-ALLOW_NEAR_BALL_RANGE*cos(alfa);
            finalPos.y=wm->ball.pos.loc.y+ALLOW_NEAR_BALL_RANGE*sin(alfa);

            rc.fin_pos.loc=finalPos;
            rc.maxSpeed=1;
        }
        else if(wm->gs == GameStateType::STATE_Kick_off_Our)
        {
            switch(wm->ourRobot[this->id].Role)
            {
            case AgentRole::AttackerMid:
                rc.fin_pos = wm->kn->AdjustKickPoint(Vector2D(wm->ball.pos.loc.x-100,wm->ball.pos.loc.y),Field::oppGoalCenter);
                break;
            case AgentRole::AttackerRight:
                rc.fin_pos = wm->kn->AdjustKickPoint(Vector2D(wm->ball.pos.loc.x - 250 , 1550),Field::oppGoalCenter);
                break;
            case AgentRole::AttackerLeft:
                rc.fin_pos = wm->kn->AdjustKickPoint(Vector2D(wm->ball.pos.loc.x - 250 , -1550),Field::oppGoalCenter);
                break;
            default:
                break;
            }

            rc.maxSpeed = 1;
            rc.useNav = true;
            rc.isBallObs = true;
            rc.isKickObs = true;
        }
        else if(wm->gs == GameStateType::STATE_Kick_off_Opp)
        {
            QList<int> opps = wm->kn->ActiveOppAgents();
            opps.removeOne(wm->ref_goalie_opp);

            QList<int> nearest;
            int i = 0;
            switch(wm->ourRobot[this->id].Role)
            {
            case AgentRole::AttackerMid:
                rc.fin_pos = wm->ourRobot[this->id].pos;
                break;
            case AgentRole::AttackerRight:
                nearest = wm->kn->findNearestOppositeTo(Vector2D(0,Field::MaxY/2));

                while( i<nearest.size() )
                {
                    if( wm->kn->IsInsideSecureArea(wm->oppRobot[nearest.at(i)].pos.loc,wm->ball.pos.loc)
                            /*||  (wm->oppRobot[id].pos.loc -Vector2D(0,Field::MaxY/2)).length()>1000*/)
                        nearest.removeAt(i);
                    else
                        i++;
                }
                if( !nearest.isEmpty() )
                {
                    Line2D tmp(wm->oppRobot[nearest.at(0)].pos.loc,Field::ourGoalCenter);
                    Line2D fixedLine(Vector2D(-2*ROBOT_RADIUS,Field::MinY), Vector2D(-2*ROBOT_RADIUS,Field::MaxY));
                    Vector2D interSection = tmp.intersection(fixedLine);
                    rc.fin_pos.loc = interSection;
                    rc.fin_pos.dir = (wm->oppRobot[nearest.at(0)].pos.loc-Field::ourGoalCenter).dir().degree()*AngleDeg::DEG2RAD;
                }
                else
                {
                    rc.fin_pos = wm->ourRobot[this->id].pos;
                }
                break;
            case AgentRole::AttackerLeft:
                //rc.fin_pos = wm->kn->AdjustKickPoint(Vector2D(wm->ball.pos.loc.x - 250 , -1550),Field::oppGoalCenter);
                nearest = wm->kn->findNearestOppositeTo(Vector2D(0,Field::MinY/2));
                while( i<nearest.size() )
                {
                    if( wm->kn->IsInsideSecureArea(wm->oppRobot[nearest.at(i)].pos.loc,wm->ball.pos.loc)
                            /*||  (wm->oppRobot[id].pos.loc -Vector2D(0,Field::MinY/2)).length()>1000*/)
                        nearest.removeAt(i);
                    else
                        i++;
                }
                if( !nearest.isEmpty() )
                {
                    Line2D tmp(wm->oppRobot[nearest.at(0)].pos.loc,Field::ourGoalCenter);
                    Line2D fixedLine(Vector2D(-2*ROBOT_RADIUS,Field::MinY), Vector2D(-2*ROBOT_RADIUS,Field::MaxY));
                    Vector2D interSection = tmp.intersection(fixedLine);
                    rc.fin_pos.loc = interSection;
                    rc.fin_pos.dir = (wm->oppRobot[nearest.at(0)].pos.loc-Field::ourGoalCenter).dir().degree()*AngleDeg::DEG2RAD;
                }
                else
                {
                    rc.fin_pos = wm->ourRobot[this->id].pos;
                }
                break;
            default:
                break;
            }
        }
        else if( wm->gs == GameStateType::STATE_Penalty_Our )
        {
            Vector2D pos;
            switch (wm->ourRobot[this->id].Role)
            {
            case AgentRole::AttackerMid :
                rc.fin_pos.loc = Field::ourPenaltyParallelLineCenter;
                break;
            case AgentRole::AttackerRight :
                pos.assign(Field::ourPenaltyParallelLineCenter.x,Field::ourPenaltyParallelLineCenter.y + (Field::MaxY*0.75));
                rc.fin_pos = wm->kn->AdjustKickPoint(pos,Field::oppGoalCenter);
                break;
            case AgentRole::AttackerLeft :
                pos.assign(Field::ourPenaltyParallelLineCenter.x,Field::ourPenaltyParallelLineCenter.y - (Field::MaxY*0.75));
                rc.fin_pos = wm->kn->AdjustKickPoint(pos,Field::oppGoalCenter);
                break;
            default:
                break;
            }
            //-------I should Check THis!!!----
            if(wm->kn->isOccupied(rc.fin_pos.loc))
                rc.fin_pos.loc = rc.fin_pos.loc - Vector2D(100,100);
        }
        else if( wm->gs == GameStateType::STATE_Penalty_Opp )
        {
            switch (wm->ourRobot[this->id].Role)
            {
            case AgentRole::AttackerMid :
                rc.fin_pos.loc = Field::oppPenaltyParallelLineCenter;
                break;
            case AgentRole::AttackerRight :
                rc.fin_pos.loc.x = Field::oppPenaltyParallelLineCenter.x;
                rc.fin_pos.loc.y = Field::oppPenaltyParallelLineCenter.y + (Field::MaxY*0.75);
                break;
            case AgentRole::AttackerLeft :
                rc.fin_pos.loc.x = Field::oppPenaltyParallelLineCenter.x;
                rc.fin_pos.loc.y = Field::oppPenaltyParallelLineCenter.y - (Field::MaxY*0.75);
                break;
            default:
                break;
            }
            //-------I should Check THis!!!----
            if(wm->kn->isOccupied(rc.fin_pos.loc))
                rc.fin_pos.loc = rc.fin_pos.loc - Vector2D(100,100);
        }
        else
        {
            rc.fin_pos = wm->ourRobot[this->id].pos;
        }
    }
    //  Just Added for Some Tests
    //    else
    //    {
    //        rc.maxSpeed = 1.5;

    //        rc=goBehindBall();

    //        if(wm->kn->CanKick(wm->ourRobot[id].pos,wm->ball.pos.loc) && wm->cmgs.canKickBall())
    //        {
    //            rc.kickspeedx=5;
    //        }

    //        rc.useNav = true;
    //        rc.isBallObs = false;
    //        rc.isKickObs = true;
    //    }

    return rc;
}

void TacticAttacker::findCriticalPlayer()
{
    QList<int> our_agents = wm->kn->ActiveAgents();
    important_opp_agents.clear();
    important_our_agents.clear();

    //---------ActiveOppAgents
    QList<int> opp_agents;
    for(int i=0; i< PLAYERS_MAX_NUM; i++)
        if(wm->oppRobot[i].isValid)
            opp_agents.append(i);
    //---------------------

    QList<int> active_our_agents;
    QList<int> active_opp_agents;

    foreach(int i,our_agents)
    {
        if(wm->ourRobot[i].pos.loc.x>wm->ourRobot[id].pos.loc.x)
        {
            active_our_agents.append(i);
        }
    }

    foreach(int i,opp_agents)
    {
        if(wm->oppRobot[i].pos.loc.x>wm->ourRobot[id].pos.loc.x)
        {
            active_opp_agents.append(i);
        }
    }

    Line2D down(Field::oppGoalPost_R,wm->ball.pos.loc);
    Line2D up(Field::oppGoalPost_L,wm->ball.pos.loc);

    foreach (int i, active_opp_agents)
    {
        double Ymin=wm->ball.pos.loc.y+(-down.a()/down.b())*(wm->oppRobot[i].pos.loc.x-wm->ball.pos.loc.x);
        double Ymax=wm->ball.pos.loc.y+tan(-up.a()/up.b())*(wm->oppRobot[i].pos.loc.x-wm->ball.pos.loc.x);

        if(wm->oppRobot[i].pos.loc.y>Ymin && wm->oppRobot[i].pos.loc.y<Ymax)
        {
            important_opp_agents.append(i);
        }
    }

    foreach (int i, active_our_agents)
    {
        double Ymin=wm->ball.pos.loc.y+(-down.a()/down.b())*(wm->ourRobot[i].pos.loc.x-wm->ball.pos.loc.x);
        double Ymax=wm->ball.pos.loc.y+tan(-up.a()/up.b())*(wm->ourRobot[i].pos.loc.x-wm->ball.pos.loc.x);

        if(wm->ourRobot[i].pos.loc.y>Ymin && wm->ourRobot[i].pos.loc.y<Ymax)
        {
            important_our_agents.append(i);
        }
    }
}

void TacticAttacker::findInvalidRanges()
{
    numberOfInvalidRanges=0;
    uGoal=(atan( (Field::oppGoalPost_L.y-wm->ball.pos.loc.y)/(Field::oppGoalPost_L.x-wm->ball.pos.loc.x)));
    dGoal=(atan( (Field::oppGoalPost_R.y-wm->ball.pos.loc.y)/(Field::oppGoalPost_R.x-wm->ball.pos.loc.x)));
    foreach (int i, important_opp_agents)
    {
        double alpha=asin(ROBOT_RADIUS/wm->ball.pos.loc.dist(wm->oppRobot[i].pos.loc));
        double beta=atan((wm->oppRobot[i].pos.loc.y-wm->ball.pos.loc.y)/(wm->oppRobot[i].pos.loc.x-wm->ball.pos.loc.x));
        angle[numberOfInvalidRanges][1]=beta+alpha;
        angle[numberOfInvalidRanges][0]=beta-alpha;
        numberOfInvalidRanges++;
    }
    foreach (int i, important_our_agents)
    {
        double alpha=asin(ROBOT_RADIUS/wm->ball.pos.loc.dist(wm->ourRobot[i].pos.loc));
        double beta=atan((wm->ourRobot[i].pos.loc.y-wm->ball.pos.loc.y)/(wm->ourRobot[i].pos.loc.x-wm->ball.pos.loc.x));
        angle[numberOfInvalidRanges][1]=beta+alpha;
        angle[numberOfInvalidRanges][0]=beta-alpha;
        numberOfInvalidRanges++;
    }
}

void TacticAttacker::sortInvalidRanges()
{
    for(int i=0;i<numberOfInvalidRanges;i++)
    {
        for(int j=i+1;j<numberOfInvalidRanges;j++)
        {
            if(angle[i][0]>angle[j][0])
            {
                double temp=angle[i][0];
                angle[i][0]=angle[j][0];
                angle[j][0]=temp;
                temp=angle[i][1];
                angle[i][1]=angle[j][1];
                angle[j][1]=temp;
            }
        }
    }
}

void TacticAttacker::findValidRanges()
{
    numberOfValidRanges=0;
    valid_angle[numberOfValidRanges][0]=dGoal;

    for(int i=0;i<numberOfInvalidRanges+1;i++)
    {
        valid_angle[numberOfValidRanges][1]=angle[i][0];
        valid_angle[numberOfValidRanges+1][0]=angle[i][1];
        numberOfValidRanges++;
    }

    valid_angle[numberOfValidRanges-1][1]=uGoal;
}

RobotCommand TacticAttacker::goBehindBall()
{
    RobotCommand rc;
    canKick=false;

    rc.maxSpeed = 1;

    float deg=atan((0-wm->ball.pos.loc.y)/(3025-wm->ball.pos.loc.x));

    if(!wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 30, 180))
    {
        rc.fin_pos.loc= {wm->ball.pos.loc.x-110*cos(deg),wm->ball.pos.loc.y-110*sin(deg)};
        rc.fin_pos.dir=atan((0-wm->ball.pos.loc.y)/(3025-wm->ball.pos.loc.x));
    }


    if(!wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 20, 2))
    {
        double test=findBestPoint();
        rc.fin_pos.dir=test;
    }
    rc.fin_pos.loc= {wm->ball.pos.loc.x-100*cos(deg),wm->ball.pos.loc.y-100*sin(deg)};

    if(wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 10, 4))
    {
        canKick=true;
    }

    return rc;
}

RobotCommand TacticAttacker::KickTheBallIndirect()
{
    RobotCommand rc;

    rc.maxSpeed = 0.5;

    int index = findBestPlayerForPass();

    if(index != -1)
    {
        Vector2D target(wm->ourRobot[index].pos.loc.x,wm->ourRobot[index].pos.loc.y);
        Vector2D goal(target.x+300*cos(target.dir().DEG2RAD),target.y+300*sin(target.dir().DEG2RAD));

        Position kickPoint = wm->kn->AdjustKickPoint(wm->ball.pos.loc,goal);

        double kickRadius = ROBOT_RADIUS *3;
        Vector2D midle_vector=Vector2D(kickRadius*sin(abs(kickPoint.dir)),kickRadius*cos(abs(kickPoint.dir)));
        midle_vector =  midle_vector.setDir(AngleDeg(-kickPoint.dir));

        Position midlePoint;
        midlePoint.loc = (wm->ball.pos.loc+midle_vector);
        midlePoint.dir = kickPoint.dir;

        if( (midlePoint.loc - wm->ourRobot[id].pos.loc).length() > 50  && (!pastMidPoint) )
        {
            rc.fin_pos = midlePoint;
            rc.useNav = true;
        }
        else
        {
            pastMidPoint = true;
            rc.fin_pos = kickPoint;
            if( (rc.fin_pos.loc-wm->ourRobot[this->id].pos.loc).length() < 150)
            {
                if(!everyOneInTheirPos)
                {
                    rc.maxSpeed = 0;
                }
                else
                {
                    rc.useNav = false;
                    rc.maxSpeed = 0.25;
                }
            }
        }

        if(wm->kn->CanKick(wm->ourRobot[id].pos,wm->ball.pos.loc) && everyOneInTheirPos)
        {
            rc.kickspeedx = detectKickSpeed();
            pastMidPoint = false;
        }
    }
    return rc;
}

RobotCommand TacticAttacker::KickTheBallDirect()
{
    RobotCommand rc;

    rc.maxSpeed = 0.5;

    Position kickPoint = wm->kn->AdjustKickPoint(wm->ball.pos.loc,findTarget());

    rc.fin_pos = kickPoint;
    if( (rc.fin_pos.loc-wm->ourRobot[this->id].pos.loc).length() < 150)
    {
        if(!everyOneInTheirPos)
        {
            rc.maxSpeed = 0;
        }
        else
        {
            rc.useNav = false;
            rc.maxSpeed = 0.25;
        }
    }

    if(wm->kn->CanKick(wm->ourRobot[id].pos,wm->ball.pos.loc) && everyOneInTheirPos)
    {
        rc.kickspeedx = detectKickSpeed();
    }

    return rc;
}

RobotCommand TacticAttacker::StartTheGame()
{
    RobotCommand rc;

    rc.maxSpeed = 0.5;

    Vector2D target(Field::oppGoalCenter.x,Field::oppGoalCenter.y);
    rc.fin_pos = wm->kn->AdjustKickPoint(wm->ball.pos.loc,target);

    rc.useNav = false;

    if(wm->kn->CanKick(wm->ourRobot[id].pos,wm->ball.pos.loc) )
    {
        //rc.kickspeedz = 2.5;//50;
        rc.kickspeedx = detectKickSpeed();//2.5;//50;
    }

    return rc;
}

double TacticAttacker::findBestPoint()
{
    findCriticalPlayer();
    findInvalidRanges();
    sortInvalidRanges();
    findValidRanges();
    double maxLength=0;
    int index=-1;
    for(int i=0;i<numberOfValidRanges;i++)
    {
        if(maxLength<valid_angle[i][1]-valid_angle[i][0])
        {
            index=i;
            maxLength=valid_angle[i][1]-valid_angle[i][0];
        }
    }

    return (valid_angle[index][0]+valid_angle[index][1])/2;
}

int TacticAttacker::findBestPlayerForPass()
{
    int index = -1;
    double min = 10000;

    QList<int> ourAgents = wm->kn->ActiveAgents();
    QList<int> freeAgents;

    while( !ourAgents.isEmpty() )
    {
        int index = ourAgents.takeFirst();
        if(isFree(index))
            freeAgents.append(index);
    }

    while ( !freeAgents.isEmpty() )
    {
        int i = freeAgents.takeFirst();
        if(wm->ourRobot[i].isValid && this->id != i && i != wm->ref_goalie_our)
        {
            if(wm->ball.pos.loc.dist(wm->ourRobot[i].pos.loc) < min)
            {
                min = wm->ourRobot[id].pos.loc.dist(wm->ourRobot[i].pos.loc);
                index = i;
            }
        }
    }
    //qDebug()<<"Pass Receiver is "<<index;
    return index;
}

void TacticAttacker::isKicker()
{
    wm->ourRobot[this->id].Status = AgentStatus::Kicking;
}

float TacticAttacker::detectKickSpeed(Vector2D dest )
{
    float kickSpeed;

    if(wm->isSim)
    {
        switch (wm->gs ) {
        case STATE_Free_kick_Our:
            kickSpeed = 5; //Should Changed
            break;
        case STATE_Indirect_Free_kick_Our:
            kickSpeed = 2.5; //Should Changed
            break;
        case STATE_Start:
            kickSpeed = 5; //Should Changed
            break;
        default:
            break;
        }
    }
    else
    {
        switch (wm->gs ) {
        case STATE_Free_kick_Our:
            kickSpeed = 200; //Should Changed
            break;
        case STATE_Indirect_Free_kick_Our:
            kickSpeed = 25;//50; //Should Changed
            break;
        case STATE_Start:
            kickSpeed = 150; //Should Changed
            break;
        default:
            kickSpeed = 100;//Should Changed
            break;
        }
    }

    return kickSpeed;
}

void TacticAttacker::waitTimerStart()
{
    everyOneInTheirPos = false;
    waitTimer->start(4000);
}

void TacticAttacker::setPlayerToKeep(int index)
{
    this->playerToKeep = index;
}

bool TacticAttacker::isFree(int index)
{
    QList<int> oppAgents = wm->kn->ActiveOppAgents();
    bool isFree = true;

    while( !oppAgents.isEmpty() )
    {
        int indexOPP = oppAgents.takeFirst();
        if( (wm->ourRobot[index].pos.loc-wm->oppRobot[indexOPP].pos.loc).length() < DangerDist)
        {
            isFree = false;
        }

        if(!isFree)
            break;
    }
    return isFree;
}

void TacticAttacker::dontWait()
{
    everyOneInTheirPos = true;
    waitTimer->stop();
}
