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

  void on_getPathBtn_clicked();

  void on_fromComboBox_currentIndexChanged(int index);

  void on_toComboBox_currentIndexChanged(int index);

  void on_detailsButton_clicked();

private:
    Ui::MainWindow *ui;
    shared_ptr<mapCore> mapcore;

    QStringList places;
    map<QString, unsigned long long> namemap;
    bool hasFile = false;
};

#endif // MAINWINDOW_H
