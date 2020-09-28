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
   // Q_INIT_RESOURCE(textedit);

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

    /*LoginWindow lw;
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&lw);
        lw.resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
        lw.move((availableGeometry.width() - lw.width()) / 2,
                (availableGeometry.height() - lw.height()) / 2);

        int width = availableGeometry.width();
            int height = availableGeometry.height();
            width *= 0.5; // 90% of the screen size
            height *= 0.5; // 90% of the screen size
            QSize newSize( width, height );

            lw.resize(newSize);

    lw.exec();

    if(lw.getIsLogin()==1){
        MainWindow mw;*/
       /* const QRect availableGeometryMw = QApplication::desktop()->availableGeometry(&mw);
        int width = availableGeometryMw.width();
            int height = availableGeometryMw.height();
            width *= 0.9; // 90% of the screen size
            height *= 0.9; // 90% of the screen size
            QSize newSize( width, height );
            setGeometry(
                   QStyle::alignedRect(
                       Qt::LeftToRight,
                       Qt::AlignCenter,
                       newSize,
                       QApplication::desktop()->availableGeometry(&mw)
                   )
               );*/
       /* mw.show();
        return a.exec();
    }*/

    Client client;




    //QObject::connect(&client, &EchoClient::closed, &a, &QCoreApplication::quit);
    //te.show();
    //te.onTextConncet();

    return a.exec();
}

