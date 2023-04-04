#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "comicreaderwindow.h"
#include <QFileDialog>
#include "QTextStream"
#include "QDebug"
#include "QStandardPaths"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readrenList();
    // Connect openBtn signal to openBook() slot
    connect(ui->openBtn, SIGNAL(clicked()), this, SLOT(openBook()));

    // Connect createBtn signal to createBook() slot
    connect(ui->createBtn, SIGNAL(clicked()), this, SLOT(createBook()));

    crw = new ComicReaderWindow();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openBook()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
                                                    tr("Comic Book Archive (*.cbz *.cbr);;All Files (*)"));
    // TODO: Add code to add file to listWidget and open the file in the reader window
    crw->show();
    crw->openFile(fileName);
    recentList.push_back(fileName);
    writerenList();
    readrenList();
}

void MainWindow::createBook()
{
    // TODO: Add code to create new book and add it to the listWidget
}

void MainWindow::readrenList(){
    QDir *dir = new QDir(QDir::currentPath());
    dir->cdUp();
    QFile file(dir->path() + "/ComicReader/recentlist.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << ("open file error");
    }
    QTextStream in(&file);
    recentList.clear();
    while(!in.atEnd()){
        QString line = in.readLine();
        recentList.push_back(line);
    }

    //将读取到的历史记录写入框中
    ui->listWidget->clear();
    for(int i = recentList.size()-1 ; i >= 0; i--){
        ui->listWidget->addItem(recentList[i]);
    }
    ui->listWidget->repaint();
    file.close();
}

void MainWindow::writerenList(){
    QDir *dir = new QDir(QDir::currentPath());
    dir->cdUp();
    QFile file(dir->path() + "/ComicReader/recentlist.txt");
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
        qDebug() << ("open file error");
    }
    QTextStream out(&file);
    for(int i = 0; i < recentList.size(); i++){
        out << recentList[i] << endl;
    }
    file.flush();
    file.close();
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item){
    qDebug() << item->text();
    crw->openFile(item->text());
    crw->show();
}
