#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <memory>
#include "mapcore.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

  void on_loadFileBtn_clicked();

private:
    Ui::MainWindow *ui;
    shared_ptr<mapCore> mapcore;
};

#endif // MAINWINDOW_H
