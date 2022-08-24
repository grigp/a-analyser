#ifndef SIGNALANALYSISUTILS_H
#define SIGNALANALYSISUTILS_H

#include <QObject>

class SignalAccess;

namespace SignalAnalysisUtils
{

/*!
 * \brief Создает секцию сигнала
 * \param channelUid - uid канала
 * \param channelId - идентификатор канала
 * \param name - название секции
 * \param channel - номер подканала
 * \param from - начальная точка
 * \param to - конечная точка
 * \param signal - указатель на сигнал
 * \return true, если успешно
 */
bool createSection(QString& channelUid, QString& channelId, QString &name, int channel, int from, int to, SignalAccess *signal);

}

#endif // SIGNALANALYSISUTILS_H
