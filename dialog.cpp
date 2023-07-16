#include "dialog.h"
#include "ui_dialog.h"
#include "mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

int Dialog::label = -1;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    setAttribute(Qt::WA_QuitOnClose, false);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pushButton_yes_clicked()
{
    if (ui->lineEdit->text().isEmpty())
    {
        QMessageBox *box = new QMessageBox(QMessageBox::Warning, "warning", "Please input uint number!", QMessageBox::Ok);
        box->exec();
        return;
    }

    label = ui->lineEdit->text().toUInt();
    MainWindow::labels.push_back(label);

    this->close();
    qDebug()<<label;
}

void Dialog::on_pushButton_no_clicked()
{
    this->close();
}
