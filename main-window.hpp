#pragma once

#include <QMainWindow>
#include <QTcpSocket>
#include "ui/connection-form.hpp"
#include "ui/logs-form.hpp"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
  public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
  private slots:
    void read();
    void connect();
    void onDisconnected();
  private:
    void handleAnswer(QString answer);
    void write(QString message);
    Ui::MainWindow *ui;
    LogsForm logsForm;
    ConnectionForm connectionForm;
    QTcpSocket *tcpSocket;
};
