#include "client.hpp"
#include "util.hpp"

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

void Client::onDisconnected()
{

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
  QByteArray block;
  message += "\r\n";
  block.append(message);
  tcpSocket.write(block);
}

void Client::handleAnswer(QString answer)
{
  if(answer == "hi")
  {
    /*write("log "
          + connectionForm.ui->lineEditUsername->text()+ " "
          + connectionForm.ui->lineEditPassword->text());*/

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

