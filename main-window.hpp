#pragma once

#include <QMainWindow>
#include <QTcpSocket>

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
    Ui::MainWindow *ui;
    QTcpSocket *tcpSocket;
    void write(QString message);

};
