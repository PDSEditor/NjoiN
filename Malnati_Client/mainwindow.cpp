#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include "loginwindow.h"
#include "accountinterface.h"

socketManager *sock;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->label->setStyleSheet("QLabel { background-color : green; color : black; }");
     connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::open_file_on_server);
//sock=new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::newFile(){
    te = new TextEdit(this);

    const QRect availableGeometry = QApplication::desktop()->availableGeometry(te);
    te->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    te->move((availableGeometry.width() - te->width()) / 2,
           (availableGeometry.height() - te->height()) / 2);

    //APRE UNA PAGINA DI PRESENTAZIONE DEL TEXTEDIT
   // if (!mw.load(parser.positionalArguments().value(0, QLatin1String(":/example.html"))))

    te->fileNew();
    Message m;
    m.setAction('C');
    m.setParams({"newfile", this->getUsername()});
    m.setSender(this->getSiteId());
    emit(sendTextMessage(&m));
    emit(newTextEdit(te));
    te->show();
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

void MainWindow::open_file_on_server(QListWidgetItem* s){
    Message m;
    m.setAction('R');
    m.setParams({s->text(),username});
    m.setSender(siteId);
    emit(sendTextMessage(&m));


}

void MainWindow::receivedFile(QList<Symbol> tmp){
//    QList<Symbol> tmp;
//    Symbol s1,s2,s3,s4;
//    s1.setValue('a');
//    s1.setPosizione({50});
//    s2.setValue('b');
//    s2.setPosizione({75});
//    s3.setValue('c');
//    s3.setPosizione({87});
//    s4.setValue('d');
//    s4.setPosizione({93});
//    tmp.append(s1);
//    tmp.append(s2);
//    tmp.append(s3);
//    tmp.append(s4);
    te = new TextEdit(this);
    emit(newTextEdit(te));
    te->loadFile(tmp);
    te->show();

}

void MainWindow::sendUri(Message m)
{
    m.setSender(siteId);
    emit(sendTextMessage(&m));

}

void MainWindow::receiveURIerror()
{
    QMessageBox::information(this,"ERRORE","URI non corretta");
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
       addElementforUser(m.getParams().at(i));
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
    ai.setUsername(username);
   connect(&ai,&AccountInterface::changeImage,this,&MainWindow::receiveimage);
    //
    ai.exec();

}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}

void MainWindow::on_listView_indexesMoved(const QModelIndexList &indexes)
{

}

void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{

}

void MainWindow::on_pushButton_2_clicked()
{Inserturi i;
    connect(&i,&Inserturi::sendUri,this,&MainWindow::sendUri);
    i.exec();

}
