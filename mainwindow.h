#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Simulation.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool getStopBoolean();

private slots:
    void on_start_button_clicked();

    void on_stop_button_clicked();

private:
    Ui::MainWindow *ui;

    // all the simulation instances
    Simulation *igenNemOne;
    Simulation *igenNemTwo;
    Simulation *schulzeOne;
    Simulation *schulzeTwo;

    // boolean to check if stop button is pressed
    bool isStop;


};
#endif // MAINWINDOW_H
