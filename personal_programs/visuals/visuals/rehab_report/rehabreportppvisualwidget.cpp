#include "rehabreportppvisualwidget.h"
#include "ui_rehabreportppvisualwidget.h"

#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "rehabreportdefines.h"
#include "personalprogramservice.h"
#include "ratioprobesfactors.h"
#include "classicfactors.h"
#include "vectorfactors.h"
#include "dataprovider.h"

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

    showRombergResults();
    showCrossResults();

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

            if (i < arrDP.size() / 2)  //! В первой половине списка ДП
            {
                if (uidMet == RehabReportDefines::RombergTestUid)
                {
                    if (m_uidRomb1 == "")
                        m_uidRomb1 = uidTest;
                }
                else
                if (uidMet == RehabReportDefines::CrossTestUid)
                {
                    if (m_uidCross1 == "")
                        m_uidCross1 = uidTest;
                }
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

            if (i > arrDP.size() / 2)  //! Во второй половине списка ДП
            {
                if (uidMet == RehabReportDefines::RombergTestUid)
                {
                    if (m_uidRomb2 == "")
                        m_uidRomb2 = uidTest;
                }
                else
                if (uidMet == RehabReportDefines::CrossTestUid)
                {
                    if (m_uidCross2 == "")
                        m_uidCross2 = uidTest;
                }
            }
        }
    }
}

void RehabReportPPVisualWidget::showRombergResults()
{
    if (m_uidRomb1 != "" || m_uidRomb2 != "")
    {
        RatioProbesFactors *fctRatio1 {nullptr};
        ClassicFactors *fctClass11 {nullptr};
        ClassicFactors *fctClass12 {nullptr};
        VectorFactors *fctVector11 {nullptr};
        VectorFactors *fctVector12 {nullptr};
        if (m_uidRomb1 != "")
        {
            fctRatio1 = new RatioProbesFactors(m_uidRomb1);
            QString uidProbe1 {""};
            QString uidProbe2 {""};
            getProbesRomberg(m_uidRomb1, uidProbe1, uidProbe2);
            fctClass11 = new ClassicFactors(m_uidRomb1, uidProbe1, ChannelsDefines::chanStab);
            fctClass12 = new ClassicFactors(m_uidRomb1, uidProbe2, ChannelsDefines::chanStab);
            fctVector11 = new VectorFactors(m_uidRomb1, uidProbe1, ChannelsDefines::chanStab);
            fctVector12 = new VectorFactors(m_uidRomb1, uidProbe2, ChannelsDefines::chanStab);
        }
        RatioProbesFactors *fctRatio2 {nullptr};
        ClassicFactors *fctClass21 {nullptr};
        ClassicFactors *fctClass22 {nullptr};
        VectorFactors *fctVector21 {nullptr};
        VectorFactors *fctVector22 {nullptr};
        if (m_uidRomb2 != "")
        {
            fctRatio2 = new RatioProbesFactors(m_uidRomb2);
            QString uidProbe1 {""};
            QString uidProbe2 {""};
            getProbesRomberg(m_uidRomb2, uidProbe1, uidProbe2);
            fctClass21 = new ClassicFactors(m_uidRomb2, uidProbe1, ChannelsDefines::chanStab);
            fctClass22 = new ClassicFactors(m_uidRomb2, uidProbe2, ChannelsDefines::chanStab);
            fctVector21 = new VectorFactors(m_uidRomb2, uidProbe1, ChannelsDefines::chanStab);
            fctVector22 = new VectorFactors(m_uidRomb2, uidProbe2, ChannelsDefines::chanStab);
        }

        m_mdlRomb.clear();
        QStringList header = QStringList() << tr("Тест");

        auto item1 = new QStandardItem(getTestTitle(m_uidRomb1));
        auto item2 = new QStandardItem(getTestTitle(m_uidRomb2));
        m_mdlRomb.appendColumn(QList<QStandardItem*>() << item1 << item2);

        addFactorColumnToModel(RatioProbesFactorsDefines::Probe2SUid, fctRatio1, fctRatio2, &m_mdlRomb, header);
        addFactorColumnToModel(RatioProbesFactorsDefines::Probe2KFRUid, fctRatio1, fctRatio2, &m_mdlRomb, header);
        addFactorColumnToModel(ClassicFactorsDefines::SquareUid, fctClass11, fctClass21, &m_mdlRomb, header);
        addFactorColumnToModel(VectorFactorsDefines::KFRUid, fctVector11, fctVector21, &m_mdlRomb, header);
        addFactorColumnToModel(ClassicFactorsDefines::SquareUid, fctClass11, fctClass21, &m_mdlRomb, header);
        addFactorColumnToModel(ClassicFactorsDefines::LUid, fctClass11, fctClass21, &m_mdlRomb, header);
        addFactorColumnToModel(ClassicFactorsDefines::QXUid, fctClass11, fctClass21, &m_mdlRomb, header);
        addFactorColumnToModel(ClassicFactorsDefines::QYUid, fctClass11, fctClass21, &m_mdlRomb, header);
        addFactorColumnToModel(ClassicFactorsDefines::SquareUid, fctClass12, fctClass22, &m_mdlRomb, header);
        addFactorColumnToModel(VectorFactorsDefines::KFRUid, fctVector12, fctVector22, &m_mdlRomb, header);
        addFactorColumnToModel(ClassicFactorsDefines::SquareUid, fctClass12, fctClass22, &m_mdlRomb, header);
        addFactorColumnToModel(ClassicFactorsDefines::LUid, fctClass12, fctClass22, &m_mdlRomb, header);
        addFactorColumnToModel(ClassicFactorsDefines::QXUid, fctClass12, fctClass22, &m_mdlRomb, header);
        addFactorColumnToModel(ClassicFactorsDefines::QYUid, fctClass12, fctClass22, &m_mdlRomb, header);

        if (fctRatio1) delete fctRatio1;
        if (fctRatio2) delete fctRatio2;
        if (fctClass11) delete fctClass11;
        if (fctClass12) delete fctClass12;
        if (fctClass21) delete fctClass21;
        if (fctClass22) delete fctClass22;
        if (fctVector11) delete fctVector11;
        if (fctVector12) delete fctVector12;
        if (fctVector21) delete fctVector21;
        if (fctVector22) delete fctVector22;

        m_mdlRomb.setHorizontalHeaderLabels(header);
        ui->tvRomb->setModel(&m_mdlRomb);
        ui->tvRomb->header()->resizeSections(QHeaderView::ResizeToContents);
    }
    else
        ui->frRomb->setVisible(false);
}

void RehabReportPPVisualWidget::showCrossResults()
{
    if (m_uidCross1 != "" || m_uidCross2 != "")
    {

    }
    else
        ui->frCross->setVisible(false);
}

QString RehabReportPPVisualWidget::getTestTitle(const QString &uidTest) const
{
    QString retval {"-"};
    if (uidTest != "")
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(uidTest, ti))
            return ti.dateTime.toString("dd.MM.yyyy hh:mm");
    }

    return retval;
}

void RehabReportPPVisualWidget::addFactorColumnToModel(const QString &uidFactor,
                                                       MultiFactor *fg1,
                                                       MultiFactor *fg2,
                                                       QStandardItemModel *model,
                                                       QStringList& header)
{
    QString fv1 = "-";
    if (fg1)
        fv1 = fg1->factorValueFormatted(uidFactor);
    QString fv2 = "-";
    if (fg2)
        fv2 = fg2->factorValueFormatted(uidFactor);
    auto item1 = new QStandardItem(fv1);
    auto item2 = new QStandardItem(fv2);
    model->appendColumn(QList<QStandardItem*>() << item1 << item2);

    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidFactor);
    header << fi.shortName();
}

bool RehabReportPPVisualWidget::getProbesRomberg(const QString &uidTest, QString &uidProbe1, QString &uidProbe2) const
{
    uidProbe1 = "";
    uidProbe2 = "";
    if (uidTest != "")
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(uidTest, ti))
        {
            if (ti.probes.size() == 2)
            {
                uidProbe1 = ti.probes.at(0);
                uidProbe2 = ti.probes.at(1);
                return true;
            }
        }
    }
    return false;
}

bool RehabReportPPVisualWidget::getProbeCross(const QString &uidTest, QString &uidProbe) const
{
    uidProbe = "";
    if (uidTest != "")
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(uidTest, ti))
        {
            if (ti.probes.size() == 1)
            {
                uidProbe = ti.probes.at(0);
                return true;
            }
        }
    }
    return false;
}
