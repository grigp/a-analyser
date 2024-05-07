#include "rehabreportppvisualwidget.h"
#include "ui_rehabreportppvisualwidget.h"

#include "aanalyserapplication.h"
#include "metodicsfactory.h"
#include "rehabreportdefines.h"
#include "personalprogramservice.h"
#include "ratioprobesfactors.h"
#include "classicfactors.h"
#include "crossfactors.h"
#include "vectorfactors.h"
#include "rombergkoefvaluedelegate.h"
#include "stabtestdefines.h"

#include "dataprovider.h"

#include <QJsonArray>
#include <QDebug>

namespace
{
static QMap<QString, QString> RombergFactorName =
{
    std::pair<QString, QString> (RatioProbesFactorsDefines::Probe2SUid, QApplication::tr("КР S"))
  , std::pair<QString, QString> (RatioProbesFactorsDefines::Probe2KFRUid, QApplication::tr("КР КФР"))
  , std::pair<QString, QString> (ClassicFactorsDefines::SquareUid, QApplication::tr("S"))
  , std::pair<QString, QString> (VectorFactorsDefines::KFRUid, QApplication::tr("КФР"))
  , std::pair<QString, QString> (ClassicFactorsDefines::LUid, QApplication::tr("L"))
  , std::pair<QString, QString> (ClassicFactorsDefines::QXUid, QApplication::tr("СКО X"))
  , std::pair<QString, QString> (ClassicFactorsDefines::QYUid, QApplication::tr("СКО Y"))
};


}


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
    showSuccess();
}

void RehabReportPPVisualWidget::print()
{
    qDebug() << "Печать";
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

        m_success[i].val = -1;
        m_success[i].dt = "";
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
                if (value < 0)
                    value = 0;
                if (value > -1)
                {
                    midSucc += value;
                    ++nSucc;
                }
            }
        }

        m_success[i].dt = objDP["date_time"].toString();
        if (nSucc > 0)
            m_success[i].val = midSucc / nSucc;
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
        item1->setEditable(false);
        auto item2 = new QStandardItem(getTestTitle(m_uidRomb2));
        item2->setEditable(false);
        m_mdlRomb.appendColumn(QList<QStandardItem*>() << item1 << item2);

        addFactorColumnToModel(RatioProbesFactorsDefines::Probe2SUid, fctRatio1, fctRatio2, &m_mdlRomb, header, "", &RombergFactorName);
        addFactorColumnToModel(RatioProbesFactorsDefines::Probe2KFRUid, fctRatio1, fctRatio2, &m_mdlRomb, header, "", &RombergFactorName);
        addFactorColumnToModel(ClassicFactorsDefines::SquareUid, fctClass11, fctClass21, &m_mdlRomb, header, tr("Откр"), &RombergFactorName);
        addFactorColumnToModel(VectorFactorsDefines::KFRUid, fctVector11, fctVector21, &m_mdlRomb, header, tr("Откр"), &RombergFactorName);
        addFactorColumnToModel(ClassicFactorsDefines::LUid, fctClass11, fctClass21, &m_mdlRomb, header, tr("Откр"), &RombergFactorName);
        addFactorColumnToModel(ClassicFactorsDefines::QXUid, fctClass11, fctClass21, &m_mdlRomb, header, tr("Откр"), &RombergFactorName);
        addFactorColumnToModel(ClassicFactorsDefines::QYUid, fctClass11, fctClass21, &m_mdlRomb, header, tr("Откр"), &RombergFactorName);
        addFactorColumnToModel(ClassicFactorsDefines::SquareUid, fctClass12, fctClass22, &m_mdlRomb, header, tr("Закр"), &RombergFactorName);
        addFactorColumnToModel(VectorFactorsDefines::KFRUid, fctVector12, fctVector22, &m_mdlRomb, header, tr("Закр"), &RombergFactorName);
        addFactorColumnToModel(ClassicFactorsDefines::LUid, fctClass12, fctClass22, &m_mdlRomb, header, tr("Закр"), &RombergFactorName);
        addFactorColumnToModel(ClassicFactorsDefines::QXUid, fctClass12, fctClass22, &m_mdlRomb, header, tr("Закр"), &RombergFactorName);
        addFactorColumnToModel(ClassicFactorsDefines::QYUid, fctClass12, fctClass22, &m_mdlRomb, header, tr("Закр"), &RombergFactorName);

        ui->tvRomb->setItemDelegateForColumn(1, new RombergKoefValueDelegate(ui->tvRomb));
        ui->tvRomb->setItemDelegateForColumn(2, new RombergKoefValueDelegate(ui->tvRomb));

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
        ui->tvRomb->header()->resizeSection(0, 200);
        for (int i = 1; i < m_mdlRomb.columnCount(); ++i)
            ui->tvRomb->header()->resizeSection(i, 100);
        ui->tvRomb->setSelectionMode(QAbstractItemView::NoSelection);
    }
    else
        ui->frRomb->setVisible(false);
}

void RehabReportPPVisualWidget::showCrossResults()
{
    if (m_uidCross1 != "" || m_uidCross2 != "")
    {
        CrossFactors *fctCross1 {nullptr};
        if (m_uidRomb1 != "")
        {
            QString uidProbe {""};
            getProbeCross(m_uidCross1, uidProbe);
            fctCross1 = new CrossFactors(m_uidCross1, uidProbe);
        }
        CrossFactors *fctCross2 {nullptr};
        if (m_uidCross2 != "")
        {
            QString uidProbe {""};
            getProbeCross(m_uidCross2, uidProbe);
            fctCross2 = new CrossFactors(m_uidCross2, uidProbe);
        }

        m_mdlCross.clear();
        QStringList header = QStringList() << tr("Тест");

        auto item1 = new QStandardItem(getTestTitle(m_uidCross1));
        item1->setEditable(false);
        auto item2 = new QStandardItem(getTestTitle(m_uidCross2));
        item2->setEditable(false);
        m_mdlCross.appendColumn(QList<QStandardItem*>() << item1 << item2);

        for (int i = 0; i < fctCross1->size(); ++i)
            addFactorColumnToModel(fctCross1->factorUid(i), fctCross1, fctCross2, &m_mdlCross, header, "", nullptr);

        if (fctCross1) delete fctCross1;
        if (fctCross2) delete fctCross2;

        m_mdlCross.setHorizontalHeaderLabels(header);
        ui->tvCross->setModel(&m_mdlCross);
        ui->tvCross->header()->resizeSection(0, 200);
        for (int i = 1; i < m_mdlCross.columnCount(); ++i)
            ui->tvCross->header()->resizeSection(i, 150);
        ui->tvCross->setSelectionMode(QAbstractItemView::NoSelection);
    }
    else
        ui->frCross->setVisible(false);
}

void RehabReportPPVisualWidget::showSuccess()
{
    ui->wgtSuccess->setKind(DynamicDiagramDefines::KindBar);
    ui->wgtSuccess->setVolume(DynamicDiagramDefines::Volume3D);
    ui->wgtSuccess->setBottomText(tr("Дневные программы"));

    ui->wgtSuccess->setAxisSpaceLeft(30);

    for (int i = 0; i < m_success.size(); ++i)
    {
        if (m_success[i].val > -1)
        {
            auto item = new DiagItem(m_success[i].val, m_success[i].dt);
            ui->wgtSuccess->appendItem(item);
        }
    }
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
                                                       QStringList& header,
                                                       const QString& fDetail,
                                                       const QMap<QString, QString>* factorNames)
{
    QString fvs1 = "-";
    double fv1 = -1;
    if (fg1)
    {
        fvs1 = fg1->factorValueFormatted(uidFactor);
        fv1 = fg1->factorValue(uidFactor);
    }
    QString fvs2 = "-";
    double fv2 = -1;
    if (fg2)
    {
        fvs2 = fg2->factorValueFormatted(uidFactor);
        fv2 = fg2->factorValue(uidFactor);
    }
    auto item1 = new QStandardItem(fvs1);
    item1->setEditable(false);
    item1->setData(uidFactor, RehabReportDefines::FactorUidRole);
    item1->setData(fv1, RehabReportDefines::FactorValueRole);
    auto item2 = new QStandardItem(fvs2);
    item2->setEditable(false);
    item2->setData(uidFactor, RehabReportDefines::FactorUidRole);
    item2->setData(fv2, RehabReportDefines::FactorValueRole);

    if (uidFactor == RatioProbesFactorsDefines::Probe2SUid || uidFactor == RatioProbesFactorsDefines::Probe2KFRUid)
    {
        DataDefines::NormSideValue nv1 = getNormValue(fv1);
        DataDefines::NormSideValue nv2 = getNormValue(fv2);

        item1->setData(nv1, StabTestDefines::NormRole);
        item2->setData(nv2, StabTestDefines::NormRole);
    }


    model->appendColumn(QList<QStandardItem*>() << item1 << item2);

    QString fn = "";
    if (factorNames)
    {
        fn = factorNames->value(uidFactor);
        if (fDetail != "")
            fn = fn + " " + fDetail;
    }
    else
    {
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidFactor);
        fn = fi.shortName();
    }
    header << fn;
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

DataDefines::NormSideValue RehabReportPPVisualWidget::getNormValue(const double value) const
{
    DataDefines::NormSideValue retval = DataDefines::nsvMissing;
    if (value >= 0 && value < 100)
        retval = DataDefines::nsvBelow;
    else
    if (value >= 100 && value <= 250)
        retval = DataDefines::nsvNorm;
    if (value > 250)
        retval = DataDefines::nsvAbove;

    return retval;
}
