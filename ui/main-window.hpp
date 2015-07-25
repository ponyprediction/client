#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include <QTimer>
#include "ui/connection-form.hpp"
#include "ui/logs-form.hpp"
#include "core/client.hpp"
#include "core/job.hpp"
#include "ui/control-form.hpp"
#include "ui/local-form.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

    void newLog(const QString & log);

private slots:

    void onRefresh();
    void onConnect();
    void onConnected();
    void onTimeoutConnect();
    void onDisconnect();
    void onDisconnected();
    void onLoginRefused();
    void onLogged();

    void setMutationFrequencyAuto(bool value);
    void setMutationFrequency(double value);
    void setMutationFrequencyDown(double value);
    void setMutationFrequencyUp(double value);
    void setMutationFrequencyMax(double value);
    void setMutationFrequencyMin(double value);
    void setMutationIntensityAuto(bool value);
    void setMutationIntensity(double value);
    void setMutationIntensityDown(double value);
    void setMutationIntensityUp(double value);
    void setMutationIntensityMax(double value);
    void setMutationIntensityMin(double value);

    void setLimitDeviation(double value);

    void trainLocally();
    void solveLocally();
    void saveBrain();

    void onJobReceived(QString jobId,
                       QString problemsJson,
                       QString bestBrainJson);

    void sendBrain();
    void askBrain();
    void setBrain(QString brainJson);

private:

    void addLog(const QString & message);
    void startTraining(QString jobId,
                       QString trainingSetJson,
                       QString bestBrainJson, Brain::Mode mode);

    void solve(QString problemsJson,
               QString brainJson);

    Ui::MainWindow * ui;
    ConnectionForm connectionForm;
    LogsForm logsForm;
    ControlForm controlForm;
    LocalForm localForm;
    Client client;
    Job * job;
    QTimer timerRefresh;
    QTimer timerSendBrain;
};
