#include "evolventavisualize.h"
#include "ui_evolventavisualize.h"

#include "evolventacalculator.h"
#include "decartcoordinatessignal.h"

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

}
