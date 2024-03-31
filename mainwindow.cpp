#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    ui->astar_btn->setVisible(true);
    ui->bfs_btn->setVisible(true);
    ui->dfs_btn->setVisible(true);
    ui->greedy_btn->setVisible(true);
    ui->random_btn->setVisible(true);

    ui->menu_widget->setVisible(true);

    ui->img_label->setVisible(false);

    ui->ok_button->setDisabled(true);
    ui->speed_slider->setDisabled(false);
    ui->speed_slider->setValue(5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::swapScene(){
    //qDebug() << ui->menu_widget->isVisible();
    if (ui->menu_widget->isVisible()){
        ui->menu_widget->hide();
        ui->img_label->show();
    }
    else{
        ui->menu_widget->show();
        ui->img_label->hide();
    }

    QApplication::processEvents();

}

void MainWindow::showImage(const QSharedPointer<QImage> img) const{
    QPixmap pixmap = QPixmap::fromImage(*img);
    int w = ui->img_label->width();
    int h = ui->img_label->height();
    //qDebug() << "Label: " << w << " " << h;
    ui->img_label->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio));

    QApplication::processEvents();
}

QString MainWindow::getMapFile(){
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Open Document"), QDir::currentPath(),
        tr("text files (*.txt)"));

    //qDebug() << "!!" << filename;
    return filename;
}


void MainWindow::runAnimation(QSharedPointer<Algo> model) const{
    QSharedPointer<QImage> img = model->getImage();
    showImage(img);
    QThread::msleep(500);
    int iteration = 0;
    while(!model->isFinished()){

        model->step();
        //qDebug() << "It: " << iteration++;
        img = model->getImage();
        showImage(img);

        //sleep or wait key
        if(ui->speed_slider->value() > 0){
            QThread::msleep(ui->speed_slider->value());
        }

    }
    //QThread::sleep(1);
    bool isPath = model->vizualize_path();
    qDebug() << "Vizualizing path" << isPath;
    int open, close, path;
    model->getInfo(&open, &close, &path);
    displayInfo(open, close, path);
    ui->ok_button->setDisabled(false);

    img = model->getImage();
    showImage(img);
}


void MainWindow::on_ok_button_clicked()
{
    ui->ok_button->setDisabled(true);
    displayInfo(0,0,0);
    swapScene();
}

void MainWindow::run(char algo_name){
    QString filename = getMapFile();
    if (filename == ""){
        return;
    }
    QSharedPointer<Map> map = QSharedPointer<Map>( new Map(filename));

    swapScene();
    QSharedPointer<Algo> model;


    switch (algo_name) {
    case 'b':
        model = QSharedPointer<Bfs>(new Bfs(map));
        break;
    case 'd':
        model = QSharedPointer<Dfs>(new Dfs(map));
        break;
    case 'r':
        model = QSharedPointer<Random>(new Random(map));
        break;
    case 'g':
        model = QSharedPointer<Greedy>(new Greedy(map));
        break;
    case 'a':
        model = QSharedPointer<AStar>(new AStar(map));
        break;
    default:
        swapScene();
        return;
    }

    //QMetaObject::invokeMethod(this, "runModel", Qt::QueuedConnection, model);
    qDebug() << "Anim started";
    runAnimation(model);
    qDebug() << "Anim finished";
    //swapScene();
}

void MainWindow::displayInfo(int open, int close, int path) const{
    ui->open_label->setText("Open: " + QString::number(open));
    ui->closed_label->setText("Closed: " + QString::number(close));
    ui->path_label->setText("Path: " + QString::number(path));

}


void MainWindow::on_bfs_btn_clicked()
{
    run('b');
}

void MainWindow::on_dfs_btn_clicked()
{
    run('d');
}

void MainWindow::on_random_btn_clicked()
{
    run('r');
}


void MainWindow::on_greedy_btn_clicked()
{
    run('g');
}


void MainWindow::on_astar_btn_clicked()
{
    run('a');
}

