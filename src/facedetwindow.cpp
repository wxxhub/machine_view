#include "include/facedetwindow.h"
#include "ui_facedetwindow.h"

FaceDetWindow::FaceDetWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FaceDetWindow)
{
    ui->setupUi(this);
}

FaceDetWindow::~FaceDetWindow()
{
    delete ui;
}

void FaceDetWindow::on_returnMain_clicked()
{
    this->parentWidget()->show();
    delete this;
}
