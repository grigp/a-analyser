#include "summarieswidget.h"
#include "ui_summarieswidget.h"

#include <QStandardItemModel>
#include <QTableView>
#include <QDir>
#include <QFileDialog>
#include <QDebug>

#include "aanalyserapplication.h"
#include "summary.h"
#include "summarywidget.h"
#include "datadefines.h"
#include "metodicsfactory.h"
#include "dataprovider.h"
#include "stringinputdialog.h"
#include "opensummarydialog.h"
#include "settingsprovider.h"
#include "amessagebox.h"


SummariesWidget::SummariesWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::SummariesWidget)
{
    ui->setupUi(this);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::addTestToSummary,
            this, &SummariesWidget::addTestToSummary);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::addTestToSummaryBegin,
            this, &SummariesWidget::addTestToSummaryBegin);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::addTestToSummaryEnd,
            this, &SummariesWidget::addTestToSummaryEnd);

    restoreSplitterPosition();
    m_mdlLS = new QStandardItemModel(ui->tvSummaries);
    m_mdlLS->setHorizontalHeaderLabels(QStringList() << tr("Сводки"));
    ui->tvSummaries->setModel(m_mdlLS);
    ui->tvSummaries->header()->resizeSections(QHeaderView::ResizeToContents);

    connect(m_mdlLS, &QStandardItemModel::itemChanged, this, &SummariesWidget::on_itemChanged);

    #ifdef Q_OS_WIN32
    #elif
    ui->btnOpenInExcel->setVisible(false);
    #endif
}

SummariesWidget::~SummariesWidget()
{
    delete ui;
}

QString SummariesWidget::uid()
{
    return ClientWidgets::uidSummariesWidgetUid;
}

QString SummariesWidget::name()
{
    return tr("Сводки показателей");
}

bool SummariesWidget::isCloseProgramQuery()
{
    QObjectList children = ui->frSummaries->children();
    foreach(QObject * child, children)
        if (child->isWidgetType())
        {
            auto wgt = dynamic_cast<SummaryWidget*>(child);
            if (wgt)
            {
                if (wgt->model()->isModify())
                {
                    auto mr = AMessageBox::question(nullptr,
                                                    tr("Предупреждение"),
                                                    tr("Сводка") + " \"" + wgt->model()->name() + "\" " + tr("изменена") + ". " +
                                                    tr("Сохранить изменения?"),
                                                    AMessageBox::StandardButtons(AMessageBox::Yes | AMessageBox::No | AMessageBox::Cancel));
                    if (mr == AMessageBox::Yes)
                        saveSummary(wgt);
                    if (mr == AMessageBox::Cancel)
                        return false;
                }
            }
        }

    return true;
}

void SummariesWidget::onShow()
{
}

void SummariesWidget::onHide()
{

}

void SummariesWidget::addTestToSummaryBegin()
{
    m_cntAdded = 0;
    m_cntAll = 0;
    m_isActivePresent = true;
    m_isAnotherMethod = false;
}

void SummariesWidget::addTestToSummaryEnd()
{
    if (!m_isActivePresent && m_cntAdded == 0)
        AMessageBox::information(nullptr, tr("Сообщение"), tr("Отсутствует активная сводка"));
    else
    {

        QString sAnotherMethod = "";
        if (m_isAnotherMethod && m_cntAdded <= m_cntAll)
            sAnotherMethod = tr("Активная сводка создана для другой методики");
        if (m_cntAdded != m_cntAll)
            AMessageBox::information(nullptr,
                                     tr("Сообщение"),
                                     tr("В сводку добавлено") + " " + QString::number(m_cntAdded) + " " + tr("тестов") + " " +
                                     tr("из") + " " + QString::number(m_cntAll) + ".  " + sAnotherMethod);
    }
}

void SummariesWidget::addTestToSummary(const QString testUid,
                                       const SummaryDefines::AddTestMode mode,
                                       const QString summaryName,
                                       const SummaryDefines::Kind kind)
{
    ++m_cntAll;

    //! В новую сводку
    bool isAdded = false;
    if (mode == SummaryDefines::atmNew)
        isAdded = addTestToNewSummary(testUid, summaryName, kind);
    else
    //! В активную сводку
    if (mode == SummaryDefines::atmActive)
        isAdded = addTestToActiveSummary(testUid, kind);

    //! Показываем виджет сводок
    if (isAdded)
    {
        ++m_cntAdded;
        static_cast<AAnalyserApplication*>(QApplication::instance())->summaries();
    }
}

void SummariesWidget::on_selectIndex(QModelIndex index)
{
    hideAllWidgets();
    static_cast<AAnalyserApplication*>(QApplication::instance())->setActiveSummary("", SummaryDefines::skNone);
    QVariant var = index.data(lsWidgetRole);
    if (var.isValid())
    {
        SummaryWidget* wgt = var.value<SummaryWidget*>();
        if (wgt)
        {
            wgt->setVisible(true);
            static_cast<AAnalyserApplication*>(QApplication::instance())->setActiveSummary(wgt->model()->uidMethodic(),
                                                                                           wgt->model()->kind());
        }
    }
}

void SummariesWidget::on_openSummary()
{
    auto dlg = new OpenSummaryDialog(nullptr);
    dlg->setStyleSheet(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow()->styleSheet());
    connect(dlg, &OpenSummaryDialog::deleteSummary, this, &SummariesWidget::on_deleteSummary);
    if (dlg->exec() == QDialog::Accepted)
    {
        auto fn = dlg->summaryFileName();
        if (fn != "")
        {
            QModelIndex index = QModelIndex();
            auto wgt = isSummaryOpen(fn, index);
            if (!wgt)
            {
                //! Создаем новую сводку
                auto summary = openSummaryFirst();

                //! Открываем ее
                summary->open(fn);

                //! Создаем виджет и т.д.
                openSummarySecond(summary);
            }
            else
            {
                //! Прячем все сводки и снимаем выделение
                hideAllWidgets();
                static_cast<AAnalyserApplication*>(QApplication::instance())->setActiveSummary("", SummaryDefines::skNone);

                //! Показываем эту
                wgt->setVisible(true);
                ui->tvSummaries->selectionModel()->clear();
                if (index != QModelIndex())
                {
                    ui->tvSummaries->selectionModel()->setCurrentIndex(index, QItemSelectionModel::Select);
                    ui->tvSummaries->scrollTo(index);

                    static_cast<AAnalyserApplication*>(QApplication::instance())->setActiveSummary(wgt->model()->uidMethodic(),
                                                                                                   wgt->model()->kind());
                }
            }

        }
        else
            AMessageBox::information(nullptr, tr("Предупреждение"), tr("Сводка не выбрана"));
    }
    disconnect(dlg, &OpenSummaryDialog::deleteSummary, this, &SummariesWidget::on_deleteSummary);
    delete dlg;
}

void SummariesWidget::on_saveSummary()
{
    auto selIdx = ui->tvSummaries->selectionModel()->currentIndex();
    if (selIdx != QModelIndex())
    {
        QVariant var = selIdx.data(lsWidgetRole);
        if (var.isValid())
        {
            SummaryWidget* wgt = var.value<SummaryWidget*>();
            if (wgt)
            {
                saveSummary(wgt);
                AMessageBox::information(nullptr, tr("Информация"), tr("Сводка сохранена") + " : \"" + wgt->model()->name() + "\"");
            }
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Нет выбранной сводки"));
}

void SummariesWidget::on_closeSummary()
{
    auto selIdx = ui->tvSummaries->selectionModel()->currentIndex();
    if (selIdx != QModelIndex())
    {
        QVariant var = selIdx.data(lsWidgetRole);
        if (var.isValid())
        {
            SummaryWidget* wgt = var.value<SummaryWidget*>();
            if (wgt)
            {
                if (wgt->model()->isModify())
                {
                    auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Сохранить сводку"),
                                                    AMessageBox::StandardButtons(AMessageBox::Yes | AMessageBox::No | AMessageBox::Cancel));
                    if (mr == AMessageBox::Yes)
                        saveSummary(wgt);
                    if (mr != AMessageBox::Cancel)
                        closeSummary(wgt);
                }
                else
                    closeSummary(wgt);
            }
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Сводка не выбрана"));
}

void SummariesWidget::on_deleteTest()
{
    auto selIdx = ui->tvSummaries->selectionModel()->currentIndex();
    if (selIdx != QModelIndex())
    {
        QVariant var = selIdx.data(lsWidgetRole);
        if (var.isValid())
        {
            SummaryWidget* wgt = var.value<SummaryWidget*>();
            if (wgt)
            {
                auto selIdx = wgt->selectedIndex();
                if (selIdx != QModelIndex())
                {
                    if ((wgt->model()->kind() == SummaryDefines::skAll && selIdx.row() > Summary::RowFactors) ||
                        (wgt->model()->kind() == SummaryDefines::skPrimary && selIdx.row() > Summary::RowPrimaryFactors))
                    {
                        auto fio = wgt->model()->index(selIdx.row(), 0).data(Summary::PatientFioRole).toString();
                        auto dt =  wgt->model()->index(selIdx.row(), 0).data(Summary::TestDateTimeRole).toDateTime();

                        auto mr = AMessageBox::question(nullptr,
                                                        tr("Предупреждение"),
                                                        tr("Удалить тест из сводки") + " (" + fio + " - " +
                                                        dt.toString("dd.MM.yyyy hh:mm") + ")?");
                        if (mr == AMessageBox::Yes)
                            wgt->model()->removeTest(selIdx.row());
                    }
                }
                else
                    AMessageBox::information(nullptr, tr("Предупреждение"), tr("Тест не выбран"));
            }
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Нет выбранной сводки"));
}

void SummariesWidget::on_exportSummary()
{
    auto selIdx = ui->tvSummaries->selectionModel()->currentIndex();
    if (selIdx != QModelIndex())
    {
        QVariant var = selIdx.data(lsWidgetRole);
        if (var.isValid())
        {
            SummaryWidget* wgt = var.value<SummaryWidget*>();
            if (wgt)
            {
                QString path = DataDefines::aanalyserDocumentsPath();
                QString selFltr = "";
                auto fileName = QFileDialog::getSaveFileName(this,
                                                             tr("Файл для экспорта сводки"),
                                                             path,
                                                             SummaryDefines::emfnSummary + ";;" + SummaryDefines::emfnText,
                                                             &selFltr);

                if (selFltr == SummaryDefines::emfnSummary)
                    wgt->model()->save(fileName);
                else
                if (selFltr == SummaryDefines::emfnText)
                    wgt->model()->saveAsText(fileName);
            }
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Нет выбранной сводки"));
}

void SummariesWidget::on_importSummary()
{
    QString path = DataDefines::aanalyserDocumentsPath();
    auto fn = QFileDialog::getOpenFileName(this, tr("Файл сводки"), path, SummaryDefines::emfnSummary);
    if (fn != "")
    {
        QString fnNew = DataDefines::aanalyserSummariesPath() + QUuid::createUuid().toString() + "." + SummaryDefines::SummarySuffix;
        QFile::copy(fn, fnNew);

        //! Создаем новую сводку
        auto summary = openSummaryFirst();

        //! Открываем ее
        summary->open(fnNew);

        //! Создаем виджет и т.д.
        openSummarySecond(summary);
    }
}

void SummariesWidget::on_openInExcel()
{
#ifdef Q_OS_WIN32
    auto selIdx = ui->tvSummaries->selectionModel()->currentIndex();
    if (selIdx != QModelIndex())
    {
        QVariant var = selIdx.data(lsWidgetRole);
        if (var.isValid())
        {
            SummaryWidget* wgt = var.value<SummaryWidget*>();
            if (wgt)
                wgt->model()->openInMSExcel();
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Нет выбранной сводки"));
#endif
}

void SummariesWidget::on_deleteSummary(const QString &fileName)
{
    for (int i = 0; i < m_mdlLS->rowCount(); ++i)
    {
        QVariant var = m_mdlLS->index(i, 0).data(lsWidgetRole);
        if (var.isValid())
        {
            SummaryWidget* wgt = var.value<SummaryWidget*>();
            if (wgt)
                if (wgt->model()->fileName() == fileName)
                    closeSummary(wgt);
        }

    }
}

void SummariesWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void SummariesWidget::on_itemChanged(QStandardItem *item)
{
    QVariant var = item->data(lsWidgetRole);
    if (var.isValid())
    {
        SummaryWidget* wgt = var.value<SummaryWidget*>();
        if (wgt)
        {
            wgt->model()->setName(item->data(Qt::DisplayRole).toString());
        }
    }
}

Summary *SummariesWidget::openSummaryFirst()
{
    //! Прячем все сводки и снимаем выделение
    hideAllWidgets();
    ui->tvSummaries->selectionModel()->clear();

    //! Создаем новую сводку
    return new Summary();
}

void SummariesWidget::openSummarySecond(Summary *summary)
{
    //! Создаем виджет для показа сводки
    auto wgt = new SummaryWidget(ui->frSummaries);
    wgt->setModel(summary);
    ui->frSummaries->layout()->addWidget(wgt);

    //! Добавляем запись в окно открытых сводок
    auto item = new QStandardItem(summary->name());
    QVariant var;
    var.setValue(wgt);
    item->setData(var, lsWidgetRole);
    m_mdlLS->appendRow(item);
    static_cast<AAnalyserApplication*>(QApplication::instance())->setActiveSummary(wgt->model()->uidMethodic(),
                                                                                   wgt->model()->kind());

    //! Выделяем в нем запись
    ui->tvSummaries->selectionModel()->setCurrentIndex(item->index(), QItemSelectionModel::Select);
    ui->tvSummaries->scrollTo(item->index());
}

bool SummariesWidget::addTestToNewSummary(const QString testUid, const QString summaryName, const SummaryDefines::Kind kind)
{
    //! Создаем новую сводку
    auto summary = openSummaryFirst();

    //! Дополняем ее параметрами и тестом
    summary->setName(summaryName);
    summary->setKind(kind);
    summary->addTest(testUid);

    //! Создаем виджет и т.д.
    openSummarySecond(summary);

    return true;
}

bool SummariesWidget::addTestToActiveSummary(const QString testUid, const SummaryDefines::Kind kind)
{
    Q_UNUSED(kind);
    auto selIdx = ui->tvSummaries->selectionModel()->currentIndex();
    if (selIdx != QModelIndex())
    {
        QVariant var = selIdx.data(lsWidgetRole);
        if (var.isValid())
        {
            SummaryWidget* wgt = var.value<SummaryWidget*>();
            if (wgt)
            {
                DataDefines::TestInfo ti;
                if (DataProvider::getTestInfo(testUid, ti))
                {
                    auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(ti.metodUid);
                    if (mi.uid == wgt->model()->uidMethodic())
                    {
                        wgt->model()->addTest(testUid);
                        return true;
                    }
                    else
                        m_isAnotherMethod = true;
//                        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Активная сводка создана для другой методики"));
                }
            }
        }
    }
    else
        m_isActivePresent = false;
//        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Отсутствует активная сводка"));
    return false;
}

void SummariesWidget::hideAllWidgets()
{
    QObjectList children = ui->frSummaries->children();
    foreach(QObject * child, children)
        if (child->isWidgetType())
            static_cast<QWidget*>(child)->setVisible(false);
}

void SummariesWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("SummariesWidget", "SplitterPosition", ui->splitter->saveState());
}

void SummariesWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("SummariesWidget", "SplitterPosition").toByteArray();
    if (val == "")
        val = QByteArray::fromRawData("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x01\x1B\x00\x00\x03""b\x01\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x01\x00", 31);
    ui->splitter->restoreState(val);
}

void SummariesWidget::saveSummary(SummaryWidget *wgt)
{
    if (wgt->model()->fileName() == "")
    {
        QDir dir(DataDefines::aanalyserSummariesPath());
        if (!dir.exists())
            dir.mkpath(DataDefines::aanalyserSummariesPath());
        wgt->model()->setFileName(DataDefines::aanalyserSummariesPath() + wgt->model()->uid() + "." + SummaryDefines::SummarySuffix);
    }
    wgt->model()->save();
}

void SummariesWidget::closeSummary(SummaryWidget *wgt)
{
    auto selIdx = ui->tvSummaries->selectionModel()->currentIndex();
    if (selIdx != QModelIndex())
    {
        m_mdlLS->removeRow(selIdx.row());
    }
    wgt->deleteLater();

    if (m_mdlLS->rowCount() == 0)
        ui->lblPicture->setVisible(true);
    else
    {
        auto selIdx = ui->tvSummaries->selectionModel()->currentIndex();
        if (selIdx != QModelIndex())
            on_selectIndex(selIdx);
    }
}

SummaryWidget* SummariesWidget::isSummaryOpen(const QString &fn, QModelIndex& index)
{
    index = QModelIndex();
    for (int i = 0; i < m_mdlLS->rowCount(); ++i)
    {
        QVariant var = m_mdlLS->item(i)->data(lsWidgetRole);
        if (var.isValid())
        {
            SummaryWidget* wgt = var.value<SummaryWidget*>();
            if (wgt)
                if (wgt->model()->fileName() == fn)
                {
                    index = m_mdlLS->item(i)->index();
                    return wgt;
                }
        }
    }
    return nullptr;
}

