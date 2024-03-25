#include "dopuskwidget.h"
#include "ui_dopuskwidget.h"

#include "dopuskcalculator.h"
#include "vectorfactors.h"
#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "reportelements.h"
#include "stabtestvisualize.h"

#include <QDebug>

DopuskWidget::DopuskWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DopuskWidget)
{
    ui->setupUi(this);
}

DopuskWidget::~DopuskWidget()
{
    delete ui;
}

void DopuskWidget::calculate(DopuskCalculator *calculator, const QString &testUid)
{
    m_testUid = testUid;
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(m_testUid, ti))
    {
        auto format = static_cast<AAnalyserApplication*>(QApplication::instance())->
                getFactorInfo(VectorFactorsDefines::KFRUid).format();

        QString fctName = static_cast<AAnalyserApplication*>(QApplication::instance())->
                getFactorInfo(VectorFactorsDefines::KFRUid).name();

        m_values[0] = calculator->factors(0)->factorValue(VectorFactorsDefines::KFRUid);
        m_sOpenEyesResult = QString("%1 %").arg(m_values[0], 3, 'f', format);
        ui->lblOpenEyesResult->setText(m_sOpenEyesResult);
        m_sOpenEyesFactor = fctName;
        ui->lblOpenEyesFactor->setText(fctName);


        m_values[1] = calculator->factors(1)->factorValue(VectorFactorsDefines::KFRUid);
        m_sCloseEyesResult = QString("%1 %").arg(m_values[1], 3, 'f', format);
        ui->lblCloseEyesResult->setText(m_sCloseEyesResult);
        m_sCloseEyesFactor = fctName;
        ui->lblCloseEyesFactor->setText(fctName);

        m_values[2] = calculator->factors(2)->factorValue(VectorFactorsDefines::KFRUid);
        m_sTargetResult = QString("%1 %").arg(m_values[2], 3, 'f', format);
        ui->lblTargetResult->setText(m_sTargetResult);
        m_sTargetFactor = fctName;
        ui->lblTargetFactor->setText(fctName);

        getPersonalNorms();
        getGroupNorm(ti.metodUid, ti.condition);

        showConslution();

        connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
                &AAnalyserApplication::personalNormRecalculated,
                this, &DopuskWidget::on_changePersonalNorm);
        connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
                &AAnalyserApplication::personalNormDeleted,
                this, &DopuskWidget::on_changePersonalNorm);
        connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
                &AAnalyserApplication::personalNormContainedChange,
                this, &DopuskWidget::on_changePersonalNorm);
        connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
                &AAnalyserApplication::changeTestCondition,
                this, &DopuskWidget::on_changeTestCondition);
    }
}

void DopuskWidget::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    //! Получаем указатель на экземпляр визуализатора
    auto vis = static_cast<AAnalyserApplication*>(QCoreApplication::instance())->getOpenedTest(testUid);
    StabTestVisualize* visualPanel = static_cast<StabTestVisualize*>(vis);
    DopuskWidget* visual = static_cast<DopuskWidget*>(visualPanel->widget());

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    if (printer->orientation() == QPrinter::Portrait)
    {
        //! Открытые глаза
        painter->setFont(QFont("Sans", 16, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 1.2),
                          tr("Открытые глаза"));
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 1.3),
                          visual->m_sOpenEyesFactor + " : " + visual->m_sOpenEyesResult);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 1.4),
                          tr("Групповая норма") + " : " + visual->m_sOpenEyesGroupNormResume);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 1.5),
                          tr("Индивидуальная норма") + " : " + visual->m_sOpenEyesPersonalNormResume);


        ReportElements::drawWidget(painter, visual->m_wgtOpenEyesNorm,
                                   static_cast<int>(paper.width() * 0.35), static_cast<int>(paper.height() * 0.35),
                                   paper.x() + paper.width() / 10 * 6, paper.y() + paper.height() / 7);

        //! Закрытые глаза
        painter->setFont(QFont("Sans", 16, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 3),
                          tr("Закрытые глаза"));
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 3.1),
                          visual->m_sCloseEyesFactor + " : " + visual->m_sCloseEyesResult);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 3.2),
                          tr("Групповая норма") + " : " + visual->m_sCloseEyesGroupNormResume);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 3.3),
                          tr("Индивидуальная норма") + " : " + visual->m_sCloseEyesPersonalNormResume);

        ReportElements::drawWidget(painter, visual->m_wgtCloseEyesNorm,
                                   static_cast<int>(paper.width() * 0.35), static_cast<int>(paper.height() * 0.35),
                                   paper.x() + paper.width() / 10 * 6, static_cast<int>(paper.y() + paper.height() / 7 * 2.8));

        //! Мишень
        painter->setFont(QFont("Sans", 16, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 4.8),
                          tr("Мишень"));
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 4.9),
                          visual->m_sTargetFactor + " : " + visual->m_sTargetResult);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 5.0),
                          tr("Групповая норма") + " : " + visual->m_sTargetGroupNormResume);
        painter->drawText(paper.x() + paper.width() / 12,
                          static_cast<int>(paper.y() + paper.height() / 7 * 5.1),
                          tr("Индивидуальная норма") + " : " + visual->m_sTargetPersonalNormResume);

        ReportElements::drawWidget(painter, visual->m_wgtTargetNorm,
                                   static_cast<int>(paper.width() * 0.35), static_cast<int>(paper.height() * 0.35),
                                   paper.x() + paper.width() / 10 * 6, static_cast<int>(paper.y() + paper.height() / 7 * 4.6));
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! Открытые глаза
        painter->setFont(QFont("Sans", 16, QFont::Bold, false));
        painter->drawText(static_cast<int>(paper.x() + paper.width() / 12 * 1.7),
                          static_cast<int>(paper.y() + paper.height() / 7 * 1.2),
                          tr("Открытые глаза"));
        painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 7 * 4.4),
                          visual->m_sOpenEyesFactor + " : " + visual->m_sOpenEyesResult);
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 7 * 4.6),
                          tr("Групповая норма") + " : " + visual->m_sOpenEyesGroupNormResume);
        painter->drawText(paper.x() + paper.width() / 10,
                          static_cast<int>(paper.y() + paper.height() / 7 * 4.8),
                          tr("Индивидуальная норма") + " : " + visual->m_sOpenEyesPersonalNormResume);


        ReportElements::drawWidget(painter, visual->m_wgtOpenEyesNorm,
                                   static_cast<int>(paper.width() * 0.35), static_cast<int>(paper.height() * 0.35),
                                   paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 7 * 1.4));

        //! Закрытые глаза
        painter->setFont(QFont("Sans", 16, QFont::Bold, false));
        painter->drawText(static_cast<int>(paper.x() + paper.width() / 12 * 5.3),
                          static_cast<int>(paper.y() + paper.height() / 7 * 1.2),
                          tr("Закрытые глаза"));
        painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 4),
                          static_cast<int>(paper.y() + paper.height() / 7 * 4.4),
                          visual->m_sCloseEyesFactor + " : " + visual->m_sCloseEyesResult);
        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 4),
                          static_cast<int>(paper.y() + paper.height() / 7 * 4.6),
                          tr("Групповая норма") + " : " + visual->m_sCloseEyesGroupNormResume);
        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 4),
                          static_cast<int>(paper.y() + paper.height() / 7 * 4.8),
                          tr("Индивидуальная норма") + " : " + visual->m_sCloseEyesPersonalNormResume);

        ReportElements::drawWidget(painter, visual->m_wgtCloseEyesNorm,
                                   static_cast<int>(paper.width() * 0.35), static_cast<int>(paper.height() * 0.35),
                                   paper.x() + paper.width() / 10 * 4, static_cast<int>(paper.y() + paper.height() / 7 * 1.4));

        //! Мишень
        painter->setFont(QFont("Sans", 16, QFont::Bold, false));
        painter->drawText(static_cast<int>(paper.x() + paper.width() / 12 * 9.3),
                          static_cast<int>(paper.y() + paper.height() / 7 * 1.2),
                          tr("Мишень"));
        painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 7),
                          static_cast<int>(paper.y() + paper.height() / 7 * 4.4),
                          visual->m_sTargetFactor + " : " + visual->m_sTargetResult);
        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 7),
                          static_cast<int>(paper.y() + paper.height() / 7 * 4.6),
                          tr("Групповая норма") + " : " + visual->m_sTargetGroupNormResume);
        painter->drawText(static_cast<int>(paper.x() + paper.width() / 10 * 7),
                          static_cast<int>(paper.y() + paper.height() / 7 * 4.8),
                          tr("Индивидуальная норма") + " : " + visual->m_sTargetPersonalNormResume);

        ReportElements::drawWidget(painter, visual->m_wgtTargetNorm,
                                   static_cast<int>(paper.width() * 0.35), static_cast<int>(paper.height() * 0.35),
                                   paper.x() + paper.width() / 10 * 7, static_cast<int>(paper.y() + paper.height() / 7 * 1.4));

    }

    //! Нижний колонтитул
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void DopuskWidget::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, TestCalculator *calculator)
{
    Q_UNUSED(painter);
    Q_UNUSED(rect);
    Q_UNUSED(testUid);
    Q_UNUSED(calculator);
}

void DopuskWidget::on_changePersonalNorm(const QString &patientUid,
                                         const QString &methodUid,
                                         const QString &conditionUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(m_testUid, ti))
    {
        if (patientUid == ti.patientUid && methodUid == ti.metodUid && conditionUid == ti.condition)
        {
            getGroupNorm(methodUid, conditionUid);
            getPersonalNorms();
        }
    }
}

void DopuskWidget::on_changeTestCondition(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
        on_changePersonalNorm(ti.patientUid, ti.metodUid, ti.condition);
}

void DopuskWidget::getGroupNorm(const QString &methodUid, const QString &conditionUid)
{
    m_groupNorms.clear();
    QList<DataDefines::GroupNormInfo> gni;
    if (static_cast<AAnalyserApplication*>(QApplication::instance())->getGroupNorms(methodUid, conditionUid, gni))
    {
        foreach (auto gNorm, gni)
            m_groupNorms.insert(gNorm.probeNum, GroupNorm(gNorm.border, gNorm.conditionBorder));
    }
}

void DopuskWidget::getPersonalNorms()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->getPersonalNorm(m_testUid, m_pnil);
    showConslution();
}

void DopuskWidget::showConslution()
{
    for (int i = 0; i < 3; ++i)
    {
        NormBounds pn(-1, -1, -1, -1);
        if (i < m_pnil.size())
        {
            pn.normValLo = static_cast<int>(m_pnil.at(i).value - m_pnil.at(i).stdDeviation * 1);
            pn.normValHi = static_cast<int>(m_pnil.at(i).value + m_pnil.at(i).stdDeviation * 1);
            pn.condNormLo = static_cast<int>(m_pnil.at(i).value - m_pnil.at(i).stdDeviation * 2);
            pn.condNormHi = static_cast<int>(m_pnil.at(i).value + m_pnil.at(i).stdDeviation * 2);
        }
        DataDefines::NormValue pnv = getPersonalNormValue(i, pn);
        QColor pnColor = DataDefines::normValueToColor(pnv);

        DataDefines::NormValue gnv = getGroupNormValue(i);
        QColor gnColor = DataDefines::normValueToColor(gnv);

        auto drawNorm = [&](RGBLed *gnLed, RGBLed *pnLed,
                QLabel *gnResume, QLabel *pnResume,
                CircleNormIndicator *cni)
        {
            gnLed->setColor(gnColor);
            gnResume->setText(DataDefines::normValueToString(gnv));
            QPalette palette = gnResume->palette();
            palette.setColor(gnResume->foregroundRole(), DataDefines::normValueToColorDark(gnv));
            gnResume->setPalette(palette);

            pnLed->setColor(pnColor);
            pnResume->setText(DataDefines::normValueToString(pnv));
            palette = pnResume->palette();
            palette.setColor(pnResume->foregroundRole(), DataDefines::normValueToColorDark(pnv));
            pnResume->setPalette(palette);

            cni->setValue(static_cast<int>(m_values[i]));
            if (m_groupNorms.contains(i))
            {
                NormBounds gn(static_cast<int>(m_groupNorms.value(i).conditionBound),
                              static_cast<int>(m_groupNorms.value(i).bound), 100, 100);
                cni->setGroupNorm(gn);
            }
            else
            {
                NormBounds gn(-1, -1, -1, -1);
                cni->setGroupNorm(gn);
            }
            cni->setPersonalNorm(pn);
        };

        if (i == 0)
            drawNorm(ui->wgtOpenEyesGroupNorm, ui->wgtOpenEyesPersonalNorm,
                     ui->lblOpenEyesGroupNormResume, ui->lblOpenEyesPersonalNormResume,
                     ui->wgtOpenEyesNorm);
        else
        if (i == 1)
            drawNorm(ui->wgtCloseEyesGroupNorm, ui->wgtCloseEyesPersonalNorm,
                     ui->lblCloseEyesGroupNormResume, ui->lblCloseEyesPersonalNormResume,
                     ui->wgtCloseEyesNorm);
        else
        if (i == 2)
            drawNorm(ui->wgtTargetGroupNorm, ui->wgtTargetPersonalNorm,
                     ui->lblTargetGroupNormResume, ui->lblTargetPersonalNormResume,
                     ui->wgtTargetNorm);
    }

    //! Количество нормообразующих обследований
    auto cnt = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getPersonalNormContainedTestCount(m_testUid);
    ui->lblNormContainsCount->setText(QString(tr("Количество нормообразующих обследований") + " - %1").arg(cnt));

    m_wgtOpenEyesNorm = ui->wgtOpenEyesNorm;
    m_wgtCloseEyesNorm = ui->wgtCloseEyesNorm;
    m_wgtTargetNorm = ui->wgtTargetNorm;
    m_sOpenEyesGroupNormResume = ui->lblOpenEyesGroupNormResume->text();
    m_sCloseEyesGroupNormResume = ui->lblCloseEyesGroupNormResume->text();
    m_sTargetGroupNormResume = ui->lblTargetGroupNormResume->text();
    m_sOpenEyesPersonalNormResume = ui->lblOpenEyesPersonalNormResume->text();
    m_sCloseEyesPersonalNormResume = ui->lblCloseEyesPersonalNormResume->text();
    m_sTargetPersonalNormResume = ui->lblTargetPersonalNormResume->text();
}

DataDefines::NormValue DopuskWidget::getGroupNormValue(const int numProbe)
{
    DataDefines::NormValue gnv = DataDefines::MissingNorm;
    if (m_groupNorms.contains(numProbe))
    {
        GroupNorm gnvalues(m_groupNorms.value(numProbe).bound, m_groupNorms.value(numProbe).conditionBound);
        if (m_values[numProbe] >= gnvalues.bound)
            gnv = DataDefines::Normal;
        else
        if (m_values[numProbe] < gnvalues.bound && m_values[numProbe] > gnvalues.conditionBound)
            gnv = DataDefines::ConditionNormal;
        else
        if (m_values[numProbe] < gnvalues.conditionBound)
            gnv = DataDefines::NotNormal;
    }
    return gnv;
}

DataDefines::NormValue DopuskWidget::getPersonalNormValue(const int numProbe, const NormBounds &pn)
{
    if (pn.normValLo == -1 || pn.normValHi == -1 || pn.condNormLo == -1 || pn.condNormHi == -1)
        return DataDefines::MissingNorm;
    else
    if (m_values[numProbe] < pn.condNormLo || m_values[numProbe] > pn.condNormHi)
        return DataDefines::NotNormal;
    else
    if (m_values[numProbe] < pn.normValLo || m_values[numProbe] > pn.normValHi)
        return DataDefines::ConditionNormal;
    else
        return DataDefines::Normal;
}


