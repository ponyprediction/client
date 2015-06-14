#include "main-window.hpp"
#include "ui_main-window.h"
#include "ui_connection-form.h"
#include "ui_logs-form.h"
#include "ui_control-form.h"
#include "ui_local-form.h"
#include "core/util.hpp"
#include <QFile>
#include <QFileDialog>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    connectionForm(this),
    logsForm(this),
    controlForm(this),
    localForm(this),
    client(Util::getLineFromConf("ip"),
           Util::getLineFromConf("port").toInt()),
    timerRefresh(this),
    timerSendBrain(this)
{
    // UI
    ui->setupUi(this);
    QObject::connect(this,
                     SIGNAL(newLog(QString)),
                     logsForm.ui->plainTextEditLogs,
                     SLOT(appendPlainText(QString)));
    addLog("Starting up UI...");
    controlForm.setVisible(false);
    ui->mainWidget->layout()->addWidget(&connectionForm);
    ui->mainWidget->layout()->addWidget(&localForm);
    ui->logsWidget->layout()->addWidget(&logsForm);
    //
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
                     SIGNAL(jobReceived(QString,QString,QString)),
                     this,
                     SLOT(onJobReceived(QString,QString,QString)));
    QObject::connect(&client,
                     SIGNAL(timeoutConnect()),
                     this,
                     SLOT(onTimeoutConnect()));
    QObject::connect(&client,
                     SIGNAL(brainSentSuccessfully()),
                     this,
                     SLOT(askBrain()));
    QObject::connect(&client,
                     SIGNAL(brainReceived(QString)),
                     this,
                     SLOT(setBrain(QString)));
    // Local
    QObject::connect(localForm.ui->buttonLocalTraining,
                     SIGNAL(clicked()),
                     this,
                     SLOT(trainLocally()));
    QObject::connect(localForm.ui->buttonLocalSolving,
                     SIGNAL(clicked()),
                     this,
                     SLOT(solveLocally()));
    // Training
    QObject::connect(controlForm.ui->buttonSave,
                     SIGNAL(clicked()),
                     this,
                     SLOT(saveBrain()));
    // Training - Frequency
    QObject::connect(controlForm.ui->radioButtonMutationFrequencyAuto,
                     SIGNAL(clicked(bool)),
                     this,
                     SLOT(setMutationFrequencyAuto(bool)));
    QObject::connect(controlForm.ui->doubleSpinBoxMutationFrequency,
                     SIGNAL(valueChanged(double)),
                     this,
                     SLOT(setMutationFrequency(double)));
    QObject::connect(controlForm.ui->doubleSpinBoxMutationFrequencyUp,
                     SIGNAL(valueChanged(double)),
                     this,
                     SLOT(setMutationFrequencyUp(double)));
    QObject::connect(controlForm.ui->doubleSpinBoxMutationFrequencyDown,
                     SIGNAL(valueChanged(double)),
                     this,
                     SLOT(setMutationFrequencyDown(double)));
    QObject::connect(controlForm.ui->doubleSpinBoxMutationFrequencyMin,
                     SIGNAL(valueChanged(double)),
                     this,
                     SLOT(setMutationFrequencyMin(double)));
    QObject::connect(controlForm.ui->doubleSpinBoxMutationFrequencyMax,
                     SIGNAL(valueChanged(double)),
                     this,
                     SLOT(setMutationFrequencyMax(double)));
    // Training - Intensity
    QObject::connect(controlForm.ui->radioButtonMutationIntensityAuto,
                     SIGNAL(clicked(bool)),
                     this,
                     SLOT(setMutationIntensityAuto(bool)));
    QObject::connect(controlForm.ui->doubleSpinBoxMutationIntensity,
                     SIGNAL(valueChanged(double)),
                     this,
                     SLOT(setMutationIntensity(double)));
    QObject::connect(controlForm.ui->doubleSpinBoxMutationIntensityUp,
                     SIGNAL(valueChanged(double)),
                     this,
                     SLOT(setMutationIntensityUp(double)));
    QObject::connect(controlForm.ui->doubleSpinBoxMutationIntensityDown,
                     SIGNAL(valueChanged(double)),
                     this,
                     SLOT(setMutationIntensityDown(double)));
    QObject::connect(controlForm.ui->doubleSpinBoxMutationIntensityMin,
                     SIGNAL(valueChanged(double)),
                     this,
                     SLOT(setMutationIntensityMin(double)));
    QObject::connect(controlForm.ui->doubleSpinBoxMutationIntensityMax,
                     SIGNAL(valueChanged(double)),
                     this,
                     SLOT(setMutationIntensityMax(double)));
    //
    QObject::connect(&timerRefresh, SIGNAL(timeout()), this, SLOT(onRefresh()));
    QObject::connect(&timerSendBrain, SIGNAL(timeout()),
                     this, SLOT(sendBrain()));
    addLog("UI ready");
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onRefresh()
{
    // Frequency
    controlForm.ui->doubleSpinBoxMutationFrequency->setValue(
                job->getMutationFrequency());
    controlForm.ui->doubleSpinBoxMutationFrequencyDown->setValue(
                job->getMutationFrequencyDown());
    controlForm.ui->doubleSpinBoxMutationFrequencyUp->setValue(
                job->getMutationFrequencyUp());
    controlForm.ui->doubleSpinBoxMutationFrequencyMin->setValue(
                job->getMutationFrequencyMin());
    controlForm.ui->doubleSpinBoxMutationFrequencyMax->setValue(
                job->getMutationFrequencyMax());
    controlForm.ui->doubleSpinBoxMutationFrequency->setEnabled(
                !job->getMutationFrequencyAuto());
    controlForm.ui->radioButtonMutationFrequencyAuto->setChecked(
                job->getMutationFrequencyAuto());
    // Intensity
    controlForm.ui->doubleSpinBoxMutationIntensity->setValue(
                job->getMutationIntensity());
    controlForm.ui->doubleSpinBoxMutationIntensityDown->setValue(
                job->getMutationIntensityDown());
    controlForm.ui->doubleSpinBoxMutationIntensityUp->setValue(
                job->getMutationIntensityUp());
    controlForm.ui->doubleSpinBoxMutationIntensityMin->setValue(
                job->getMutationIntensityMin());
    controlForm.ui->doubleSpinBoxMutationIntensityMax->setValue(
                job->getMutationIntensityMax());
    controlForm.ui->doubleSpinBoxMutationIntensity->setEnabled(
                !job->getMutationIntensityAuto());
    controlForm.ui->radioButtonMutationIntensityAuto->setChecked(
                job->getMutationIntensityAuto());
    // Ratios
    controlForm.ui->labelRatio->setText(
                QString::number(job->getBestRatio(), 'f', 6));
    controlForm.ui->labelAverageRatio->setText(
                QString::number(job->getAverageRatio(), 'f', 6));
}


void MainWindow::addLog(const QString & message)
{
    emit newLog(message);
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

void MainWindow::onTimeoutConnect()
{
    connectionForm.ui->lineEditUsername->setEnabled(true);
    connectionForm.ui->lineEditPassword->setEnabled(true);
    connectionForm.ui->buttonConnect->setEnabled(true);
}


void MainWindow::onLoginRefused()
{
    Util::writeError("The server refused the connection. "
                     "Bad username / Bad password / Unconfirmed account");
    connectionForm.ui->lineEditUsername->setEnabled(true);
    connectionForm.ui->lineEditPassword->setEnabled(true);
    connectionForm.ui->buttonConnect->setEnabled(true);
}


void MainWindow::onLogged()
{
    connectionForm.ui->buttonConnect->setVisible(false);
    connectionForm.ui->buttonDisconnect->setVisible(true);
    client.askJob();
}


void MainWindow::startTraining(QString jobId,
                               QString trainingSetJson,
                               QString bestBrainJson,
                               Brain::Mode mode)
{
    bool ok = true;
    int brainCount = Util::getLineFromConf("brainCount").toInt();
    if(ok)
    {
        job = new Job(jobId, trainingSetJson, bestBrainJson, brainCount, ok, mode);
    }
    if(ok)
    {
        job->start();
        connectionForm.setVisible(false);
        ui->mainWidget->layout()->removeWidget(&connectionForm);
        localForm.setVisible(false);
        ui->mainWidget->layout()->removeWidget(&localForm);
        controlForm.setVisible(true);
        ui->mainWidget->layout()->addWidget(&controlForm);
        timerRefresh.start(30);
    }
}


void MainWindow::solve(QString problemsJson, QString brainJson)
{
    (void) problemsJson;
    (void) brainJson;
    bool ok = true;
    QString str;
    Brain::Mode mode = Brain::SINGLE_SHOW;
    QString modeStr;
    if(ok)
    {
        modeStr = Util::getLineFromConf("mode", &ok);
    }
    if(ok)
    {
        if(modeStr == "single-win")
        {
            mode = Brain::SINGLE_WIN;
        }
        else if(modeStr == "single-show")
        {
            mode = Brain::SINGLE_SHOW;
        }
        else
        {
            ok = false;
            Util::writeError("invalid mode : " + modeStr);
        }
    }
    if(ok)
    {
        str = Job::getPrediction(problemsJson, brainJson, ok, mode);
    }
    if(ok)
    {
        Util::write("Prediction : " + str);
    }
    Util::write("not done");
}


void MainWindow::setMutationFrequencyAuto(bool value)
{
    job->setMutationFrequencyAuto(value);
}
void MainWindow::setMutationFrequency(double value)
{
    job->setMutationFrequency(value);
}
void MainWindow::setMutationFrequencyDown(double value)
{
    job->setMutationFrequencyDown(value);
}
void MainWindow::setMutationFrequencyUp(double value)
{
    job->setMutationFrequencyUp(value);
}
void MainWindow::setMutationFrequencyMax(double value)
{
    job->setMutationFrequencyMax(value);
}
void MainWindow::setMutationFrequencyMin(double value)
{
    job->setMutationFrequencyMin(value);
}


void MainWindow::setMutationIntensityAuto(bool value)
{
    job->setMutationIntensityAuto(value);
}
void MainWindow::setMutationIntensity(double value)
{
    job->setMutationIntensity(value);
}
void MainWindow::setMutationIntensityDown(double value)
{
    job->setMutationIntensityDown(value);
}
void MainWindow::setMutationIntensityUp(double value)
{
    job->setMutationIntensityUp(value);
}
void MainWindow::setMutationIntensityMax(double value)
{
    job->setMutationIntensityMax(value);
}
void MainWindow::setMutationIntensityMin(double value)
{
    job->setMutationIntensityMin(value);
}


void MainWindow::trainLocally()
{
    bool ok = true;
    QString trainingSet;
    QFile file(Util::getLineFromConf("trainingSetFilename"));
    QString bestBrain;
    QString jobId("local");
    Brain::Mode mode = Brain::SINGLE_SHOW;
    QString modeStr;
    //
    if(ok)
    {
        modeStr = Util::getLineFromConf("mode", &ok);
    }
    if(ok)
    {
        if(modeStr == "single-win")
        {
            mode = Brain::SINGLE_WIN;
        }
        else if(modeStr == "single-show")
        {
            mode = Brain::SINGLE_SHOW;
        }
        else
        {
            ok = false;
            Util::writeError("invalid mode : " + modeStr);
        }
    }
    if(ok && !file.open(QFile::ReadOnly))
    {
        ok = false;
        Util::writeError("can't open training set : "
                         + QFileInfo(file).absoluteFilePath());
    }
    if(ok)
    {
        trainingSet = file.readAll();
        file.close();
        file.setFileName(Util::getLineFromConf("brainFilename"));
    }
    if(ok && !file.open(QFile::ReadOnly))
    {
        ok = false;
        Util::writeError("can't open brain : "
                         + QFileInfo(file).absoluteFilePath());
    }
    if(ok)
    {
        bestBrain = file.readAll();
        file.close();
        startTraining(jobId, trainingSet, bestBrain, mode);
    }
}


void MainWindow::solveLocally()
{
    QString problems;
    QFile file(Util::getLineFromConf("problemsFilename"));
    file.open(QFile::ReadOnly);
    problems = file.readAll();
    file.close();
    QString brain;
    file.setFileName(Util::getLineFromConf("brainFilename"));
    file.open(QFile::ReadOnly);
    brain = file.readAll();
    file.close();
    solve(problems, brain);
}


void MainWindow::saveBrain()
{
    QString fileName = QFileDialog::getSaveFileName(
                this,"Save Brain",
                Util::getLineFromConf("pathToBrains") + "/default.brain",
                "Brain (*.brain)");
    job->saveBestBrain(fileName);
}


void MainWindow::onJobReceived(QString jobId,
                               QString problemsJson,
                               QString bestBrainJson)
{
    bool ok = true;
    Brain::Mode mode = Brain::SINGLE_SHOW;
    QString modeStr;
    /* TODO : replace with mode from server */
    if(ok)
    {
        modeStr = Util::getLineFromConf("mode", &ok);
    }
    if(ok)
    {
        if(modeStr == "single-win")
        {
            mode = Brain::SINGLE_WIN;
        }
        else if(modeStr == "single-show")
        {
            mode = Brain::SINGLE_SHOW;
        }
        else
        {
            ok = false;
            Util::writeError("invalid mode : " + modeStr);
        }
    }
    /**/
    if(ok)
    {
        startTraining(jobId, problemsJson, bestBrainJson, mode);
        timerSendBrain.start(Util::getLineFromConf("intervalSendBrain").toInt());
    }
}

void MainWindow::sendBrain()
{
    client.sendBrain(job->getBestBrain());
}


void MainWindow::askBrain()
{
    client.askBrain(job->getId());
}


void MainWindow::setBrain(QString brainJson)
{
    job->setBrain(brainJson);
}


void MainWindow::onDisconnect()
{
    client.disconnect();
}


void MainWindow::onDisconnected()
{
    connectionForm.ui->lineEditUsername->setEnabled(true);
    connectionForm.ui->lineEditPassword->setEnabled(true);
    connectionForm.ui->buttonConnect->setEnabled(true);
    connectionForm.ui->buttonConnect->setVisible(true);
    connectionForm.ui->buttonDisconnect->setVisible(false);
    if(job)
    {
        timerRefresh.stop();
        timerSendBrain.stop();
        connectionForm.setVisible(true);
        ui->mainWidget->layout()->addWidget(&connectionForm);
        localForm.setVisible(true);
        ui->mainWidget->layout()->addWidget(&localForm);
        controlForm.setVisible(false);
        ui->mainWidget->layout()->removeWidget(&controlForm);
        job->stop();
        delete job;
    }
}
