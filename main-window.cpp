#include "main-window.hpp"
#include "ui_main-window.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  tcpSocket(new QTcpSocket(this))
{
  // UI
  ui->setupUi(this);
  QObject::connect(ui->buttonConnect, SIGNAL(clicked()),
                   this, SLOT(connect()));
  QObject::connect(tcpSocket, SIGNAL(readyRead()),
                   this, SLOT(read()));
  QObject::connect(tcpSocket, SIGNAL(disconnected()),
                   this, SLOT(onDisconnected()));
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::write(QString message)
{
  QByteArray block;
  message += "\r\n";
  block.append(message);
  tcpSocket->write(block);
}

void MainWindow::read()
{
  QString answer = tcpSocket->readAll();
  answer = answer.left(answer.size()-2);
  handleAnswer(answer);
}

void MainWindow::connect()
{
  tcpSocket->connectToHost("192.168.1.63",50000);
}

void MainWindow::onDisconnected()
{

}

void MainWindow::handleAnswer(QString answer)
{
  if(answer == "hi")
  {
    write("log "
          + ui->lineEditUsername->text()+ " "
          + ui->lineEditPassword->text());
  }
  if(answer == "bye")
  {
    // TODO disconnect
  }
}
