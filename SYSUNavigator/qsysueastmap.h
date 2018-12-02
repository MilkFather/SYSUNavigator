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
  void loadMapFromFile(QString &pth);
    ~QSysuEastMap();

  void drawPath(vector<unsigned long long> nodes, unsigned long long dest);

private slots:
  void paintEvent(QPaintEvent *event);

private:
    Ui::QSysuEastMap *ui;
    shared_ptr<mapCore> mapcore;

    vector<unsigned long long> highlight;

    QPainterPath getPolygon(vector<unsigned long long> nodes);

    void drawBuilding(unsigned long long idx);
    void drawRoad(unsigned long long idx);
    void drawRelation(unsigned long long idx);
};

#endif // QSYSUEASTMAP_H
