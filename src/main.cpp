
// http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml
// http://www.possibility.com/Cpp/CppCodingStandard.html
// http://geosoft.no/development/cppstyle.html

#include <QApplication>
#include <signal.h>
#include <stdlib.h>
#include <assert.h>
#include <iostream>
#include "mainwindow.h"
#include "soccer.h"
#include "bee.h"

using namespace std;

int main(int argc, char *argv[])
{


    QCoreApplication::setOrganizationName("KN2C");
    QCoreApplication::setOrganizationDomain("www.kn2c.ir");
    QCoreApplication::setApplicationName("KN2C SSL");

    QApplication a(argc, argv);
    cout << endl;
    cout << "KN2C Small Size League Robocup Team" << endl;
    cout << "K.N.Toosi University of Technology" << endl;
    cout << "Computer & Electrical Eng. Faculty" << endl;
    cout << "Version 7.0.0" << endl;
    cout << endl;


    Soccer soccer;
   soccer.wm->mark_coef[1]=50;
   soccer.wm->pos_coef[2]=50;
   soccer.wm->pos_coef[3]=50;
   soccer.wm->pos_coef[4]=50;
   soccer.wm->pos_coef[1]=50;

   soccer.wm->mark_coef[2]=50;
   soccer.wm->mark_coef[3]=50;
   soccer.wm->mark_coef[4]=50;


    MainWindow w(&soccer);
   MyThread thread1(&soccer);

  thread1.start();


////////////////////////////////

  cout << "Computer & Electrical Eng. Faculty" << endl;
    bool gui = true;
    if(argc > 1)
        if(strcmp(argv[1], "nogui") == 0)
            gui = false;
    if(gui) w.show();

    return a.exec();
}

