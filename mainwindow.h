#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QPushButton"
#include "QListWidget"
#include "QLabel"
#include "QSpinBox"
#include "QVector"
#include "comicreaderwindow.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ComicReaderWindow *crw;

    //数据结构
    QVector<QString> recentList; //最近打开文件

    //方法
private slots:
    void openBook();    //打开按钮绑定方法
    void createBook();  //创建按钮绑定方法
    void readrenList(); //读取最近打开文件
    void writerenList();    //写入最近打开文件
    void on_listWidget_itemClicked(QListWidgetItem *item);    //打开list中的文件
    void saveFile();    //保存成cbz文件
};

#endif // MAINWINDOW_H
