#ifndef DROPTESTDEFINES_H
#define DROPTESTDEFINES_H

#include <QCoreApplication>
#include <QString>
#include <QMap>

namespace DropTestDefines {


enum Columns
{
      ColContact = 1
    , ColNoContact = 2
    , ColHeight = 5
    , ColPower = 6
    , ColStiffness = 7
    , ColInitSpeed = 8
    , ColRSI = 9
};


static QMap<int, QString> FactorsByColumn {
    std::pair<int, QString> (ColContact, QCoreApplication::tr("Контактная фаза, сек"))
  , std::pair<int, QString> (ColNoContact, QCoreApplication::tr("Бесконтактная фаза, сек"))
  , std::pair<int, QString> (ColHeight, QCoreApplication::tr("Высота прыжка, м"))
  , std::pair<int, QString> (ColPower, QCoreApplication::tr("Мощность"))
  , std::pair<int, QString> (ColStiffness, QCoreApplication::tr("Жесткость"))
  , std::pair<int, QString> (ColInitSpeed, QCoreApplication::tr("Начальная скорость, м/сек"))
  , std::pair<int, QString> (ColRSI, QCoreApplication::tr("Индекс реактивной силы"))
};


}

#endif // DROPTESTDEFINES_H
