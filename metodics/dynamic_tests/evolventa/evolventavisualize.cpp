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

struct MainFctTblPair
{
    QString x;
    QString y;
    MainFctTblPair(const QString ax, const QString ay)
        : x(ax), y(ay)
    {}
};
//static const QList<QString> list={"1", "2"};

static QList<MainFctTblPair> FactorsMain = {
      MainFctTblPair(EvolventaFactorsDefines::CommonErrorsFrontal::SummErr, EvolventaFactorsDefines::CommonErrorsSagittal::SummErr)
    , MainFctTblPair(EvolventaFactorsDefines::CommonErrorsFrontal::MidErr, EvolventaFactorsDefines::CommonErrorsSagittal::MidErr)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::MidErr, EvolventaFactorsDefines::FragmentExtremumSagittal::MidErr)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::MidErr, EvolventaFactorsDefines::FragmentBranchSagittal::MidErr)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::CrsCnt, EvolventaFactorsDefines::FragmentExtremumSagittal::CrsCnt)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::CrsCnt, EvolventaFactorsDefines::FragmentBranchSagittal::CrsCnt)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::ExtrCnt, EvolventaFactorsDefines::FragmentExtremumSagittal::ExtrCnt)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::ExtrCnt, EvolventaFactorsDefines::FragmentBranchSagittal::ExtrCnt)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::MidAmpl, EvolventaFactorsDefines::FragmentExtremumSagittal::MidAmpl)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::MidAmpl, EvolventaFactorsDefines::FragmentBranchSagittal::MidAmpl)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::CMidErr, EvolventaFactorsDefines::FragmentExtremumSagittal::CMidErr)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::CMidErr, EvolventaFactorsDefines::FragmentBranchSagittal::CMidErr)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::CMidAmp, EvolventaFactorsDefines::FragmentExtremumSagittal::CMidAmp)
    , MainFctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::CMidAmp, EvolventaFactorsDefines::FragmentBranchSagittal::CMidAmp)
    , MainFctTblPair(EvolventaFactorsDefines::Frontal::InterCntR, EvolventaFactorsDefines::Sagittal::InterCntR)
    , MainFctTblPair(EvolventaFactorsDefines::Frontal::InterLenR, EvolventaFactorsDefines::Sagittal::InterLenR)
    , MainFctTblPair(EvolventaFactorsDefines::Frontal::SemiWavLen, EvolventaFactorsDefines::Sagittal::SemiWavLen)
    , MainFctTblPair(EvolventaFactorsDefines::Frontal::SemiWavAmpl, EvolventaFactorsDefines::Sagittal::SemiWavAmpl)
};


///< Карта показателей, выводимых в таблицу основных показателей
///< Соответствие показателя для фронтали показателю для сагиттали
//static QMap<QString, QString> FactorsMain =
//{
//      std::pair<QString, QString> (EvolventaFactorsDefines::CommonErrorsFrontal::SummErr, EvolventaFactorsDefines::CommonErrorsSagittal::SummErr)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::CommonErrorsFrontal::MidErr, EvolventaFactorsDefines::CommonErrorsSagittal::MidErr)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentExtremumFrontal::MidErr, EvolventaFactorsDefines::FragmentExtremumSagittal::MidErr)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentBranchFrontal::MidErr, EvolventaFactorsDefines::FragmentBranchSagittal::MidErr)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentExtremumFrontal::CrsCnt, EvolventaFactorsDefines::FragmentExtremumSagittal::CrsCnt)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentBranchFrontal::CrsCnt, EvolventaFactorsDefines::FragmentBranchSagittal::CrsCnt)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentExtremumFrontal::ExtrCnt, EvolventaFactorsDefines::FragmentExtremumSagittal::ExtrCnt)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentBranchFrontal::ExtrCnt, EvolventaFactorsDefines::FragmentBranchSagittal::ExtrCnt)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentExtremumFrontal::MidAmpl, EvolventaFactorsDefines::FragmentExtremumSagittal::MidAmpl)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentBranchFrontal::MidAmpl, EvolventaFactorsDefines::FragmentBranchSagittal::MidAmpl)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentExtremumFrontal::CMidErr, EvolventaFactorsDefines::FragmentExtremumSagittal::CMidErr)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentBranchFrontal::CMidErr, EvolventaFactorsDefines::FragmentBranchSagittal::CMidErr)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentExtremumFrontal::CMidAmp, EvolventaFactorsDefines::FragmentExtremumSagittal::CMidAmp)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::FragmentBranchFrontal::CMidAmp, EvolventaFactorsDefines::FragmentBranchSagittal::CMidAmp)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::Frontal::InterCntR, EvolventaFactorsDefines::Sagittal::InterCntR)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::Frontal::InterLenR, EvolventaFactorsDefines::Sagittal::InterLenR)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::Frontal::SemiWavLen, EvolventaFactorsDefines::Sagittal::SemiWavLen)
//    , std::pair<QString, QString> (EvolventaFactorsDefines::Frontal::SemiWavAmpl, EvolventaFactorsDefines::Sagittal::SemiWavAmpl)
//};

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
    }
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
    auto *model = new QStandardItemModel(ui->tvMainFactors);

    foreach (auto uids, FactorsMain)
    {
        auto uidFctX = uids.x;
        auto uidFctY = uids.y;

        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidFctX);
        QString fn = fi.name();
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto *itemName = new QStandardItem(fn);
        itemName->setEditable(false);
        auto *itemX = new QStandardItem(QString::number(m_calculator->factorValue(uidFctX)));
        itemX->setEditable(false);
        auto *itemY = new QStandardItem(QString::number(m_calculator->factorValue(uidFctY)));
        itemY->setEditable(false);
        model->appendRow(QList<QStandardItem*>() << itemName << itemX << itemY);

    }
    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Фронталь") << tr("Сагитталь"));
    ui->tvMainFactors->setModel(model);
    ui->tvMainFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvMainFactors->header()->resizeSection(0, 450);
}

void EvolventaVisualize::showKorrectionFactorsTable()
{

}
