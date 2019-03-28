#ifndef COUNTRICEWINDOW_H
#define COUNTRICEWINDOW_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>

namespace Ui {
class CountRiceWindow;
}

class CountRiceWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CountRiceWindow(QWidget *parent = 0);
    ~CountRiceWindow();

    void setOrigionImage(cv::Mat image);

private slots:
    void on_pushButton_clicked();

    void on_returnMain_clicked();

    void on_countRice_clicked();

    QImage toQImage(cv::Mat image);

private:
    Ui::CountRiceWindow *ui;

    cv::Mat origion_image_;

    void showOrigionImage();
};

#endif // COUNTRICEWINDOW_H
