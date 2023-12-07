#ifndef STABILAN01DEFINES_H
#define STABILAN01DEFINES_H

#include <QString>
#include <QSet>
#include <QCoreApplication>

namespace Stabilan01Defines {


///< -------------------------------------------------------
///< Модель стабилоанализатора
enum Model
{
      smcSt01 = 1
    , smcSt02
    , smcKSK123_20 = 20
    , smcKSK123_21
    , smcKSK123_22
    , smcKSK123_23
    , smcKSK123_30 = 30
    , smcKSK123_31
    , smcKSK123_32
    , smcKSK123_33
    , smcStabilan01_01 = 101
    , smcStabilan01_02
    , smcStabilan01_03
    , smcStabilan01_05 = 105
    , smcStabilan01_12 = 112
    , smcStabilan01_13 = 113
    , smcStabilan01_15 = 115
    , smcStabilan01_16 = 116
    , smcStabilan01_05NG = 205
    , smcStabilan01_12NG = 212
};

static const QString smnSt01 = QCoreApplication::tr("СТ-01");
static const QString smnSt02 = QCoreApplication::tr("СТ-02");
static const QString smnKSK123_20 = QCoreApplication::tr("КСК-123-2.0");
static const QString smnKSK123_21 = QCoreApplication::tr("КСК-123-2.1");
static const QString smnKSK123_22 = QCoreApplication::tr("КСК-123-2.2");
static const QString smnKSK123_23 = QCoreApplication::tr("КСК-123-2.3");
static const QString smnKSK123_30 = QCoreApplication::tr("КСК-123-3.0");
static const QString smnKSK123_31 = QCoreApplication::tr("КСК-123-3.1");
static const QString smnKSK123_32 = QCoreApplication::tr("КСК-123-3.2");
static const QString smnKSK123_33 = QCoreApplication::tr("КСК-123-3.3");
static const QString smnStabilan01_01 = QCoreApplication::tr("Стабилан-01. Исполнение 01");
static const QString smnStabilan01_02 = QCoreApplication::tr("Стабилан-01. Исполнение 02");
static const QString smnStabilan01_03 = QCoreApplication::tr("Стабилан-01. Исполнение 03");
static const QString smnStabilan01_05 = QCoreApplication::tr("Стабилан-01. Исполнение 05");
static const QString smnStabilan01_12 = QCoreApplication::tr("Стабилан-01. Исполнение 12");
static const QString smnStabilan01_13 = QCoreApplication::tr("Стабилан-01. Исполнение 13");
static const QString smnStabilan01_15 = QCoreApplication::tr("Стабилан-01. Исполнение 15");
static const QString smnStabilan01_16 = QCoreApplication::tr("Стабилан-01. Исполнение 16");
static const QString smnStabilan01_05NG = QCoreApplication::tr("Стабилан-01. Исполнение 05 (после 2014)");
static const QString smnStabilan01_12NG = QCoreApplication::tr("Стабилан-01. Исполнение 12 (после 2014)");


///< -------------------------------------------------------
///< Типы центровки

enum ZeroingType
{
      ztFast = 0
    , ztAveragePrev
    , ztAverageNext
};

static const QString ztnFast = QCoreApplication::tr("Мгновенная");
static const QString ztnAveragePrev = QCoreApplication::tr("С усреднением после команды");
static const QString ztnAverageNext = QCoreApplication::tr("С усреднением по предыстории");

//! Исполнения стабилана с пульсом
static QSet<Model> ModelsWithPulse =
{
    smcKSK123_21,
    smcKSK123_23,
    smcKSK123_31,
    smcKSK123_33,
    smcStabilan01_02,
    smcStabilan01_03,
    smcStabilan01_05,
    smcStabilan01_12,
    smcStabilan01_13,
    smcStabilan01_15,
    smcStabilan01_16,
    smcStabilan01_05NG,
    smcStabilan01_12NG,
};

//! Исполнения стабилана с тензоканалами
static QSet<Model> ModelsWithTenso =
{
    smcStabilan01_05,
    smcStabilan01_15,
    smcStabilan01_16,
    smcStabilan01_12NG,
};

//! Исполнения стабилана с миограммами
static QSet<Model> ModelsWithMyo =
{
    smcKSK123_22,
    smcKSK123_23,
    smcKSK123_32,
    smcKSK123_33,
    smcStabilan01_12,
    smcStabilan01_13,
    smcStabilan01_15,
    smcStabilan01_16,
    smcStabilan01_05NG,
};

/*!
 * \brief источник расчета координат ЦД The SKGSource enum
 */
enum SKGSource
{
      ssStabilan = 0  ///< Считать будет стабилан
    , ssSelf          ///< Считать самостоятельно
};

}

#endif // STABILAN01DEFINES_H
