#ifndef STABILAN01DEFINES_H
#define STABILAN01DEFINES_H

#include <QString>

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

static const QString smnSt01 = "СТ-01";
static const QString smnSt02 = "СТ-02";
static const QString smnKSK123_20 = "КСК-123-2.0";
static const QString smnKSK123_21 = "КСК-123-2.1";
static const QString smnKSK123_22 = "КСК-123-2.2";
static const QString smnKSK123_23 = "КСК-123-2.3";
static const QString smnKSK123_30 = "КСК-123-3.0";
static const QString smnKSK123_31 = "КСК-123-3.1";
static const QString smnKSK123_32 = "КСК-123-3.2";
static const QString smnKSK123_33 = "КСК-123-3.3";
static const QString smnStabilan01_01 = "Стабилан-01. Исполнение 01";
static const QString smnStabilan01_02 = "Стабилан-01. Исполнение 02";
static const QString smnStabilan01_03 = "Стабилан-01. Исполнение 03";
static const QString smnStabilan01_05 = "Стабилан-01. Исполнение 05";
static const QString smnStabilan01_12 = "Стабилан-01. Исполнение 12";
static const QString smnStabilan01_13 = "Стабилан-01. Исполнение 13";
static const QString smnStabilan01_15 = "Стабилан-01. Исполнение 15";
static const QString smnStabilan01_16 = "Стабилан-01. Исполнение 16";
static const QString smnStabilan01_05NG = "Стабилан-01. Исполнение 05 (после 2014)";
static const QString smnStabilan01_12NG = "Стабилан-01. Исполнение 12 (после 2014)";


///< -------------------------------------------------------
///< Типы центровки

enum ZeroingType
{
      ztFast = 0
    , ztAveragePrev
    , ztAverageNext
};

static const QString ztnFast = "Мгновенная";
static const QString ztnAveragePrev = "С усреднением после команды";
static const QString ztnAverageNext = "С усреднением по предыстории";

}

#endif // STABILAN01DEFINES_H
