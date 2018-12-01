#include "mapcore.h"
#include <QFile>
#include <QDebug>

shared_ptr<mapCore> mapCore::m_instance = shared_ptr<mapCore>(nullptr);

shared_ptr<mapCore> mapCore::getInstance(void) {
  if (mapCore::m_instance == nullptr) {
    mapCore::m_instance = shared_ptr<mapCore>(new mapCore());
  }
  return mapCore::m_instance;
}

mapCore::mapCore() {

}

mapCore::~mapCore() {

}

void mapCore::clear(void) {
  nodeList.clear();
  wayList.clear();
  nodeMap.clear();
  wayMap.clear();
}

void mapCore::readFile(QString filename) {
  clear();
  QFile f(filename);
  if (f.open(QIODevice::ReadOnly | QIODevice::Text)) {
    QXmlStreamReader reader(&f);
    while (!f.atEnd()) {
      // 读取下一个元素
      QXmlStreamReader::TokenType nType = reader.readNext();
      switch (nType) {
        case QXmlStreamReader::StartElement: {  // 开始元素
          QString strElementName = reader.name().toString();
          if (QString::compare(strElementName, "node") == 0) {  // 根元素
            assert(reader.attributes().hasAttribute("id"));
            assert(reader.attributes().hasAttribute("lat"));
            assert(reader.attributes().hasAttribute("lon"));
            // add new node to vector
            mapNode n;
            n.id = reader.attributes().value("id").toULongLong();
            n.latitude = reader.attributes().value("lat").toDouble();
            n.longitude = reader.attributes().value("lon").toDouble();
            nodeList.push_back(n);
            nodeMap[n.id] = nodeList.size() - 1;
          } else if (QString::compare(strElementName, "way") == 0) {
            assert(reader.attributes().hasAttribute("id"));
            parseWay(reader, reader.attributes().value("id").toULongLong());
          } else if (QString::compare(strElementName, "bounds") == 0) {
            minlat = reader.attributes().value("minlat").toDouble();
            minlon = reader.attributes().value("minlon").toDouble();
            maxlat = reader.attributes().value("maxlat").toDouble();
            maxlon = reader.attributes().value("maxlon").toDouble();
          } else if (QString::compare(strElementName, "relation") == 0) {
            assert(reader.attributes().hasAttribute("id"));
            parseRelation(reader, reader.attributes().value("id").toULongLong());
          }
          }
          break;

        default:
          break;
      }
      if (reader.hasError()) {  // 解析出错
        qDebug() << QString::fromLocal8Bit("错误信息：%1  行号：%2  列号：%3  字符位移：%4").arg(reader.errorString()).arg(reader.lineNumber()).arg(reader.columnNumber()).arg(reader.characterOffset());
      }
    }
    qDebug() << QString("读取了%1个节点和%2个路径").arg(nodeList.size()).arg(wayList.size());
    qDebug() << QString("nodeMap: %1, wayMap: %2").arg(nodeMap.size()).arg(wayMap.size());
    f.close();  // 关闭文件
  }
}

void mapCore::parseWay(QXmlStreamReader &reader, unsigned long long id) {
  mapWay w;
  w.id = id;
  while (!reader.atEnd()) {
    reader.readNext();
    if (reader.isStartElement()) {  // 开始元素
      if (QString::compare(reader.name().toString(), "nd") == 0) {
        // add node to
        unsigned long long i = reader.attributes().value("ref").toULongLong();
        w.nodes.push_back(i);
      } else if (QString::compare(reader.name().toString(), "tag") == 0) {
        w.tags[reader.attributes().value("k").toString()] = reader.attributes().value("v").toString();
      }
    } else if (reader.isEndElement()) {  // 结束元素
      QString strElementName = reader.name().toString();
      if (QString::compare(strElementName, "way") == 0) {
        wayList.push_back(w);
        wayMap[w.id] = wayList.size() - 1;
        break;  // 跳出循环（解析 QXmlStreamReader::EndDocument）
      }
    }
  }
}

void mapCore::parseRelation(QXmlStreamReader &reader, unsigned long long id) {
  mapRelation r;
  r.id = id;
  while (!reader.atEnd()) {
    reader.readNext();
    if (reader.isStartElement()) {
      if (QString::compare(reader.name().toString(), "member") == 0) {
        // is this a node?
        if (QString::compare(reader.attributes().value("type").toString(), "node") == 0) {
          // this is a node
          unsigned long long i = reader.attributes().value("ref").toULongLong();
          r.nodes.push_back(i);
        } else if (QString::compare(reader.attributes().value("type").toString(), "way") == 0) {
          if (QString::compare(reader.attributes().value("role").toString(), "outer") == 0) {
            unsigned long long i = reader.attributes().value("ref").toULongLong();
            r.ways_outer.push_back(i);
          } else if (QString::compare(reader.attributes().value("role").toString(), "inner") == 0) {
            unsigned long long i = reader.attributes().value("ref").toULongLong();
            r.ways_inner.push_back(i);
          }
        }
      } else if (QString::compare(reader.name().toString(), "tag") == 0) {
        r.tags[reader.attributes().value("k").toString()] = reader.attributes().value("v").toString();
      }
    } else if (reader.isEndElement()) {  // 结束元素
      QString strElementName = reader.name().toString();
      if (QString::compare(strElementName, "relation") == 0) {
        relationList.push_back(r);
        relationMap[r.id] = relationList.size() - 1;
        break;  // 跳出循环（解析 QXmlStreamReader::EndDocument）
      }
    }
  }
}
