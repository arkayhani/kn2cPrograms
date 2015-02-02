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
timer_vel_interval = 40;
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
vel.loc = vel.loc + (((pos.loc - last.pos.loc) / (time - last.time)) - vel.loc);// filter!!!!
vel.dir = (pos.dir - last.pos.dir) / (time - last.time);
vel_postc.pos = pos;
vel_postc.time = time;
}


void MobileObject::seenAt(vector<Position> p, double t, int c)
{
    if(p.size()<1) return;
    isValid = true;
    timer_seen.start(timer_seen_interval); //restart

    PositionTimeCamera ans;
    ans.time = t;
    ans.camera = c;

    Vector2D buff;
    int min_i = 0;
    buff = p[min_i].loc - pos_predicted.loc ;
    double min_d = buff.length();
    for(size_t i=0; i < p.size(); i++)
    {
        buff = p[i].loc - pos_predicted.loc ;
        double d = buff.length();
        if(d < min_d)
        {
            min_d = d;
            min_i = i;
        }
    }

    ans.pos.loc = p[min_i].loc;
    ans.pos.dir = p[min_i].dir;

    camera = ans.camera;
    time = ans.time;
    pos = ans.pos;
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
pos_predicted.loc = pos.loc + vel.loc * (time - last.time);
pos_predicted.dir = pos.dir + vel.dir * (time - last.time);
vel_postc.pos = pos;
vel_postc.time = time;
}
