#ifndef SIGNALEXPORTER_H
#define SIGNALEXPORTER_H

#include <QObject>

class ExportFilter;

/*!
 * \brief Класс экспортера сигналов The SignalExporter class
 */
class SignalExporter
{
public:
    SignalExporter(const QString &probeUid, const QString& channelId);
    SignalExporter(const QStringList &testUids);

    ~SignalExporter();

    /*!
     * \brief Режим экспортирования The Mode enum
     */
    enum Mode
    {
          mdUndefined = 0
        , mdOnce       ///< Одиночный
        , mdBatch      ///< Пакетный
    };

private:

    void getFilters();

    void doExport();

    QString m_probeUid {""};
    QString m_channelId {""};
    QStringList m_testUids;
    Mode m_mode {mdUndefined};

    bool m_isSeparate {false};

    QList<ExportFilter*> m_filters;
};

#endif // SIGNALEXPORTER_H
