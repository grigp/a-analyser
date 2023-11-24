#ifndef STABTESTPARAMS_H
#define STABTESTPARAMS_H

#include <QString>
#include <QList>
#include <QJsonArray>
#include <QJsonObject>
#include <QColor>

#include "basedefines.h"

namespace StabTestParams
{

/*!
 * \brief Типы проб
 */
enum ProbeKinds
{
      pkUndefined = 0      ///< Не задана
    , pkBackground         ///< Фоновая
    , pkCloseEyes          ///< Закрытые глаза
    , pkTarget             ///< Мишень
    , pkFiveZones          ///< Четыре зоны
    , pkRugStand           ///< Стойка на коврике
    , pkMandibular         ///< Мандибулярная (сжатые челюсти)
    , pkHeadToRight        ///< Голова повернута направо
    , pkHeadToLeft         ///< Голова повернута налево
    , pkHeadTiltRight      ///< Голова наклонена вправо
    , pkHeadTiltLeft       ///< Голова наклонена влево
    , pkHeadTiltForward    ///< Голова наклонена вперед
    , pkHeadTiltBackward   ///< Голова наклонена назад
    , pkLinesToRight       ///< Движение полос вправо
    , pkLinesToLeft        ///< Движение полос влево
    , pkLinesUp            ///< Движение полос вверх
    , pkLinesDown          ///< Движение полос вниз
//    , pkDynamic            ///< Динамическая
    , pkLastValue          ///< Последнее значение
};

/*!
 * \brief Параметры оптокинетической стимуляции The StimulLinesParams struct
 */
struct StimulLinesParams
{
    BaseDefines::Directions direction;
    int width;
    int speed;
    int dutyCycle;
    QColor color;
    StimulLinesParams()
    {
        direction = BaseDefines::dirRight;
        width = 120;
        speed = 200;
        dutyCycle = 1;
        color = Qt::black;
    }
};

/*!
 * \brief Параметры пробы в тесте ProbeParams struct
 */
struct ProbeParams
{
    QString name;          ///< Название пробы
    int probeKind;         ///< Назначение пробы (код назначения из константного списка)
    bool autoEnd;          ///< Автоматическое завершение
    int time;              ///< Длительность
    int latentTime;        ///< Задержка привыкания
    int stimulCode;        ///< Код видеостимуляции 0 - нет, 1 - цветные круги, 2 - звуковые сигналы, 3 - мишень
    bool zeroingEnabled;   ///< Разрешена ли центровка
    int scale;             ///< Номер масштаба 0 - 1, 1 - 2, 2 - 4, 3 - 8, 4 - 16, 5 - 32, 6 - 64, 7 -128

    QJsonObject stimParams {QJsonObject()};      ///< Параметры окна пациента, если они есть
    StimulLinesParams stimLines; ///< Параметры оптокинетической стимуляции

    ProbeParams()
    {
        name = "";
        probeKind = 0;
        autoEnd = true;
        time = 0;
        latentTime = 0;
        stimulCode = 0;
        zeroingEnabled = false;
        scale = 0;
    }
};

/*!
 * \brief Параметры теста на момент проведения StabTestParams struct
 */
struct StabTestParams
{
    int condition;                 ///< Условия проведения теста
    QList<ProbeParams> probes;     ///< Пробы с параметрами
    StabTestParams() {}
    StabTestParams(const QJsonObject params);
};

}

#endif // STABTESTPARAMS_H
