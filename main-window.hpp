#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include "ui/connection-form.hpp"
#include "ui/logs-form.hpp"
#include "core/client.hpp"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addLog(const QString & message);
  private slots:
    void onConnect();
    void onConnected();
    void onDisconnect();
    void onDisconnected();
    void onLoginRefused();
    void onLogged();
    void onTest();
    void onTest2();
  private:
    Ui::MainWindow *ui;
    LogsForm logsForm;
    ConnectionForm connectionForm;
    Client client;
    bool isWorking;
};
