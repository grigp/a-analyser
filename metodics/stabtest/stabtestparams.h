#ifndef STABTESTPARAMS_H
#define STABTESTPARAMS_H

#include <QString>

namespace StabTestParams
{

/*!
 * \brief Параметры пробы в тесте ProbeParams struct
 */
struct ProbeParams
{
    QString name;          ///< Название пробы
    bool autoEnd;          ///< Автоматическое завершение
    int time;              ///< Длительность
    int latentTime;        ///< Задержка привыкания
    int stimulCode;        ///< Код видеостимуляции 0 - нет, 1 - цветные круги, 2 - звуковые сигналы, 3 - мишень
    bool zeroingEnabled;   ///< Разрешена ли центровка
    int scale;             ///< Номер масштаба 0 - 1, 1 - 2, 2 - 4, 3 - 8, 4 - 16, 5 - 32, 6 - 64, 7 -128
    ProbeParams()
    {
        name = "";
        autoEnd = true;
        time = 0;
        latentTime = 0;
        stimulCode = 0;
        zeroingEnabled = false;
        scale = 0;
    }
};

}

#endif // STABTESTPARAMS_H
