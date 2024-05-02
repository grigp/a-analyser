#include "rehabreportppvisualwidget.h"
#include "ui_rehabreportppvisualwidget.h"

#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "rehabreportdefines.h"
#include "personalprogramservice.h"

#include <QJsonArray>
#include <QDebug>

RehabReportPPVisualWidget::RehabReportPPVisualWidget(PPVisualDescriptor* visual, QJsonObject &objPP, QWidget *parent) :
    PPVisual(visual, objPP, parent),
    ui(new Ui::RehabReportPPVisualWidget)
{
    ui->setupUi(this);
}

RehabReportPPVisualWidget::~RehabReportPPVisualWidget()
{
    delete ui;
}

bool RehabReportPPVisualWidget::isValid()
{
    //! Допустимо, если:
    //! 1) В каждой ДП, кроме первой, последней и, возможно промежуточной, имеются игры. В первой и последней они также могут быть, но не обязательно
    //! 2) ИП уже завершена, то есть не активна

    auto objPP = objectPP()["pp"].toObject();
    auto arrDP = objPP["dp_list"].toArray();
    auto isActive = objectPP()["active"].toBool(true);

    int cntDPWithGames {0};

    for (int i = 0; i < arrDP.size(); ++i)
    {
        auto objDP = arrDP.at(i).toObject();
        auto arrTests = objDP["test_list"].toArray();

        for (int j = 0; j < arrTests.size(); ++j)
        {
            auto objT = arrTests.at(j).toObject();
            auto uidMet = objT["uid"].toString();
            auto uidTest = objT["test_uid"].toString();

            auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(uidMet);
            if (mi.kindUid == RehabReportDefines::TrenEasyTestKindUid ||
                mi.kindUid == RehabReportDefines::TrenHardTestKindUid)
            {
                ++cntDPWithGames;
            }
        }
    }

    return cntDPWithGames >= arrDP.size() - 3 && !isActive;
}

void RehabReportPPVisualWidget::calculate()
{
    calculateStraight();
    calculateReverse();


    qDebug() << m_success;
    qDebug() << "romb" << m_uidRomb1 << m_uidRomb2;
    qDebug() << "cross" << m_uidCross1 << m_uidCross2;
}

void RehabReportPPVisualWidget::calculateStraight()
{
    auto objPP = objectPP()["pp"].toObject();
    auto arrDP = objPP["dp_list"].toArray();

    m_success.resize(arrDP.size());

    for (int i = 0; i < arrDP.size(); ++i)
    {
        auto objDP = arrDP.at(i).toObject();
        auto arrTests = objDP["test_list"].toArray();

        m_success[i] = 0;
        double midSucc {0};
        int nSucc {0};

        for (int j = 0; j < arrTests.size(); ++j)
        {
            auto objT = arrTests.at(j).toObject();
            auto uidMet = objT["uid"].toString();
            auto uidTest = objT["test_uid"].toString();

            if (uidMet == RehabReportDefines::RombergTestUid)
            {
                if (m_uidRomb1 == "")
                    m_uidRomb1 = uidTest;
            }
            if (uidMet == RehabReportDefines::CrossTestUid)
            {
                if (m_uidCross1 == "")
                    m_uidCross1 = uidTest;
            }

            auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(uidMet);
            if (mi.kindUid == RehabReportDefines::TrenEasyTestKindUid ||
                mi.kindUid == RehabReportDefines::TrenHardTestKindUid)
            {
                auto value = PersonalProgramService::getSuccessFactorValue(uidTest);
                if (value > -1)
                {
                    midSucc += value;
                    ++nSucc;
                }
            }
        }

        if (nSucc > 0)
            m_success[i] = midSucc / nSucc;
    }
}

void RehabReportPPVisualWidget::calculateReverse()
{
    auto objPP = objectPP()["pp"].toObject();
    auto arrDP = objPP["dp_list"].toArray();

    for (int i = arrDP.size(); i >= 0; --i)
    {
        auto objDP = arrDP.at(i).toObject();
        auto arrTests = objDP["test_list"].toArray();

        for (int j = 0; j < arrTests.size(); ++j)
        {
            auto objT = arrTests.at(j).toObject();
            auto uidMet = objT["uid"].toString();
            auto uidTest = objT["test_uid"].toString();

            if (uidMet == RehabReportDefines::RombergTestUid)
            {
                if (m_uidRomb2 == "")
                    m_uidRomb2 = uidTest;
            }
            if (uidMet == RehabReportDefines::CrossTestUid)
            {
                if (m_uidCross2 == "")
                    m_uidCross2 = uidTest;
            }
        }
    }
}
