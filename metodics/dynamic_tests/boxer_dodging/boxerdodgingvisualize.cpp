#include "boxerdodgingvisualize.h"
#include "ui_boxerdodgingvisualize.h"

#include "boxerdodgingcalculator.h"
#include "boxerdodgingmultifactor.h"
#include "dynamicdiagram.h"

#include <QDebug>

BoxerDodgingVisualize::BoxerDodgingVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoxerDodgingVisualize)
{
    ui->setupUi(this);
}

BoxerDodgingVisualize::~BoxerDodgingVisualize()
{
    delete ui;
}

void BoxerDodgingVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new BoxerDodgingCalculator(testUid, this);
        m_calculator->calculate();

        showDiagram(ui->wgtDiagLatent,
                    tr("Латентный период, сек"),
                    BoxerDodgingFactorsDefines::LeftLatUid,
                    BoxerDodgingFactorsDefines::RightLatUid,
                    BoxerDodgingFactorsDefines::AheadLatUid,
                    BoxerDodgingFactorsDefines::BackLatUid);
        showDiagram(ui->wgtDiagTime,
                    tr("Время реакции. сек"),
                    BoxerDodgingFactorsDefines::LeftTimeUid,
                    BoxerDodgingFactorsDefines::RightTimeUid,
                    BoxerDodgingFactorsDefines::AheadTimeUid,
                    BoxerDodgingFactorsDefines::BackTimeUid);
        showDiagram(ui->wgtDiagAmpl,
                    tr("Амплитуда, мм"),
                    BoxerDodgingFactorsDefines::LeftAmplUid,
                    BoxerDodgingFactorsDefines::RightAmplUid,
                    BoxerDodgingFactorsDefines::AheadAmplUid,
                    BoxerDodgingFactorsDefines::BackAmplUid);
        showDiagram(ui->wgtDiagErrors,
                    tr("Ошибки"),
                    BoxerDodgingFactorsDefines::LeftErrorsUid,
                    BoxerDodgingFactorsDefines::RightErrorsUid,
                    BoxerDodgingFactorsDefines::AheadErrorsUid,
                    BoxerDodgingFactorsDefines::BackErrorsUid);

        showTable();
    }
}


void BoxerDodgingVisualize::showDiagram(DynamicDiagram* diag, const QString title,
                                        const QString leftFctUid, const QString rightFctUid, const QString aheadFctUid, const QString backFctUid)
{
    diag->setKind(DynamicDiagram::KindBar);
    diag->setVolume(DynamicDiagram::Volume3D);
    diag->setTitle(title);

    diag->setAxisSpaceLeft(30);

    auto itemL = new DiagItem(m_calculator->factorValue(leftFctUid), tr("Влево"));
    diag->appendItem(itemL);
    auto itemR = new DiagItem(m_calculator->factorValue(rightFctUid), tr("Вправо"));
    diag->appendItem(itemR);
    auto itemA = new DiagItem(m_calculator->factorValue(aheadFctUid), tr("Вперед"));
    diag->appendItem(itemA);
    auto itemB = new DiagItem(m_calculator->factorValue(backFctUid), tr("Назад"));
    diag->appendItem(itemB);
}

void BoxerDodgingVisualize::showTable()
{
    auto *model = new QStandardItemModel(ui->tvTable);

    model->appendRow(
                getLineItems(tr("Влево"),
                             BoxerDodgingFactorsDefines::LeftLatUid,
                             BoxerDodgingFactorsDefines::LeftTimeUid,
                             BoxerDodgingFactorsDefines::LeftAmplUid,
                             BoxerDodgingFactorsDefines::LeftErrorsUid,
                             BoxerDodgingDefines::bdsLeftDodging));
    model->appendRow(
                getLineItems(tr("Вправо"),
                             BoxerDodgingFactorsDefines::RightLatUid,
                             BoxerDodgingFactorsDefines::RightTimeUid,
                             BoxerDodgingFactorsDefines::RightAmplUid,
                             BoxerDodgingFactorsDefines::RightErrorsUid,
                             BoxerDodgingDefines::bdsRightDodging));
    model->appendRow(
                getLineItems(tr("Вперед"),
                             BoxerDodgingFactorsDefines::AheadLatUid,
                             BoxerDodgingFactorsDefines::AheadTimeUid,
                             BoxerDodgingFactorsDefines::AheadAmplUid,
                             BoxerDodgingFactorsDefines::AheadErrorsUid,
                             BoxerDodgingDefines::bdsAheadBend));
    model->appendRow(
                getLineItems(tr("Назад"),
                             BoxerDodgingFactorsDefines::BackLatUid,
                             BoxerDodgingFactorsDefines::BackTimeUid,
                             BoxerDodgingFactorsDefines::BackAmplUid,
                             BoxerDodgingFactorsDefines::BackErrorsUid,
                             BoxerDodgingDefines::bdsBackBend));
    model->appendRow(
                getLineItems(tr("Среднее / Сумма"),
                             BoxerDodgingFactorsDefines::AverageLatUid,
                             BoxerDodgingFactorsDefines::AverageTimeUid,
                             BoxerDodgingFactorsDefines::AverageAmplUid,
                             BoxerDodgingFactorsDefines::AllErrorsUid,
                             BoxerDodgingDefines::bdsBase));


    model->setHorizontalHeaderLabels(QStringList() << tr("Этап")
                                                   << tr("Лат.период")
                                                   << tr("Время реакции")
                                                   << tr("Амплитуда")
                                                   << tr("Попытки")
                                                   << tr("Ошибки"));
    ui->tvTable->setModel(model);
    ui->tvTable->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvTable->header()->resizeSection(0, 250);

}


QList<QStandardItem *> BoxerDodgingVisualize::getLineItems(const QString title,
                                                           const QString latFctUid,
                                                           const QString timeFctUid,
                                                           const QString amplFctUid,
                                                           const QString errFctUid,
                                                           const BoxerDodgingDefines::Stages stage)
{
    auto *itemName = new QStandardItem(title);
    itemName->setEditable(false);
    auto *itemLat = new QStandardItem(m_calculator->factorValueFormatted(latFctUid));
    itemLat->setEditable(false);
    auto *itemTime = new QStandardItem(m_calculator->factorValueFormatted(timeFctUid));
    itemTime->setEditable(false);
    auto *itemAmpl = new QStandardItem(m_calculator->factorValueFormatted(amplFctUid));
    itemAmpl->setEditable(false);
    auto *itemTryes = new QStandardItem(QString::number(m_calculator->count(stage)));
    itemTryes->setEditable(false);
    auto *itemErrors = new QStandardItem(m_calculator->factorValueFormatted(errFctUid));
    itemErrors->setEditable(false);

    return QList<QStandardItem*>() << itemName << itemLat << itemTime << itemAmpl << itemTryes << itemErrors;
}
