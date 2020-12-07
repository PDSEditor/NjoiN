#ifndef TEXTEDITWINDOW_H
#define TEXTEDITWINDOW_H

#include <QWidget>
#include <message.h>

class TextEditWindow : public QWidget
{
    Q_OBJECT
public:
    explicit TextEditWindow(QWidget *parent = nullptr);

    QString getUri() const;
    void setUri(const QString &value);

signals:
    void openMW(QString);

protected:
    void closeEvent(QCloseEvent *e) override;

private:
    QString uri;


signals:

};

#endif // TEXTEDITWINDOW_H
