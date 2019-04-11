#include "numdetwindow2.h"
#include "ui_numdetwindow2.h"

NumDetWindow2::NumDetWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NumDetWindow2)
{
    ui->setupUi(this);
}

NumDetWindow2::~NumDetWindow2()
{
    delete ui;
}
