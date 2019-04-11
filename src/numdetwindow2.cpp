#include <QFileDialog>
#include <unistd.h>
#include <sys/stat.h>
#include <iostream>

#include "include/numdetwindow2.h"
#include "ui_numdetwindow2.h"

using namespace std;
using namespace cv;

NumDetWindow2::NumDetWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NumDetWindow2),
    cut_image_cache_file_("cutImageCache")
{
    ui->setupUi(this);
    if (access(cut_image_cache_file_.c_str(), F_OK) == -1)
        mkdir(cut_image_cache_file_.c_str(), 07755);
}

NumDetWindow2::~NumDetWindow2()
{
    delete ui;
}

void NumDetWindow2::on_returnMain_clicked()
{
    this->parentWidget()->show();
    delete this;
}

void NumDetWindow2::on_cutImage_clicked()
{
    std::string file_name = QFileDialog::getOpenFileName(this, tr("open file")).toStdString();
    Mat image = imread(file_name, CV_LOAD_IMAGE_GRAYSCALE);
    if (image.empty())
        return;

    list<Mat> cut_mat_list_ = image_process_->cutImage(image, 7, 7);

    char image_name[36];
    list<Mat>::iterator iter = cut_mat_list_.begin();
    for (int i = 0; iter != cut_mat_list_.end(); iter++, i++)
    {
        sprintf(image_name, "%s/cut_%d.jpg",cut_image_cache_file_.c_str(), i);
//        printf("image_name: %s\n", image_name);
        imwrite(image_name, *iter);
    }
}
