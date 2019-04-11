#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_toCountRice_clicked();

    void on_toNumDet_clicked();

    void on_toPCAFaceDet_clicked();

    void on_toNumDet_2_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
