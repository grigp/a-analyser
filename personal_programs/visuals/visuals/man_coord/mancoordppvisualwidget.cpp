#include "mancoordppvisualwidget.h"
#include "ui_mancoordppvisualwidget.h"

#include <QJsonArray>
#include <QSpacerItem>
#include <QDebug>

#include "coordfactorwidget.h"

namespace
{
static const QString StepDeviationTestUid = "{74B5B3BD-A29A-4F29-9404-F56568051949}";
static const QString StepOffsetTestUid = "{3664D67C-8D21-4AC8-B2DD-D5DECA43360D}";
static const QString EvolventaTestUid = "{94021282-6B50-432F-A715-D34B3FEC4FD7}";
static const QString TriangleTestUid = "{F13585D9-32C3-47CE-9C37-2B8BB7560A7D}";

}

ManCoordPPVisualWidget::ManCoordPPVisualWidget(PPVisualDescriptor* visual, QJsonObject &objPP, QWidget *parent) :
    PPVisual (visual, objPP, parent),
    ui(new Ui::ManCoordPPVisualWidget)
{
    ui->setupUi(this);

    ui->lblGeneralCoordFactor->setStyleSheet("font-size: 20pt; color: rgb(0, 100, 100);");
}

ManCoordPPVisualWidget::~ManCoordPPVisualWidget()
{
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
            if (uidMet == StepDeviationTestUid)
            {
                if (m_stepDevTestUid == QUuid().toString())
                    m_stepDevTestUid = uidTest;
            }
            else
            if (uidMet == StepOffsetTestUid)
            {
                if (m_stepOffsTestUid == QUuid().toString())
                    m_stepOffsTestUid = uidTest;
            }
            else
            if (uidMet == EvolventaTestUid)
            {
                if (m_evolventaTestUid == QUuid().toString())
                    m_evolventaTestUid = uidTest;
            }
            else
            if (uidMet == TriangleTestUid)
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
    auto fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(1, tr("Способность занимать определенную позу после смещения тела"), 0);
    fw->setComponent(tr("Дифференциация"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);

    fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(2, tr("Способность к ритмичному движению"), 0);
    fw->setComponent(tr("Ритм"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Стабильность ритма"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);

    fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(3, tr("Точность вступления в сложное движение"), 0);
    fw->setComponent(tr("Порог чувствительности"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Опережение маркера цели"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Латентный период"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Время начала движения после появления сигнала"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);

    fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(4, tr("Точность при финальном требовании"), 0);
    fw->setComponent(tr("Амплитуда броска"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Изменение позиции треугольника"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);

    fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(5, tr("Точность выполнения процесса"), 0);
    fw->setComponent(tr("Средняя ошибка по фронтали"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Средняя ошибка по сагиттали"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Средняя ошибка по площади (обучение)"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);

    fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(6, tr("Точность воспроизводства требуемой амплитуды"), 0);
    fw->setComponent(tr("Амплитуда броска"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Среднее изменение амплитуды треугольника (обучение)"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);

    fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(7, tr("Ориентация в пространстве"), 0);
    fw->setComponent(tr("Точность позиции по фронтали"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Точность позиции по сагиттали"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);

    fw = new CoordFactorWidget(ui->frFactors);
    fw->setFactor(8, tr("Способность воспринимать заданное движение (обучаемость)"), 0);
    fw->setComponent(tr("Точность повторения"), "0", "0 %", "0", "0");
    fw->setComponent(tr("Точность формы"), "0", "0 %", "0", "0");
    ui->frFactors->layout()->addWidget(fw);

    static_cast<QBoxLayout*>(ui->frFactors->layout())->insertSpacerItem(9, new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));
}
