#include <QtCore/QString>
#include <QtCore/QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "include/countricewindow.h"
#include "ui_countricewindow.h"

#include "include/image_process.h"

using namespace cv;
using namespace std;

CountRiceWindow::CountRiceWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CountRiceWindow)
{
    ui->setupUi(this);
}

CountRiceWindow::~CountRiceWindow()
{
    delete ui;
}

void CountRiceWindow::setOrigionImage(Mat image)
{
    origion_image_ = image.clone();
}

void CountRiceWindow::showOrigionImage()
{
    QImage img =QImage((const unsigned char*)(origion_image_.data),
                        origion_image_.cols,origion_image_.rows,
                        origion_image_.cols*origion_image_.channels(),
                        QImage::Format_RGB888);

    ui->origionImage->setPixmap(QPixmap::fromImage(img));
    ui->origionImage->resize(ui->origionImage->pixmap()->size());
}

void CountRiceWindow::on_pushButton_clicked()
{
    std::string file_name = QFileDialog::getOpenFileName(this, tr("open file"), " ",  tr("pngfile(*.png);jpegfile(*.jpeg);bmpfile(*.bmp);jpgfile(*.jpg)")).toStdString();
    Mat image = imread(file_name);
    setOrigionImage(image);
    showOrigionImage();
}

void CountRiceWindow::on_returnMain_clicked()
{
    this->parentWidget()->show();
    delete this;
}

void CountRiceWindow::on_countRice_clicked()
{
    if (origion_image_.empty())
    {
        QMessageBox::information(this,"提示","请添加图片");
        return;
    }
    CountRice count_rice;

    Mat result_image;
    count_rice.countRice(origion_image_, result_image);

    QImage img = toQImage(result_image);
    printf("rice num:%d ", count_rice.riceNumber());
    QString tip = "米粒数量: "+ QString::number(count_rice.riceNumber());
    ui->riceNum->setText(tip);
    ui->resultImage->setPixmap(QPixmap::fromImage(img));
    ui->resultImage->resize(ui->resultImage->pixmap()->size());
    ui->resultImage->move(result_image.cols+100, 30);
}

QImage CountRiceWindow::toQImage(cv::Mat image)
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
