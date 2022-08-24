#include "signalanalysisutils.h"

#include <QByteArray>

#include "signalaccess.h"
#include "anysignal.h"
#include "dataprovider.h"

bool SignalAnalysisUtils::createSection(QString &channelUid, QString& channelId, QString &name, int channel, int from, int to, SignalAccess *signal)
{
    //! Создаем сигнал секции из нужного подканала
    //! В сигнале секции два подканала. Нулевой - исходный, второй потом будем преобразовывать
    auto* section = new AnySignal(signal->frequency(), 2);
    for (int i = from; i < to; ++i)
    {
        if (i >= 0 && i < signal->size())
        {
            QVector<double> value;
            value.clear();
            value << signal->value(channel, i) << signal->value(channel, i);
            section->appendValue(value);
        }
    }
    section->setChannelId(channelId);

    //! Запишем сигнал в байтовый массив
    QByteArray data;
    section->toByteArray(data);

    //! Создадим секцию
    auto res = DataProvider::createSection(channelUid, name, channel, from, to, data);

    delete section;
    return res;
}
