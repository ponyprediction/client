#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <QString>
#include <QTcpSocket>
#include "core/problem.hpp"
#include "core/brain.hpp"

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QString ip, int port);
    ~Client();
    void connect();
    void disconnect();
    void log(QString username, QString password);

    void askProblems();
    void askBrain();
    void sendBrain(QString brain);


signals:
    void connectionEstablished();
    void loginRefused();
    void logged();
    void disconnected();
    void jobReceived(int id,
                     QString problems,
                     QString bestBrain);
private slots:
    void read();
    void onDisconnected();
private:
    void handleAnswer(QString answer);
    void handleRequest(QString request);
    void write(QString message);
    QString ip;
    int port;
    QTcpSocket tcpSocket;

    QString jobJson;
    QString brainJson;

    bool jobIsSet;
    bool brainIsSet;

    QString currentAnswer;
};

#endif // CLIENT_HPP
