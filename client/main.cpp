//#include "mainwindow.h"

#include "textedit.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QCommandLineParser>
#include <QCommandLineOption>
//#include "loginwindow.h"
//#include "socketmanager.h"
#include "client.h"
#include <QStyle>
#include <QDialog>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Rich Text");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(a);
    QString ip_address("127.0.0.1");
    if (argc == 2){
        ip_address = QString(argv[1]);
    }

    Client client;

    bool res=client.Login();
   if(res)
       return a.exec();
   else
       return 0;
}

