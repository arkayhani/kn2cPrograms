#include "mobileobject.h"
#include "constants.h"

MobileObject::MobileObject() :
    QObject(0)
{
    time = 0;
    camera = 0;
    isValid = false;

    connect(&timer_seen, SIGNAL(timeout()), this, SLOT(timer_seen_timeout()));
    connect(&timer_vel, SIGNAL(timeout()), this, SLOT(timer_vel_timeout()));

    timer_seen_interval = 500;
    timer_vel_interval  = 40;

    for(int i=0; i<LAST_COUNT; i++)
        last_postc[i].time = -1;
}

void MobileObject::timer_seen_timeout()
{
    if(isValid==true)
    {
        //qDebug() << "isValid == false";
        isValid = false;
        pos.loc = {0, 0};
        pos.dir = 0;
        vel.loc = {0, 0};
        vel.dir = 0;
    }
}

void MobileObject::timer_vel_timeout()
{
    if(!isValid)
    {
        vel.loc = {0, 0};
        vel.dir = 0;
        return;
    }

    PositionTimeCamera last = vel_postc;
    vel.loc = vel.loc + (((pos.loc - last.pos.loc) / (time - last.time)) - vel.loc);
    vel.dir = (pos.dir - last.pos.dir) / (time - last.time);

    vel_postc.pos = pos;
    vel_postc.time = time;

}

void MobileObject::seenAt(vector<Position> p, double t, int c)
{
    if(p.size()<1) return;
    isValid = true;
    timer_seen.start(timer_seen_interval); //restart
    //    if(!timer_vel.isActive()) timer_vel.start(timer_vel_interval);

    PositionTimeCamera ans;
    ans.time = t;
    ans.camera = c;
    ans.pos=p[0];



    //    int min_i = 0;
    //    double min_d = pos.loc.dist2(p[0].loc);
    //    for(size_t i=0; i < p.size(); i++)
    //    {
    //        double d = pos.loc.dist2(p[i].loc);
    //        if(d < min_d)
    //        {
    //            min_d = d;
    //            min_i = i;
    //        }
    //    }

    //    Position sel_pos = p[0];
    //    ans.pos.loc = pos.loc + (sel_pos.loc - pos.loc) * 0.5;
    //    ans.pos.dir = fabs(pos.dir) + (fabs(sel_pos.dir) - fabs(pos.dir)) * 0.8;
    //    if(sel_pos.dir < 0 ) ans.pos.dir *= -1;
    PositionTimeCamera res = ans;//last_postc[min_i];
//    if((pos_predicted.loc - res.pos.loc).length() > STRANGE_ERR)
//    {
//        res.pos.loc = pos_predicted.loc * 0.7 + res.pos.loc * 0.3;
//    }
//    else
//    {
//        //res.pos.loc = pos_predicted.loc * 0.1 + res.pos.loc * 0.9;
//    }

//    if((pos_predicted.dir - res.pos.dir) > STRANGE_WERR)
//    {
//        res.pos.dir = pos_predicted.dir * 0.7 + res.pos.dir * 0.3;
//    }
//    else
//    {
//        //res.pos.dir = pos_predicted.dir * 0.1 + res.pos.dir * 0.9;
//    }

    //res.pos.dir = pos_predicted.dir * 0.01 + res.pos.dir * 0.99;
    res.pos.loc = pos_predicted.loc * 0.5 + res.pos.loc * 0.5;

    camera = res.camera;
    time = res.time;
    pos = res.pos;

    vel_calc();
}

void MobileObject::appendPostc(PositionTimeCamera &postc)
{
    for(int i = LAST_COUNT-1; i>0; i--)
        last_postc[i] = last_postc[i-1];
    last_postc[0] = postc;
}

void MobileObject::vel_calc()
{

    PositionTimeCamera last = vel_postc;
    vel.loc = vel.loc + (((pos.loc - last.pos.loc) / (time - last.time)) - vel.loc)*0.12;
    vel.dir = (pos.dir - last.pos.dir) / (time - last.time);

    pos_predicted.loc =  pos.loc + vel.loc * (time - last.time);
    pos_predicted.dir = pos.dir + vel.dir * (time - last.time);

    vel_postc.pos = pos;
    vel_postc.time = time;
}
