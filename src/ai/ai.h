#ifndef AI_H
#define AI_H

#include <QtCore>
#include <QObject>
#include "constants.h"
#include "outputbuffer.h"
#include "play.h"
#include "fpscounter.h"
#include <QUdpSocket>

class AI : public QObject
{
    Q_OBJECT
public:
    explicit AI(WorldModel* worldmodel, OutputBuffer* outputbuffer, QObject *parent = 0);
    FPSCounter fps;
    Play* getCurrentPlay();
    Tactic* getCurrentTactic(int i);

public slots:
    void Start();
    void Stop();

private:
    WorldModel *wm;
    OutputBuffer *outputbuffer;
    QTimer timer;
    Play *current_play;
    Tactic *current_tactic[PLAYERS_MAX_NUM];
    QUdpSocket *udp;
    QHostAddress ip;

private slots:
    void timer_timeout();

private:
    QList<Play*> plays;

};

#endif // AI_H
