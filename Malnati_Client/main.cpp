<<<<<<< HEAD
#include "mainwindow.h"

#include "textedit.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "loginwindow.h"



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

    LoginWindow *lw = new LoginWindow();
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(lw);
    lw->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    lw->move((availableGeometry.width() - lw->width()) / 2,
            (availableGeometry.height() - lw->height()) / 2);
    lw->show();


    //te.show();
    //te.onTextConncet();
    return a.exec();

}

=======
#include "mainwindow.h"

#include "textedit.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QCommandLineParser>
#include <QCommandLineOption>



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

    TextEdit mw;

    const QRect availableGeometry = QApplication::desktop()->availableGeometry(&mw);
    mw.resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    mw.move((availableGeometry.width() - mw.width()) / 2,
            (availableGeometry.height() - mw.height()) / 2);

    //APRE UNA PAGINA DI PRESENTAZIONE DEL TEXTEDIT
   // if (!mw.load(parser.positionalArguments().value(0, QLatin1String(":/example.html"))))

    mw.fileNew();

    mw.show();
    mw.onTextConncet();
    return a.exec();

}
>>>>>>> 3e4cc998bb356e7d0d56f393ca13f3a6e9896e64
