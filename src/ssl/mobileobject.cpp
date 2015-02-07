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
//    if(!isValid)
//    {
//        vel.loc = {0, 0};
//        vel.dir = 0;
//        return;
//    }
//    PositionTimeCamera last = vel_postc;
//    vel.loc = vel.loc + (((pos.loc - last.pos.loc) / (time - last.time)) - vel.loc);// filter!!!!
//    vel.dir = (pos.dir - last.pos.dir) / (time - last.time);
//    vel_postc.pos = pos;
//    vel_postc.time = time;
}


void MobileObject::seenAt(vector<Position> p, double t, int c)
{
    //    if(p.size()<1) return;
    //   isValid = true;
    //   timer_seen.start(timer_seen_interval); //restart
    //   if(!timer_vel.isActive()) timer_vel.start(timer_vel_interval);
    //   PositionTimeCamera ans;
    //   ans.time = t;
    //   ans.camera = c;
    //   int min_i = 0;
    //   double min_d = pos.loc.dist2(p[0].loc);
    //   for(size_t i=0; i < p.size(); i++)
    //   {
    //   double d = pos.loc.dist2(p[i].loc);
    //   if(d < min_d)
    //   {
    //   min_d = d;
    //   min_i = i;
    //   }
    //   }
    //   Position sel_pos = p[min_i];
    //   ans.pos.loc = pos.loc + (sel_pos.loc - pos.loc) * 0.5;
    //   ans.pos.dir = fabs(pos.dir) + (fabs(sel_pos.dir) - fabs(pos.dir)) * 0.8;
    //   if(sel_pos.dir < 0 ) ans.pos.dir *= -1;
    //   appendPostc(ans);
    //   /*
    //   camera = ans.camera;
    //   time = ans.time;
    //   pos = ans.pos;
    //   return;
    //   */
    //   min_i = 0;
    //   min_d = pos.loc.dist2(last_postc[0].pos.loc);
    //   for(int i=0; i<LAST_COUNT; i++)
    //   {
    //   if(last_postc[i].time < 0) continue;
    //   double d = pos.loc.dist2(last_postc[i].pos.loc);
    //   if(d < min_d)
    //   {
    //   min_d = d;
    //   min_i = i;
    //   }
    //   }
    //   PositionTimeCamera res = last_postc[min_i];
    //   camera = res.camera;
    //   time = res.time;
    //   pos = res.pos;
    /////////////////////////////////////////
    if(p.size()<1) return;
    //isValid = true;
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
    //qDebug() << p.size();
    //for(int i =0; i>p.size())

    ans.pos.loc = p[min_i].loc;
    ans.pos.dir = p[min_i].dir;


    Position pos_buff;
    int camera_buff;

    if(vel_postc.pos.loc.x == pos.loc.x )
    {
        pos_buff = pos;
        camera_buff =camera;
        camera = ans.camera;
        time = ans.time;
        pos = ans.pos;
        vel_calc();
    }


    if((pos.loc - ans.pos.loc).length() < 270 || (pos.loc.x == 0 ) || isValid == false )
        //if the replacement of  object were smaller than 270 mm
        //in  15 ms ,that is it's speed is smaller than 4m/s or it is not the start of the game that all locations are at {0.0} or it is not after
        //while the  object was absent in field
    {
        if(ans.camera == 1 && camera_buff == 0 && (pos_buff.loc - ans.pos.loc).length() <60)//if both cameras see the object ,lower camera index shows the location
            // of tht object for reducing noise of different cammera settings
        {
            pos = pos_buff;
            qDebug()<<"d";
        }//this filter is foolish,only it is needed when cameras give different locations for objects in their interface!!
        else
        {
            camera = ans.camera;
            time = ans.time;
            pos = ans.pos;
            vel_calc();
        }

    }


    isValid = true;

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
    vel.dir = (pos.dir - last.pos.dir) / (time - last.time );


    pos_predicted.loc = pos.loc + vel.loc * (time - last.time);
    pos_predicted.dir = pos.dir + vel.dir * (time - last.time);


    vel_postc.pos = pos;
    vel_postc.time = time;
}
