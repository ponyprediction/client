#include "client.hpp"
#include "util.hpp"
#include <QFile>
#include <QStringList>


Client::Client(QString ip, int port) :
    ip(ip),
    port(port),
    tcpSocket(),
    trainingSetJson(),
    brainJson(),
    trainingSetIsSet(false),
    brainIsSet(false),
    currentAnswer(""),
    timerConnect(this)
{
    //
    timerConnect.setSingleShot(true);
    //
    QObject::connect(&tcpSocket, SIGNAL(readyRead()),
                     this, SLOT(read()));
    QObject::connect(&tcpSocket, SIGNAL(disconnected()),
                     this, SLOT(onDisconnected()));
    QObject::connect(&timerConnect, SIGNAL(timeout()),
                     this, SLOT(onTimeoutConnect()));
    //tcpSocket.
}


Client::~Client()
{
}


void Client::connect()
{
    timerConnect.start(Util::getLineFromConf(
                           "intervalTimeoutConnection").toInt());
    tcpSocket.connectToHost(ip, port);
}


void Client::onTimeoutConnect()
{
    Util::writeWarning("can't connect to server");
    tcpSocket.disconnectFromHost();
    emit timeoutConnect();
}


void Client::disconnect()
{
    handleRequest("exit");
}


void Client::log(QString username, QString password)
{
    handleRequest("log " + username + " " + password);
}

void Client::askJob()
{
    handleRequest("getjobid");
}


void Client::askProblems(const QString & jobId)
{
    handleRequest("gettrainingset " + jobId);
}


void Client::askBrain(const QString & jobId)
{
    handleRequest("getbrain " + jobId);
}


void Client::sendBrain(QString brain)
{
    handleRequest("sendbrain " + brain);
}


void Client::handleRequest(QString request)
{
    write(request);
}


void Client::onDisconnected()
{
    brainIsSet = false;
    trainingSetIsSet = false;
    emit disconnected();
}




void Client::handleAnswer(QString answer)
{
    if(answer == "hi")
    {
        timerConnect.stop();
        emit connectionEstablished();
    }
    else if(answer == "welcome")
    {
        emit logged();
    }
    else if(answer == "wtf")
    {
        Util::writeError("The server doesn't seem to understand the request");
    }
    else if(answer == "bye")
    {
        tcpSocket.disconnectFromHost();
        emit disconnected();
    }
    else if(answer == "unicorn")
    {
        emit loginRefused();
        disconnect();
    }
    else if(answer == "brainreceived")
    {
        emit brainSentSuccessfully();
    }
    else if(answer.startsWith("trainingset "))
    {
        trainingSetJson = answer.remove(0,12);
        trainingSetIsSet = true;
    }
    else if(answer.startsWith("brain "))
    {
        brainJson = answer.remove(0,6);
        if(!brainIsSet && trainingSetIsSet)
        {
            brainIsSet = true;
            emit jobReceived(jobId, trainingSetJson, brainJson);
        }
        else if(brainIsSet && trainingSetIsSet)
        {
            emit brainReceived(brainJson);
        }
    }
    else if(answer.startsWith("jobid "))
    {
        jobId = answer.remove(0,6);
        askProblems(jobId);
        askBrain(jobId);
    }
}


void Client::read()
{
    QString answerTotal = tcpSocket.readAll();
    QStringList list = answerTotal.split("\r\n");
    for(int i = 0 ; i < list.size() ; i++)
    {
        if(i)
        {
            handleAnswer(currentAnswer);
            currentAnswer = list[i];
        }
        else
        {
            currentAnswer += list[i];
        }
    }
}


void Client::write(QString message)
{
    message += "\r\n";
    tcpSocket.write(message.toUtf8());
}
