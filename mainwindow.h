#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QVector>
#include <QPoint>
#include <QAction>
#include <QColor>
#include <QMenu>
#include <QFileDialog>
#include <QTextStream>

#include "dialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVector<QRect> rects;//矩形集合
    static QVector<int> labels;//矩形集合
    QPoint pos;

protected:
    void paintEvent(QPaintEvent *);//重写窗体重绘事件
    void mousePressEvent(QMouseEvent *);//重写鼠标按下事件
    void mouseReleaseEvent(QMouseEvent *);//重写鼠标释放事件
    void mouseMoveEvent(QMouseEvent *);//重写鼠标移动事件
    void keyPressEvent(QKeyEvent *e); //重写按键事件

private:
    Ui::MainWindow *ui;
    int ispress = 0; //左键按下标志
    int isopen = 0; //打开图片标志
    QPixmap pixmap;
    Dialog *dialog;
    QString picsPath;
    QStringList imagesList;
    float width_ratio = 1.0f;
    float height_ratio = 1.0f;
    int num = 0;

private slots:
    void OpenPic(); //打开图片
    void SaveTxt(); //保存图片
    void NextPic(); //下张图片
    void ShowPic(QString picPath); //显示图片

};
#endif // MAINWINDOW_H
