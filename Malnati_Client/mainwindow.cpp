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
     connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::open_file_on_server);
//sock=new socketManager(QUrl(QStringLiteral("ws://localhost:1234")));
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::newFile(){

    it= new InsertTitle(documents);

    connect(it,&InsertTitle::setTitle,this,&MainWindow::receiveTitle);
    connect(it,&InsertTitle::showMw,this,&MainWindow::openMw);

    it->exec();


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

void MainWindow::setURI(QString u)
{
    openURI=u;
}

QString MainWindow::getURI()
{
    return openURI;
}

void MainWindow::sendNewImage(QByteArray &bArray){
//    QByteArray bArray;
//    QBuffer buffer(&bArray);
//    buffer.open(QIODevice::WriteOnly);
//    q.save(&buffer,"PNG");

    Message m;
    m.setAction('G');
    m.setSender(siteId);
    m.setParams({username, bArray});
//    emit sendMessage(&m);
    emit sendImage(&m);

}

void MainWindow::sendNewPwd(QString &oldPsw, QString &newPwd){
    Message m;
    m.setAction('P');
    m.setSender(siteId);
    m.setParams({username, oldPsw, newPwd});
    emit (sendPwd(&m));
}

void MainWindow::open_file_on_server(QListWidgetItem* s){
    Message m;
    m.setAction('R');
    m.setParams({s->text(),username});
    m.setSender(siteId);
    openURI=s->text();
    flaglocal=1;
    emit(sendTextMessage(&m));


}

void MainWindow::receivedFile(QList<Symbol> tmp){

    this->teWindow = new TextEditWindow();
    connect(teWindow,&TextEditWindow::openMW,this,&MainWindow::openMw);
    this->teWindow->setUri(openURI);

    this->usersWindow = new QWidget();
    layout =new QHBoxLayout();
    layoutUsers = new QVBoxLayout();

    te = new TextEdit();

    layout->addWidget(te);
    layout->addWidget(usersWindow);

    //this->dockOnline->setf

    this->dockOnline = new QDockWidget(tr("Utenti online"));
    this->dockOnline->setParent(this->usersWindow);

    dockOnline->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    onlineUsers = new QListWidget(dockOnline);

    dockOnline->setWidget(onlineUsers);
    addDockWidget(Qt::RightDockWidgetArea, dockOnline);

    layoutUsers->addWidget(dockOnline);

    this->dockOffline = new QDockWidget(tr("Utenti offline"));
    this->dockOffline->setParent(this->usersWindow);

    dockOffline->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    offlineUsers = new QListWidget(dockOffline);

    dockOffline->setWidget(offlineUsers);
    addDockWidget(Qt::RightDockWidgetArea, dockOffline);

    layoutUsers->addWidget(dockOffline);

    emit(newTextEdit(te,siteId));
    te->setFileName(openURI.left(openURI.lastIndexOf('_')));
    te->setSiteid(siteId);
    te->setURI(openURI);
    connect(te,&TextEdit::openMW,this,&MainWindow::openMw);
    connect(this, &MainWindow::updateUsersOnTe, te, &TextEdit::updateUsersOnTe);



    layoutUsers->addStretch();
    layout->addLayout(layoutUsers);
    this->teWindow->setLayout(layout);
    this->teWindow->show();
    te->loadFile(tmp);
    addElementforUser(openURI);
    this->hide();

}

void MainWindow::sendUri(Message m)
{
    m.setSender(siteId);
    openURI=m.getParams().at(0);
    emit(sendTextMessage(&m));

}

void MainWindow::receiveURIerror()
{
    QMessageBox::information(this,"ERRORE","URI non corretta");
}

void MainWindow::closeMw()
{
    on_actionClose_triggered();
}

void MainWindow::showUsers(Message m)
{
    this->onlineUsers->clear();
    this->offlineUsers->clear();
    std::vector<QColor> listcolor={Qt::red, Qt::green, Qt::blue, Qt::cyan,Qt::darkYellow,Qt::lightGray, Qt::darkRed, Qt::darkGreen, Qt::darkBlue, Qt::darkCyan, Qt::darkGray};

    bool online = true;

    QList<QString> onlineUserTE;

    for (auto user_siteId : m.getParams()) {

        if(user_siteId == "___")
            online = false;
        else {

            QStringList list = user_siteId.split("_");
            QString user = list.at(0);
            int siteId = list.at(1).toInt();

            QColor q;

            if(siteId == this->getSiteId())
                q = Qt::black;

            else {
                int pos=siteId%5;
                q=listcolor.at(pos);
            }


            if(online) {
                this->onlineUsers->addItem(user);
                this->onlineUsers->item(this->onlineUsers->count()-1)->setForeground(q);
                onlineUserTE.append(user);
            }
            else{
                this->offlineUsers->addItem(user);
                this->offlineUsers->item(this->offlineUsers->count()-1)->setForeground(q);
            }
        }

    }

    emit(updateUsersOnTe(onlineUserTE));

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
    QString username = m.getParams().at(0);
    int siteId = m.getSender();

    QByteArray barray;
    barray = m.getParams().at(2).toLatin1(); //in base64

    QPixmap image;
    image.loadFromData(QByteArray::fromBase64(barray), "PNG");

    setUsername(username);
    setSiteId(siteId);
    setImage(image);

    QList<QString> tmp;
    long size = m.getParams().size();
    for(int i=3; i<size; i++){
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
    ai.setImagePic(image);
   connect(&ai, &AccountInterface::changeImage, this, &MainWindow::sendNewImage);
   connect(this, &MainWindow::receivedNewImage, &ai, &AccountInterface::receiveNewImage);
   connect(&ai, &AccountInterface::changePassword, this, &MainWindow::sendNewPwd);
   connect(this, &MainWindow::receivedNewPsw, &ai, &AccountInterface::receiveNewPsw);
    //
    ai.exec();

}

void MainWindow::receiveNewImageMW(Message &m){
    emit(receivedNewImage(m));
    return;
}
void MainWindow::receiveNewPswMW(Message &m){
    emit(receivedNewPsw(m));
}

void MainWindow::on_actionClose_triggered()
{
    this->close();
}



void MainWindow::on_pushButton_2_clicked()
{   i = new Inserturi();
    connect(i,&Inserturi::sendUri,this,&MainWindow::sendUri);
    i->exec();
}


void MainWindow::receiveTitle(QString title)
{
    this->hide();

    this->teWindow = new TextEditWindow();
    connect(teWindow,&TextEditWindow::openMW,this,&MainWindow::openMw);
    this->teWindow->setUri(title+"_"+username);

    this->usersWindow = new QWidget();
    layout =new QHBoxLayout();
    layoutUsers = new QVBoxLayout();

    te = new TextEdit();

    layout->addWidget(te);
    layout->addWidget(usersWindow);

    //this->dockOnline->setStyle()

    this->dockOnline = new QDockWidget(tr("Utenti online"));
    this->dockOnline->setParent(this->usersWindow);

    dockOnline->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    onlineUsers = new QListWidget(dockOnline);
    onlineUsers->addItem(this->username);

    dockOnline->setWidget(onlineUsers);
    addDockWidget(Qt::RightDockWidgetArea, dockOnline);

    layoutUsers->addWidget(dockOnline);

    this->dockOffline = new QDockWidget(tr("Utenti offline"));
    this->dockOffline->setParent(this->usersWindow);

    dockOffline->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    offlineUsers = new QListWidget(dockOffline);

    dockOffline->setWidget(offlineUsers);
    addDockWidget(Qt::RightDockWidgetArea, dockOffline);

    layoutUsers->addWidget(dockOffline);


    const QRect availableGeometry = QApplication::desktop()->availableGeometry(te);
    te->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    te->move((availableGeometry.width() - te->width()) / 2,
           (availableGeometry.height() - te->height()) / 2);

    //APRE UNA PAGINA DI PRESENTAZIONE DEL TEXTEDIT
   // if (!mw.load(parser.positionalArguments().value(0, QLatin1String(":/example.html"))))
    te->setFileName(title);
    te->setURI(title+"_"+username);
    te->setSiteid(siteId);
    addElementforUser(title+"_"+username);
    te->fileNew();
    connect(te,&TextEdit::closeDocument,this,&MainWindow::documentClosed);
    connect(te,&TextEdit::openMW,this,&MainWindow::openMw);
    connect(this, &MainWindow::updateUsersOnTe, te, &TextEdit::updateUsersOnTe);

    Message m;
    m.setAction('C');
    m.setParams({title, this->getUsername()});
    //emit(sendMessage(&m));
    m.setSender(siteId);
    emit(sendTextMessage(&m));
    emit(newTextEdit(te,siteId));
    //te->show();


    layoutUsers->addStretch();
    layout->addLayout(layoutUsers);
    this->teWindow->setLayout(layout);
    this->teWindow->show();
}

void MainWindow::openMw(QString fileName)
{
    this->show();

    if (fileName!="") {
        Message m;
        m.setAction('X');
        m.setSender(this->getSiteId());
        m.setParams({fileName, this->username});
        emit(sendTextMessage(&m));
    }
}

void MainWindow::documentClosed(QString fileName)
{

}
