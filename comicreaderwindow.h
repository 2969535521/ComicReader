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
    QString filename;   //文件名
    int currentPage = 1;    //当前页码
    int totalPage;  //总页码
    bool isSingle = true;  //是否单页
    //数据结构
    QVector<QString> bookmarkList; //最近书签文件
    QVector<QImage> imageList;  //存放图片文件

public slots:
    void openFile(QString fileName);    //打开文件并显示第一页
    void displayPage();  //显示指定页
    void prevPage();    //前一页
    void nextpage();    //后一页
    void firstPage();   // 跳转到第一页
    void lastPage();    // 跳转到最后一页
    void singlePage();  // 单页模式
    void doublePage();   // 双页模式
    void addBookmark(); //添加书签
    void showBookmarkList();    //显示书签列表
    void saveImage();   //保存图像
    void onComboBoxItemActivated(int index);    //书签响应函数
    void showPage();    //展示图片函数
    void readImageFile(QString filepath); //读取文件


};

#endif // COMICREADERWINDOW_H
