#include "signalexporter.h"

#include <QDir>
#include <QDebug>

#include "aanalyserapplication.h"
#include "textexportfilter.h"
#include "binaryexportfilter.h"
#include "signalexportdialog.h"
#include "metodicsfactory.h"

#include "datadefines.h"
#include "dataprovider.h"
#include "channelsutils.h"
#include "stabilogram.h"
#include "balistogram.h"
#include "myogram.h"
#include "breathsignal.h"
#include "dynamosignal.h"
#include "ritmogram.h"

SignalExporter::SignalExporter(const QString &probeUid, const QString &channelId)
{
    m_probeUid = probeUid;
    m_channelId = channelId;
    m_mode = SignalExportDefines::mdOnce;
    getFilters();
    doExport();
}

SignalExporter::SignalExporter(const QStringList &testUids)
{
    m_testUids = testUids;
    m_mode = SignalExportDefines::mdBatch;
    getFilters();
    doExport();
}

SignalExporter::~SignalExporter()
{
    foreach (auto filter, m_filters)
        delete filter;
    m_filters.clear();
}

void SignalExporter::getFilters()
{
    m_filters.clear();
    m_filters << new TextExportFilter();
    m_filters << new BinaryExportFilter();
}

void SignalExporter::doExport()
{
    auto dlg = new SignalExportDialog(nullptr);
    foreach (auto filter, m_filters)
        dlg->addFilter(filter->uid(), filter->name(), filter->suffixFilter());
    if (m_mode == SignalExportDefines::mdOnce)
    {
        m_signal = createSignal(m_probeUid, m_channelId);
        if (m_signal)
        {
            QStringList titles;
            titles << "";  //! Для режима "все отведения в один файл"
            for (int i = 0; i < m_signal->subChansCount(); ++i)
                titles << m_signal->subChanName(i);
            dlg->setFileNameSelectors(titles);
        }
    }
    else
    if (m_mode == SignalExportDefines::mdBatch)
        dlg->setFolderSelector();

    if (dlg->exec() == QDialog::Accepted)
    {
        if (m_mode == SignalExportDefines::mdOnce)
            doExportOnce(dlg);
        else
        if (m_mode == SignalExportDefines::mdBatch)
            doExportBatch(dlg);
    }
    if (m_signal)
        delete m_signal;
    delete dlg;
}

void SignalExporter::doExportOnce(SignalExportDialog* dialog)
{
    QStringList names;
    for (int i = 0; i < dialog->fileNameSelectorCount(); ++i)
        names << dialog->fileName(i);
    doExportSignal(dialog, m_signal, names);
}

void SignalExporter::doExportBatch(SignalExportDialog* dialog)
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->initProgress(QCoreApplication::tr("Экспорт сигналов нескольких тестов"),
                                                                               0, m_testUids.size(),
                                                                               QCoreApplication::tr("Инициализация"));
    int n = 0;
    foreach (auto testUid, m_testUids)
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(testUid, ti))
        {
            DataDefines::PatientKard pk;
            DataProvider::getPatient(ti.patientUid, pk);
            MetodicDefines::MetodicInfo mi =
                    static_cast<AAnalyserApplication*>(QApplication::instance())->
                    getMetodics()->metodic(ti.metodUid);

            static_cast<AAnalyserApplication*>(QApplication::instance())->
                    setProgressPosition(n + 1,
                                        QCoreApplication::tr("Тест") + " " + QString::number(n + 1) + ") " +
                                        pk.fio + " - " + mi.name + " (" + ti.dateTime.toString("dd.MM.yyyy hh:mm:ss") + ")");

            foreach (auto probeUid, ti.probes)
            {
                DataDefines::ProbeInfo pi;
                if (DataProvider::getProbeInfo(probeUid, pi))
                {
                    foreach (auto ci, pi.channels)
                    {
                        auto signal = createSignal(probeUid, ci.channelId);
                        if (signal)
                        {
                            QStringList names;
                            QDir dir(dialog->fileName(0));
                            QString baseName = dir.absoluteFilePath(QString(pk.fio + "_" + mi.name + "_" +
                                                                            ti.dateTime.toString("dd_MM_yyyy_hh_mm_ss") + "_" +
                                                                            pi.name + "_" +
                                                                            ChannelsUtils::instance().channelName(ci.channelId)));
                            if (dialog->filesMode() == SignalExportDefines::fSingle)
                                names << baseName + "." + filterSuffix(dialog->filter());
                            if (dialog->filesMode() == SignalExportDefines::fDifferent)
                                for (int i = 0; i < signal->subChansCount(); ++i)
                                    names << baseName + "_" + signal->subChanName(i) + "." + filterSuffix(dialog->filter());
                            doExportSignal(dialog, signal, names);
                            delete signal;
                        }
                    }
                }
            }

            ++n;
        }
    }
    static_cast<AAnalyserApplication*>(QApplication::instance())->doneProgress();
}

void SignalExporter::doExportSignal(SignalExportDialog *dialog, SignalAccess *signal, QStringList &names)
{
    foreach (auto filter, m_filters)
    {
        if (filter->uid() == dialog->filter())
        {
            if (dialog->filesMode() == SignalExportDefines::fSingle)
            {
                QString s = names.at(0);
                filter->doExport(signal, s);
            }
            else
            if (dialog->filesMode() == SignalExportDefines::fDifferent)
                filter->doExport(signal, names);
        }
    }
}

QString SignalExporter::filterSuffix(const QString &filterUid) const
{
    foreach (auto filter, m_filters)
        if (filter->uid() == filterUid)
            return filter->suffix();
    return "";
}

SignalAccess *SignalExporter::createSignal(const QString &probeUid, const QString &channelId) const
{
    QByteArray data;
    if (DataProvider::getChannel(probeUid, channelId, data))
    {
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctStabilogram)
            return new Stabilogram(data);
        else
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctBalistogram)
            return new Balistogram(data);
        else
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctMyogram)
            return new Myogram(data);
        else
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctDynamo)
            return new DynamoSignal(data);
        else
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctBalistogram)
            return new BreathSignal(data);
        else
        if (ChannelsUtils::instance().channelType(channelId) == ChannelsDefines::ctRitmogram)
            return new Ritmogram(data);
    }
    return nullptr;
}
