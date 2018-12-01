#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <vector>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mapcore = mapCore::getInstance();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadFileBtn_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, "Load map", "", tr("OpenStreetMap (*.osm)"));
    if (!(filename == ""))
      ui->mapWidget->loadMapFromFile(filename);
}
