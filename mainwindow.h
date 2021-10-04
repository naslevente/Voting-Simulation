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

   // slot to invoke update of probability table
   void UpdateProbabilityTable();

   // slot to start new simulations until probability numbers start to converge
   void StartSimulationIterations();

   // slot to continue running new simulations while probability numbers converge
   void ContinueSimulationIterations();

public slots:
    void StartProcess();
    void UpdateIterationLabel(int);

signals:

    // signal to deal with cleanup after probability numbers have converged
    void CleanupMainThread();

    // signal to begin simulation
    void BeginSimulation();

private:
    Ui::MainWindow *ui;

    // thread to deal with the start/stop button
    QThread *mainThread;
    SimulationWorker *worker;

    // private boolean fields
    bool isConverged = false;

    // !! private methods

    // number of candidates, number of voters, and number of iterations fields
    int numCandidates;
    int numVoters;
    int numberOfIterations;

    // boolean variable to indicate whether or not probability numbers have converged
    int isConvereged;

    // mainwindow gui elements
    QPushButton *startButton;
    QPushButton *finishButton;
    QLabel *iterationLabel;
    QTextEdit *votersInput;
    QTextEdit *candidatesInput;
    QTableWidget *tableWidget;

    // method to setup ui elements
    void SetupApplication();

    // method to update probability table
    void UpdateTableWidget(std::vector<std::vector<double>>);

};
#endif // MAINWINDOW_H
