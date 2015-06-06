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


void Client::askProblems()
{
    handleRequest("gettrainingset");
}


void Client::askBrain()
{
    handleRequest("getbrain");
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
    else if(answer.startsWith("trainingset"))
    {
        trainingSetJson = answer.remove(0,12);
        trainingSetIsSet = true;
    }
    else if(answer.startsWith("brain"))
    {
        brainJson = answer.remove(0,6);
        brainIsSet = true;
        if(brainIsSet && trainingSetIsSet)
        {
            emit jobReceived(0, trainingSetJson, brainJson);
            brainIsSet = false;
        }
    }
    else if(answer == "brainreceived")
    {
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
