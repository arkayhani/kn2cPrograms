#ifndef TACTICTEST_H
#define TACTICTEST_H

#include "tactic.h"
#define DangerDist 300

class TacticTest : public Tactic
{
    Q_OBJECT
public:
    explicit TacticTest(WorldModel *worldmodel, QObject *parent = 0);
    virtual RobotCommand getCommand();

    RobotCommand goBehindBall();
    int findBestPlayerForPass();
    void setKickerID(int index);

    bool kicked;

private:
    bool canKick;
    bool firstKick;
    double alfa;
    int state;
    QTimer *timer;
    bool isFree(int index);
private slots:
    void timerEvent();

};

#endif // TACTICTEST_H
