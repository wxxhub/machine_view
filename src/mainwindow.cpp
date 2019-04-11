#include <QtCore/QString>
#include <QtCore/QFile>
#include <QFileDialog>

#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/countricewindow.h"
#include "include/numdetwindow.h"
#include "include/facedetwindow.h"
#include "include/numdetwindow2.h"

#include "include/image_process.h"

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("image/logo.ico"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toCountRice_clicked()
{
    CountRiceWindow *count_rice_window = new CountRiceWindow(this);
    count_rice_window->show();
    this->hide();
}

void MainWindow::on_toNumDet_clicked()
{
    NumDetWindow *num_det_window = new NumDetWindow(this);
    num_det_window->show();
    this->hide();
}

void MainWindow::on_toPCAFaceDet_clicked()
{
    FaceDetWindow *face_det_window = new FaceDetWindow(this);
    face_det_window->show();
    this->hide();
}

void MainWindow::on_toNumDet_2_clicked()
{
    NumDetWindow2 *num_det_window2 = new NumDetWindow2(this);
    num_det_window2->show();
    this->hide();
}
