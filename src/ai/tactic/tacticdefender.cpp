#include "tacticdefender.h"

TacticDefender::TacticDefender(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticDefender", worldmodel, parent)
{
    numberOfDefenders=NUMOFDEFENDERS;

}

RobotCommand TacticDefender::getCommand()
{
    RobotCommand rc;

    if(!wm->ourRobot[id].isValid) return rc;

    if(wm->ourRobot[this->id].Status == AgentStatus::FollowingBall)
    {
        rc.maxSpeed = 2;

        Vector2D v;
        v = wm->kn->PredictDestination(wm->ourRobot[this->id].pos.loc,
                wm->ball.pos.loc,rc.maxSpeed,wm->ball.vel.loc);
        Position p = wm->kn->AdjustKickPoint(v, findTarget() );

        rc.fin_pos = p;

        if(wm->kn->IsReadyForKick(wm->ourRobot[id].pos, p, wm->ball.pos.loc))
        {
            rc.kickspeedx = 5;//150;
        }

        rc.useNav = false;
        rc.isBallObs = true;
        rc.isKickObs = true;

    }
    else if(wm->ourRobot[this->id].Status == AgentStatus::RecievingPass)
    {
        rc.fin_pos = wm->ourRobot[this->id].pos;
    }
    else if(wm->ourRobot[this->id].Status == AgentStatus::BlockingPass)
    {

    }
    else if(wm->ourRobot[this->id].Status == AgentStatus::BlockingRobot)
    {

    }
    else if(wm->ourRobot[this->id].Status == AgentStatus::BlockingRobot)
    {

    }
    else if(wm->ourRobot[this->id].Status == AgentStatus::Idle)
    {
        double ballDeg,beta,teta,alpha=356,gama;
        double dtgc;//distanceToGoldanCenter
        double dtgc2;
        double ballDistance;//ballDistanceToGoalCenter

        //Geometric calculations
        ballDeg=atan((wm->ball.pos.loc.y-0.0)/(wm->ball.pos.loc.x+(float)(Field::MaxX)));

        if(ballDeg>75.0*3.14/180)
        {
            ballDeg=75.0*3.14/180;
        }

        if(ballDeg<-75.0*3.14/180)
        {
            ballDeg=-75.0*3.14/180;
        }

        gama=AngleDeg::PI/2.0-abs(ballDeg);
        beta=asin((float)(Field::ourGoalPost_L.y)*sin(gama)/(2.0*Field::goalCircleEX_R));
        alpha=atan((float)(Field::ourGoalCC_L.y-Field::ourGoalCC_R.y)/(2.0*(float)(Field::goalCircleEX_R)));
        dtgc=(float)(Field::goalCircleEX_R)*sin(AngleDeg::PI-gama-beta)/sin(gama);
        dtgc2=sqrt(dtgc*dtgc+(float)(2*ROBOT_RADIUS+10)*(float)(2*ROBOT_RADIUS+10));
        teta=atan((float)(2*ROBOT_RADIUS+10)/dtgc);
        ballDistance=pow((pow(wm->ball.pos.loc.x+(float)(FIELD_MAX_X),2)+pow(wm->ball.pos.loc.y,2)),0.5);

        Vector2D finalPos,leftPos,rightPos;
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
        //@kamin
        if (time < timer.elapsed())
        {
           time = timer.elapsed()/100;
        }

        if(time == 0)
        {
           timer.start();
           time =100;
        }
        //@kamin
         Vector2D err= Vector2D(wm->ourRobot[id].pos.loc.x+460,wm->ourRobot[id].pos.loc.y);

        switch(wm->ourRobot[this->id].Role)
        {
        case AgentRole::DefenderMid:


        flag=1;
            switch (flag)
            {
            case 0:
                //@kamin
                qDebug() << wm->ourRobot[id].pos.loc.x << wm->ourRobot[id].pos.loc.y;
                ballDeg = (float)((int)(time*3) % 360)*asin(1)/90;//*2*asin(1);//4*asin((float)(time % 100)/100.0) /*- asin(1)*/;
                //qDebug() << "ploc" <<-cos( 0.18716 * M_PI);ballDeg*90/asin(1);
                if(0)//abs(ballDeg)<alpha)
                    //@kamin
                {
                    rc.fin_pos.dir=ballDeg;
                    rc.fin_pos.loc={-(float)(FIELD_MAX_X)+Field::goalCircleEX_R,0.0+tan(ballDeg)*Field::goalCircleEX_R};
                }

                else
                {
                    rc.fin_pos.dir=ballDeg;
                    rc.fin_pos.loc={-(float)(FIELD_MAX_X)+cos(ballDeg)*dtgc+1400,0.0+sin(ballDeg)*dtgc};
                }

                if(wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 40, 6))
                {
                    rc.fin_pos.dir=wm->ourRobot[id].pos.dir;
                    rc.fin_pos.loc= wm->ourRobot[id].pos.loc;

                }


                if(err.r()<100)
                    flag=1;

                break;

            case 1:

                finalPos.x=wm->ball.pos.loc.x-100*cos(alfa);
                finalPos.y=wm->ball.pos.loc.y+100*sin(alfa);

                rc.fin_pos.loc=finalPos;
                if(wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 10, 6))
                {
                    if (wm->kn->CanKick(wm->ourRobot[id].pos,wm->ball.pos.loc))
                    {
                        rc.kickspeedx =250;
                        qDebug() << "qqqqqqqqqqqq";
                    }


                }

                break;

            }
            //@kamin
//            qDebug() << wm->ourRobot[id].pos.loc.x << wm->ourRobot[id].pos.loc.y;
//            ballDeg = (float)((int)(time*3) % 360)*asin(1)/90;//*2*asin(1);//4*asin((float)(time % 100)/100.0) /*- asin(1)*/;
//            //qDebug() << "ploc" <<-cos( 0.18716 * M_PI);ballDeg*90/asin(1);
//            if(0)//abs(ballDeg)<alpha)
//                //@kamin
//            {
//                rc.fin_pos.dir=ballDeg;
//                rc.fin_pos.loc={-(float)(FIELD_MAX_X)+Field::goalCircleEX_R,0.0+tan(ballDeg)*Field::goalCircleEX_R};
//            }

//            else
//            {
//                rc.fin_pos.dir=ballDeg;
//                rc.fin_pos.loc={-(float)(FIELD_MAX_X)+cos(ballDeg)*dtgc+1400,0.0+sin(ballDeg)*dtgc};
//            }

//            if(wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 40, 6))
//            {
//                rc.fin_pos.dir=wm->ourRobot[id].pos.dir;
//                rc.fin_pos.loc= wm->ourRobot[id].pos.loc;

//            }
            //@kamin
//            rc.fin_pos.dir=wm->ourRobot[id].pos.dir;
//            rc.fin_pos.loc= {wm->ourRobot[id].pos.loc.x+2000,wm->ourRobot[id].pos.loc.y};
//            if(wm->ourRobot[id].pos.loc.x>2000) flag=1;
//            if(flag==1)
//            {
//                rc.fin_pos.dir=90;wm->ourRobot[id].pos.dir;
//                rc.fin_pos.loc= {wm->ourRobot[id].pos.loc.x-2000,wm->ourRobot[id].pos.loc.y};
//            }
//            if(wm->ourRobot[id].pos.loc.x<-2000)flag=0;

            //@kamout
            //@kamin
            if (wm->kn->CanKick(wm->ourRobot[id].pos,wm->ball.pos.loc))
            {
                rc.kickspeedx =250;
                qDebug() << "qqqqqqqqqqqq";
                flag = 0;
            }

            rc.fin_pos.loc.x = 0;

            rc.fin_pos.loc.y = 0;


            //@kamout

        break;

        case AgentRole::DefenderLeft:
            rc.fin_pos.dir=ballDeg;
            rc.fin_pos.loc={-(float)(FIELD_MAX_X)+cos(ballDeg+teta)*dtgc2,0.0+sin(ballDeg+teta)*dtgc2};
            if(dtgc+2*ROBOT_RADIUS<ballDistance && ballDistance<dtgc+4*ROBOT_RADIUS && wm->ball.pos.loc.y>0)
            {
                if(!wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 30, 180))
                {
                    rc.fin_pos.loc= {wm->ball.pos.loc.x-110*cos(ballDeg),wm->ball.pos.loc.y-110*sin(ballDeg)};
                    rc.fin_pos.dir=ballDeg;
                }

                if(wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 20, 4))
                {

                    if(!wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 20, 2))
                    {
                        rc.fin_pos.dir=ballDeg;
                    }
                    rc.fin_pos.loc= {wm->ball.pos.loc.x-100*cos(ballDeg),wm->ball.pos.loc.y-100*sin(ballDeg)};
                }


           }
        break;

        case AgentRole::DefenderRight:
            rc.fin_pos.dir=ballDeg;
            rc.fin_pos.loc={-(float)(FIELD_MAX_X)+cos(ballDeg-teta)*dtgc2,0.0+sin(ballDeg-teta)*dtgc2};
            if(dtgc+2*ROBOT_RADIUS<ballDistance && ballDistance<dtgc+4*ROBOT_RADIUS && wm->ball.pos.loc.y<0)
            {
                if(!wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 30, 180))
                {
                    rc.fin_pos.loc= {wm->ball.pos.loc.x-110*cos(ballDeg),wm->ball.pos.loc.y-110*sin(ballDeg)};
                    rc.fin_pos.dir=ballDeg;
                }

                if(wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 200, 180))
                {

//                    if(!wm->kn->ReachedToPos(wm->ourRobot[id].pos, rc.fin_pos, 20, 40))
//                    {
                        rc.fin_pos.dir=wm->ourRobot[id].pos.dir;
//                    }
                    rc.fin_pos.loc= wm->ourRobot[id].pos.loc;//{wm->ball.pos.loc.x-100*cos(ballDeg),wm->ball.pos.loc.y-100*sin(ballDeg)};
                }
            }
        break;
        }


        // JAFARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRr
//        double radius=1000;
//        Vector2D center(-1000,0);

//        rc.fin_pos.loc.x=center.x + radius*cos(alfa);
//        rc.fin_pos.loc.y=center.y + radius*sin(alfa);
//        rc.fin_pos.dir=AngleDeg::deg2rad(alfa);
//        alfa=alfa+20;
        // END OF JAFARRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRr
        rc.maxSpeed=2;

        rc.useNav = true;
        rc.isBallObs = true;
        rc.isKickObs = true;

    }

    return rc;
}

void TacticDefender::setDefenderPos(int pos)
{
    this->position=pos;
}
