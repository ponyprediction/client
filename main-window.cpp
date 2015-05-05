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
  client("localhost", 50000)
{
  // UI
  addLog("Starting up UI...");
  ui->setupUi(this);
  ui->mainWidget->layout()->addWidget(&connectionForm);
  ui->logsWidget->layout()->addWidget(&logsForm);
  QObject::connect(connectionForm.ui->buttonConnect, SIGNAL(clicked()),
                   this, SLOT(onConnect()));
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

void MainWindow::onConnect()
{
  client.connect();
  connectionForm.ui->lineEditUsername->setEnabled(false);
  connectionForm.ui->lineEditPassword->setEnabled(false);
  connectionForm.ui->buttonConnect->setEnabled(false);
}

void MainWindow::onDisconnected()
{

}
