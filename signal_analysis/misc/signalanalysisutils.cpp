#include "signalanalysisutils.h"

#include <QByteArray>

#include "signalaccess.h"
#include "anysignal.h"
#include "dataprovider.h"

bool SignalAnalysisUtils::createSection(QString &channelUid, QString& channelId, QString &name, int channel, int from, int to, SignalAccess *signal)
{
    //! Создаем данные для секции
    double mid = 0;
    QVector<double> sectionValues;
    for (int i = from; i < to; ++i)
        if (i >= 0 && i < signal->size())
        {
            sectionValues << signal->value(channel, i);
            mid += signal->value(channel, i);
        }
    //! И центруем их
    mid /= sectionValues.size();
    for (int i = 0; i < sectionValues.size(); ++i)
        sectionValues[i] -= mid;

    //! Создаем сигнал секции из нужного подканала
    //! В сигнале секции два подканала. Нулевой - исходный, второй потом будем преобразовывать
    auto* section = new AnySignal(signal->frequency(), 2);
    foreach (auto val, sectionValues)
    {
        QVector<double> value;
        value.clear();
        value << val << val;
        section->appendValue(value);
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
