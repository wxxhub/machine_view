#include <QtCore/QString>
#include <QtCore/QFile>
#include <QFileDialog>
#include <QMessageBox>

#include "include/numdetwindow.h"
#include "ui_numdetwindow.h"

#include "include/numberdetector.h"

using namespace std;
using namespace cv;

NumDetWindow::NumDetWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NumDetWindow)
{
    num_detector_ = new NumberDetector();
    pac_ = new WxxPCA();

    table_model_->setColumnCount(2);
    table_model_->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("数字"));
    table_model_->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("准确率"));

    byes_table_model_->setColumnCount(2);
    byes_table_model_->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("数字"));
    byes_table_model_->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("准确率"));

    pca_table_model_->setColumnCount(2);
    pca_table_model_->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("数字"));
    pca_table_model_->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("准确率"));

    ui->setupUi(this);

    ui->createProgressBar->setValue(0);

    ui->resultTable->setModel(table_model_);
    ui->resultTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->resultTable->setColumnWidth(0,40);
    ui->resultTable->setColumnWidth(1,60);
    ui->resultTable->setAlternatingRowColors(true);
    ui->resultTable->verticalHeader()->hide();

    ui->byesResultTable->setModel(byes_table_model_);
    ui->byesResultTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->byesResultTable->setColumnWidth(0,40);
    ui->byesResultTable->setColumnWidth(1,60);
    ui->byesResultTable->setAlternatingRowColors(true);
    ui->byesResultTable->verticalHeader()->hide();

    ui->PCAResultTable->setModel(pca_table_model_);
    ui->PCAResultTable->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
    ui->PCAResultTable->setColumnWidth(0,40);
    ui->PCAResultTable->setColumnWidth(1,60);
    ui->PCAResultTable->setAlternatingRowColors(true);
    ui->PCAResultTable->verticalHeader()->hide();

    ui->matchBackgroud->setStyleSheet("background-color: #06b1cf;");
}

NumDetWindow::~NumDetWindow()
{
    delete ui;
}

void NumDetWindow::on_returnMain_clicked()
{
    this->parentWidget()->show();
    delete this;
}

void NumDetWindow::on_createTemplate_clicked()
{
    std::string file_direct = QFileDialog::getExistingDirectory(this,"请选择模板路径","./").toStdString();

    ui->createState->setText("正在生成模板...");
    for (int i = 0; i < 10; i++)
    {
        num_detector_->createTemple(file_direct, i);
        ui->createProgressBar->setValue((i+1)*10);
    }
    ui->createState->setText("生成模板完成!");
}

void NumDetWindow::on_matchNum_clicked()
{
//    QString lastPath = setting.value("LastFilePath");
    num_detector_->reloadTemple();
    std::string file_name = QFileDialog::getOpenFileName(this, tr("open file")).toStdString();
    Mat image = imread(file_name, CV_LOAD_IMAGE_GRAYSCALE);
    int result = 0;
    num_detector_->match(image, result);
    QString lab_text = "匹配结果: " + QString::number(result);
    ui->matchResult->setText(lab_text);
}

void NumDetWindow::on_matchStatistics_clicked()
{
    std::string file_direct = QFileDialog::getExistingDirectory(this,"请选择测试图片路径","./").toStdString();
    for (int i=0; i<10; i++)
    {
        float success_rate = num_detector_->detectStatistics(file_direct, i);
        table_model_->setItem(i, 0, new QStandardItem(QString::number(i)));
        table_model_->setItem(i, 1, new QStandardItem(QString::number(success_rate)));
    }
    ui->resultTable->setColumnWidth(0,40);
    ui->resultTable->setColumnWidth(1,60);
}

void NumDetWindow::on_createBayesTemplate_clicked()
{
    ui->createProgressBar->setValue(0);
    std::string file_direct = QFileDialog::getExistingDirectory(this,"请选择模板路径","./").toStdString();
    ui->createState->setText("正在生成贝叶斯模板...");
    num_detector_->createBayesTemple(file_direct);
    ui->createProgressBar->setValue(100);
    ui->createState->setText("生成贝叶斯模板完成!");
}

void NumDetWindow::on_byesDetec_clicked()
{
    std::string file_name = QFileDialog::getOpenFileName(this, tr("open file")).toStdString();
    Mat image = imread(file_name, CV_LOAD_IMAGE_GRAYSCALE);
    int result = 0;
    num_detector_->byesMatch(image, result);
    QString lab_text = "byes结果: " + QString::number(result);
    ui->byesResult->setText(lab_text);
}

void NumDetWindow::on_byesMatchStatistics_clicked()
{
    std::string file_direct = QFileDialog::getExistingDirectory(this,"请选择测试图片路径","./").toStdString();
    for (int i=0; i<10; i++)
    {
        float success_rate = num_detector_-> byesDetectStatistics(file_direct, i);
        byes_table_model_->setItem(i, 0, new QStandardItem(QString::number(i)));
        byes_table_model_->setItem(i, 1, new QStandardItem(QString::number(success_rate)));
    }
    ui->byesResultTable->setColumnWidth(0,40);
    ui->byesResultTable->setColumnWidth(1,60);
}

void NumDetWindow::on_reloadPJI_clicked()
{
    num_detector_->analysisPJI();
}

void NumDetWindow::on_createPCA_clicked()
{

}
