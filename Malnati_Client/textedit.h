#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QMainWindow>
#include <QMap>
#include <QPointer>
#include <QTextCursor>
#include <symbol.h>
#include <crdt.h>
#include <QPrinter>
#include <QLabel>
#include <showuridialog.h>


#define DEFAULT_SIZE 12

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QFontComboBox;
class QTextEdit;
class QTextCharFormat;
class QMenu;
class QPrinter;
QT_END_NAMESPACE


struct User
{
    QString user;
    QLabel *label;
    QTextCursor cursor;
};


class TextEdit : public QMainWindow
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = 0);
    ~TextEdit();
    bool load(const QString &f);
    void setCrdt(Crdt *crdtclient);
    void setSocketM(socketManager *sockclient);
    void loadFile(QList<Symbol>);
    void setFileName(QString fileName);
    void setURI(QString);
    QString getURI();
    Qt::Alignment insertalign(QChar c);
    QChar findalign(Qt::Alignment);
    void setUsername(QString usern);
   // void moveCursor(int pos, QString userId);

public slots:
    void fileNew();
    void receiveSymbol(Message *m);
    void receiveAllign(Message m);
    void setSiteid(int);
    void updateUsersOnTe(QMap<QString,QColor>);
    void moveCursor(int pos, QString userId);


signals:
    void sendMessage(Message *m);
    void sendTextMessage(Message *m);
    void openMW(QString);
    void closeDocument(QString fileName);
    void cursorPositionChangedSignal(int position);


protected:
//    void closeEvent(QCloseEvent *e) override;

private slots:
    void filePrint();
    void filePrintPreview();
    void filePrintPdf();

    void textBold();
    void textUnderline();
    void textItalic();
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void textStyle(int styleIndex);
    void textColor();
    void textAlign(QAction *a);
    void showUriWindow();

    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();
    void onTextChanged(int position, int charsRemoved, int charsAdded);

    void clipboardDataChanged();
    void about();
    void printPreview(QPrinter *);

private:
    void closeEvent(QCloseEvent *e) override;
    void setupFileActions();
    void setupEditActions();
    void setupTextActions();
    void setupUriActions();
    void setCurrentFileName();


    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    void alignmentChanged(Qt::Alignment a);
    Symbol searchSymbolToErase(char c);
    bool externAction, flagc=false,alignAction;
    QMap<int,QColor> colors;
    std::vector<QColor> listcolor={Qt::red, Qt::green, Qt::blue, Qt::cyan,Qt::darkYellow,Qt::lightGray, Qt::darkRed, Qt::darkGreen, Qt::darkBlue, Qt::darkCyan, Qt::darkGray};
    int contcolor=0;
    int siteid;

    //local format
    bool localb,locall,localu;
    QString localFamily;
    qreal localsize;

    QAction *actionSave;
    QAction *actionTextBold;
    QAction *actionTextUnderline;
    QAction *actionTextItalic;
    QAction *actionTextColor;
    QAction *actionAlignLeft;
    QAction *actionAlignCenter;
    QAction *actionAlignRight;
    QAction *actionAlignJustify;
    QAction *actionUndo;
    QAction *actionRedo;
#ifndef QT_NO_CLIPBOARD
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
#endif

    QComboBox *comboStyle;
    QFontComboBox *comboFont;
    QComboBox *comboSize;

    QToolBar *tb;
    QString fileName,URI,username;
    QTextEdit *textEdit;
    Crdt *crdt;
    socketManager *sockm;
//    std :: vector<Symbol> *symbols;
    ShowUriDialog *shu;
    QTextCursor *m_localCursor = nullptr;
    QMap<QString,User> m_onlineUsers;
    Q_INVOKABLE void updateCursors();
    bool handlingOperation;
    bool localOperation;

};

#endif // TEXTEDIT_H
