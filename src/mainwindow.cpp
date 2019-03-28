#include <QtCore/QString>
#include <QtCore/QFile>
#include <QFileDialog>

#include "include/mainwindow.h"
#include "ui_mainwindow.h"
#include "include/countricewindow.h"

#include "include/image_process.h"

using namespace cv;
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
