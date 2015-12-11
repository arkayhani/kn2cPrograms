#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "soccer.h"
#include "renderarea.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Soccer* soccer, QWidget *parent = 0);
    ~MainWindow();

private:
    QTimer timer;
    RenderArea *_render;
    Soccer* sc;
    QString returnRole(AgentRole Role);
    QString returnStatus(AgentStatus Status);

    void disableLearningUi();
    void enableLearningUi();

private slots:
    void timer_timeout();
    void on_btnSaveVars_clicked();
    void on_btnLoadVars_clicked();

    void on_startHillClimbing_button_clicked();

    void on_savePolicies_button_clicked();

    void on_loadPolicies_button_clicked();

    void on_playpause_button_clicked();

    void on_stoprecord_button_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
