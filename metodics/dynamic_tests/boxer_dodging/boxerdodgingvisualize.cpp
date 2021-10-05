#include "boxerdodgingvisualize.h"
#include "ui_boxerdodgingvisualize.h"

#include "boxerdodgingcalculator.h"

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

//        auto crm = BaseUtils::CirceRoundRuleModeValueIndex.value(m_calculator->circeRoundRuleMode());
//        ui->wgtDiag->setCirceRoundRuleMode(crm);
//        auto dm = BaseUtils::DirectionModeValueIndex.value(m_calculator->directionMode());
//        ui->wgtDiag->setDirection(dm);

//        for (int i = 0; i < 8; ++i)
//            ui->wgtDiag->setData(i, static_cast<int>(m_calculator->getValue(i)));

//        ui->wgtDiag->endUpdate();

//        ui->lblAverageQuality->setText(tr("Среднее качество выполнения задания") + " " +
//                                       QString::number(m_calculator->getAverageValue(), 'f', 0) + " " + tr("%"));
//        ui->lblAverageQuality->setStyleSheet("font-size: 18pt;");
    }
}
