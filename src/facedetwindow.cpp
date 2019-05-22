#include <QtCore/QString>
#include <QtCore/QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "include/facedetwindow.h"
#include "ui_facedetwindow.h"

using namespace std;
using namespace cv;

FaceDetWindow::FaceDetWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FaceDetWindow)
{
    pca_ = new WxxPCA();
    ui->setupUi(this);

    video_thread_ = boost::thread(boost::bind(&FaceDetWindow::videoThread, this));
}

FaceDetWindow::~FaceDetWindow()
{
    cap_.release();
    video_thread_.interrupt();
    delete ui;
}

void FaceDetWindow::on_returnMain_clicked()
{
    this->parentWidget()->show();
    delete this;
}

void FaceDetWindow::on_importDate_clicked()
{
    String link_path = QFileDialog::getOpenFileName(this, tr("open link file"), " ",  tr("txtfile(*.txt)")).toStdString();
    ui->tip->setText("正在添加图片,请稍等...");
    bool read_result = pca_->readImages(link_path);
    if (read_result == false)
    {
        ui->tip->setText("添加图片失败");
        QMessageBox::information(this,"错误","添加图片失败");
    }
    else
    {
        ui->tip->setText("添加图片成功");
         QMessageBox::information(this,"错误","添加图片成功");
    }
}

void FaceDetWindow::videoThread()
{
    ui->imageShow->setText("摄像头未开启");
    Mat image;
    while (true)
    {
        if (cap_.isOpened())
        {
            cap_>>image;
            if (image.empty())
                continue;

            QImage q_img = toQImage(image);
            ui->imageShow->clear();
            ui->imageShow->setPixmap(QPixmap::fromImage(q_img));
            ui->openCapture->setText("关闭摄像头");
        }
        else
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(500));
        }
    }
}

QImage FaceDetWindow::toQImage(Mat image)
{
    switch (image.type()) {
    case CV_8UC1:
        {
            QImage img((const unsigned char *)(image.data), image.cols, image.rows, image.cols, QImage::Format_Grayscale8);
            return img;
        }
        break;

    case CV_8UC3:
        {
            QImage img((const unsigned char *)(image.data), image.cols, image.rows, image.cols * 3, QImage::Format_RGB888);
            return img.rgbSwapped();
        }
        break;

    case CV_8UC4:
        {
            QImage img((const unsigned char *)(image.data), image.cols, image.rows, image.cols * 4, QImage::Format_ARGB32);
            return img;
        }
        break;

    default:
        {
            QImage img;
            return img;
        }
        break;
    }
}

void FaceDetWindow::on_openCapture_clicked()
{
    if (cap_.isOpened())
    {
        ui->openCapture->setText("打开摄像头");
        ui->imageShow->setText("摄像头已关闭");
        cap_.release();
    }
    else
    {
        ui->imageShow->setText("摄像头打开中");
        cap_.open(0);
    }
}

void FaceDetWindow::on_startTrain_clicked()
{
    ui->tip->setText("数据处理中");
    pca_->dataProcess();
    ui->tip->setText("数据处理完成");
}

void FaceDetWindow::on_test_clicked()
{
    ui->tip->setText("识别中");
    pca_->test();
    ui->tip->setText("识别完成");
}
