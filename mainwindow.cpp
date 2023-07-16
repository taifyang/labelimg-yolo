#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <iostream>

QVector<int> MainWindow::labels ={};

#define OFFSET 45

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ispress = false; //初始鼠标左键未按下
    this->centralWidget()->setMouseTracking(true);//开启鼠标实时追踪，监听鼠标移动事件，默认只有按下时才监听
    this->setMouseTracking(true);

    QAction *openAction = new QAction;
    openAction->setShortcut(QKeySequence(tr("Ctrl+O")));
    ui->toolBar->addAction(openAction);
    connect(openAction, SIGNAL(triggered()), this, SLOT(OpenPic()));

    QAction *saveAction = new QAction;
    saveAction->setShortcut(QKeySequence(tr("Ctrl+S")));
    ui->toolBar->addAction(saveAction);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(SaveTxt()));

    QAction *nextAction = new QAction;
    nextAction->setShortcut(QKeySequence(tr("Right")));
    ui->toolBar->addAction(nextAction);
    connect(nextAction, SIGNAL(triggered()), this, SLOT(NextPic()));

    dialog = new Dialog;
    ui->statusbar->showMessage("Ctrl+O打开图像文件夹 鼠标左键拖动矩形标注框 右箭头按键换下一张图片");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if(isopen == 0)//不是打开图片的，每一次新建一个空白的画布
    {
        pixmap = QPixmap(size());
        pixmap.fill(Qt::white);
    }
    QPixmap pix = pixmap.scaled(this->width(),this->height(),Qt::KeepAspectRatio);
    QPainter p(&pix);

    QPen pen;
    pen.setColor(QColor(0,0,255));
    pen.setWidth(2);
    p.setPen(pen);
    for(int i = 0; i <rects.size(); ++i)
    {
       p.drawRect(rects.at(i));
    }

    pen.setColor(QColor(255,0,0));
    p.setPen(pen);
    p.drawLine(0, pos.y(), width(), pos.y());
    p.drawLine(pos.x(), OFFSET, pos.x(), height());
    p.end();
    p.begin(this);//将当前窗体作为画布
    p.drawPixmap(0, 0, pix);//将pixmap画到窗体
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)//当鼠标左键按下
    {
        ispress = true;//左键按下标志
        QRect rect;//鼠标按下，矩形开始
        rects.append(rect);//将新矩形添加到矩形集合
        QRect& lastRect = rects.last();//拿到新矩形
        lastRect.setTopLeft(e->pos());//记录鼠标的坐标(新矩形的左上角坐标)
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(ispress)
    {
        QRect& lastRect = rects.last();//拿到最后添加的矩形
        lastRect.setBottomRight(e->pos());//更新矩形的右下角坐标
    }

    pos = e->pos();
    update();//触发窗体重绘
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
    if(ispress)
    {
        QRect& lastRect = rects.last();//拿到新矩形
        lastRect.setBottomRight(e->pos());//不是拖拽时，更新矩形的右下角坐标)
        ispress = false;
    }

    qDebug()<<rects.last();
    dialog->show();
}

void MainWindow::SaveTxt()
{
    QString picPath = picsPath + "/" + imagesList[num];
    qDebug() << picPath;

    QString fileName = picPath.replace(".bmp", ".txt").replace(".jpg", ".txt").replace(".png", ".txt");
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    QTextStream stream(&file);

    if (fileName.length() > 0)
    {
        qDebug()<<labels << rects;

        for(int i=0;i<labels.size();++i)
        {
            float x = 1.0f * rects[i].x() / this->width();
            float y = 1.0f *(rects[i].y() - OFFSET) / this->height();
            float width = abs(1.0f * rects[i].width() / this->width());
            float height = abs(1.0f * rects[i].height() / this->height());
            stream << labels[i] << " " << x << " " << y << " " << width << " " << height <<"\n";
        }
    }

    file.close();
}

QStringList getImageList(QString path)
{
    QStringList imagesList;
    QDir dir(path);
    if (!dir.exists())
    {
        qDebug() << "path is non-existent!";
        return imagesList;
    }
    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QStringList filters;
    filters << "*.bmp" << "*.jpg" << "*png";
    dir.setNameFilters(filters);
    imagesList = dir.entryList();
    return imagesList;
}

void MainWindow::ShowPic(QString picsPath)
{
    QString picPath = picsPath + "/" + imagesList[num];
    ui->statusbar->showMessage(picPath+"  "+QString::number(num+1)+"/"+QString::number(imagesList.size()));

    QPixmap pix;
    pix.load(picPath);//加载图片
    width_ratio = (float)pix.width()/this->width();
    height_ratio = (float)pix.height()/this->height();

    pix = pix.scaled(this->width(),this->height(),Qt::KeepAspectRatio);
    QPainter p(&pixmap);
    p.drawPixmap(0, OFFSET, pix);//添加工具栏的空间
    isopen = 1;//设置文件打开标志
    update();//触发窗体重绘，将图片画到窗体
}

void MainWindow::OpenPic()
{
    picsPath = QFileDialog::getExistingDirectory(this,tr("打开"),"");
    if(picsPath.isEmpty())
        return;

    imagesList = getImageList(picsPath);
    qDebug()<<imagesList;

    ShowPic(picsPath);
}

void MainWindow::NextPic()
{
    if(num+1>=imagesList.size())
        return;

    SaveTxt();
    rects.clear();
    labels.clear();
    pixmap = QPixmap(size());//新建pixmap
    pixmap.fill(Qt::white);//背景色填充为白色
    num++;

    ShowPic(picsPath);
}

void MainWindow::keyPressEvent(QKeyEvent *e) //按键事件
{
    if (e->key() == Qt::Key_Z && e->modifiers() == Qt::ControlModifier) //Ctrl+Z撤销
    {
        if(rects.size()>0)
        {
            rects.pop_back();
            update();
        }
    }
    else if (e->key() == Qt::Key_S && e->modifiers() == Qt::ControlModifier) //Ctrl+S保存
    {
        SaveTxt();
    }
}

