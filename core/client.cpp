#include "client.hpp"
#include "util.hpp"
#include <QFile>
#include <QStringList>

Client::Client(QString ip, int port) :
    ip(ip),
    port(port),
    tcpSocket()
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

void Client::askJob()
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
    // Timer ?
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
        // Parse data
        answer.remove(0,4);
        QStringList list = answer.split('|');
        // Create job
        const int & id = list[0].toInt();
        const QString & problems = list[1];
        const QString & bestBrain = list[2];
        emit jobReceived(id, problems, bestBrain);
    }
    else if(answer == "bye")
    {
        tcpSocket.disconnectFromHost();
        emit disconnected();
    }
    else if(answer.startsWith("brain"))
    {
        QString brainXML = answer.remove(0,6);
        QByteArray aa;
        aa.append(brainXML);
        QFile file("./test.brain");
        file.open(QIODevice::ReadWrite);
        file.write(aa);
        file.close();
    }
    else if(answer == "brainreceived")
    {

    }
    else
    {

    }
}

void Client::read()
{
    QString answer = tcpSocket.readAll();
    answer = answer.left(answer.size()-2);
    Util::addLog("Answer  : " + answer);
    handleAnswer(answer);
}

void Client::write(QString message)
{
    Util::addLog("Request : " + message);
    message += "\r\n";
    tcpSocket.write(message.toUtf8());
}
