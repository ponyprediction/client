#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include "ui/connection-form.hpp"
#include "ui/logs-form.hpp"
#include "core/client.hpp"
#include "core/job.hpp"
#include "ui/control-form.hpp"

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
        void onConnect();
        void onConnected();
        void onDisconnect();
        void onDisconnected();
        void onLoginRefused();
        void onLogged();
        void onTest();
        void onTest2();
        void onJobReceived(int id,
                           QString problemsXml,
                           QString bestBrainXml);
    private:
    void addLog(const QString & message);
        Ui::MainWindow *ui;
        ConnectionForm connectionForm;
        LogsForm logsForm;
        ControlForm controlForm;
        Client client;
        bool isWorking;
        Job * job;
};
