#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QString>
#include "soccer.h"
#include "population.h"
#include "algorithm2.h"

class MyThread : public QThread
{
public:
    static int a;
    static int gennum;
    static int popnum;
    static bool start2;
    // set name using initializer
    explicit MyThread(Soccer* s);

    // overriding the QThread's run() method
    void run();

    static Soccer* sc;
    //MainWindow* mn;
};

#endif // MYTHREAD_H
