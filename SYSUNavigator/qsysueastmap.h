#ifndef QSYSUEASTMAP_H
#define QSYSUEASTMAP_H

#include <QWidget>
#include <memory>
#include "mapcore.h"

using namespace std;

namespace Ui {
class QSysuEastMap;
}

class QSysuEastMap : public QWidget
{
    Q_OBJECT

public:
    explicit QSysuEastMap(QWidget *parent = nullptr);
    ~QSysuEastMap();

private slots:
  void paintEvent(QPaintEvent *event);

private:
    Ui::QSysuEastMap *ui;
    shared_ptr<mapCore> mapcore;

    QPainterPath getPolygon(vector<unsigned long long> nodes);

    void drawBuilding(unsigned long idx);
    void drawRoad(unsigned long idx);
    void drawRelation(unsigned long idx);
};

#endif // QSYSUEASTMAP_H
