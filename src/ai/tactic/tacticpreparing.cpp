#include "tacticpreparing.h"


TacticPreparing::TacticPreparing(WorldModel *worldmodel, QObject *parent) :
    Tactic("TacticPreparing", worldmodel, parent)
{
    kickPermision = false;
    kickIt = false;
}

RobotCommand TacticPreparing::getCommand()
{
    RobotCommand rc;
    if(!wm->ourRobot[id].isValid) return rc;

    rc.maxSpeed = 1;
    if( kickPermision )
    {
        rc.fin_pos.loc = Vector2D(500,0);
        rc.fin_pos.dir = 0;

        if(wm->ball.pos.loc.x > 0 && wm->ball.pos.loc.x < Field::MaxX/2)
        {
            tANDp target = findTarget();
            OperatingPosition kick = BallControl(target.pos, target.prob, this->id, rc.maxSpeed);
            rc.fin_pos = kick.pos;
            rc.useNav = kick.useNav;

            if( kick.readyToShoot )
            {
                rc.kickspeedx = 255; //detectKickSpeed(kickType::Shoot, kick.shootSensor);
                kickIt = true;
            }
        }
    }
    else
    {
        rc.fin_pos = queuePos;
    }

    rc.isBallObs = true;
    rc.isKickObs = true;

    return rc;
}
