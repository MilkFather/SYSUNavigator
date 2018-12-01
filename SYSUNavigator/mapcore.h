#ifndef MAPCORE_H
#define MAPCORE_H

#include <memory>
#include <QString>
#include <QPointF>
#include <map>
#include <vector>
#include <QXmlStreamReader>

using namespace std;

// node struct
struct mapNode {
  unsigned long id;
  double latitude, longitude;
  map<QString, QString> tags;
};

struct mapWay {
  unsigned long id;
  vector<unsigned long> nodes;
  map<QString, QString> tags;
};

struct mapRelation {
  unsigned long id;
  vector<unsigned long> nodes;
  vector<unsigned long> ways_outer;
  vector<unsigned long> ways_inner;
  map<QString, QString> tags;
};

// a singleton class, containing all ready-to-use map data
class mapCore {
public:
  double minlat, minlon, maxlat, maxlon;
  vector<mapNode> nodeList;
  vector<mapWay> wayList;
  vector<mapRelation> relationList;
  map<unsigned long, unsigned long> nodeMap;
  map<unsigned long, unsigned long> wayMap;
  map<unsigned long, unsigned long> relationMap;

  ~mapCore();
  static shared_ptr<mapCore> getInstance(void);
  void readFile(QString);
  void clear(void);
  double normalizedLat(double lat) {return (lat - minlat) / (maxlat - minlat);}
  double normalizedLon(double lon) {return (lon - minlon) / (maxlon - minlon);}

private:
  static shared_ptr<mapCore> m_instance;

  mapCore();
  mapCore(mapCore &) = delete;
  mapCore& operator=(const mapCore &) = delete;

  void parseNode(QXmlStreamReader &, unsigned long) {};
  void parseWay(QXmlStreamReader &, unsigned long);
  void parseRelation(QXmlStreamReader &, unsigned long);
};

#endif // MAPCORE_H
