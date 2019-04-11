#ifndef NUMDETWINDOW2_H
#define NUMDETWINDOW2_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>

#include "include/image_process.h"

namespace Ui {
class NumDetWindow2;
}

class NumDetWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit NumDetWindow2(QWidget *parent = 0);
    ~NumDetWindow2();

private slots:
    void on_returnMain_clicked();

    void on_cutImage_clicked();

private:
    Ui::NumDetWindow2 *ui;

    ImageProcess *image_process_;

    std::string cut_image_cache_file_;
};

#endif // NUMDETWINDOW2_H
