#ifndef FACEDETWINDOW_H
#define FACEDETWINDOW_H

#include <boost/thread.hpp>
#include <boost/timer.hpp>

#include <QMainWindow>

#include "include/facedetector.h"
#include "include/wxx_pca.h"

namespace Ui {
class FaceDetWindow;
}

class FaceDetWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit FaceDetWindow(QWidget *parent = 0);
    ~FaceDetWindow();

private slots:
    void on_returnMain_clicked();

    void on_importDate_clicked();

    void on_openCapture_clicked();

    void on_startTrain_clicked();

    void on_test_clicked();

private:
    Ui::FaceDetWindow *ui;

    WxxPCA *pca_;

    boost::thread video_thread_;
    boost::shared_mutex video_mutex_;

    cv::VideoCapture cap_;

    void videoThread();

    QImage toQImage(cv::Mat image);
};

#endif // FACEDETWINDOW_H
