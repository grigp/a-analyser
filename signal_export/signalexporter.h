#ifndef SIGNALEXPORTER_H
#define SIGNALEXPORTER_H

#include <QObject>

class ExportFilter;
class SignalAccess;
class SignalExportDialog;

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

    void doExportOnce(SignalExportDialog* dialog);

    void doExportBatch(SignalExportDialog* dialog);

    SignalAccess* createSignal(const QString& probeUid, const QString& channelId) const;

    QString m_probeUid {""};
    QString m_channelId {""};
    QStringList m_testUids;
    Mode m_mode {mdUndefined};

    bool m_isSeparate {false};

    SignalAccess *m_signal {nullptr};   ///< Экспортируемый сигнал в одиночном режиме

    QList<ExportFilter*> m_filters;
};

#endif // SIGNALEXPORTER_H
