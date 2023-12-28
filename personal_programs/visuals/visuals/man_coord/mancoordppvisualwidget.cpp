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

    ui->lblGeneralCoordFactor->setText(tr("Общий показатель координированности") + " - " + QString::number(m_factors->valGeneralCoord(), 'f', 0) + " %");
}

void ManCoordPPVisualWidget::addWgtCapacitySetPosAfterShiftUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(1, tr("Способность занимать определенную позу после смещения тела"), m_factors->valCapacitySetPosAfterShift());
    addComponent(fw, m_factors->valDifference(), tr("Дифференциация"));
    ui->frFactors->layout()->addWidget(fw);

}

void ManCoordPPVisualWidget::addWgtCapacityRitmMovingUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(2, tr("Способность к ритмичному движению"), m_factors->valCapacityRitmMoving());
    addComponent(fw, m_factors->valRitm(), tr("Ритм"));
    addComponent(fw, m_factors->valRitmStab(), tr("Стабильность ритма"));
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtMotionAccuracyBeginUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(3, tr("Точность вступления в сложное движение"), m_factors->valMotionAccuracyBegin());
    addComponent(fw, m_factors->valFirstStep(), tr("Порог чувствительности"));
    addComponent(fw, m_factors->valDAPercent(), tr("Опережение маркера цели"));
    addComponent(fw, m_factors->valLatent(), tr("Латентный период"));
    addComponent(fw, m_factors->valLatentMoving(), tr("Время начала движения после появления сигнала"));
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtFctFinalAccuracyUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(4, tr("Точность при финальном требовании"), m_factors->valFctFinalAccuracy());
    addComponent(fw, m_factors->valSprA(), tr("Амплитуда броска"));
    addComponent(fw, m_factors->valMidPosErrAnl(), tr("Изменение позиции треугольника"));
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtProcessPerformAccuracyUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(5, tr("Точность выполнения процесса"), m_factors->valProcessPerformAccuracy());
    addComponent(fw, m_factors->valEvlErrX(), tr("Средняя ошибка по фронтали"));
    addComponent(fw, m_factors->valEvlErrY(), tr("Средняя ошибка по сагиттали"));
    addComponent(fw, m_factors->valMidSquareErrTst(), tr("Средняя ошибка по площади (обучение)"));
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtAmplitudePerformAccuracyUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(6, tr("Точность воспроизводства требуемой амплитуды"), m_factors->valAmplitudePerformAccuracy());
    addComponent(fw, m_factors->valSprA1(), tr("Амплитуда броска"));
    addComponent(fw, m_factors->valMidAmplErrTst(), tr("Среднее изменение амплитуды треугольника (обучение)"));
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtOrientInSpaceUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(7, tr("Ориентация в пространстве"), m_factors->valOrientInSpace());
    addComponent(fw, m_factors->valMidAccMidX(), tr("Точность позиции по фронтали"));
    addComponent(fw, m_factors->valMidAccMidY(), tr("Точность позиции по сагиттали"));
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addWgtCapacityRepeatMovingUid()
{
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(8, tr("Способность воспринимать заданное движение (обучаемость)"), m_factors->valCapacityRepeatMoving());
    addComponent(fw, m_factors->valAccRepeat(), tr("Точность повторения"));
    addComponent(fw, m_factors->valAccForm(), tr("Точность формы"));
    ui->frFactors->layout()->addWidget(fw);
}

void ManCoordPPVisualWidget::addComponent(CoordFactorWidget* wgt, const GeneralCoordDefines::DiapValue value, const QString& name)
{
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(value.uid);
    wgt->setComponent(name,
                      QString::number(value.value, 'f', fi.format()),
                      QString::number(value.percent, 'f', 0) + " %", QString::number(value.min), QString::number(value.max));
}
