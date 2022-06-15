#include "summarieswidget.h"
#include "ui_summarieswidget.h"

#include <QStandardItemModel>
#include <QTableView>
#include <QMessageBox>
#include <QDir>
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


SummariesWidget::SummariesWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::SummariesWidget)
{
    ui->setupUi(this);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::addTestToSummary,
            this, &SummariesWidget::addTestToSummary);

    restoreSplitterPosition();
    m_mdlLS = new QStandardItemModel(ui->tvSummaries);
    m_mdlLS->setHorizontalHeaderLabels(QStringList() << tr("Сводки"));
    ui->tvSummaries->setModel(m_mdlLS);
    ui->tvSummaries->header()->resizeSections(QHeaderView::ResizeToContents);
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
                    auto mr = QMessageBox::question(nullptr,
                                                    tr("Предупреждение"),
                                                    tr("Сводка") + " \"" + wgt->model()->name() + "\" " + tr("изменена") + ". " +
                                                    tr("Сохранить изменения?"),
                                                    QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel));
                    if (mr == QMessageBox::Yes)
                        saveSummary(wgt);
                    return mr != QMessageBox::Cancel;
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

void SummariesWidget::addTestToSummary(const QString testUid,
                                       const SummaryDefines::AddTestMode mode,
                                       const QString summaryName,
                                       const SummaryDefines::Kind kind)
{
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
        static_cast<AAnalyserApplication*>(QApplication::instance())->summaries();
}

void SummariesWidget::on_selectIndex(QModelIndex index)
{
    hideAllWidgets();
    QVariant var = index.data(lsWidgetRole);
    if (var.isValid())
    {
        SummaryWidget* wgt = var.value<SummaryWidget*>();
        if (wgt)
            wgt->setVisible(true);
    }
}

void SummariesWidget::on_openSummary()
{
    auto dlg = new OpenSummaryDialog(nullptr);
    if (dlg->exec() == QDialog::Accepted)
    {
        auto fn = dlg->summaryFileName();
        if (fn != "")
        {
            //! Создаем новую сводку
            auto summary = openSummaryFirst();

            //! Открываем ее
            summary->open(fn);

            //! Создаем виджет и т.д.
            openSummarySecond(summary);
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Сводка не выбрана"));
    }
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
                QMessageBox::information(nullptr, tr("Информация"), tr("Сводка сохранена") + " : \"" + wgt->model()->name() + "\"");
            }
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нет выбранной сводки"));
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
                    auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Сохранить сводку"),
                                                    QMessageBox::StandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel));
                    if (mr == QMessageBox::Yes)
                        saveSummary(wgt);
                    if (mr != QMessageBox::Cancel)
                        closeSummary(wgt);
                }
                else
                    closeSummary(wgt);
            }
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Сводка не выбрана"));
}

void SummariesWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
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
                        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Активная сводка создана для другой методики"));
                }
            }
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Отсутствует активная сводка"));
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
        QDir dir(DataDefines::aanalyserDocumentsPath() + "summaries/");
        if (!dir.exists())
            dir.mkpath(DataDefines::aanalyserDocumentsPath() + "summaries/");
        wgt->model()->setFileName(DataDefines::aanalyserDocumentsPath() + "summaries/" + wgt->model()->uid() + ".json");
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

