#include "mailwindow.h"
#include "ui_mailwindow.h"

MailWindow::MailWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MailWindow)
{
    ui->setupUi(this);
}

MailWindow::~MailWindow()
{
    delete ui;
}

void MailWindow::SetPickedMail(Containers::Mail *pickedMail)
{
    mail = pickedMail;
    SetLabels();
}

void MailWindow::SetLabels()
{
    QString nadawca = QString::fromStdString(mail->sender->getName());
    QString nadawcaemail = nadawca;
    nadawcaemail +=" ( ";
    nadawcaemail += QString::fromStdString(mail->sender->getEmail().getFull());
    nadawcaemail +=" )";
    ui->nadawcaText->setText( nadawcaemail  );
    std::string allreceivers;
    for (auto it = mail->receivers.cbegin(); it != mail->receivers.cend() ; it++)
    {
        std::pair<Containers::Person*, Containers::Receiver> a=*it;
        Containers::Person* b = a.first;
        allreceivers += b->getName();
        allreceivers += " ,";
    }
    allreceivers.pop_back();
    ui->odbiorcaText->setText( QString::fromStdString(allreceivers) );
    ui->tematText->setText( QString::fromStdString(mail->headers.getHeader("Subject")));
    ui->czasText->setText(QString::fromStdString(mail->sendDate.formatDate("%x")));

            QString qstr= " (";
            qstr+=QString::number(mail->sendDate.getUnixTimestamp());
            qstr+=")";
            qstr+="\n";
     qstr +=QString::fromStdString(mail->content);
    //item->setText(3,qstr);

    ui->textEdit->setText(qstr);
}

void MailWindow::closeEvent(QCloseEvent *)
{
    parentWidget()->setEnabled(true);
}


