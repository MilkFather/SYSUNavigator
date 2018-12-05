#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <vector>
#include <QFileDialog>
#include <QMessageBox>

const QString des[] = {"", "软工的男生住在这里", "计院的女生住在这里", "计科保密信计的男生住在这里", "学生活动中心，面试常用地点",
                       "食堂，听说饭菜争议很大", "功能性场所，快递在这里", "没进去过几次的体育馆", "基本上被施工废了", "没用树荫，军训直接暴晒",
                       "虽然工学院已经被解散了，但是这个小广场是举办露天活动的绝佳去处", "所有的实验课都在这里上", "“屠猪馆”，大佬的好去处",
                       "日常上课都在这里", "行政办公中心", "标志性建筑，孙中山的塑像", "标志性建筑，是受到保护的文物", "学院楼，学院的学工在这里解决",
                       "在内环。拜围墙所赐现在得绕很大一圈"};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mapcore = mapCore::getInstance();

    places = (QStringList()
              << "--- 请选择 ---"
              << "至善园8号"
              << "至善园3号"
              << "慎思园10号"
              << "新活"
              << "一二饭"
              << "明德园6号"
              << "体育馆"
              << "假草"
              << "真草"
              << "工学院小广场"
              << "实验楼"
              << "图书馆"
              << "公教楼"
              << "行政楼"
              << "孙中山雕像"
              << "北门牌坊"
              << "计院学院楼"
              << "体育中心");
    namemap = {
      {"--- 请选择 ---", -1},
      {"至善园8号", 3118559989},
      {"至善园3号", 4021007762},
      {"慎思园10号", 3118559959},
      {"新活", 3118560020},
      {"一二饭", 3118560532},
      {"明德园6号", 5234760717},
      {"体育馆", 3118561057},
      {"假草", 6101622086},
      {"真草", 3118561054},
      {"工学院小广场", 3118561037},
      {"实验楼", 2618133102},
      {"图书馆", 2618133080},
      {"公教楼", 2618133056},
      {"行政楼", 2618133057},
      {"孙中山雕像", 5224380995},
      {"北门牌坊", 3117773132},
      {"计院学院楼", 5269705594},
      {"体育中心", 2618133127}
    };
    ui->fromComboBox->addItems(places);
    ui->toComboBox->addItems(places);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadFileBtn_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Load map", "", tr("OpenStreetMap (*.osm)"));
    if (!(filename == "")) {
      hasFile = true;
      ui->mapWidget->loadMapFromFile(filename);
      ui->getPathBtn->setEnabled((ui->fromComboBox->currentIndex() > 0) && (ui->toComboBox->currentIndex() > 0) && hasFile);
    }
}

void MainWindow::on_getPathBtn_clicked()
{
  unsigned long long s = namemap[ui->fromComboBox->currentText()];
  unsigned long long e = namemap[ui->toComboBox->currentText()];
    pair<double, vector<unsigned long long> > mini = mapcore->spfa(s, e);
    ui->mapWidget->drawPath(mini.second, e);
    if (mini.first < 1) {
      ui->distanceLabel->setText(QString(to_string(mini.first*1000).c_str()) + " m");
    } else {
      ui->distanceLabel->setText(QString(to_string(mini.first).c_str()) + " km");
    }
    repaint();
}

void MainWindow::on_fromComboBox_currentIndexChanged(int index)
{
    ui->getPathBtn->setEnabled((index > 0) && (ui->toComboBox->currentIndex() > 0) && hasFile);
}

void MainWindow::on_toComboBox_currentIndexChanged(int index)
{
    ui->getPathBtn->setEnabled((index > 0) && (ui->fromComboBox->currentIndex() > 0) && hasFile);
}

void MainWindow::on_detailsButton_clicked()
{
    QMessageBox msgBox;
    msgBox.setText(des[ui->toComboBox->currentIndex()]);
    msgBox.exec();
}
