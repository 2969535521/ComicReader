#include "comicreaderwindow.h"
#include "ui_comicreaderwindow.h"
#include "QDir"
#include "QDebug"
#include "QImageReader"
#include "QFileDialog"
#include "QTemporaryDir"
#include "QProcess"
#include "QFileInfoList"
#include "QtGui/private/qzipreader_p.h"

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

    ui->OnePageButton->setIcon(QIcon(":/icon/resources/one.png"));
    connect(ui->OnePageButton, SIGNAL(clicked()), this, SLOT(singlePage()));

    ui->DoublePageButton->setIcon(QIcon(":/icon/resources/two.png"));
    connect(ui->DoublePageButton, SIGNAL(clicked()), this, SLOT(doublePage()));

    ui->BookmarkButton->setIcon(QIcon(":/icon/resources/bookmark.png"));
    connect(ui->BookmarkButton, SIGNAL(clicked()), this, SLOT(addBookmark()));

    ui->saveButton->setIcon(QIcon(":/icon/resources/save.png"));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveImage()));

    connect(ui->BookmarkList, SIGNAL(activated(int)), this, SLOT(onComboBoxItemActivated(int)));

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
    filename = fileName;
    QStringList tmp = fileName.split("/");
    this->setWindowTitle(tmp[tmp.size()-1]);
    //qDebug() << fileName;
    readImageFile(fileName);
    currentPage = 1;
    showBookmarkList();
    showPage();
    ui->currentPage->setText(QString::number(currentPage));
    ui->currentPage->repaint();
    ui->totalPage->setText("/" + QString::number(totalPage));
    ui->totalPage->repaint();
}

//展示特定页，通过currentPage实现
void ComicReaderWindow::displayPage(){
    currentPage = ui->currentPage->text().toInt();
    showPage();
}

//展示前一页，通过currentPage实现
void ComicReaderWindow::prevPage(){
    if(currentPage > 1){
        if(isSingle){
            currentPage--;
            ui->currentPage->setText(QString::number(currentPage));
            ui->currentPage->repaint();
        }
        else{
            if(currentPage > 2){
                currentPage -= 2;
                ui->currentPage->setText(QString::number(currentPage));
                ui->currentPage->repaint();
            }
            else{
                currentPage--;
                ui->currentPage->setText(QString::number(currentPage));
                ui->currentPage->repaint();
            }
        }
        showPage();
    }
}

//展示后一页，通过currentPage实现
void ComicReaderWindow::nextpage(){
    if(currentPage < totalPage){
        if(isSingle){
            currentPage++;
            ui->currentPage->setText(QString::number(currentPage));
            ui->currentPage->repaint();
        }
        else{
            if(currentPage < totalPage-1){
                currentPage += 2;
                ui->currentPage->setText(QString::number(currentPage));
                ui->currentPage->repaint();
            }
            else{
                currentPage++;
                ui->currentPage->setText(QString::number(currentPage));
                ui->currentPage->repaint();
            }
        }
        showPage();
    }
}

//展示第一页，通过currentPage实现
void ComicReaderWindow::firstPage(){
    currentPage = 1;
    ui->currentPage->setText(QString::number(currentPage));
    ui->currentPage->repaint();
    showPage();
}

//展示最后一页，通过currentPage实现
void ComicReaderWindow::lastPage(){
    currentPage = totalPage;
    ui->currentPage->setText(QString::number(currentPage));
    ui->currentPage->repaint();
    showPage();
}

//单页展示
void ComicReaderWindow::singlePage(){
    isSingle = true;
    showPage();
}

//双页展示
void ComicReaderWindow::doublePage(){
    isSingle = false;
    showPage();
}

//增加书签，需要一个存储书签的数据结构，并且需要保存到本地
void ComicReaderWindow::addBookmark(){
    QString tmp = filename + " " + QString::number(currentPage);
    bookmarkList.push_back(tmp);

    QDir *dir = new QDir(QDir::currentPath());
    dir->cdUp();
    QFile file(dir->path() + "/ComicReader/bookmarklist.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        qDebug() << ("open file error");
    }
    QTextStream out(&file);
    for(int i = 0; i < bookmarkList.size(); i++){
        out << bookmarkList[i] << endl;
    }
    file.flush();
    file.close();
    showBookmarkList();
}

//书签列表，开始从本地读取 改成下拉框
void ComicReaderWindow::showBookmarkList(){
    QDir *dir = new QDir(QDir::currentPath());
    dir->cdUp();
    QFile file(dir->path() + "/ComicReader/bookmarklist.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << ("open file error");
    }
    QTextStream in(&file);
    bookmarkList.clear();
    ui->BookmarkList->clear();
    int i = 0;
    while(!in.atEnd()){
        QString line = in.readLine();
        bookmarkList.push_back(line);
        ui->BookmarkList->addItem(line);
        ui->BookmarkList->setItemData(i++, line , Qt::UserRole);
    }
    ui->BookmarkList->repaint();
    //将读取到的历史记录写入框中
    file.close();
}

//书签响应函数
void ComicReaderWindow::onComboBoxItemActivated(int index){
    // 获取被单击的Item的objectName
    QString itemName = ui->BookmarkList->itemData(index, Qt::UserRole).toString();
    //qDebug() << itemName;
    QStringList item = itemName.split(" ");
    currentPage = item[1].toInt();
    //qDebug() << item[0];
    openFile(item[0]);
}

//保存当前图片
void ComicReaderWindow::saveImage(){
    // 弹出文件保存对话框，获取用户选择的文件路径
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Image", "", "PNG Image (*.png);;JPEG Image (*.jpg)");
    if (!fileName.isEmpty()) {
        // 保存图片到用户选择的文件路径
        if (imageList[currentPage-1].save(fileName)) {
            qDebug() << "Image saved successfully.";
        } else {
            qDebug() << "Failed to save image.";
        }
    }
}

//展示图片函数
void ComicReaderWindow::showPage(){
    if(isSingle){
        ui->comicImage_1->setVisible(false);
        ui->comicImage_2->setVisible(false);
        ui->comicImage->setVisible(true);
        ui->comicImage->setPixmap(QPixmap::fromImage(imageList[currentPage-1]));
        ui->comicImage->repaint();
    }
    else{
        ui->comicImage->setVisible(false);
        ui->comicImage_1->setVisible(true);
        ui->comicImage_2->setVisible(true);
        if(currentPage < totalPage){
            if(currentPage % 2 == 1){
                ui->comicImage_1->setPixmap(QPixmap::fromImage(imageList[currentPage-1]));
                ui->comicImage_2->setPixmap(QPixmap::fromImage(imageList[currentPage]));
            }
            else{
                ui->comicImage_1->setPixmap(QPixmap::fromImage(imageList[currentPage-2]));
                ui->comicImage_2->setPixmap(QPixmap::fromImage(imageList[currentPage-1]));
            }
            ui->comicImage_1->repaint();
            ui->comicImage_2->repaint();
        }
        else{
            ui->comicImage_1->setPixmap(QPixmap::fromImage(imageList[currentPage-1]));
            ui->comicImage_1->repaint();
            ui->comicImage_2->setVisible(false);
        }
    }
}

//读取文件
void ComicReaderWindow::readImageFile(QString filePath){

        //读取到imageList中，并且把totalPage设置为imageList.size()
        // 打开cbz或cbr文件并获取图像列表
    imageList.clear();
    QZipReader reader(filePath);
        if (reader.status() != QZipReader::NoError) {
            qWarning("Failed to open cbz or cbr file");
            return;
        }
        QStringList imageExtensions = {"jpg", "jpeg", "png", "bmp", "gif"};
        for (auto fileInfo : reader.fileInfoList()) {
            //qDebug() << QFileInfo(fileInfo.filePath).suffix().toLower();
            if (imageExtensions.contains(QFileInfo(fileInfo.filePath).suffix().toLower())) {
                QImage image;
                if (image.loadFromData(reader.fileData(fileInfo.filePath))) {
                    imageList.append(image);
                }
            }
        }
        totalPage = imageList.size();
       //qDebug() << totalPage;
}

