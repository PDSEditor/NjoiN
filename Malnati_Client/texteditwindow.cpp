#include "texteditwindow.h"

TextEditWindow::TextEditWindow(QWidget *parent) : QWidget(parent)
{

}

void TextEditWindow::closeEvent(QCloseEvent *e)
{
    emit(openMW(this->getUri()));

}

QString TextEditWindow::getUri() const
{
    return uri;
}

void TextEditWindow::setUri(const QString &value)
{
    uri = value;
}
