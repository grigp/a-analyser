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

    ui->tvTests->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tvTests, &QTreeView::customContextMenuRequested, this, &SignalAnalysisWidget::on_popupMenuRequested);

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
    auto idxElement = m_mdlTests->index(index.row(), ColElement, index.parent());
    QVariant var = idxElement.data(TabWidgetRole);
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

void SignalAnalysisWidget::on_sectionCreated(QString &channelUid, const QString &name, const QString number)
{
    QStandardItem* itemWithVisuals = nullptr;  //! Первый итем с визуалами
    for (int it = 0; it < m_mdlTests->rowCount(); ++it)
    {
        auto idxTest = m_mdlTests->index(it, 0);
        auto testUid = idxTest.data(UidRole).toString();
        for (int ip = 0; ip < m_mdlTests->itemFromIndex(idxTest)->rowCount(); ++ip)
        {
            auto idxProbe = m_mdlTests->index(ip, 0, idxTest);
            auto probeUid = idxProbe.data(UidRole).toString();
            for (int ic = 0; ic < m_mdlTests->itemFromIndex(idxProbe)->rowCount(); ++ic)
            {
                auto idxChannel = m_mdlTests->index(ic, 0, idxProbe);
                auto channelId = idxChannel.data(UidRole).toString();
                if (channelUid == idxChannel.data(ChannelUidRole).toString())
                {
                    int n = 0;
                    addSection(name, number, m_mdlTests->itemFromIndex(idxChannel), itemWithVisuals, n, testUid, probeUid, channelId);
                }
            }
        }
    }
}

void SignalAnalysisWidget::on_popupMenuRequested(QPoint pos)
{
    if (!m_menu)
    {
        m_menu = new QMenu(this);

        QAction * closeTest = new QAction(tr("Закрыть тест..."), this);
        connect(closeTest, &QAction::triggered, this, &SignalAnalysisWidget::on_closeTest);
        m_menu->addAction(closeTest);

        QAction * signalExport = new QAction(tr("Экспорт сигнала..."), this);
        connect(signalExport, &QAction::triggered, this, &SignalAnalysisWidget::on_signalExport);
        m_menu->addAction(signalExport);

        QAction * deleteSection = new QAction(tr("Удалить секцию..."), this);
        connect(deleteSection, &QAction::triggered, this, &SignalAnalysisWidget::on_deleteSection);
        m_menu->addAction(deleteSection);
    }
    m_menu->popup(ui->tvTests->mapToGlobal(pos));
}

void SignalAnalysisWidget::on_closeTest()
{
    QModelIndex root = getRootTest();
    if (root != QModelIndex())
        closeTest(root);
}

void SignalAnalysisWidget::on_signalExport()
{
    auto selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
    if (selIdxs.size() == 2)
    {
        foreach (auto idx, selIdxs)
            if (idx.column() == 1)
            {
                auto puid = idx.data(ProbeUidRole).toString();
                auto cuid = idx.data(ChannelUidRole).toString();
                if (puid != "" && cuid != "")
                    signalExport(idx);
                else
                    QMessageBox::information(nullptr, tr("Предупреждение"), tr("Экспортировать можно только сигналы"));
            }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Экспортировать можно только сигналы"));
}

void SignalAnalysisWidget::on_deleteSection()
{
    auto selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
    foreach (auto idx, selIdxs)
        if (idx.column() == 0)
        {
            if (idx.data(ChannelUidRole).toString() != "" && idx.data(SectionNumberRole).toString() != "")
                deleteSection(idx);
            else
                QMessageBox::information(nullptr, tr("Предупреждение"), tr("Необходимо выбрать секцию"));
        }
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
        bool isVisualsPresent = buildElement(itemTest, n, ti.uid);
        if (!itemWithVisuals  && isVisualsPresent)  //! Если итем с визуалами еще не найден и этот итем с визуалами, то запомним на него указатель
            itemWithVisuals = itemTest;
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
                bool isVisualsPresent = buildElement(itemProbe, n, ti.uid, pi.uid);
                if (!itemWithVisuals  && isVisualsPresent)  //! Если итем с визуалами еще не найден и этот итем с визуалами, то запомним на него указатель
                    itemWithVisuals = itemProbe;
                itemProbe->setIcon(QIcon(":/images/tree/probe.png"));
                itemTest->appendRow(itemProbe);

                foreach (auto chi, pi.channels)
                {
                    QString chanName = ChannelsUtils::instance().channelName(chi.channelId);
                    auto *itemChan = new QStandardItem(chanName);
                    itemChan->setEditable(false);
                    itemChan->setData(chi.channelId, UidRole);
                    itemChan->setData(chi.uid, ChannelUidRole);
                    bool isVisualsPresent = buildElement(itemChan, n, ti.uid, pi.uid, chi.channelId);
                    if (!itemWithVisuals  && isVisualsPresent)  //! Если итем с визуалами еще не найден и этот итем с визуалами, то запомним на него указатель
                        itemWithVisuals = itemChan;
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
                                addSection(section.name, section.number, itemChan, itemWithVisuals, n, ti.uid, pi.uid, chi.channelId);
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

void SignalAnalysisWidget::addSection(QString name,
                                      QString number,
                                      QStandardItem *itemChan,
                                      QStandardItem *itemWithVisuals,
                                      int& count,
                                      const QString &testUid,
                                      const QString &probeUid,
                                      const QString &channelId)
{
    auto *itemSection = new QStandardItem(name);
    itemSection->setData(number, SectionNumberRole);
    itemSection->setData(itemChan->data(ChannelUidRole).toString(), ChannelUidRole);
    bool isVisualsPresent = buildElement(itemSection, count, testUid, probeUid, channelId, number);
    if (!itemWithVisuals  && isVisualsPresent)  //! Если итем с визуалами еще не найден и этот итем с визуалами, то запомним на него указатель
        itemWithVisuals = itemChan;
    itemSection->setIcon(QIcon(":/images/tree/signal.png"));

    itemChan->appendRow(QList<QStandardItem*>() << itemSection);
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
    auto cuid = index.data(ChannelUidRole).toString();
    auto exp = new SignalExporter(puid, cuid);
    delete exp;
}

void SignalAnalysisWidget::deleteSection(QModelIndex &index)
{
    auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Удалить секцию") + " \"" + index.data().toString() + "\"?");
    if (mr == QMessageBox::Yes)
    {
        //! Удаление строки визуализаторов, связанных с элементом
        QVariant var = index.data(TabWidgetRole);
        if (var.isValid())
        {
            QTabWidget* wgt = var.value<QTabWidget*>();
            if (wgt)
                delete wgt;
        }
        ui->lblNoVisuals->setVisible(true);

        //! Удаление секции из БД
        DataProvider::deleteSection(index.data(ChannelUidRole).toString(), index.data(SectionNumberRole).toString());
        //! Удаление строки из модели
        m_mdlTests->removeRow(index.row(), index.parent());

        //! Показ визуализаторов следующего выбранного элемента
        auto selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
        foreach (auto idx, selIdxs)
            if (idx.column() == 0)
                selectElement(idx);
    }
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

QModelIndex SignalAnalysisWidget::getRootTest()
{
    auto selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
    foreach (auto idx, selIdxs)
    {
        if (idx.column() == 0)
        {
            QModelIndex root = idx;
            while (root.parent() != QModelIndex())
                root = root.parent();
            return root;
        }
    }
    return QModelIndex();
}

bool SignalAnalysisWidget::buildElement(QStandardItem *item,
                                       int &count,
                                       const QString &testUid,
                                       const QString &probeUid,
                                       const QString &channelId,
                                       const QString &sectionNumber)
{
    auto* wgt = calculateVisualsLine(count, testUid, probeUid, channelId, sectionNumber);
    QVariant var;
    var.setValue(wgt);
    item->setData(var, TabWidgetRole);
    return wgt;
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


