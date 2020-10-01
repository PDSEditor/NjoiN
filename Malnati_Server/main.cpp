#include <QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QDebug>
#include<QDataStream>

#include "server.h"
#include "iostream"

#include <QJsonObject>

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

//        Server* server = new Server();
        Server server;
        QObject::connect(&server, &Server::closed, &a, &QCoreApplication::quit);


        QString stringa ="test";
        int number = 3;
        QJsonObject json;

        json["stringa"] = stringa;
        json["number"] = number;

        qDebug()<<json;




    return a.exec();
}
