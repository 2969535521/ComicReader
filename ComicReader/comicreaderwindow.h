#ifndef COMICREADERWINDOW_H
#define COMICREADERWINDOW_H

#include <QMainWindow>
#include "QVBoxLayout"
#include "QLabel"

namespace Ui {
class ComicReaderWindow;
}

class ComicReaderWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ComicReaderWindow(QWidget *parent = nullptr);
    ~ComicReaderWindow();

private:
    Ui::ComicReaderWindow *ui;
    int currentPage;    //当前页码
    int totalPage;  //总页码

public slots:
    void openFile(QString fileName);    //打开文件并显示第一页
    void displayPage();  //显示指定页
    void prevPage();    //前一页
    void nextpage();    //后一页
    void firstPage();   // 跳转到第一页
    void lastPage();    // 跳转到最后一页
    void singlePage();  // 单页模式
    void doublePage();   // 双页模式
    void zoomIn();  //放大
    void zoomOut(); //缩小
    void addBookmark(); //添加书签
    void showBookmarkList();    //显示书签列表
    void saveImage();   //保存图像

    //数据结构
private:

};

#endif // COMICREADERWINDOW_H
