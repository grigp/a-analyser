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


SummariesWidget::SummariesWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::SummariesWidget)
{
    ui->setupUi(this);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::addTestToSummary,
            this, &SummariesWidget::addTestToSummary);

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
                if (wgt->model()->fileName() == "")
                {
                    QDir dir(DataDefines::aanalyserDocumentsPath() + "summaries/");
                    if (!dir.exists())
                        dir.mkpath(DataDefines::aanalyserDocumentsPath() + "summaries/");
                    wgt->model()->setFileName(DataDefines::aanalyserDocumentsPath() + "summaries/" + wgt->model()->uid() + ".json");
                }
                wgt->model()->save();
                QMessageBox::information(nullptr, tr("Информация"), tr("Сводка сохранена") + " : \"" + wgt->model()->name() + "\"");
            }
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нет выбранной сводки"));
}

void SummariesWidget::on_closeSummary()
{

}

bool SummariesWidget::addTestToNewSummary(const QString testUid, const QString summaryName, const SummaryDefines::Kind kind)
{
    //! Прячем все сводки и снимаем выделение
    hideAllWidgets();
    ui->tvSummaries->selectionModel()->clear();

    //! Создаем новую сводку и виджет для ее показа
    auto summary = new Summary();
    summary->setName(summaryName);
    summary->setKind(kind);
    summary->addTest(testUid);
    auto wgt = new SummaryWidget(ui->frSummaries);
    wgt->setModel(summary);
    ui->frSummaries->layout()->addWidget(wgt);

    //! Добавляем запись в окно открытых сводок
    auto item = new QStandardItem(summaryName);
    QVariant var;
    var.setValue(wgt);
    item->setData(var, lsWidgetRole);
    m_mdlLS->appendRow(item);

    //! Выделяем в нем запись
    ui->tvSummaries->selectionModel()->setCurrentIndex(item->index(), QItemSelectionModel::Select);
    ui->tvSummaries->scrollTo(item->index());
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

