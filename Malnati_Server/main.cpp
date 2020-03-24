#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QDebug>
#include<QDataStream>

#include "server.h"
#include "iostream"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

        QCommandLineParser parser;
        parser.setApplicationDescription("QtServer textEditor");
        parser.addHelpOption();

        QCommandLineOption dbgOption(QStringList() << "d" << "debug",
                QCoreApplication::translate("main", "Debug output [default: off]."));
        parser.addOption(dbgOption);
        QCommandLineOption portOption(QStringList() << "p" << "port",
                QCoreApplication::translate("main", "Port for echoserver [default: 1234]."),
                QCoreApplication::translate("main", "port"), QLatin1Literal("1234"));
        parser.addOption(portOption);
        parser.process(a);
        bool debug = parser.isSet(dbgOption);
        int port = parser.value(portOption).toInt();

        qDebug()<<"porta:"<<port;

        Server* server = new Server();
        QObject::connect(server, &Server::closed, &a, &QCoreApplication::quit);

        /*int n = 5;
        qDebug()<< (n<<1);

        QByteArray bytemex;
        for(int tmp =1; tmp<8; tmp = tmp+2) {
            for(int p=0;p<4;p++){
                bytemex.append(tmp >> (p * 8));
            }
        }

        qDebug()<<bytemex;

        QByteArray b;
        QDataStream stream(&b, QIODevice::WriteOnly);
        int i = 1357;
        stream<<i;
        qDebug()<<b; */



    return a.exec();
}
