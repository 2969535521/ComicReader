#include "comicreaderwindow.h"
#include "ui_comicreaderwindow.h"

ComicReaderWindow::ComicReaderWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ComicReaderWindow)
{
    ui->setupUi(this);
    //绑定按钮的图标与方法
    ui->b_prevpageButton->setIcon(QIcon(":/icon/resources/bigbefore.png"));
    ui->b_prevpageButton->setShortcut(QKeySequence(Qt::Key_Left));
    connect(ui->b_prevpageButton, SIGNAL(clicked()), this, SLOT(prevPage()));

    ui->b_nextpageButton->setIcon(QIcon(":/icon/resources/bignext.png"));
    ui->b_nextpageButton->setShortcut(QKeySequence(Qt::Key_Right));
    connect(ui->b_nextpageButton, SIGNAL(clicked()), this, SLOT(nextpage()));

    ui->previousPageButton->setIcon(QIcon(":/icon/resources/before.png"));
    connect(ui->previousPageButton, SIGNAL(clicked()), this, SLOT(firstPage()));

    ui->nextPageButton->setIcon(QIcon(":/icon/resources/next.png"));
    connect(ui->nextPageButton, SIGNAL(clicked()), this, SLOT(lastPage()));

    ui->ZoominButton->setIcon(QIcon(":/icon/resources/big.png"));
    connect(ui->ZoominButton, SIGNAL(clicked()), this, SLOT(zoomIn()));

    ui->ZoomoutButton->setIcon(QIcon(":/icon/resources/small.png"));
    connect(ui->ZoomoutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

    ui->OnePageButton->setIcon(QIcon(":/icon/resources/one.png"));
    connect(ui->OnePageButton, SIGNAL(clicked()), this, SLOT(singlePage()));

    ui->DoublePageButton->setIcon(QIcon(":/icon/resources/two.png"));
    connect(ui->DoublePageButton, SIGNAL(clicked()), this, SLOT(doublePage()));

    ui->BookmarkButton->setIcon(QIcon(":/icon/resources/bookmark.png"));
    connect(ui->BookmarkButton, SIGNAL(clicked()), this, SLOT(addBookmark()));

    ui->BookmarkListButton->setIcon(QIcon(":/icon/resources/list.png"));
    connect(ui->BookmarkButton, SIGNAL(clicked()), this, SLOT(showBookmarkList()));

    ui->currentPage->setText(QString::number(currentPage));
    ui->currentPage->repaint();
    ui->totalPage->setText("/" + QString::number(totalPage));
    ui->totalPage->repaint();
    connect(ui->gotoButton, SIGNAL(clicked()), this, SLOT(displayPage()));

}

ComicReaderWindow::~ComicReaderWindow()
{
    delete ui;
}

//打开文件，在mainwindow中会调用，把获取到的路径传输进来，处理文件获得totalPage，并且读取本地书签
void ComicReaderWindow::openFile(QString fileName){
    QStringList tmp = fileName.split("/");

    currentPage = 1;
    totalPage = 20;
    ui->currentPage->setText(QString::number(currentPage));
    ui->currentPage->repaint();
    ui->totalPage->setText("/" + QString::number(totalPage));
    ui->totalPage->repaint();
    QImage *img= new QImage;
    img->load(":/icon/魔王勇者.jpg");
    ui->comicImage->setPixmap(QPixmap::fromImage(*img));
}

//展示特定页，通过currentPage实现
void ComicReaderWindow::displayPage(){
    currentPage = ui->currentPage->text().toInt();
}

//展示前一页，通过currentPage实现
void ComicReaderWindow::prevPage(){}

//展示后一页，通过currentPage实现
void ComicReaderWindow::nextpage(){}

//展示第一页，通过currentPage实现
void ComicReaderWindow::firstPage(){}

//展示最后一页，通过currentPage实现
void ComicReaderWindow::lastPage(){}

//单页展示
void ComicReaderWindow::singlePage(){}

//双页展示
void ComicReaderWindow::doublePage(){}

//放大
void ComicReaderWindow::zoomIn(){}

//缩小
void ComicReaderWindow::zoomOut(){}

//增加书签，需要一个存储书签的数据结构，并且需要保存到本地
void ComicReaderWindow::addBookmark(){}

//书签列表，开始从本地读取
void ComicReaderWindow::showBookmarkList(){}

//保存当前图片
void ComicReaderWindow::saveImage(){}
