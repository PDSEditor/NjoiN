#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include "loginwindow.h"
#include "accountinterface.h"
#include "inserttitle.h"

socketManager *sock;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("QLabel { background-color : green; color : black; }");
//    for(int i=0;i<10;i++){
//        addElementforUser("Documento " + QString::number(i));
//    }
//sock=new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::newFile(){
    this->hide();
    InsertTitle it;

    connect(&it,&InsertTitle::setTitle,this,&MainWindow::receiveTitle);
    connect(&it,&InsertTitle::showMw,this,&MainWindow::openMw);

    it.exec();

    // This is available in all editors.

}

int MainWindow::getSiteId() const
{
    return siteId;
}

void MainWindow::setSiteId(int value)
{
    siteId = value;
}

void MainWindow::receiveimage(QPixmap& q){
    QByteArray bArray;
    QBuffer buffer(&bArray);
    buffer.open(QIODevice::WriteOnly);
    q.save(&buffer,"PNG");
    emit sendImage(bArray);

}

void MainWindow::setImage(QPixmap im){
    image=im;
}

QPixmap MainWindow::getImage(){
    return image;
}
void MainWindow::setUsername(QString username){
    MainWindow::username=username;
}
QString MainWindow::getUsername(){
    return username;
}
QList<QString> MainWindow::getList(){
    return documents;
}
void MainWindow::setList(QList<QString> l){
    documents=l;
}

void MainWindow::receivedInfoAccount(Message& m){
   setUsername(m.getParams().at(0));
   setSiteId(m.getSender());
   //setImage(m.getParams().at(2));
   QList<QString> tmp;
   for(int i=2;i<m.getParams().size();i++){
       documents.append(m.getParams().at(i));
       addElementforUser(documents.at(i-2));
   }

}
void MainWindow::on_pushButton_clicked()
{
    newFile();

}

void MainWindow::addElementforUser(QString string){
    ui->listWidget->addItem(string);
}

void MainWindow::on_actionNew_triggered()
{
    newFile();
}

void MainWindow::on_actionAccount_triggered()
{
    AccountInterface ai;
    //
   connect(&ai,&AccountInterface::changeImage,this,&MainWindow::receiveimage);
    //
    ai.exec();

}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::receiveTitle(QString title)
{
    te = new TextEdit(this);

    const QRect availableGeometry = QApplication::desktop()->availableGeometry(te);
    te->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    te->move((availableGeometry.width() - te->width()) / 2,
           (availableGeometry.height() - te->height()) / 2);

    //APRE UNA PAGINA DI PRESENTAZIONE DEL TEXTEDIT
   // if (!mw.load(parser.positionalArguments().value(0, QLatin1String(":/example.html"))))
    te->setFileName(title);
    te->fileNew();

    Message m;
    m.setAction('C');
    m.setParams({title, this->getUsername()});
    //emit(sendMessage(&m));
    emit(sendTextMessage(&m));
    emit(newTextEdit(te));
    te->show();
}

void MainWindow::openMw()
{
    this->show();
}
