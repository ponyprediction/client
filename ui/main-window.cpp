#include "main-window.hpp"
#include "ui_main-window.h"
#include "ui_connection-form.h"
#include "ui_logs-form.h"
#include "core/util.hpp"
#include <QFile>

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
    QObject::connect(&client,
                     SIGNAL(jobReceived(int,QString,QString)),
                     this,
                     SLOT(onJobReceived(int,QString,QString)));
    // Other
    QObject::connect(ui->buttonTest, SIGNAL(clicked()), this, SLOT(onTest()));
    QObject::connect(ui->buttonTest2, SIGNAL(clicked()), this, SLOT(onTest2()));
    addLog("UI ready");
    //
    QString problems;
    QFile file("C:/Users/Loic/pony-prediction/problems/test.problems");
    file.open(QFile::ReadOnly);
    problems = file.readAll();
    file.close();
    QString bestBrain;
    file.setFileName("C:/Users/Loic/pony-prediction/brains/test.brain");
    file.open(QFile::ReadOnly);
    bestBrain = file.readAll();
    file.close();

    onJobReceived(42, problems, bestBrain);
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

void MainWindow::onJobReceived(int id,
                               QString problemsXml,
                               QString bestBrainXml)
{
    int brainCount = 4;
    int interval = 10;
    //qDebug() << problemsXml;
    //qDebug() << bestBrainXml;
    job = Job(id, problemsXml, bestBrainXml, brainCount, interval);
    //job.start();
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
