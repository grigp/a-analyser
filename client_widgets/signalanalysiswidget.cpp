#include "signalanalysiswidget.h"
#include "ui_signalanalysiswidget.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "settingsprovider.h"
#include "channelsutils.h"
#include "editcommanddelegate.h"
#include "visualsfactory.h"
#include "visualdescriptor.h"
#include "visuals.h"
#include "databaseresultwidget.h"
#include "signalexporter.h"

SignalAnalysisWidget::SignalAnalysisWidget(QWidget *parent) :
    ClientWidget(parent)
    , m_mdlTests(new QStandardItemModel(this))
    , ui(new Ui::SignalAnalysisWidget)
{
    ui->setupUi(this);

    restoreSplitterPosition();
    ui->tvTests->setModel(m_mdlTests);
    ui->tvTests->setItemDelegateForColumn(ColButtons,
                                          new EditCommandDelegate(EditCommandDelegate::CmdClose, ColButtons, ui->tvTests));
    ui->tvTests->viewport()->installEventFilter(this);
    m_mdlTests->setHorizontalHeaderLabels(QStringList() << tr("Элементы") << "");

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::dbConnected,
            this, &SignalAnalysisWidget::on_dbConnected);
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
    auto ti = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentTest();
    if (ti.uid != "")
    {
        if (!isTestOpened(ti.uid))
            openTest(ti.uid);
        else
            selectTest(ti.uid);
    }
}

void SignalAnalysisWidget::onHide()
{
}

bool SignalAnalysisWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tvTests->viewport())
    {
        if (event->type() == QEvent::MouseButtonRelease)
        {
            QMouseEvent * me = static_cast<QMouseEvent*>(event);
            if (me->button() == Qt::LeftButton && me->modifiers() == Qt::NoModifier)
            {
                QModelIndex index = ui->tvTests->indexAt(me->pos());
                if (index.column() == ColButtons)
                {
                    if (index.data(ButtonActionRole) == baClose)
                        closeTest(index);
                    else
                    if (index.data(ButtonActionRole) == baExport)
                        signalExport(index);
                }
            }
        }
    }

    return false;
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
    QVariant var = m_mdlTests->index(index.row(), ColElement, index.parent()).data(TabWidgetRole);
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

void SignalAnalysisWidget::on_dbConnected()
{
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::sectionCreated,
            this, &SignalAnalysisWidget::on_sectionCreated);
}

void SignalAnalysisWidget::on_sectionCreated(QString &channelUid, const QString number)
{
    qDebug() << Q_FUNC_INFO << "Создана секция:" << channelUid << number;

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
    QStandardItem* itemWithVisuals = nullptr;  //! Первый итем с визуалами

    auto calcVisLine = [&](QStandardItem* item,
                           int& count,
                           const QString &testUid,
                           const QString &probeUid = "",
                           const QString &channelId = "",
                           const QString &sectionNumber = "")
    {
        auto* wgt = calculateVisualsLine(count, testUid, probeUid, channelId, sectionNumber);
        //! Если итем с визуалами еще не найден и этот итем с визуалами, то запомним на него указатель
        if (!itemWithVisuals  && wgt)
            itemWithVisuals = item;
        QVariant var;
        var.setValue(wgt);
        item->setData(var, TabWidgetRole);
    };

    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        DataDefines::PatientKard kard;
        DataProvider::getPatient(ti.patientUid, kard);

        auto *itemTest = new QStandardItem(kard.fio + " - " +
                                           getMethodName(ti.metodUid) +
                                           " (" + ti.dateTime.toString("dd.MM.yyyy hh:mm") + ")");
        itemTest->setEditable(false);
        itemTest->setData(ti.uid, UidRole);
        int n = 0;
        calcVisLine(itemTest, n, ti.uid);
        itemTest->setIcon(QIcon(":/images/tree/test.png"));
        auto *itemTestClose = new QStandardItem("");
        itemTestClose->setEditable(false);
        itemTestClose->setData(ti.uid, UidRole);
        itemTestClose->setData(baClose, ButtonActionRole);
        itemTestClose->setData(EditCommandDelegate::CmdClose, EditCommandDelegate::CommandRole);
        m_mdlTests->appendRow(QList<QStandardItem*>() << itemTest << itemTestClose);

        for (int i = 0; i < ti.probes.size(); ++i)
        {
            DataDefines::ProbeInfo pi;
            if (DataProvider::getProbeInfo(ti.probes.at(i), pi))
            {
                auto *itemProbe = new QStandardItem(pi.name);
                itemProbe->setEditable(false);
                itemProbe->setData(pi.uid, UidRole);
                calcVisLine(itemProbe, n, ti.uid, pi.uid);
                itemProbe->setIcon(QIcon(":/images/tree/probe.png"));
                itemTest->appendRow(itemProbe);

                foreach (auto chi, pi.channels)
                {
                    QString chanName = ChannelsUtils::instance().channelName(chi.channelId);
                    auto *itemChan = new QStandardItem(chanName);
                    itemChan->setEditable(false);
                    itemChan->setData(chi.channelId, UidRole);
                    itemChan->setData(chi.uid, ChannelUidRole);
                    calcVisLine(itemChan, n, ti.uid, pi.uid, chi.channelId);
                    itemChan->setIcon(QIcon(":/images/tree/signal.png"));
                    auto *itemChanExport = new QStandardItem("");
                    itemChanExport->setEditable(false);
                    itemChanExport->setData(pi.uid, ProbeUidRole);
                    itemChanExport->setData(chi.channelId, ChannelUidRole);
                    itemChanExport->setData(baExport, ButtonActionRole);
                    itemChanExport->setData(EditCommandDelegate::CmdExport, EditCommandDelegate::CommandRole);
                    if (n > 0)
                    {
                        itemProbe->appendRow(QList<QStandardItem*>() << itemChan << itemChanExport);

                        QList<DataDefines::SectionInfo> sections;
                        if (DataProvider::getSections(chi.uid, sections))
                        {
                            foreach (auto section, sections)
                            {
                                auto *itemSection = new QStandardItem(section.name);
                                itemSection->setData(section.number, SectionNumberRole);
                                calcVisLine(itemSection, n, ti.uid, pi.uid, chi.channelId, section.number);
                                itemSection->setIcon(QIcon(":/images/tree/signal.png"));

                                itemChan->appendRow(QList<QStandardItem*>() << itemSection);
                            }

                            ui->tvTests->expand(itemChan->index());
                        }
                    }
                    else
                        delete itemChan;
                }
                ui->tvTests->expand(itemProbe->index());
            }
        }

        ui->tvTests->expand(itemTest->index());
    }

    if (itemWithVisuals)
    {
        ui->tvTests->selectionModel()->clear();
        ui->tvTests->selectionModel()->select(itemWithVisuals->index(), QItemSelectionModel::Select);
        selectElement(itemWithVisuals->index());
    }

    ui->tvTests->header()->resizeSection(ColElement, 320);
    ui->tvTests->header()->resizeSection(ColButtons, 50);
}

void SignalAnalysisWidget::selectTest(const QString testUid)
{
    for (int i = 0; i < m_mdlTests->rowCount(); ++i)
    {
        auto idx = m_mdlTests->index(i, ColElement);
        if (idx.data(UidRole).toString() == testUid)
        {
            auto idxWithLine = getFirstVisualLineIndex(idx);
            if (idxWithLine.isValid() && idxWithLine != QModelIndex())
            {
                ui->tvTests->selectionModel()->clear();
                ui->tvTests->selectionModel()->select(idxWithLine, QItemSelectionModel::Select);
                selectElement(idxWithLine);
            }
        }
    }
}

QModelIndex SignalAnalysisWidget::getFirstVisualLineIndex(QModelIndex &index)
{
    QVariant var = m_mdlTests->index(index.row(), ColElement, index.parent()).data(TabWidgetRole);
    if (var.isValid())
    {
        QTabWidget* wgt = var.value<QTabWidget*>();
        if (wgt)
            return index;
    }

    for (int i = 0; i < index.model()->rowCount(index); ++i)
    {
        auto idx = index.model()->index(i, ColElement, index);
        auto idxR = getFirstVisualLineIndex(idx);
        if (idxR.isValid() && idxR != QModelIndex())
            return idxR;
    }

    return QModelIndex();
}

void SignalAnalysisWidget::closeTest(QModelIndex& index)
{
    auto idxRoot = m_mdlTests->index(index.row(), ColElement, index.parent());
    auto res = QMessageBox::question(nullptr, tr("Запрос"), tr("Закрыть тест ") + idxRoot.data().toString() + " ?");
    if (res == QMessageBox::Yes)
    {
        closeVisuals(idxRoot);
        m_mdlTests->removeRow(idxRoot.row(), idxRoot.parent());
        if (m_mdlTests->rowCount() == 0)
            static_cast<AAnalyserApplication*>(QApplication::instance())->showClientPage(ClientWidgets::uidDatabaseResultWidgetUid);
    }
}

void SignalAnalysisWidget::closeVisuals(QModelIndex &index)
{
    //! Удалить линейку визуалов, если она есть
    QVariant var = index.data(TabWidgetRole);
    if (var.isValid())
    {
        QTabWidget* wgt = var.value<QTabWidget*>();
        if (wgt)
            delete wgt;
    }

    //! Пройтись по детям и вызвать рекурсивно удаление линеек визуалов у них
    int childCount = index.model()->rowCount(index);
    for (int i = 0; i < childCount; ++i)
    {
        auto idx = index.model()->index(i, ColElement, index);
        closeVisuals(idx);
    }
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

void SignalAnalysisWidget::signalExport(QModelIndex &index)
{
    auto puid = index.data(ProbeUidRole).toString();
    auto cid = index.data(ChannelUidRole).toString();
    auto exp = new SignalExporter(puid, cid);
    delete exp;
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

QTabWidget *SignalAnalysisWidget::calculateVisualsLine(int &count,
                                                       const QString &testUid,
                                                       const QString &probeUid,
                                                       const QString &channelId,
                                                       const QString &sectionNumber)
{
    QTabWidget *tw = nullptr;
    count = 0;
    BaseDefines::TestLevel level = BaseDefines::tlNone;
    if (probeUid == "" && channelId == "" && sectionNumber == "")
        level = BaseDefines::tlTest;
    else
    if (channelId == "" && sectionNumber == "")
        level = BaseDefines::tlProbe;
    else
    if (sectionNumber == "")
        level = BaseDefines::tlChannel;
    else
        level = BaseDefines::tlSection;

    auto *app = static_cast<AAnalyserApplication*>(QApplication::instance());
    for (int i = 0; i < app->visualCount(level); ++i)
    {
        auto *visual = app->getVisual(level, i);
        auto *wgt = visual->getVisualWidget(nullptr, testUid, probeUid, channelId, sectionNumber);
        if (wgt->isValid())
        {
            wgt->calculate();

            if (count == 0)
            {
                tw = new QTabWidget(ui->frVisualContainer);
                tw->clear();
            }
            tw->addTab(wgt, visual->name());

            ++count;
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


