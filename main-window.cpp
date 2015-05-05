#include "main-window.hpp"
#include "ui_main-window.h"
#include "ui_connection-form.h"
#include "ui_logs-form.h"
#include "core/util.hpp"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  connectionForm(this),
  logsForm(this),
  tcpSocket(new QTcpSocket(this))
{
  // UI
  addLog("Starting up UI...");
  ui->setupUi(this);
  ui->mainWidget->layout()->addWidget(&connectionForm);
  ui->logsWidget->layout()->addWidget(&logsForm);
  QObject::connect(connectionForm.ui->buttonConnect, SIGNAL(clicked()),
                   this, SLOT(connect()));
  QObject::connect(tcpSocket, SIGNAL(readyRead()),
                   this, SLOT(read()));
  QObject::connect(tcpSocket, SIGNAL(disconnected()),
                   this, SLOT(onDisconnected()));
  addLog("UI ready");
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::addLog(const QString & message)
{
  logsForm.ui->textEditLogs->append(message);
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
  tcpSocket->connectToHost("192.168.1.63", 50000);
  connectionForm.ui->lineEditUsername->setEnabled(false);
  connectionForm.ui->lineEditPassword->setEnabled(false);
  connectionForm.ui->buttonConnect->setEnabled(false);
}

void MainWindow::onDisconnected()
{

}

void MainWindow::handleAnswer(QString answer)
{
  if(answer == "hi")
  {
    write("log "
          + connectionForm.ui->lineEditUsername->text()+ " "
          + connectionForm.ui->lineEditPassword->text());
  }
  if(answer == "welcome")
  {

  }
  if(answer == "wtf")
  {
    Util::addLog("Server doesn't seem to understand the request");
  }
  if(answer == "unicorn")
  {

  }
  if(answer == "job")
  {

  }
  if(answer == "bye")
  {
    // TODO disconnect
  }
}
