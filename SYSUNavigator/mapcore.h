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
  unsigned long long id;
  double latitude, longitude;
  map<QString, QString> tags;
};

struct mapWay {
  unsigned long long id;
  vector<unsigned long long> nodes;
  map<QString, QString> tags;
};

struct mapRelation {
  unsigned long long id;
  vector<unsigned long long> nodes;
  vector<unsigned long long> ways_outer;
  vector<unsigned long long> ways_inner;
  map<QString, QString> tags;
};

// a singleton class, containing all ready-to-use map data
class mapCore {
public:
  double minlat, minlon, maxlat, maxlon;
  vector<mapNode> nodeList;
  vector<mapWay> wayList;
  vector<mapRelation> relationList;
  map<unsigned long long, unsigned long long> nodeMap;
  map<unsigned long long, unsigned long long> wayMap;
  map<unsigned long long, unsigned long long> relationMap;

  ~mapCore();
  static shared_ptr<mapCore> getInstance(void);
  void readFile(QString);
  void clear(void);
  double normalizedLat(double lat) {return (lat - minlat) / (maxlat - minlat);}
  double normalizedLon(double lon) {return (lon - minlon) / (maxlon - minlon);}

  static double getDis(double lon1, double lat1, double lon2, double lat2);
  pair<double, vector<unsigned long long> > spfa(unsigned long long s, unsigned long long e);

private:
  static shared_ptr<mapCore> m_instance;
  vector<vector<pair<unsigned long long, double> > > linkedNodes;

  mapCore();
  mapCore(mapCore &) = delete;
  mapCore& operator=(const mapCore &) = delete;

  void parseNode(QXmlStreamReader &, unsigned long long) {};
  void parseWay(QXmlStreamReader &, unsigned long long);
  void parseRelation(QXmlStreamReader &, unsigned long long);
};

#endif // MAPCORE_H
