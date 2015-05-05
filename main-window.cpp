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
    client(Util::getLineFromConf("ip"), Util::getLineFromConf("port").toInt()),
    isWorking(false)
{
    // UI
    addLog("Starting up UI...");
    ui->setupUi(this);
    ui->mainWidget->layout()->addWidget(&connectionForm);
    ui->logsWidget->layout()->addWidget(&logsForm);
    connectionForm.ui->buttonDisconnect->setVisible(false);
    QObject::connect(connectionForm.ui->buttonConnect, SIGNAL(clicked()),
                     this, SLOT(onConnect()));
    QObject::connect(connectionForm.ui->buttonDisconnect, SIGNAL(clicked()),
                     this, SLOT(onDisconnect()));
    // Client
    QObject::connect(&client, SIGNAL(connectionEstablished()),
                     this, SLOT(onConnected()));
    QObject::connect(&client, SIGNAL(loginRefused()),
                     this, SLOT(onLoginRefused()));
    QObject::connect(&client, SIGNAL(logged()),
                     this, SLOT(onLogged()));
    QObject::connect(&client, SIGNAL(disconnected()),
                     this, SLOT(onDisconnected()));
    // Other
    QObject::connect(ui->buttonTest, SIGNAL(clicked()), this, SLOT(onTest()));
    QObject::connect(ui->buttonTest2, SIGNAL(clicked()), this, SLOT(onTest2()));
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

void MainWindow::onConnected()
{
    client.log(connectionForm.ui->lineEditUsername->text(),
               connectionForm.ui->lineEditPassword->text());
}

void MainWindow::onLoginRefused()
{
    Util::addLog("The server refused the connection. "
                 "Bad username / Bad password / Unconfirmed account");
    connectionForm.ui->lineEditUsername->setEnabled(true);
    connectionForm.ui->lineEditPassword->setEnabled(true);
    connectionForm.ui->buttonConnect->setEnabled(true);
    client.disconnect();
}

void MainWindow::onLogged()
{
    connectionForm.ui->buttonConnect->setVisible(false);
    connectionForm.ui->buttonDisconnect->setVisible(true);
    if(!isWorking)
        client.askJob();
}

void MainWindow::onDisconnect()
{
    client.disconnect();
}

void MainWindow::onDisconnected()
{
    connectionForm.ui->buttonConnect->setVisible(true);
    connectionForm.ui->buttonDisconnect->setVisible(false);
    connectionForm.ui->lineEditUsername->setEnabled(true);
    connectionForm.ui->lineEditPassword->setEnabled(true);
    connectionForm.ui->buttonConnect->setEnabled(true);
}

void MainWindow::onTest()
{
    client.sendBrain("<brain>lol c'est pas un brain, je suis un hacker</brain");
}

void MainWindow::onTest2()
{
    client.askBrain();
}
