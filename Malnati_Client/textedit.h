#ifndef TEXTEDIT_H
#define TEXTEDIT_H

#include <QMainWindow>
#include <QMap>
#include <QPointer>
#include <symbol.h>
#include <crdt.h>
#include <QPrinter>

QT_BEGIN_NAMESPACE
class QAction;
class QComboBox;
class QFontComboBox;
class QTextEdit;
class QTextCharFormat;
class QMenu;
class QPrinter;
QT_END_NAMESPACE




class TextEdit : public QMainWindow
{
    Q_OBJECT

public:
    TextEdit(QWidget *parent = 0);
    TextEdit(QList<Symbol> file,QWidget *parent = 0);
    bool load(const QString &f);
    void setCrdt(Crdt *crdtclient);
    void setSocketM(socketManager *sockclient);
    void loadFile(QList<Symbol>);
    void setFileName(QString fileName);


public slots:
    void fileNew();
    void receiveSymbol(Message *m);


signals:
    void sendMessage(Message *m);


protected:
    void closeEvent(QCloseEvent *e) override;

private slots:
    void fileOpen();
    bool fileSave();
    bool fileSaveAs();
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

    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();
    void onTextChanged(int position, int charsRemoved, int charsAdded);

    void clipboardDataChanged();
    void about();
    void printPreview(QPrinter *);

private:
    void setupFileActions();
    void setupEditActions();
    void setupTextActions();
    bool maybeSave();
    void setCurrentFileName(const QString &fileName);

    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    void fontChanged(const QFont &f);
    void colorChanged(const QColor &c);
    void alignmentChanged(Qt::Alignment a);
    Symbol searchSymbolToErase(char c);
    bool externAction, flagc=false;
    QMap<int,QColor> colors;
    std::vector<QColor> listcolor={Qt::red,Qt::cyan,Qt::yellow,Qt::green,Qt::gray};
    int contcolor=0;

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
    QString fileName;
    QTextEdit *textEdit;
    Crdt *crdt;
    socketManager *sockm;
    std :: vector<Symbol> *symbols;
};

#endif // TEXTEDIT_H
