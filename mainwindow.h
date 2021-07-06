#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QtCore>
#include <QLabel>
#include <QTextEdit>
#include "Simulation.h"
#include "simulationworker.h"

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
   void on_stop_button_clicked();

   void on_finish_button_clicked();

public slots:
    void StartProcess();
    void UpdateIterationLabel(int);

private:
    Ui::MainWindow *ui;

    // thread to deal with the start/stop button
    QThread *mainThread;
    SimulationWorker *worker;

    // private methods
    void UpdateProbabilityTable(int);

    // number of candidates and number of voters fields
    int numCandidates;
    int numVoters;

    // mainwindow gui elements
    QPushButton *startButton;
    QPushButton *finishButton;
    QLabel *iterationLabel;
    QTextEdit *votersInput;
    QTextEdit *candidatesInput;
    QTableWidget *tableWidget;

    // method to setup ui elements
    void SetupApplication();

    // method to update table widget
    void UpdateTableWidget(std::vector<std::vector<double>>);

};
#endif // MAINWINDOW_H
