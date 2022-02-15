#include "signalanalysiswidget.h"
#include "ui_signalanalysiswidget.h"

#include <QApplication>
#include <QDebug>

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "settingsprovider.h"
#include "channelsutils.h"
#include "editcommanddelegate.h"
#include "visualsfactory.h"
#include "visualdescriptor.h"
#include "visuals.h"

SignalAnalysisWidget::SignalAnalysisWidget(QWidget *parent) :
    ClientWidget(parent)
    , m_mdlTests(new QStandardItemModel(this))
    , ui(new Ui::SignalAnalysisWidget)
{
    ui->setupUi(this);

    restoreSplitterPosition();
    ui->tvTests->setModel(m_mdlTests);
    ui->tvTests->setItemDelegateForColumn(ColCloseBtn,
                                          new EditCommandDelegate(EditCommandDelegate::CmdDelete, ColCloseBtn, ui->tvTests));
    m_mdlTests->setHorizontalHeaderLabels(QStringList() << tr("Элементы") << "");
}

SignalAnalysisWidget::~SignalAnalysisWidget()
{
    delete ui;
}

QString SignalAnalysisWidget::uid()
{
    return ClientWidgets::uidSignalAnalysisWidgetUid;
}

QString SignalAnalysisWidget::name()
{
    return tr("Анализ сигналов");
}

void SignalAnalysisWidget::onShow()
{
    auto ti = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedTest();
    if (ti.uid != "")
        openTest(ti.uid);
}

void SignalAnalysisWidget::onHide()
{
}

void SignalAnalysisWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void SignalAnalysisWidget::selectElement(QModelIndex index)
{
    QObjectList children = ui->frVisualContainer->children();
    foreach(QObject * child, children)
        if (child->isWidgetType())
            static_cast<QWidget*>(child)->setVisible(false);

    bool tabPresent = false;
    QVariant var = m_mdlTests->index(index.row(), 0, index.parent()).data(TabWidgetRole);
    if (var.isValid())
    {
        QTabWidget* wgt = var.value<QTabWidget*>();
        if (wgt)
        {
            wgt->setVisible(true);
            tabPresent = true;
        }
    }

    ui->lblNoVisuals->setVisible(!tabPresent);
}

void SignalAnalysisWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("SignalAnalysisWidget", "SplitterPosition", ui->splitter->saveState());
}

void SignalAnalysisWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("SignalAnalysisWidget", "SplitterPosition").toByteArray();
    if (val == "")
        val = QByteArray::fromRawData("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x01\x1B\x00\x00\x03""b\x01\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x01\x00", 31);
    ui->splitter->restoreState(val);
}

void SignalAnalysisWidget::openTest(const QString testUid)
{
    auto calcVisLine = [&](QStandardItem* item,
                           const QString &testUid, const QString &probeUid = "", const QString &channelId = "")
    {
        auto* wgt = calculateVisualsLine(testUid, probeUid, channelId);
        QVariant var;
        var.setValue(wgt);
        item->setData(var, TabWidgetRole);
    };

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        DataDefines::PatientKard kard;
        DataProvider::getPatient(ti.patientUid, kard);

        auto *itemTest = new QStandardItem(kard.fio + " " +
                                           getMethodName(ti.metodUid) +
                                           " (" + ti.dateTime.toString("dd.MM.yyyy hh:mm") + ")");
        itemTest->setEditable(false);
        itemTest->setData(ti.uid, UidRole);
        calcVisLine(itemTest, ti.uid);
        itemTest->setIcon(QIcon(":/images/tree/test.png"));
        auto *itemTestClose = new QStandardItem("");
        itemTestClose->setEditable(false);
        itemTestClose->setData(ti.uid, UidRole);
        m_mdlTests->appendRow(QList<QStandardItem*>() << itemTest << itemTestClose);

        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                auto *itemProbe = new QStandardItem(pi.name);
                itemProbe->setEditable(false);
                itemProbe->setData(pi.uid, UidRole);
                calcVisLine(itemProbe, ti.uid, pi.uid);
                itemProbe->setIcon(QIcon(":/images/tree/probe.png"));
                itemTest->appendRow(itemProbe);

                foreach (auto chi, pi.channels)
                {
                    QString chanName = ChannelsUtils::instance().channelName(chi.channelId);
                    auto *itemChan = new QStandardItem(chanName);
                    itemChan->setEditable(false);
                    itemChan->setData(chi.channelId, UidRole);
                    itemChan->setData(chi.uid, ChannelUidRole);
                    calcVisLine(itemChan, ti.uid, pi.uid, chi.channelId);
                    itemChan->setIcon(QIcon(":/images/tree/signal.png"));
                    itemProbe->appendRow(itemChan);
                }
                ui->tvTests->expand(itemProbe->index());
            }
        }

        ui->tvTests->expand(itemTest->index());
    }

//    ui->tvTests->expandAll();
    ui->tvTests->header()->resizeSection(ColElement, 320);
    ui->tvTests->header()->resizeSection(ColCloseBtn, 50);
}

void SignalAnalysisWidget::closeTest(const QString uid)
{

}

QString SignalAnalysisWidget::getMethodName(const QString &metUid)
{
    if (m_metList.size() == 0)
        m_metList = DataProvider::getListMetodics();
    foreach (auto mi, m_metList)
        if (mi.uid == metUid)
            return mi.name;
    return "";
}

QList<QString> SignalAnalysisWidget::getTests()
{
    QList<QString> retval;
    for (int i = 0; i < m_mdlTests->rowCount(); ++i)
        retval << m_mdlTests->index(i, 0).data(UidRole).toString();
    return retval;
}

bool SignalAnalysisWidget::isTestOpened(const QString &testUid)
{
    for (int i = 0; i < m_mdlTests->rowCount(); ++i)
        if (m_mdlTests->index(i, 0).data(UidRole).toString() == testUid)
            return true;
    return false;
}

QTabWidget *SignalAnalysisWidget::calculateVisualsLine(const QString &testUid, const QString &probeUid, const QString &channelId)
{
    QTabWidget *tw = nullptr;
    VisualDefines::Level level = VisualDefines::vlNone;
    if (probeUid == "" && channelId == "")
        level = VisualDefines::vlTest;
    else
    if (channelId == "")
        level = VisualDefines::vlProbe;
    else
        level = VisualDefines::vlChannel;

    int n = 0;
    auto *app = static_cast<AAnalyserApplication*>(QApplication::instance());
    for (int i = 0; i < app->visualCount(level); ++i)
    {
        auto *visual = app->getVisual(level, i);
        auto *wgt = visual->getVisualWidget(nullptr, testUid, probeUid, channelId);
        if (wgt->isValid())
        {
            wgt->calculate();

            if (n == 0)
            {
                tw = new QTabWidget(ui->frVisualContainer);
                tw->clear();
            }
            tw->addTab(wgt, visual->name());

            ++n;
        }
        else
            delete wgt;

    }

    if (tw)
    {
        ui->frVisualContainer->layout()->addWidget(tw);
        tw->setVisible(false);
    }

    return tw;
}
