#ifndef FACEDETWINDOW_H
#define FACEDETWINDOW_H

#include <QMainWindow>

#include "include/facedetector.h"

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

private:
    Ui::FaceDetWindow *ui;
};

#endif // FACEDETWINDOW_H
