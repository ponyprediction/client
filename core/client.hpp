#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "core/problem.hpp"
#include "core/brain.hpp"
#include <QString>
#include <QTcpSocket>
#include <QTimer>


class Client : public QObject
{

    Q_OBJECT

public:

    Client(QString ip, int port);
    ~Client();
    void connect();
    void disconnect();
    void log(QString username, QString password);

    void askJob();
    void askProblems(const QString & jobId);
    void askBrain(const QString & jobId);
    void sendBrain(QString brain);

signals:

    void connectionEstablished();
    void timeoutConnect();
    void loginRefused();
    void logged();
    void disconnected();
    void jobReceived(QString jobId,
                     QString problemsJson,
                     QString bestBrainJson);
    void brainSentSuccessfully();
    void brainReceived(QString brainJson);
    void jobIdReceived(QString jobId);

private slots:

    void read();
    void onDisconnected();
    void onTimeoutConnect();

private:

    void handleAnswer(QString answer);
    void handleRequest(QString request);
    void write(QString message);
    QString ip;
    int port;
    QTcpSocket tcpSocket;

    QString trainingSetJson;
    QString brainJson;

    bool trainingSetIsSet;
    bool brainIsSet;

    QString currentAnswer;

    QTimer timerConnect;

    QString jobId;
};

#endif // CLIENT_HPP
