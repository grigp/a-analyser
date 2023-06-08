#ifndef PULSESPECTRFACTORSDEFINES_H
#define PULSESPECTRFACTORSDEFINES_H

#include <QCoreApplication>
#include <QString>
#include <QList>

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

    SpectrArea(const double l, const double h, const QString n, const QString sn)
        : lo(l), hi(h), name(n), shortName(sn)
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
        << SpectrArea(0.0001, 0.003, QCoreApplication::tr("Ультранизкая частота"), "ULF")
        << SpectrArea(0.003, 0.04, QCoreApplication::tr("Очень низкая частота"), "VLF")
        << SpectrArea(0.04, 0.15, QCoreApplication::tr("Низкая частота"), "LF")
        << SpectrArea(0.15, 0.4, QCoreApplication::tr("Высокая частота"), "HF");

}

#endif // PULSESPECTRFACTORSDEFINES_H
