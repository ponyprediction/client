#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QString>
#include <QTcpSocket>

class Client : public QObject
{
    Q_OBJECT
  public:
    Client(QString ip, int port);
    ~Client();
    void connect();
    void write(QString message);
  private slots:
    void read();
    void onDisconnected();
  private:
    void handleAnswer(QString answer);
    QString ip;
    int port;
    QTcpSocket tcpSocket;
};

#endif // CLIENT_HPP
