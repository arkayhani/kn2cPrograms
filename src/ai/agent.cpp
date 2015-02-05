#include "agent.h"
#include "worldmodel.h"

Agent::Agent() :
    Robot()
{
    wm = 0;
    id = -1;
}

void Agent::setID(int id)
{
    this->id = id;
    nav.setID(id);
}

void Agent::setWorldModel(WorldModel *wm)
{
    this->wm = wm;
    nav.setWorldModel(wm);
}

void Agent::setOutputBuffer(OutputBuffer *outputBuffer)
{
    this->outputBuffer = outputBuffer;
}

void Agent::SendCommand(RobotCommand rc)
{
    if(!wm->ourRobot[id].isValid) return;

    ControllerInput ci = nav.calc(rc);
    ControllerResult co = ctrl.calc(ci);
        //qDebug() << "loc " << wm->ourRobot[3].pos.loc.x << wm->ourRobot[3].pos.loc.y<< "floc" << rc.fin_pos.loc.x << rc.fin_pos.loc.y;

//    if(id==3)
//    {
//       qDebug() << "ploc" << "X:"<<rc.fin_pos.loc.x << "Y"<<rc.fin_pos.loc.y;
//       qDebug() << "floc" << "X:"<<wm->ourRobot[3].pos.loc.x << "Y"<<wm->ourRobot[3].pos.loc.y;

//    }


    // Real Game Packet
    RobotData reRD;
    //@kamin
    reRD.RID = id;


    reRD.M0  = co.msR.M0;wm->ourRobot[id].vel.loc.x;
    reRD.M1  = co.msR.M1;
    reRD.M2  = co.msR.M2;1000*(-wm->ourRobot[id].vel.loc.x * sin(ci.cur_pos.dir) + wm->ourRobot[id].vel.loc.y * cos(ci.cur_pos.dir));co.msR.M2;
    reRD.M3  = co.msR.M3;1000*(wm->ourRobot[id].vel.loc.x * cos(ci.cur_pos.dir) + wm->ourRobot[id].vel.loc.y * sin(ci.cur_pos.dir));

    //@kamin
    reRD.KCK = (quint8) rc.kickspeedx;
    reRD.CHP = 0;
    outputBuffer->wpck.AddRobot(reRD);

    // grSim Packet
    grRobotData grRD;
    grRD.rid    = id;
    grRD.velx   = co.rs.VX;
    grRD.vely   = co.rs.VY;
    grRD.velw   = co.rs.VW;
    grRD.wheel1 = co.msS.M0;
    grRD.wheel2 = co.msS.M1;
    grRD.wheel3 = co.msS.M2;
    grRD.wheel4 = co.msS.M3;
    grRD.kickspeedx = rc.kickspeedx;
    grRD.kickspeedz = rc.kickspeedz;
    grRD.spinner = 0;
    outputBuffer->grpck.AddRobot(grRD);
}

void Agent::Halt()
{
    // Real Game Packet
    RobotData reRD;
    reRD.RID = id;
    reRD.M0 = 0;
    reRD.M1 = 0;
    reRD.M2 = 0;
    reRD.M3 = 0;
    reRD.KCK = 0;
    reRD.CHP = 0;
    outputBuffer->wpck.AddRobot(reRD);
    // grSim Packet
    grRobotData grRD;
    grRD.rid=id;
    grRD.velx = 0;
    grRD.vely = 0;
    grRD.velw = 0;
    grRD.wheel1=0;
    grRD.wheel2=0;
    grRD.wheel3=0;
    grRD.wheel4=0;
    grRD.kickspeedx=0;
    grRD.kickspeedz=0;
    grRD.spinner=0;
    outputBuffer->grpck.AddRobot(grRD);
}

