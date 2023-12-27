#include "mancoordppvisualwidget.h"
#include "ui_mancoordppvisualwidget.h"

#include <QJsonArray>
#include <QSpacerItem>
#include <QDebug>

#include "aanalyserapplication.h"
#include "coordfactorwidget.h"
#include "generalcoordfactors.h"


ManCoordPPVisualWidget::ManCoordPPVisualWidget(PPVisualDescriptor* visual, QJsonObject &objPP, QWidget *parent) :
    PPVisual (visual, objPP, parent),
    ui(new Ui::ManCoordPPVisualWidget)
{
    ui->setupUi(this);

    ui->lblGeneralCoordFactor->setStyleSheet("font-size: 20pt; color: rgb(0, 100, 100);");
}

ManCoordPPVisualWidget::~ManCoordPPVisualWidget()
{
    if (m_factors)
        delete m_factors;
    delete ui;
}

bool ManCoordPPVisualWidget::isValid()
{
    auto objPP = objectPP()["pp"].toObject();
    auto arrDP = objPP["dp_list"].toArray();

    for (int i = 0; i < arrDP.size(); ++i)
    {
        auto objDP = arrDP.at(i).toObject();
        auto arrTests = objDP["test_list"].toArray();

        for (int j = 0; j < arrTests.size(); ++j)
        {
            auto objT = arrTests.at(j).toObject();
            auto uidMet = objT["uid"].toString();
            auto uidTest = objT["test_uid"].toString();
            if (uidMet == GeneralCoordDefines::StepDeviationTestUid)
            {
                if (m_stepDevTestUid == QUuid().toString())
                    m_stepDevTestUid = uidTest;
            }
            else
            if (uidMet == GeneralCoordDefines::StepOffsetTestUid)
            {
                if (m_stepOffsTestUid == QUuid().toString())
                    m_stepOffsTestUid = uidTest;
            }
            else
            if (uidMet == GeneralCoordDefines::EvolventaTestUid)
            {
                if (m_evolventaTestUid == QUuid().toString())
                    m_evolventaTestUid = uidTest;
            }
            else
            if (uidMet == GeneralCoordDefines::TriangleTestUid)
            {
                if (m_triangleTestUid == QUuid().toString())
                    m_triangleTestUid = uidTest;
            }
        }
    }

    return (m_stepDevTestUid != QUuid().toString()) &&
           (m_stepOffsTestUid != QUuid().toString()) &&
           (m_evolventaTestUid != QUuid().toString()) &&
           (m_triangleTestUid != QUuid().toString());
}

void ManCoordPPVisualWidget::calculate()
{
    m_factors = new GeneralCoordFactors(QStringList() << m_stepDevTestUid << m_stepOffsTestUid << m_evolventaTestUid << m_triangleTestUid);

    addWgtCapacitySetPosAfterShiftUid();
    addWgtCapacityRitmMovingUid();
    addWgtMotionAccuracyBeginUid();
    addWgtFctFinalAccuracyUid();
    addWgtProcessPerformAccuracyUid();
    addWgtAmplitudePerformAccuracyUid();
    addWgtOrientInSpaceUid();
    addWgtCapacityRepeatMovingUid();

    static_cast<QBoxLayout*>(ui->frFactors->layout())->insertSpacerItem(9, new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
}

void ManCoordPPVisualWidget::addWgtCapacitySetPosAfterShiftUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(1, tr("Способность занимать определенную позу после смещения тела"), 0);
    addComponent(fw, m_factors->valDifference(), tr("Дифференциация"));
    ui->frFactors->layout()->addWidget(fw);

}

void ManCoordPPVisualWidget::addWgtCapacityRitmMovingUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(2, tr("Способность к ритмичному движению"), 0);
    addComponent(fw, m_factors->valRitm(), tr("Ритм"));
    addComponent(fw, m_factors->valRitmStab(), tr("Стабильность ритма"));
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtMotionAccuracyBeginUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(3, tr("Точность вступления в сложное движение"), 0);
    addComponent(fw, m_factors->valFirstStep(), tr("Порог чувствительности"));
    addComponent(fw, m_factors->valDAPercent(), tr("Опережение маркера цели"));
    addComponent(fw, m_factors->valLatent(), tr("Латентный период"));
    addComponent(fw, m_factors->valLatentMoving(), tr("Время начала движения после появления сигнала"));
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtFctFinalAccuracyUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(4, tr("Точность при финальном требовании"), 0);
    fw->setComponent(tr("Амплитуда броска"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Изменение позиции треугольника"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtProcessPerformAccuracyUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(5, tr("Точность выполнения процесса"), 0);
    fw->setComponent(tr("Средняя ошибка по фронтали"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Средняя ошибка по сагиттали"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Средняя ошибка по площади (обучение)"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtAmplitudePerformAccuracyUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(6, tr("Точность воспроизводства требуемой амплитуды"), 0);
    fw->setComponent(tr("Амплитуда броска"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Среднее изменение амплитуды треугольника (обучение)"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtOrientInSpaceUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(7, tr("Ориентация в пространстве"), 0);
    fw->setComponent(tr("Точность позиции по фронтали"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Точность позиции по сагиттали"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtCapacityRepeatMovingUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(8, tr("Способность воспринимать заданное движение (обучаемость)"), 0);
    fw->setComponent(tr("Точность повторения"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Точность формы"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addComponent(CoordFactorWidget* wgt, const GeneralCoordDefines::DiapValue value, const QString& name)
{
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(value.uid);
    wgt->setComponent(name,
                      QString::number(value.value, 'f', fi.format()),
                      QString::number(value.percent, 'f', 0) + " %", QString::number(value.min), QString::number(value.max));
}
