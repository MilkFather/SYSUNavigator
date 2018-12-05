#include "qsysueastmap.h"
#include <QPainter>
#include <QDebug>
#include <vector>

QSysuEastMap::QSysuEastMap(QWidget *parent) :
    QWidget(parent)
{
    mapcore = mapCore::getInstance();
}

void QSysuEastMap::loadMapFromFile(QString &pth) {
  mapcore -> readFile(pth);
  this->repaint();
}

void QSysuEastMap::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);

  // buildings first
  for (size_t i = 0; i < mapcore->wayList.size(); i++) {
    if (mapcore->wayList[i].tags.count("highway") <= 0) {
      drawBuilding(i);
    }
  }
  // relations next
  for (size_t i = 0; i < mapcore->relationList.size(); i++) {
    if (mapcore->relationList[i].tags.count("waterway") > 0 ||
        mapcore->relationList[i].tags.count("leisure") > 0 ||
        mapcore->relationList[i].tags.count("building") > 0 ||
        mapcore->relationList[i].tags.count("landuse") > 0 ||
        mapcore->relationList[i].tags.count("natural") > 0) {
      drawRelation(i);
    }
  }

  for (size_t i = 0; i < mapcore->wayList.size(); i++) {
    if (mapcore->wayList[i].tags.count("highway") > 0) {
      drawRoad(i);
    }
  }
  // redraw some roads to layer them
  for (size_t i = 0; i < mapcore->wayList.size(); i++) {
    if (mapcore->wayList[i].tags.count("highway") > 0 && mapcore->wayList[i].tags["highway"] == "primary") {
      drawRoad(i);
    }
  }
  for (size_t i = 0; i < mapcore->wayList.size(); i++) {
    if (mapcore->wayList[i].tags.count("highway") > 0 && (mapcore->wayList[i].tags["highway"] == "pedestrian" || mapcore->wayList[i].tags["highway"] == "footway")) {
      drawRoad(i);
    }
  }

  if (highlight.size() > 0) {
    QPainter painter(this);
    QPen pen;
    painter.setRenderHint(QPainter::Antialiasing, true);
    pen.setCapStyle(Qt::PenCapStyle::RoundCap);
    pen.setColor(QColor(0, 0, 180));
    pen.setWidth(6);
    painter.setPen(pen);
    for (size_t i = 0; i < highlight.size() - 1; i++) {
      mapNode node1 = mapcore->nodeList[mapcore->nodeMap[highlight[i]]];
      mapNode node2 = mapcore->nodeList[mapcore->nodeMap[highlight[i + 1]]];
      double x1 = width() * mapcore->normalizedLon(node1.longitude);
      double y1 = height() - height() * mapcore->normalizedLat(node1.latitude);
      double x2 = width() * mapcore->normalizedLon(node2.longitude);
      double y2 = height() - height() * mapcore->normalizedLat(node2.latitude);
      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }
    //highlight.clear();
  }
}

QPainterPath QSysuEastMap::getPolygon(vector<unsigned long long> nodes) {
  QPainterPath pth;
  mapNode node1 = mapcore->nodeList[mapcore->nodeMap[nodes[0]]];
  double x1 = width() * mapcore->normalizedLon(node1.longitude);
  double y1 = height() - height() * mapcore->normalizedLat(node1.latitude);
  pth.moveTo(x1, y1);

  for (size_t j = 1; j < nodes.size(); j++) {
    if (mapcore->nodeMap.count(nodes[j]) && mapcore->nodeMap.count(nodes[j + 1])) {
      mapNode node1 = mapcore->nodeList[mapcore->nodeMap[nodes[j]]];
      double x1 = width() * mapcore->normalizedLon(node1.longitude);
      double y1 = height() - height() * mapcore->normalizedLat(node1.latitude);

      pth.lineTo(x1, y1);
    }
  }
  return pth;
}

void QSysuEastMap::drawBuilding(unsigned long long idx) {
  QPainter painter(this);
  QBrush brush;
  painter.setRenderHint(QPainter::Antialiasing, true);

  if (mapcore->wayList[idx].tags.count("natural") > 0) {
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    // natural
    QString tp = mapcore->wayList[idx].tags["natural"];
    if (tp == "wood") {
      brush.setColor(QColor(171, 210, 156));
    } else if (tp == "water") {
      brush.setColor(QColor(180, 208, 208));
    }
  } else if (mapcore->wayList[idx].tags.count("landuse") > 0) {
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    QString tp = mapcore->wayList[idx].tags["landuse"];
    if (tp == "grass") {
      brush.setColor(QColor(207, 237, 164));
    } else if (tp == "residential") {
      brush.setColor(QColor(225, 225, 225));
    } else if (tp == "construction") {
      brush.setColor(QColor(199, 199, 180));
    }
  } else if (mapcore->wayList[idx].tags.count("building") > 0) {
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    QString tp = mapcore->wayList[idx].tags["building"];
    brush.setColor(QColor(218, 208, 201));
  } else if (mapcore->wayList[idx].tags.count("amenity") > 0) {

  } else if (mapcore->wayList[idx].tags.count("leisure") > 0) {
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    QString tp = mapcore->wayList[idx].tags["leisure"];
    if (tp == "park") {
      brush.setColor(QColor(205, 247, 201));
    } else if (tp == "pitch" || tp == "track") {
      brush.setColor(QColor(170, 224, 203));
    } else if (tp == "swimming_pool") {
      brush.setColor(QColor(170, 211, 222));
    } else if (tp == "stadium" || tp == "sports_centre") {
      brush.setColor(QColor(240, 240, 216));
    }
  }

  painter.fillPath(getPolygon(mapcore->wayList[idx].nodes), brush);
}

void QSysuEastMap::drawRoad(unsigned long long idx) {
  QPainter painter(this);
  QPen pen;
  painter.setRenderHint(QPainter::Antialiasing, true);
  QString tp = mapcore->wayList[idx].tags["highway"];
  pen.setCapStyle(Qt::PenCapStyle::RoundCap);
  if (tp == "primary") {
    pen.setColor(QColor(253, 214, 161));
    pen.setWidth(10);
  } else if (tp == "secondary" || tp == "secondary_link") {
    pen.setColor(QColor(246, 250, 186));
    pen.setWidth(9);
  } else if (tp == "tertiary") {
    pen.setColor(QColor(254, 254, 254));
    pen.setWidth(8);
  } else if (tp == "unclassified" || tp == "residential") {
    pen.setColor(QColor(254, 254, 254));
    pen.setWidth(4);
  } else if (tp == "service") {
    pen.setColor(QColor(254, 254, 254));
    pen.setWidth(2);
  } else if (tp == "pedestrian") {
    pen.setColor(QColor(221, 221, 232));
    pen.setWidth(4);
  } else if (tp == "track") {
    pen.setColor(QColor(172, 131, 39));
    pen.setWidth(1);
    pen.setStyle(Qt::PenStyle::DashDotLine);
  } else if (tp == "footway") {
    pen.setColor(QColor(249, 139, 125));
    pen.setWidth(1);
    pen.setStyle(Qt::PenStyle::DotLine);
  } else if (tp == "steps") {
    pen.setColor(QColor(249, 139, 125));
    pen.setWidth(3);
    pen.setStyle(Qt::PenStyle::CustomDashLine);
    pen.setCapStyle(Qt::PenCapStyle::FlatCap);
    QVector<qreal> dashes;
    dashes << 0.5 << 0.125;
    pen.setDashPattern(dashes);
  } else if (tp == "cycleway") {
    pen.setColor(QColor(111, 111, 249));
    pen.setWidth(1);
    pen.setStyle(Qt::PenStyle::DotLine);
  }

  painter.setPen(pen);
  vector<unsigned long long> ns = mapcore->wayList[idx].nodes;
  for (size_t j = 0; j < ns.size() - 1; j++) {
    if (mapcore->nodeMap.count(ns[j]) && mapcore->nodeMap.count(ns[j + 1])) {
      mapNode node1 = mapcore->nodeList[mapcore->nodeMap[ns[j]]];
      mapNode node2 = mapcore->nodeList[mapcore->nodeMap[ns[j + 1]]];
      double x1 = width() * mapcore->normalizedLon(node1.longitude);
      double y1 = height() - height() * mapcore->normalizedLat(node1.latitude);
      double x2 = width() * mapcore->normalizedLon(node2.longitude);
      double y2 = height() - height() * mapcore->normalizedLat(node2.latitude);

      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
    }
  }
}

void QSysuEastMap::drawRelation(unsigned long long idx) {
  QPainter painter(this);
  QBrush brush;
  painter.setRenderHint(QPainter::Antialiasing, true);

  if (mapcore->relationList[idx].tags.count("waterway") > 0) {
    // water
    //brush.setStyle(Qt::BrushStyle::SolidPattern);
    //brush.setColor(QColor(180, 208, 208));
  } else if (mapcore->relationList[idx].tags.count("leisure") > 0) {
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    QString tp = mapcore->relationList[idx].tags["leisure"];
    if (tp == "track") {
      brush.setColor(QColor(170, 224, 203));
    }
  } else if (mapcore->relationList[idx].tags.count("building") > 0) {
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    QString tp = mapcore->relationList[idx].tags["building"];
    // university, yes, commercial
    brush.setColor(QColor(218, 208, 201));
  } else if (mapcore->relationList[idx].tags.count("landuse") > 0) {
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    QString tp = mapcore->relationList[idx].tags["landuse"];
    // resdential
    brush.setColor(QColor(225, 225, 225));
  } else if (mapcore->relationList[idx].tags.count("natural") > 0) {
    brush.setStyle(Qt::BrushStyle::SolidPattern);
    QString tp = mapcore->relationList[idx].tags["natural"];
    // water
    brush.setColor(QColor(180, 208, 208));
  }

  QPainterPath polygon;
  for (size_t i = 0; i < mapcore->relationList[idx].ways_outer.size(); i++) {
    QPainterPath poly = getPolygon(mapcore->wayList[mapcore->wayMap[mapcore->relationList[idx].ways_outer[i]]].nodes);
    polygon.addPath(poly);
  }
  for (size_t i = 0; i < mapcore->relationList[idx].ways_inner.size(); i++) {
    QPainterPath poly = getPolygon(mapcore->wayList[mapcore->wayMap[mapcore->relationList[idx].ways_inner[i]]].nodes);
    polygon = polygon.subtracted(poly);
  }
  painter.fillPath(polygon, brush);
}

void QSysuEastMap::drawPath(vector<unsigned long long> nodes, unsigned long long dest) {
  highlight = nodes;
  highlight.push_back(dest);
  repaint();
}

QSysuEastMap::~QSysuEastMap()
{
    //delete ui;
}
