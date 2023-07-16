#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    static int label;

private slots:
    void on_pushButton_yes_clicked();

    void on_pushButton_no_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
