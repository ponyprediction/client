#include "client.hpp"
#include "util.hpp"
#include <QFile>
#include <QStringList>


Client::Client(QString ip, int port) :
    ip(ip),
    port(port),
    tcpSocket(),
    jobJson(),
    brainJson(),
    jobIsSet(false),
    brainIsSet(false),
    currentAnswer("")
{
    QObject::connect(&tcpSocket, SIGNAL(readyRead()),
                     this, SLOT(read()));
    QObject::connect(&tcpSocket, SIGNAL(disconnected()),
                     this, SLOT(onDisconnected()));
}


Client::~Client()
{

}


void Client::connect()
{
    tcpSocket.connectToHost(ip, port);
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
    handleRequest("getjob");
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
        emit connectionEstablished();
    }
    else if(answer == "welcome")
    {
        emit logged();
    }
    else if(answer == "wtf")
    {
        Util::addLog("Server doesn't seem to understand the request");
    }
    else if(answer == "unicorn")
    {
        emit loginRefused();
    }
    else if(answer.startsWith("job"))
    {
        jobJson = answer.remove(0,4);
        jobIsSet = true;
        if(brainIsSet && jobIsSet)
        {
            emit jobReceived(0, brainJson,jobJson);
            brainIsSet = false;
            jobIsSet = false;
        }
    }
    else if(answer == "bye")
    {
        tcpSocket.disconnectFromHost();
        emit disconnected();
    }
    else if(answer.startsWith("brain"))
    {
        brainJson = answer.remove(0,6);
        brainIsSet = true;
        if(brainIsSet && jobIsSet)
        {
            emit jobReceived(0,jobJson,brainJson);
            brainIsSet = false;
            jobIsSet = false;
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
