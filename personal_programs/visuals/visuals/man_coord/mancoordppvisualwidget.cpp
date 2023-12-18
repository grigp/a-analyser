#include "mancoordppvisualwidget.h"
#include "ui_mancoordppvisualwidget.h"

#include <QJsonArray>
#include <QDebug>

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

}
