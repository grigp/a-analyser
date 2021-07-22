#include "evolventavisualize.h"
#include "ui_evolventavisualize.h"

#include <QStandardItemModel>
#include <QDebug>

#include "aanalyserapplication.h"
#include "evolventacalculator.h"
#include "decartcoordinatessignal.h"
#include "evolventafactors.h"

namespace
{

struct FctTblPair
{
    QString first;
    QString second;
    FctTblPair(const QString f, const QString s)
        : first(f), second(s)
    {}
};

///< Список показателей, выводимых в таблицу основных показателей
///< Соответствие показателя для фронтали показателю для сагиттали
static QList<FctTblPair> FactorsMain = {
      FctTblPair(EvolventaFactorsDefines::CommonErrorsFrontal::SummErr, EvolventaFactorsDefines::CommonErrorsSagittal::SummErr)
    , FctTblPair(EvolventaFactorsDefines::CommonErrorsFrontal::MidErr, EvolventaFactorsDefines::CommonErrorsSagittal::MidErr)
    , FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::MidErr, EvolventaFactorsDefines::FragmentExtremumSagittal::MidErr)
    , FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::MidErr, EvolventaFactorsDefines::FragmentBranchSagittal::MidErr)
    , FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::CrsCnt, EvolventaFactorsDefines::FragmentExtremumSagittal::CrsCnt)
    , FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::CrsCnt, EvolventaFactorsDefines::FragmentBranchSagittal::CrsCnt)
    , FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::ExtrCnt, EvolventaFactorsDefines::FragmentExtremumSagittal::ExtrCnt)
    , FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::ExtrCnt, EvolventaFactorsDefines::FragmentBranchSagittal::ExtrCnt)
    , FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::MidAmpl, EvolventaFactorsDefines::FragmentExtremumSagittal::MidAmpl)
    , FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::MidAmpl, EvolventaFactorsDefines::FragmentBranchSagittal::MidAmpl)
    , FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::CMidErr, EvolventaFactorsDefines::FragmentExtremumSagittal::CMidErr)
    , FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::CMidErr, EvolventaFactorsDefines::FragmentBranchSagittal::CMidErr)
    , FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::CMidAmp, EvolventaFactorsDefines::FragmentExtremumSagittal::CMidAmp)
    , FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::CMidAmp, EvolventaFactorsDefines::FragmentBranchSagittal::CMidAmp)
    , FctTblPair(EvolventaFactorsDefines::Frontal::InterCntR, EvolventaFactorsDefines::Sagittal::InterCntR)
    , FctTblPair(EvolventaFactorsDefines::Frontal::InterLenR, EvolventaFactorsDefines::Sagittal::InterLenR)
    , FctTblPair(EvolventaFactorsDefines::Frontal::SemiWavLen, EvolventaFactorsDefines::Sagittal::SemiWavLen)
    , FctTblPair(EvolventaFactorsDefines::Frontal::SemiWavAmpl, EvolventaFactorsDefines::Sagittal::SemiWavAmpl)
};

static QList<FctTblPair> FactorsKorrect = {
      FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::Percent, EvolventaFactorsDefines::CorrectionsKognitive::Percent)
    , FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::Error, EvolventaFactorsDefines::CorrectionsKognitive::Error)
    , FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::Amplitude, EvolventaFactorsDefines::CorrectionsKognitive::Amplitude)
    , FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::TimeSumm, EvolventaFactorsDefines::CorrectionsKognitive::TimeSumm)
    , FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::TimeMid, EvolventaFactorsDefines::CorrectionsKognitive::TimeMid)
    , FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::Power, EvolventaFactorsDefines::CorrectionsKognitive::Power)
};

}

EvolventaVisualize::EvolventaVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EvolventaVisualize)
{
    ui->setupUi(this);
}

EvolventaVisualize::~EvolventaVisualize()
{
    delete ui;
}

void EvolventaVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new EvolventaCalculator(testUid, this);
        m_calculator->calculate();

        showEvolventa();
        showMainFactorsTable();
        showKorrectionFactorsTable();
        showWithoutTableFactors();
    }

    setTableSpecialStyle();
}

void EvolventaVisualize::showEvolventa()
{
    auto *sigX = m_calculator->frontal();
    double max = sigX->absMaxValue();
    ui->wgtGraphX->appendSignal(sigX, tr("Фронталь"));
    ui->wgtGraphX->setDiapazone(0, -max, max);
    ui->wgtGraphX->addMarker(0, m_calculator->timeUpwinding(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgUpwinding));
    ui->wgtGraphX->addMarker(0, m_calculator->timeHold(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgHold));
    ui->wgtGraphX->addMarker(0, m_calculator->timeConvolution(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgConvolution));
    ui->wgtGraphX->setIsFillBetweenSubchans(0, true);

    auto *sigY = m_calculator->sagittal();
    max = sigY->absMaxValue();
    ui->wgtGraphY->appendSignal(sigY, tr("Сагитталь"));
    ui->wgtGraphY->setDiapazone(0, -max, max);
    ui->wgtGraphY->addMarker(0, m_calculator->timeUpwinding(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgUpwinding));
    ui->wgtGraphY->addMarker(0, m_calculator->timeHold(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgHold));
    ui->wgtGraphY->addMarker(0, m_calculator->timeConvolution(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgConvolution));
    ui->wgtGraphY->setIsFillBetweenSubchans(0, true);
}

void EvolventaVisualize::showMainFactorsTable()
{
    auto *model = new QStandardItemModel(ui->tvEvolvMainFactors);

    foreach (auto uids, FactorsMain)
    {
        auto uidFctX = uids.first;
        auto uidFctY = uids.second;

        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidFctX);
        QString fn = fi.name();
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto *itemName = new QStandardItem(fn);
        itemName->setEditable(false);
        auto *itemX = new QStandardItem(m_calculator->factorValueFormatted(uidFctX));
        itemX->setEditable(false);
        auto *itemY = new QStandardItem(m_calculator->factorValueFormatted(uidFctY));
        itemY->setEditable(false);
        model->appendRow(QList<QStandardItem*>() << itemName << itemX << itemY);

    }
    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Фронталь") << tr("Сагитталь"));
    ui->tvEvolvMainFactors->setModel(model);
    ui->tvEvolvMainFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvEvolvMainFactors->header()->resizeSection(0, 370);
}

void EvolventaVisualize::showKorrectionFactorsTable()
{
    auto *model = new QStandardItemModel(ui->tvEvolvKorrectionFactors);

    foreach (auto uids, FactorsKorrect)
    {
        auto uidFctM = uids.first;
        auto uidFctK = uids.second;

        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidFctM);
        QString fn = fi.name();
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto *itemName = new QStandardItem(fn);
        itemName->setEditable(false);
        auto *itemM = new QStandardItem(m_calculator->factorValueFormatted(uidFctM));
        itemM->setEditable(false);
        auto *itemK = new QStandardItem(m_calculator->factorValueFormatted(uidFctK));
        itemK->setEditable(false);
        model->appendRow(QList<QStandardItem*>() << itemName << itemM << itemK);

    }
    model->setHorizontalHeaderLabels(QStringList() << tr("Длительность") << tr("100-160 мс") << tr("200-280 мс"));
    ui->tvEvolvKorrectionFactors->setModel(model);
    ui->tvEvolvKorrectionFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvEvolvKorrectionFactors->header()->resizeSection(0, 200);
}

//void EvolventaVisualize::showKorrectionFactorsTable()
//{
//    auto *model = new QStandardItemModel(ui->tvEvolvKorrectionFactors);

//    QStringList header;
//    QList<QStandardItem*> itemsMC;
//    QList<QStandardItem*> itemsKC;

//    auto *itemNameM = new QStandardItem(tr("100 - 160 мс"));
//    itemNameM->setEditable(false);
//    itemsMC << itemNameM;
//    auto *itemNameK = new QStandardItem(tr("200 - 280 мс"));
//    itemNameK->setEditable(false);
//    itemsKC << itemNameK;
//    header << tr("длительность");

//    foreach (auto uids, FactorsKorrect)
//    {
//        auto uidFctM = uids.first;
//        auto uidFctK = uids.second;

//        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidFctM);
//        QString fn = fi.name();
//        if (fi.measure() != "")
//            fn = fn + ", " + fi.measure();
//        header << fn;

//        auto *itemM = new QStandardItem(QString::number(m_calculator->factorValue(uidFctM)));
//        itemM->setEditable(false);
//        auto *itemK = new QStandardItem(QString::number(m_calculator->factorValue(uidFctK)));
//        itemK->setEditable(false);

//        itemsMC << itemM;
//        itemsKC << itemK;
//    }

//    model->appendRow(itemsMC);
//    model->appendRow(itemsKC);
//    model->setHorizontalHeaderLabels(header);
//    ui->tvEvolvKorrectionFactors->setModel(model);
//    for (int i = 0; i < header.size(); ++i)
//        ui->tvEvolvMainFactors->header()->resizeSection(i, 70);
//}

void EvolventaVisualize::setTableSpecialStyle()
{
    QFile styleSheet(":/qss/evolv.qss");
    styleSheet.open(QFile::ReadOnly);
    QByteArray ba = styleSheet.readAll();
    ui->tvEvolvMainFactors->setStyleSheet(ba);
    ui->tvEvolvKorrectionFactors->setStyleSheet(ba);
    styleSheet.close();
}

void EvolventaVisualize::showWithoutTableFactors()
{
    auto valKorCount = m_calculator->factorValueFormatted(EvolventaFactorsDefines::KorrCount);
    ui->lblCorrectionsCount->setText(tr("Общее количество коррекций") + " - " + valKorCount);

    auto valOrv = m_calculator->factorValueFormatted(EvolventaFactorsDefines::DAPercent);
    ui->lblOutrunningValue->setText(tr("Опережение маркера цели") + " " + valOrv + "%");

    auto valPM = m_calculator->factorValue(EvolventaFactorsDefines::CorrectionsMotor::Power);
    auto valPK = m_calculator->factorValue(EvolventaFactorsDefines::CorrectionsKognitive::Power);
    auto valCdv = (valPK - valPM) / (valPK + valPM) * 100;
    ui->lblCorrectionDominanceValue->setText(tr("Преобладание коррекций") + " " + QString::number(valCdv, 'f', 0) + "%");
}
