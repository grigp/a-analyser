#ifndef PULSESPECTRFACTORSDEFINES_H
#define PULSESPECTRFACTORSDEFINES_H

#include <QCoreApplication>
#include <QString>
#include <QList>
#include <QColor>

namespace PulseSpectrFactorsDefines
{

/*!
 * \brief Зона на спектре The SpectrArea struct
 */
struct SpectrArea
{
    double lo {-1};
    double hi {-1};
    QString name {""};
    QString shortName {""};
    QColor colorBackground {Qt::lightGray};
    QColor colorText {Qt::gray};

    SpectrArea(const double l, const double h, const QString n, const QString sn, const QColor cb, const QColor ct)
        : lo(l), hi(h), name(n), shortName(sn), colorBackground(cb), colorText(ct)
    {}
};

/*!
 * \brief Номера зон на спектре пульса The SAZN enum
 */
enum SAZN
{
      ULF = 0
    , VLF
    , LF
    , HF
};

/*!
 * \brief Перечень зон на спектре пульса SpectrAreases
 */
const QList<SpectrArea> SpectrAreases = QList<SpectrArea>()
        << SpectrArea(0.0001, 0.003, QCoreApplication::tr("Ультранизкая частота"), "ULF", QColor(180, 190, 190), QColor(10, 100, 100))
        << SpectrArea(0.003, 0.04, QCoreApplication::tr("Очень низкая частота"), "VLF", QColor(200, 210, 210), QColor(30, 120, 120))
        << SpectrArea(0.04, 0.15, QCoreApplication::tr("Низкая частота"), "LF", QColor(220, 230, 230), QColor(50, 140, 140))
        << SpectrArea(0.15, 0.4, QCoreApplication::tr("Высокая частота"), "HF", QColor(240, 250, 250), QColor(80, 160, 160));

}

#endif // PULSESPECTRFACTORSDEFINES_H
