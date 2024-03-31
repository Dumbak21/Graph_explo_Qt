#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QPixmap>
#include <QMainWindow>
#include <QFileDialog>
#include <QMetaObject>

#include "map.h"
#include "algo.h"


QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_bfs_btn_clicked();

    void on_ok_button_clicked();

    void on_dfs_btn_clicked();

    void on_random_btn_clicked();

    void on_greedy_btn_clicked();

    void on_astar_btn_clicked();

private:
    Ui::MainWindow *ui;

    void swapScene();
    void showImage(const QSharedPointer<QImage> img) const;
    QString getMapFile();

    void displayInfo(int open, int close, int path) const;

    void runAnimation(QSharedPointer<Algo> model) const;
    void runModel(QSharedPointer<Algo> model);

    void run(char algo_name);

};
#endif // MAINWINDOW_H
