/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QColor>

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QTextCodec>
#include <QTextEdit>
#include <QStatusBar>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include<QPushButton>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QMessageBox>
#include <QMimeData>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#if QT_CONFIG(printdialog)
#include <QPrintDialog>
#endif
#include <QPrinter>
#if QT_CONFIG(printpreviewdialog)
#include <QPrintPreviewDialog>
#endif
#endif
#endif

#include "textedit.h"
#include<QDebug>
#include "message.h"
#include "showuridialog.h"

#ifdef Q_OS_MAC
const QString rsrcPath = ":/images/mac";
#else
const QString rsrcPath = ":/images/win";
#endif




TextEdit::TextEdit(QWidget *parent)
    : QMainWindow(parent)
{
#ifdef Q_OS_OSX
    setUnifiedTitleAndToolBarOnMac(true);
#endif
    setWindowTitle(QCoreApplication::applicationName());





    textEdit = new QTextEdit(this);
    externAction=false;
    //symbols = new std::vector<Symbol>();
//    QTextDocument document = textEdit->document();

    connect(textEdit, &QTextEdit::currentCharFormatChanged,
            this, &TextEdit::currentCharFormatChanged);
    connect(textEdit, &QTextEdit::cursorPositionChanged,
            this, &TextEdit::cursorPositionChanged);


    setCentralWidget(textEdit);

    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setupFileActions();
    setupEditActions();
    setupTextActions();
    setupUriActions();


    {
        QMenu *helpMenu = menuBar()->addMenu(tr("Help"));
        helpMenu->addAction(tr("About"), this, &TextEdit::about);
        helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
    }

    QFont textFont("Helvetica");
    //
    //
    textEdit->setFont(textFont);
    fontChanged(textEdit->font());
    colorChanged(textEdit->textColor());
    alignmentChanged(textEdit->alignment());


    //prova!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    textEdit->setFontFamily("kalapi");
    localFamily="kalapi";
    textEdit->setFontPointSize(12);
    localsize=12;
    textEdit->currentCharFormat().setFontItalic(false);
    textEdit->currentCharFormat().setFontUnderline(false);
    //prova!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    this->localOperation = false;


    connect(textEdit->document(), &QTextDocument::modificationChanged,
            actionSave, &QAction::setEnabled);
    connect(textEdit->document(), &QTextDocument::modificationChanged,
            this, &QWidget::setWindowModified);
    connect(textEdit->document(), &QTextDocument::undoAvailable,
            actionUndo, &QAction::setEnabled);
    connect(textEdit->document(), &QTextDocument::redoAvailable,
            actionRedo, &QAction::setEnabled);
    connect(textEdit->document(), &QTextDocument::contentsChange,
            this, &TextEdit::onTextChanged);

//    connect(textEdit->document(), &QTextDocument::cursorPositionChanged, this, [&](){
//        int line = textEdit->textCursor().blockNumber()+1;
//        int pos = textEdit->textCursor().columnNumber()+1;
//        int TLines = textEdit->document()->blockCount();

//        //statusbar->showMessage(QString("Line:%1 Col:%2 TotLines:%3").arg(line).arg(pos).arg(TLines));
//    });

//    connect(textEdit, &QTextEdit::cursorPositionChanged, this, [&](){
//        int pos = textEdit->textCursor().position();
//        //resetActionToggle();
//        if (localOperation )//|| handlingOperation)
//            localOperation = false;
//        else{
//            Message m;
//            m.setAction('Z');
//            m.setSender(siteid);
//            QString userName = URI.right(URI.lastIndexOf('_'));
//            m.setParams({QString(pos),userName});
//            emit sendTextMessage(&m);
//        }

//    });



    setWindowModified(textEdit->document()->isModified());
    actionSave->setEnabled(textEdit->document()->isModified());
    actionUndo->setEnabled(textEdit->document()->isUndoAvailable());
    actionRedo->setEnabled(textEdit->document()->isRedoAvailable());
    textEdit->textChanged();
#ifndef QT_NO_CLIPBOARD
    actionCut->setEnabled(false);
    connect(textEdit, &QTextEdit::copyAvailable, actionCut, &QAction::setEnabled);
    actionCopy->setEnabled(false);
    connect(textEdit, &QTextEdit::copyAvailable, actionCopy, &QAction::setEnabled);

    connect(QApplication::clipboard(), &QClipboard::dataChanged, this, &TextEdit::clipboardDataChanged);
#endif

    textEdit->setFocus();
    setCurrentFileName(QString());

#ifdef Q_OS_MACOS
    // Use dark text on light background on macOS, also in dark mode.
    QPalette pal = textEdit->palette();
    pal.setColor(QPalette::Base, QColor(Qt::white));
    pal.setColor(QPalette::Text, QColor(Qt::black));
    textEdit->setPalette(pal);
#endif


}



void TextEdit::closeEvent(QCloseEvent *e)
{

    if (maybeSave() && e != nullptr)

        e->accept();
    else{
     if(e!=nullptr)
        e->ignore();

    }


    emit(openMW(this->getURI()));

    //emit(closeDocument(this->fileName));

}

void TextEdit::setupFileActions()
{
    QToolBar *tb = addToolBar(tr("File Actions"));
    QMenu *menu = menuBar()->addMenu(tr("&File"));

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(rsrcPath + "/filenew.png"));
    QAction *a = menu->addAction(newIcon,  tr("&New"), this, &TextEdit::fileNew);
    tb->addAction(a);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::New);

    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(rsrcPath + "/fileopen.png"));
    a = menu->addAction(openIcon, tr("&Open..."), this, &TextEdit::fileOpen);
    a->setShortcut(QKeySequence::Open);
    tb->addAction(a);

    menu->addSeparator();

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(rsrcPath + "/filesave.png"));
    actionSave = menu->addAction(saveIcon, tr("&Save"), this, &TextEdit::fileSave);
    actionSave->setShortcut(QKeySequence::Save);
    actionSave->setEnabled(false);
    tb->addAction(actionSave);

    a = menu->addAction(tr("Save &As..."), this, &TextEdit::fileSaveAs);
    a->setPriority(QAction::LowPriority);
    menu->addSeparator();

#ifndef QT_NO_PRINTER
    const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png"));
    a = menu->addAction(printIcon, tr("&Print..."), this, &TextEdit::filePrint);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(QKeySequence::Print);
    tb->addAction(a);

    const QIcon filePrintIcon = QIcon::fromTheme("fileprint", QIcon(rsrcPath + "/fileprint.png"));
    menu->addAction(filePrintIcon, tr("Print Preview..."), this, &TextEdit::filePrintPreview);

    const QIcon exportPdfIcon = QIcon::fromTheme("exportpdf", QIcon(rsrcPath + "/exportpdf.png"));
    a = menu->addAction(exportPdfIcon, tr("&Export PDF..."), this, &TextEdit::filePrintPdf);
    a->setPriority(QAction::LowPriority);
    a->setShortcut(Qt::CTRL + Qt::Key_D);
    tb->addAction(a);

    menu->addSeparator();
#endif

    a = menu->addAction(tr("&Quit"), this, &QWidget::close);
    a->setShortcut(Qt::CTRL + Qt::Key_Q);
}

void TextEdit::setupEditActions()
{
    QToolBar *tb = addToolBar(tr("Edit Actions"));
    QMenu *menu = menuBar()->addMenu(tr("&Edit"));

    const QIcon undoIcon = QIcon::fromTheme("edit-undo", QIcon(rsrcPath + "/editundo.png"));
    actionUndo = menu->addAction(undoIcon, tr("&Undo"), textEdit, &QTextEdit::undo);
    actionUndo->setShortcut(QKeySequence::Undo);
    tb->addAction(actionUndo);

    const QIcon redoIcon = QIcon::fromTheme("edit-redo", QIcon(rsrcPath + "/editredo.png"));
    actionRedo = menu->addAction(redoIcon, tr("&Redo"), textEdit, &QTextEdit::redo);
    actionRedo->setPriority(QAction::LowPriority);
    actionRedo->setShortcut(QKeySequence::Redo);
    tb->addAction(actionRedo);
    menu->addSeparator();

#ifndef QT_NO_CLIPBOARD
    const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(rsrcPath + "/editcut.png"));
    actionCut = menu->addAction(cutIcon, tr("Cu&t"), textEdit, &QTextEdit::cut);
    actionCut->setPriority(QAction::LowPriority);
    actionCut->setShortcut(QKeySequence::Cut);
    tb->addAction(actionCut);

    const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(rsrcPath + "/editcopy.png"));
    actionCopy = menu->addAction(copyIcon, tr("&Copy"), textEdit, &QTextEdit::copy);
    actionCopy->setPriority(QAction::LowPriority);
    actionCopy->setShortcut(QKeySequence::Copy);
    tb->addAction(actionCopy);

    const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(rsrcPath + "/editpaste.png"));
    actionPaste = menu->addAction(pasteIcon, tr("&Paste"), textEdit, &QTextEdit::paste);
    actionPaste->setPriority(QAction::LowPriority);
    actionPaste->setShortcut(QKeySequence::Paste);
    tb->addAction(actionPaste);
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        actionPaste->setEnabled(md->hasText());
#endif
}

void TextEdit::setupTextActions()
{
    QToolBar *tb = addToolBar(tr("Format Actions"));
    QMenu *menu = menuBar()->addMenu(tr("F&ormat"));

    const QIcon boldIcon = QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png"));
    actionTextBold = menu->addAction(boldIcon, tr("&Bold"), this, &TextEdit::textBold);
    actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    actionTextBold->setFont(bold);
    tb->addAction(actionTextBold);
    actionTextBold->setCheckable(true);

    const QIcon italicIcon = QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png"));
    actionTextItalic = menu->addAction(italicIcon, tr("&Italic"), this, &TextEdit::textItalic);
    actionTextItalic->setPriority(QAction::LowPriority);
    actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    actionTextItalic->setFont(italic);
    tb->addAction(actionTextItalic);
    actionTextItalic->setCheckable(true);

    const QIcon underlineIcon = QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png"));
    actionTextUnderline = menu->addAction(underlineIcon, tr("&Underline"), this, &TextEdit::textUnderline);
    actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    actionTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    actionTextUnderline->setFont(underline);
    tb->addAction(actionTextUnderline);
    actionTextUnderline->setCheckable(true);

    menu->addSeparator();

    const QIcon leftIcon = QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png"));
    actionAlignLeft = new QAction(leftIcon, tr("&Left"), this);
    actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    actionAlignLeft->setCheckable(true);
    actionAlignLeft->setPriority(QAction::LowPriority);
    const QIcon centerIcon = QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png"));
    actionAlignCenter = new QAction(centerIcon, tr("C&enter"), this);
    actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    actionAlignCenter->setCheckable(true);
    actionAlignCenter->setPriority(QAction::LowPriority);
    const QIcon rightIcon = QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png"));
    actionAlignRight = new QAction(rightIcon, tr("&Right"), this);
    actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
    actionAlignRight->setCheckable(true);
    actionAlignRight->setPriority(QAction::LowPriority);
    const QIcon fillIcon = QIcon::fromTheme("format-justify-fill", QIcon(rsrcPath + "/textjustify.png"));
    actionAlignJustify = new QAction(fillIcon, tr("&Justify"), this);
    actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    actionAlignJustify->setCheckable(true);
    actionAlignJustify->setPriority(QAction::LowPriority);

    // Make sure the alignLeft  is always left of the alignRight
    QActionGroup *alignGroup = new QActionGroup(this);
    connect(alignGroup, &QActionGroup::triggered, this, &TextEdit::textAlign);

    if (QApplication::isLeftToRight()) {
        alignGroup->addAction(actionAlignLeft);
        alignGroup->addAction(actionAlignCenter);
        alignGroup->addAction(actionAlignRight);
    } else {
        alignGroup->addAction(actionAlignRight);
        alignGroup->addAction(actionAlignCenter);
        alignGroup->addAction(actionAlignLeft);
    }
    alignGroup->addAction(actionAlignJustify);

    tb->addActions(alignGroup->actions());
    menu->addActions(alignGroup->actions());

    menu->addSeparator();

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = menu->addAction(pix, tr("&Color..."), this, &TextEdit::textColor);
    tb->addAction(actionTextColor);

    tb = addToolBar(tr("Format Actions"));
    tb->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(tb);

    comboStyle = new QComboBox(tb);
    tb->addWidget(comboStyle);
    comboStyle->addItem("Standard");
    comboStyle->addItem("Bullet List (Disc)");
    comboStyle->addItem("Bullet List (Circle)");
    comboStyle->addItem("Bullet List (Square)");
    comboStyle->addItem("Ordered List (Decimal)");
    comboStyle->addItem("Ordered List (Alpha lower)");
    comboStyle->addItem("Ordered List (Alpha upper)");
    comboStyle->addItem("Ordered List (Roman lower)");
    comboStyle->addItem("Ordered List (Roman upper)");
    comboStyle->addItem("Heading 1");
    comboStyle->addItem("Heading 2");
    comboStyle->addItem("Heading 3");
    comboStyle->addItem("Heading 4");
    comboStyle->addItem("Heading 5");
    comboStyle->addItem("Heading 6");

    connect(comboStyle, QOverload<int>::of(&QComboBox::activated), this, &TextEdit::textStyle);

    comboFont = new QFontComboBox(tb);
    tb->addWidget(comboFont);
    connect(comboFont, QOverload<const QString &>::of(&QComboBox::activated), this, &TextEdit::textFamily);

    comboSize = new QComboBox(tb);
    comboSize->setObjectName("comboSize");
    tb->addWidget(comboSize);
    comboSize->setEditable(true);

    const QList<int> standardSizes = QFontDatabase::standardSizes();
    foreach (int size, standardSizes)
        comboSize->addItem(QString::number(size));
    comboSize->setCurrentIndex(standardSizes.indexOf(QApplication::font().pointSize()));

    connect(comboSize, QOverload<const QString &>::of(&QComboBox::activated), this, &TextEdit::textSize);
}

void TextEdit::setupUriActions()
{
    QToolBar *tb = addToolBar(tr("Mostra Uri"));
    QMenu *menu = menuBar()->addMenu(tr("&Uri"));

    const QIcon newIcon = QIcon::fromTheme("showUri", QIcon(rsrcPath + "/../Icons/icons8-collegamento-24.png"));
    QAction *a = menu->addAction(newIcon, tr("&showUri"), this, &TextEdit::showUriWindow);
    tb->addAction(a);
    a->setPriority(QAction::LowPriority);

}



bool TextEdit::load(const QString &f)
{
    if (!QFile::exists(f))
        return false;
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str)) {
        textEdit->setHtml(str);
    } else {
        str = QString::fromLocal8Bit(data);
        textEdit->setPlainText(str);
    }

    setCurrentFileName(f);
    return true;
}

void TextEdit::setCrdt(Crdt *crdtclient)
{
    crdt=crdtclient;
}

void TextEdit::setSocketM(socketManager *sockclient)
{
    sockm=sockclient;
    connect(this, &TextEdit::sendMessage, sockm, &socketManager::binaryMessageToServer);
    connect(this, &TextEdit::sendTextMessage, sockm, &socketManager::messageToServer);
    connect(sockm,&socketManager::receiveAllign,this,&TextEdit::receiveAllign);
//    connect(sockm, &socketManager::updateCursor,this,&TextEdit::updateCursors);
}

void TextEdit::setFileName(QString fileName)
{
    this->fileName = fileName;
}

void TextEdit::setURI(QString u)
{
    URI=u;
}

QString TextEdit::getURI()
{
    return URI;
}



bool TextEdit::maybeSave()
{
    if (!textEdit->document()->isModified())
        return true;

    const QMessageBox::StandardButton ret =
        QMessageBox::warning(this, QCoreApplication::applicationName(),
                             tr("The document has been modified.\n"
                                "Do you want to save your changes?"),
                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    if (ret == QMessageBox::Save)
        return fileSave();
    else if (ret == QMessageBox::Cancel)
        return false;
    return true;
}

void TextEdit::setCurrentFileName(const QString &fileName)
{
    //this->fileName = fileName;
    //textEdit->document()->setModified(false);

    QString shownName;
    if (this->fileName.isEmpty())
        shownName = "untitled.txt";
    else
        shownName = QFileInfo(this->fileName).fileName();

    setWindowTitle(tr("%1[*] - %2").arg(shownName, QCoreApplication::applicationName()));
    setWindowModified(false);

}

void TextEdit::fileNew()
{
    if (maybeSave()) {
        //textEdit->clear();
        (setCurrentFileName(QString()));
    }

}

void TextEdit::receiveSymbol(Message *m)
{

    externAction=true;
    localOperation = false;
   QTextCursor curs = textEdit->textCursor(),oldcurs;
       //QTextCursor curs ,oldcurs=textEdit->textCursor();
       //textEdit->setTextCursor(curs);
    QColor col;
    QTextCharFormat qform, preqform;
    //preqform=textEdit->currentCharFormat();
    if(colors.find(m->getSymbol().getSiteId())==colors.end()){
        int pos=m->getSymbol().getSiteId()%5;
        //
        QColor q=listcolor.at(pos);
        //
        colors.insert(m->getSymbol().getSiteId(),q);
    }
    col=colors.take(m->getSymbol().getSiteId());
    //textEdit->setTextBackgroundColor(col);


    qform.setBackground(col);
    qform.setFontFamily(m->getSymbol().getFamily());
    qform.setFontItalic(m->getSymbol().getItalic());
    qform.setFontUnderline(m->getSymbol().getUnderln());
    //qform.setFontPointSize((float)m->getSize());
    qform.setFontPointSize(m->getSymbol().getSize());
    if(m->getSymbol().getBold())
        qform.setFontWeight(QFont::Bold);

    //curs.setCharFormat(qform);

    int position,oldposition;
    oldposition=textEdit->textCursor().position();
    Symbol tmp;
    tmp.setValue(m->getSymbol().getValue());
    tmp.setPosizione(m->getSymbol().getPosizione());
    if(m->getAction()=='I'){
        position=crdt->remoteinsert(tmp);
        curs.setPosition(position);
        if(tmp.getValue()=='\0')
             curs.insertText((QChar)'\n',qform);
        else
             curs.insertText((QChar)tmp.getValue(),qform);
    }
    else if(m->getAction()=='D'){
        position=crdt->remotedelete(tmp);
        curs.setPosition(position+1);
        curs.deletePreviousChar();

    }



}

void TextEdit::receiveAllign(Message m)
{
    QTextCursor c=textEdit->textCursor();
    int start,end;
    QChar a;
    Qt::Alignment al;
    start=m.getParams().at(0).toInt();
    end=m.getParams().at(1).toInt();
    a=m.getParams().at(2).toLatin1().at(0);
    al=insertalign(a);
    for(int i=start;i<=end;i++){
        crdt->getSymbols()[i].setAlign(a);
    }
    c.setPosition(start);
    textEdit->setTextCursor(c);
    alignAction=false;
    externAction=true;
    textEdit->setAlignment(al);

}

void TextEdit::setSiteid(int s)
{
    siteid=s;

}

void TextEdit::updateUsersOnTe(QList<QString> users)
{
    this->m_onlineUsers = users;
}



void TextEdit::fileOpen()
{
    QFileDialog fileDialog(this, tr("Open File..."));
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setMimeTypeFilters(QStringList() << "text/html" << "text/plain");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    const QString fn = fileDialog.selectedFiles().first();
    if (load(fn))
        statusBar()->showMessage(tr("Opened \"%1\"").arg(QDir::toNativeSeparators(fn)));
    else
        statusBar()->showMessage(tr("Could not open \"%1\"").arg(QDir::toNativeSeparators(fn)));

}

bool TextEdit::fileSave()
{
    if (fileName.isEmpty())
        return fileSaveAs();
    if (fileName.startsWith(QStringLiteral(":/")))
        return fileSaveAs();

    QTextDocumentWriter writer(fileName);
    bool success = writer.write(textEdit->document());
    if (success) {
        textEdit->document()->setModified(false);
        statusBar()->showMessage(tr("Wrote \"%1\"").arg(QDir::toNativeSeparators(fileName)));
    } else {
        statusBar()->showMessage(tr("Could not write to file \"%1\"")
                                 .arg(QDir::toNativeSeparators(fileName)));
    }
    return success;
}

bool TextEdit::fileSaveAs()
{
    QFileDialog fileDialog(this, tr("Save as..."));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList mimeTypes;
    mimeTypes << "application/vnd.oasis.opendocument.text" << "text/html" << "text/plain" << "application/pdf";
    fileDialog.setMimeTypeFilters(mimeTypes);
    if (fileDialog.exec() != QDialog::Accepted)
        return false;

    if(fileDialog.selectedMimeTypeFilter()
            .split("/").contains("pdf")){
        QString fileName = fileDialog.selectedFiles().first();
        if(!fileName.split('.').contains("pdf"))
            fileName = fileName.append(".pdf");
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);
        textEdit->document()->print(&printer);
        statusBar()->showMessage(tr("Exported \"%1\"")
                                 .arg(QDir::toNativeSeparators(fileName)));
        return true;
    }
    else if(fileDialog.selectedMimeTypeFilter().contains("opendocument")){
        fileDialog.setDefaultSuffix("odt");
    }
    else if(fileDialog.selectedMimeTypeFilter().contains("plain")){
        fileDialog.setDefaultSuffix("txt");
    }
    else{ if(fileDialog.selectedMimeTypeFilter().contains("html")){
        fileDialog.setDefaultSuffix("html");
    }
       else fileDialog.setDefaultSuffix("odt");
    }
    const QString fn = fileDialog.selectedFiles().first();
    setCurrentFileName(fn);
    return fileSave();
}

void TextEdit::filePrint()
{
#if QT_CONFIG(printdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (textEdit->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle(tr("Print Document"));
    if (dlg->exec() == QDialog::Accepted)
        textEdit->print(&printer);
    delete dlg;
#endif
}

void TextEdit::filePrintPreview()
{
#if QT_CONFIG(printpreviewdialog)
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, &QPrintPreviewDialog::paintRequested, this, &TextEdit::printPreview);
    preview.exec();
#endif
}

void TextEdit::printPreview(QPrinter *printer)
{
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    textEdit->print(printer);
#endif
}


void TextEdit::filePrintPdf()
{
#ifndef QT_NO_PRINTER
//! [0]
    QFileDialog fileDialog(this, tr("Export PDF"));
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setMimeTypeFilters(QStringList("application/pdf"));
    fileDialog.setDefaultSuffix("pdf");
    if (fileDialog.exec() != QDialog::Accepted)
        return;
    QString fileName = fileDialog.selectedFiles().first();
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    textEdit->document()->print(&printer);
    statusBar()->showMessage(tr("Exported \"%1\"")
                             .arg(QDir::toNativeSeparators(fileName)));
//! [0]
#endif
}

void TextEdit::textBold()
{
    QTextCharFormat fmt;
    fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textUnderline()
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(actionTextUnderline->isChecked());
    mergeFormatOnWordOrSelection(fmt);

}

void TextEdit::textItalic()
{
    QTextCharFormat fmt;
    fmt.setFontItalic(actionTextItalic->isChecked());
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textFamily(const QString &f)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    localFamily=f;
    flagc=true;
    mergeFormatOnWordOrSelection(fmt);
}

void TextEdit::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        localsize=p.toFloat();
        flagc=true;
        mergeFormatOnWordOrSelection(fmt);
    }
}

void TextEdit::textStyle(int styleIndex)
{
    QTextCursor cursor = textEdit->textCursor();
    QTextListFormat::Style style = QTextListFormat::ListStyleUndefined;

    switch (styleIndex) {
    case 1:
        style = QTextListFormat::ListDisc;
        break;
    case 2:
        style = QTextListFormat::ListCircle;
        break;
    case 3:
        style = QTextListFormat::ListSquare;
        break;
    case 4:
        style = QTextListFormat::ListDecimal;
        break;
    case 5:
        style = QTextListFormat::ListLowerAlpha;
        break;
    case 6:
        style = QTextListFormat::ListUpperAlpha;
        break;
    case 7:
        style = QTextListFormat::ListLowerRoman;
        break;
    case 8:
        style = QTextListFormat::ListUpperRoman;
        break;
    default:
        break;
    }

    cursor.beginEditBlock();

    QTextBlockFormat blockFmt = cursor.blockFormat();

    if (style == QTextListFormat::ListStyleUndefined) {
        blockFmt.setObjectIndex(-1);
        int headingLevel = styleIndex >= 9 ? styleIndex - 9 + 1 : 0; // H1 to H6, or Standard
        blockFmt.setHeadingLevel(headingLevel);
        cursor.setBlockFormat(blockFmt);

        int sizeAdjustment = headingLevel ? 4 - headingLevel : 0; // H1 to H6: +3 to -2
        QTextCharFormat fmt;
        fmt.setFontWeight(headingLevel ? QFont::Bold : QFont::Normal);
        fmt.setProperty(QTextFormat::FontSizeAdjustment, sizeAdjustment);
        cursor.select(QTextCursor::LineUnderCursor);
        cursor.mergeCharFormat(fmt);
        textEdit->mergeCurrentCharFormat(fmt);
    } else {
        QTextListFormat listFmt;
        if (cursor.currentList()) {
            listFmt = cursor.currentList()->format();
        } else {
            listFmt.setIndent(blockFmt.indent() + 1);
            blockFmt.setIndent(0);
            cursor.setBlockFormat(blockFmt);
        }
        listFmt.setStyle(style);
        cursor.createList(listFmt);
    }

    cursor.endEditBlock();
}

void TextEdit::textColor()
{
    QColor col = QColorDialog::getColor(textEdit->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
    colorChanged(col);
}

void TextEdit::textAlign(QAction *a)
{
    alignAction=true;
    if (a == actionAlignLeft)
        textEdit->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
    else if (a == actionAlignCenter)
        textEdit->setAlignment(Qt::AlignHCenter);
    else if (a == actionAlignRight)
        textEdit->setAlignment(Qt::AlignRight | Qt::AlignAbsolute);
    else if (a == actionAlignJustify)
        textEdit->setAlignment(Qt::AlignJustify);

}

void TextEdit::showUriWindow()
{
   shu = new ShowUriDialog(this);
   shu->setUriDialog(URI);
   shu->exec();
}

void TextEdit::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
    colorChanged(format.foreground().color());
}

void TextEdit::onTextChanged(int position, int charsRemoved, int charsAdded)
{
    if(alignAction==true){
        alignAction=false;
        QChar c;
        int max,min, p=textEdit->textCursor().position();
        min=position;
        max=min+charsAdded-2;
        for(int i=0;i<max;i++){
            crdt->getSymbols()[i].setAlign(findalign(textEdit->alignment()));
        }

        Message m;
        m.setAction('B');
        m.setSender(siteid);
        m.setParams({QString::number(min),QString::number(max),findalign(textEdit->alignment())});
        emit(sendTextMessage(&m));


    }
else{
   if(externAction==false){



       QTextCursor  cursor = textEdit->textCursor();

        qDebug() << "position: " << position;
        qDebug() << "charater: " << textEdit->document()->characterAt(position).unicode();

        if(charsRemoved!=0 && charsAdded==0){
            for(int i=0; i<charsRemoved; i++){
                Message m=crdt->localErase(position);
                emit(sendMessage(&m));
            }
        }else
        if(charsAdded!= 0){
            if(charsAdded==charsRemoved){

                //                dati da passare

                int pos=cursor.selectionStart();
                for(int i=0;i<charsAdded;i++){
                    Message mc,mi;
                    Symbol s=crdt->getSymbols().at(pos+i);
                    //eliminazione vecchio carattere
                    mc.setSymbol(s);
                    mc.setAction('D');
                    emit(sendMessage(&mc));
                    //invio carattere modificato
                    mi.setAction('I');
                    s.setBold(actionTextBold->isChecked());
                    s.setItalic(actionTextItalic->isChecked());
                    s.setUnderln(actionTextUnderline->isChecked());
                    s.setFamily(localFamily);
                    s.setSize(localsize);
                    s.setAlign(findalign(textEdit->alignment()));
                    mi.setSymbol(s);
                    emit(sendMessage(&mi));
                }
            }
            else if(charsRemoved!=charsAdded){
                if(position == 0 && charsAdded > 1){
                    charsAdded--;
                    charsRemoved--;
                }
//                charsAdded--;
                int pos=cursor.selectionStart()-charsAdded;
//                if(pos==0) charsRemoved = 0;
                for(int i=0;i<charsRemoved;i++){
                    Message mc;
                    Symbol s=crdt->getSymbols().at(pos+i);
                    //eliminazione vecchio carattere
                    mc.setSymbol(s);
                    mc.setAction('D');
                    emit(sendMessage(&mc));
                }
                for(int i=0;i<charsAdded;i++){
                    Message mi=crdt->localInsert(textEdit->document()->characterAt(position), position-1, position);
                    Symbol s=mi.getSymbol();
                    mi.setAction('I');
                    s.setBold(actionTextBold->isChecked());
                    s.setItalic(actionTextItalic->isChecked());
                    s.setUnderln(actionTextUnderline->isChecked());
                    s.setFamily(localFamily);
                    s.setSize(localsize);
                    s.setAlign(findalign(textEdit->alignment()));
                    mi.setSymbol(s);
                    position+=1;
                    emit(sendMessage(&mi));
                }
            }
            else{
                if(charsRemoved>0)
                charsAdded--;
            for(int i=0; i<charsAdded; i++){
                qDebug() << "char: " << textEdit->document()->characterAt(position);
                Message m = crdt->localInsert(textEdit->document()->characterAt(position).unicode(), position-1, position);
                Symbol s=m.getSymbol();
                s.setSize(textEdit->fontPointSize());
                s.setFamily(textEdit->fontFamily());
                s.setUnderln(textEdit->currentCharFormat().fontUnderline());
                s.setItalic(textEdit->currentCharFormat().fontItalic());
                s.setBold(actionTextBold->isChecked());
                s.setAlign(findalign(textEdit->alignment()));
                m.setSymbol(s);
                position+=1;
                emit(sendMessage(&m));
            }
            }
        }
   }
externAction=false;
localOperation=true;
    }

}

void TextEdit::cursorPositionChanged()
{

    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat form;
    form.setFontItalic(textEdit->currentCharFormat().fontItalic());
   form.setFontFamily(localFamily);
    form.setFontUnderline(textEdit->currentCharFormat().fontUnderline());
    form.setFontPointSize(localsize);
    form.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    if(!cursor.hasSelection()){
    textEdit->setTextBackgroundColor(Qt::transparent);
    if(flagc==true){
    textEdit->setFontFamily(localFamily);
    textEdit->setFontPointSize(localsize);
    flagc=false;
    }
    textEdit->setFontItalic(textEdit->fontItalic());
    textEdit->setFontUnderline(textEdit->fontUnderline());
    textEdit->setFontWeight(textEdit->fontWeight());

    }




    alignmentChanged(textEdit->alignment());
    QTextList *list = textEdit->textCursor().currentList();

    if (list) {
        switch (list->format().style()) {
        case QTextListFormat::ListDisc:
            comboStyle->setCurrentIndex(1);
            break;
        case QTextListFormat::ListCircle:
            comboStyle->setCurrentIndex(2);
            break;
        case QTextListFormat::ListSquare:
            comboStyle->setCurrentIndex(3);
            break;
        case QTextListFormat::ListDecimal:
            comboStyle->setCurrentIndex(4);
            break;
        case QTextListFormat::ListLowerAlpha:
            comboStyle->setCurrentIndex(5);
            break;
        case QTextListFormat::ListUpperAlpha:
            comboStyle->setCurrentIndex(6);
            break;
        case QTextListFormat::ListLowerRoman:
            comboStyle->setCurrentIndex(7);
            break;
        case QTextListFormat::ListUpperRoman:
            comboStyle->setCurrentIndex(8);
            break;
        default:
            comboStyle->setCurrentIndex(-1);
            break;
        }
    } else {
        int headingLevel = textEdit->textCursor().blockFormat().headingLevel();
        comboStyle->setCurrentIndex(headingLevel ? headingLevel + 8 : 0);
    }

}

void TextEdit::clipboardDataChanged()
{
#ifndef QT_NO_CLIPBOARD
    if (const QMimeData *md = QApplication::clipboard()->mimeData())
        actionPaste->setEnabled(md->hasText());
#endif
}

void TextEdit::about()
{
    QMessageBox::about(this, tr("About"), tr("This example demonstrates Qt's "
        "rich text editing facilities in action, providing an example "
        "document for you to experiment with."));
}

void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textEdit->textCursor();
    if (cursor.hasSelection()){
        //cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);}
    textEdit->mergeCurrentCharFormat(format);
}

void TextEdit::fontChanged(const QFont &f)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(f).family()));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(f.pointSize())));
    actionTextBold->setChecked(f.bold());
    actionTextItalic->setChecked(f.italic());
    actionTextUnderline->setChecked(f.underline());
}

void TextEdit::colorChanged(const QColor &c)
{
    QPixmap pix(16, 16);
    pix.fill(c);
    actionTextColor->setIcon(pix);
}

void TextEdit::alignmentChanged(Qt::Alignment a)
{
    if (a & Qt::AlignLeft)
        actionAlignLeft->setChecked(true);
    else if (a & Qt::AlignHCenter)
        actionAlignCenter->setChecked(true);
    else if (a & Qt::AlignRight)
        actionAlignRight->setChecked(true);
    else if (a & Qt::AlignJustify)
        actionAlignJustify->setChecked(true);
}

void TextEdit::updateCursors()
{
    for (auto it = m_onlineUsers.begin(); it != m_onlineUsers.end(); it++) {
        User user;
        user.user = *it;
        QRect remoteCoord = textEdit->cursorRect(user.cursor);
        int height = remoteCoord.bottom()-remoteCoord.top();
        user.label->resize(1000, height+5);

        /* update label dimension according to remote cursor position */
        QFont l_font=user.label->font();
        QTextCharFormat fmt=user.cursor.charFormat();
        int font_size=static_cast<int>(fmt.fontPointSize());
        if(font_size==0)
            font_size=DEFAULT_SIZE;
        QFont new_font(l_font.family(),static_cast<int>((font_size/2)+3),QFont::Bold);
        user.label->setFont(new_font);

        user.label->move(remoteCoord.left(), remoteCoord.top()-(user.label->fontInfo().pointSize()/3));
        user.label->setVisible(true);
    }
}



void TextEdit::loadFile(QList<Symbol> file)
{
    setCurrentFileName(QString());
    QString tmp;
    bool u=true;
    QChar al,q;
    std::vector<Symbol> vtmp;
    QTextCursor curs=textEdit->textCursor();
    QTextCharFormat qform;
    QColor col;
    foreach(Symbol s,file){
        externAction=true;
        if(s.getSiteId()==siteid)
            col=Qt::transparent;
        else
            col=listcolor.at(s.getSiteId()%11);
        qform.setBackground(col);
        qform.setFontFamily(s.getFamily());
        qform.setFontItalic(s.getItalic());
        qform.setFontUnderline(s.getUnderln());
        qform.setFontPointSize(s.getSize());
        if(s.getBold())
            qform.setFontWeight(QFont::Bold);
        if(al!=s.getAlign()){
            al=s.getAlign();
            textEdit->setTextCursor(curs);
            textEdit->setAlignment(insertalign(al));
            externAction=true;
        }
        if(s.getValue()=='\0')
            curs.insertText((QChar)'\n',qform);
        else
            curs.insertText(s.getValue(),qform);
        vtmp.push_back(s);
    }
    crdt->setSymbols(vtmp);
}

Qt::Alignment TextEdit::insertalign(QChar c){
    Qt::Alignment alline;
    switch (c.toLatin1()) {
    case 'L' :
        alline=Qt::AlignLeft;
        break;
    case 'C' :
        alline= Qt::AlignHCenter;
        break;
    case 'R' :
        alline= Qt::AlignRight;
        break;
    case 'J' :
        alline= Qt::AlignJustify;
        break;
    default:
        break;
    }
    return alline;

}

QChar TextEdit::findalign(Qt::Alignment al){
    QChar c;
    if(al==Qt::AlignLeft || al==(Qt::AlignLeft | Qt::AlignAbsolute))
        c='L';
    else if(al==Qt::AlignHCenter || al==(Qt::AlignCenter | Qt::AlignAbsolute))
        c='C';
    else if(al==Qt::AlignRight || al==(Qt::AlignRight | Qt::AlignAbsolute))
        c='R';
    else if(al==Qt::AlignJustify || al==(Qt::AlignJustify | Qt::AlignAbsolute))
        c='J';
    return c;
}

void TextEdit::moveCursor(int pos, QString userId)
{
    if(m_onlineUsers.contains(userId)){
    User user ;
    user.user = userId;
    user.cursor.setPosition(pos);
    QRect remoteCoord = textEdit->cursorRect(user.cursor);
    int height = remoteCoord.bottom()-remoteCoord.top();
    user.label->resize(1000, height+5);

    /* update label dimension according to remote cursor position */
    QFont l_font=user.label->font();
    QTextCharFormat fmt=user.cursor.charFormat();
    int font_size=static_cast<int>(fmt.fontPointSize());
    if(font_size==0)
        font_size=DEFAULT_SIZE;
    QFont new_font(l_font.family(),static_cast<int>((font_size/2)+3),QFont::Bold);
    user.label->setFont(new_font);

    user.label->move(remoteCoord.left(), remoteCoord.top()-(user.label->fontInfo().pointSize()/3));
    user.label->setVisible(true);
    }
}
