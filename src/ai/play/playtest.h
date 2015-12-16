#ifndef PLAYTEST_H
#define PLAYTEST_H
#include "play.h"
#include "man2man.h"
#include "positioning.h"

class PlayTest : public Play
{
    Q_OBJECT
public:
    explicit PlayTest(WorldModel *worldmodel, QObject *parent = 0);
    virtual void execute();
    virtual int enterCondition();
    void setTactics(int index);
    void pressing(int ballOwner);
    int findBallOwner();
    int findOppReciever();
    void setPlayer2Keep(int ourR,int oppR);
    QList<AgentRegion> freeRegions();

private:
    TacticGoalie*   tGolie;
    TacticDefender* tDefenderLeft;
    TacticDefender* tDefenderRight;
    TacticAttacker* tAttackerLeft;
    TacticAttacker* tAttackerRight;
    TacticAttacker* tAttackerMid;

    QString game_status;

    Vector2D midPrevious;
    Vector2D rightPrevious;
    Vector2D leftPrevious;

    virtual void initRole();
    void coach();
    void setGameOnPos(int ourR,Vector2D loc);
    bool roleIsValid(AgentRole role);
    bool isDefender(int index);
};
#endif // PLAYTEST_H
