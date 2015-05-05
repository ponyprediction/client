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
        void disconnect();
        void log(QString username, QString password);
        void write(QString message);
        void askJob();
        void askBrain();
        void sendBrain(QString brain);
    private slots:
        void read();
        void onDisconnected();
    signals:
        void connectionEstablished();
        void loginRefused();
        void logged();
        void disconnected();
    private:
        void handleAnswer(QString answer);
        QString ip;
        int port;
        QTcpSocket tcpSocket;
};

#endif // CLIENT_HPP
