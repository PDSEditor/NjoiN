#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include "loginwindow.h"
#include "accountinterface.h"
#include "inserttitle.h"
#include "simplecrypt.h"

socketManager *sock;
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    ui->label->setStyleSheet("background-image: url(:/images/Icons/logo-icon.png);background-repeat:none;background-position:center; text-align:top; color:yellow;");
    setWindowTitle("N Joi' N");

     connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::open_file_on_server);
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::newFile(){

    it= new InsertTitle(documents);

    connect(it,&InsertTitle::setTitle,this,&MainWindow::receiveTitle);
    connect(it,&InsertTitle::showMw,this,&MainWindow::openMw);
    it->setUsername(this->username);

    it->exec();

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

    Message m;
    m.setAction('G');
    m.setSender(siteId);
    m.setParams({username, bArray});
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
    this->teWindow->setWindowTitle(openURI.left(openURI.lastIndexOf('_'))+".txt");

    this->usersWindow = new QWidget();
    layout =new QHBoxLayout();
    layoutUsers = new QVBoxLayout();

    te = new TextEdit();
    te->setUsername(username);
    layout->addWidget(te);
    layout->addWidget(usersWindow);

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
    //cursore
    //connect(this, &MainWindow::updateUsersOnTe, te, &TextEdit::updateUsersOnTe);




    layoutUsers->addStretch();
    layout->addLayout(layoutUsers);
    this->teWindow->setLayout(layout);
    this->teWindow->show();
    te->loadFile(tmp);
    QList<QListWidgetItem*> q=ui->listWidget->findItems(openURI,Qt::MatchExactly);
    if(q.size()==0)
        addElementforUser(openURI);
    this->hide();

}

void MainWindow::sendUri(Message m)
{
    m.setSender(siteId);
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));
    QString decrypted = crypto.decryptToString(m.getParams()[0]);
    openURI=decrypted;
    m.setParams({decrypted});
    emit(sendTextMessage(&m));

}

void MainWindow::receiveURIerror()
{
    QMessageBox::critical(this,"ERRORE","URI non corretta");
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
                int pos=siteId%11;
                q=listcolor.at(pos);
            }


            if(online) {
                QList<QListWidgetItem*> a=this->onlineUsers->findItems(user,Qt::MatchExactly);
                if(a.size()==0){
                    this->onlineUsers->addItem(user);
                    this->onlineUsers->item(this->onlineUsers->count()-1)->setForeground(q);
                    onlineUserTE.append(user);
                    if(user!=username)
                        onlineUserColor.insert(user,q);
                }
            }
            else{
                QList<QListWidgetItem*> a=this->offlineUsers->findItems(user,Qt::MatchExactly);
                if(a.size()==0){
                    this->offlineUsers->addItem(user);
                    this->offlineUsers->item(this->offlineUsers->count()-1)->setForeground(q);
                }
            }
        }

    }

    emit(updateUsersOnTe(onlineUserColor));

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

    //SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f023));

    for(int i=3; i<size; i++){
        documents.append(m.getParams().at(i));
        //QString encrypted = crypto.encryptToString(m.getParams().at(i));
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
    ai.setUsername(username);
    ai.setImagePic(image);
    connect(&ai, &AccountInterface::changeImage, this, &MainWindow::sendNewImage);
    connect(this, &MainWindow::receivedNewImage, &ai, &AccountInterface::receiveNewImage);
    connect(&ai, &AccountInterface::changePassword, this, &MainWindow::sendNewPwd);
    connect(this, &MainWindow::receivedNewPsw, &ai, &AccountInterface::receiveNewPsw);
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
    this->teWindow->setWindowTitle(title+".txt");

    this->usersWindow = new QWidget();
    layout =new QHBoxLayout();
    layoutUsers = new QVBoxLayout();

    te = new TextEdit();    
    te->setUsername(username);
    layout->addWidget(te);
    layout->addWidget(usersWindow);
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
    te->setFileName(title);
    te->setURI(title+"_"+username);
    te->setSiteid(siteId);
    addElementforUser(title+"_"+username);
    te->fileNew();
//    connect(te,&TextEdit::closeDocument,this,&MainWindow::documentClosed);
    connect(te,&TextEdit::openMW,this,&MainWindow::openMw);
//cursore
   // connect(this, &MainWindow::updateUsersOnTe, te, &TextEdit::updateUsersOnTe);


    Message m;
    m.setAction('C');
    m.setParams({title, this->getUsername()});
    m.setSender(siteId);
    emit(sendTextMessage(&m));
    emit(newTextEdit(te,siteId));


    layoutUsers->addStretch();
    layout->addLayout(layoutUsers);
    this->teWindow->setLayout(layout);
    this->teWindow->show();
}

void MainWindow::openMw(QString fileName)
{
    this->show();

    if (fileName!="") {
        Message m,mc;
        m.setAction('X');
        m.setSender(this->getSiteId());
        m.setParams({fileName, this->username});
        emit(sendTextMessage(&m));
        //messaggio per cursore
        mc.setAction('Z');
        mc.setSender(this->getSiteId());
        mc.setParams({QString::number(-24),username});
        emit(sendTextMessage(&mc));
        emit(closeTextEdit(this->te));
    }
}

void MainWindow::documentClosed(QString fileName)
{

}

void MainWindow::on_actionOpen_triggered()
{
    i = new Inserturi();
    connect(i,&Inserturi::sendUri,this,&MainWindow::sendUri);
    i->exec();
}
