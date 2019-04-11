#ifndef NUMDETWINDOW2_H
#define NUMDETWINDOW2_H

#include <QMainWindow>

namespace Ui {
class NumDetWindow2;
}

class NumDetWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit NumDetWindow2(QWidget *parent = 0);
    ~NumDetWindow2();

private:
    Ui::NumDetWindow2 *ui;
};

#endif // NUMDETWINDOW2_H
