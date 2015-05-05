#include "client.hpp"
#include "util.hpp"
#include <QFile>

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
    write("exit");
}

void Client::log(QString username, QString password)
{
    write("log " + username + " " + password);
}

void Client::onDisconnected()
{
    emit disconnected();
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

void Client::askJob()
{
    write("getjob");
}

void Client::askBrain()
{
    write("getbrain");
}

void Client::sendBrain(QString brain)
{
    QString request = "sendbrain " + brain;
    write(request);
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
        answer.remove(0,4);
        QStringList strs = answer.split(' ');
        int jobId = strs[0];
        QString inputs = ef;
        QString brain = ee;
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
