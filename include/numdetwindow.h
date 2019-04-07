#ifndef NUMDETWINDOW_H
#define NUMDETWINDOW_H

#include <QMainWindow>
#include <opencv2/highgui/highgui.hpp>
#include <QStandardItemModel>


#include "include/numberdetector.h"

namespace Ui {
class NumDetWindow;
}

class NumDetWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NumDetWindow(QWidget *parent = 0);
    ~NumDetWindow();

private slots:
    void on_returnMain_clicked();

    void on_createTemplate_clicked();

    void on_matchNum_clicked();

    void on_matchStatistics_clicked();

    void on_createBayesTemplate_clicked();

    void on_byesDetec_clicked();

    void on_byesMatchStatistics_clicked();

private:
    Ui::NumDetWindow *ui;

    NumberDetector num_detector_;
    QStringList table_header_;
    QStandardItemModel  *table_model_ = new QStandardItemModel();
    QStandardItemModel  *byes_table_model_ = new QStandardItemModel();
};

#endif // NUMDETWINDOW_H
