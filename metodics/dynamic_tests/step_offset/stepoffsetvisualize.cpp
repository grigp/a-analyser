#include "stepoffsetvisualize.h"
#include "ui_stepoffsetvisualize.h"

#include <QByteArray>
#include <QStandardItemModel>
#include <QDebug>


#include "basedefines.h"
#include "baseutils.h"
#include "aanalyserapplication.h"
#include "stepoffsetcalculator.h"
#include "stepoffsetfactors.h"
#include "settingsprovider.h"
#include "reportelements.h"

namespace
{
Transients *wgtProcess {nullptr};
QStandardItemModel *mdlFactors {nullptr};
QString sForce {""};
QString sDirection {""};
QString sDeviation {""};
QString sRepeatCount {""};

QString sReactionTime {""};
QString sStatism {""};
QString sLatent {""};
QString sSpurtSpeed {""};
QString sSpurtAmpl {""};
QString sStabilityDeviation {""};
QString sRetentionDeviation {""};
QString sPrecisionKogn {""};
QString sPrecisionMotor {""};
QString sProcessKind {""};
QString sCorrectionPredominace {""};
QString sCorrectionResume {""};
QColor sCorrectionResumeColor;
DualStateDiagram *wgtCorrectionDiag {nullptr};
}


StepOffsetVisualize::StepOffsetVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StepOffsetVisualize)
{
    auto val = SettingsProvider::valueFromRegAppCopy("StepOffsetTestWidget", "CurrentPage").toInt();

    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(val);
    restoreSplitterPosition();
}

StepOffsetVisualize::~StepOffsetVisualize()
{
    delete ui;
}

void StepOffsetVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new StepOffsetCalculator(testUid, this);
        m_calculator->calculate();

        showTransients();
        showTable();
        showParams();
        showConslution();
        showConslutionStrategy();
    }
}

void StepOffsetVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);

    //! Поле для нижнего колонтитула
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));

    //! Показатели для заключения
    auto drawConslutionFactors = [&](const int start, const int offset)
    {
        int pos = start;
        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, tr("Результативность"));
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sReactionTime);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sStatism);
        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        pos += (offset + 1);
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, tr("Эффективность"));
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sLatent);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sSpurtSpeed);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sSpurtAmpl);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sStabilityDeviation);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sRetentionDeviation);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sPrecisionKogn);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sPrecisionMotor);
        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        pos += (offset + 1);
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, tr("Стратегия"));
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sProcessKind);

        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sCorrectionPredominace);
        painter->setPen(sCorrectionResumeColor);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, sCorrectionResume);
    };

    //! Параметры воздействия
    auto drawParams = [&]()
    {
        painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 10 * 8, sForce);
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 8.2), sDirection);
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 8.4), sDeviation);
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 8.6), sRepeatCount);
    };

    if (printer->orientation() == QPrinter::Portrait)
    {
        //! Диаграмма. Копируется из виджета
        ReportElements::drawWidget(painter, wgtProcess,
                                   static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.8),
                                   paper.x() + paper.width()/10, paper.y() + paper.height()/7);

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 7 * 3,
                        paper.width() / 10 * 9,
                        paper.height() / 3);
        ReportElements::drawTable(painter, mdlFactors, rectTable,
                                  QList<int>() << 3 << 2 << 2, ReportElements::Table::tvsStretched,
                                  8, -1, QFont::Bold);

        //! Параметры воздействия
        drawParams();

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 2
        printer->newPage();

        drawConslutionFactors(7, 2);

        //! Диаграмма преобладания коррекций.
        ReportElements::drawWidget(painter, wgtCorrectionDiag,
                                   static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.8),
                                   paper.x() + paper.width()/10, paper.y() + paper.height() / 90 * 39);


    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! Диаграмма. Копируется из виджета
        ReportElements::drawWidget(painter, wgtProcess,
                                   static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.9),
                                   paper.x() + paper.width()/20, paper.y() + paper.height()/7);

        //! Параметры воздействия
        drawParams();

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 2
        printer->newPage();

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 7,
                        paper.width() / 10 * 9,
                        paper.height() / 3);
        ReportElements::drawTable(painter, mdlFactors, rectTable,
                                  QList<int>() << 3 << 2 << 2, ReportElements::Table::tvsStretched,
                                  8, -1, QFont::Bold);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 3
        printer->newPage();

        drawConslutionFactors(7, 3);

        //! Диаграмма преобладания коррекций.
        ReportElements::drawWidget(painter, wgtCorrectionDiag,
                                   static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.8),
                                   paper.x() + paper.width()/10, paper.y() + paper.height() / 90 * 54);
    }

    //! Нижний колонтитул
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void StepOffsetVisualize::curPageChanged(int pageIdx)
{
    SettingsProvider::setValueToRegAppCopy("StepOffsetTestWidget", "CurrentPage", pageIdx);
}

void StepOffsetVisualize::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void StepOffsetVisualize::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("StepOffsetTestWidget", "ProcessSplitterPosition", ui->splProcess->saveState());
}

void StepOffsetVisualize::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("StepOffsetTestWidget", "ProcessSplitterPosition").toByteArray();
    ui->splProcess->restoreState(val);
}

void StepOffsetVisualize::showTransients()
{
    int min = qMin(m_calculator->bufferCompensationCount(), m_calculator->bufferReturnCount());
    ui->wgtProcess->setFreq(m_calculator->freq());

    //! Время броска от стимула = время броска + время размаха.
    //! Если время размаха == 0, то время броска + латентный период.
    double spurtTime = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SpurtTimeUid) +
                       m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SwingTimeUid);
    if (m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SwingTimeUid) - 0 < 1e-10)
        spurtTime = spurtTime + m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::LatentUid);
    ui->wgtProcess->setParams(StepOffsetDefines::stgCompensaton,
                              m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::LatentUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SwingTimeUid),
                              spurtTime,
                              m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::ReactionTimeUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::StatismUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid));

    spurtTime = m_calculator->factorValue(StepOffsetFactorsDefines::Return::SpurtTimeUid) +
                           m_calculator->factorValue(StepOffsetFactorsDefines::Return::SwingTimeUid);
        if (m_calculator->factorValue(StepOffsetFactorsDefines::Return::SwingTimeUid) - 0 < 1e-10)
            spurtTime = spurtTime + m_calculator->factorValue(StepOffsetFactorsDefines::Return::LatentUid);
    ui->wgtProcess->setParams(StepOffsetDefines::stgReturn,
                              m_calculator->factorValue(StepOffsetFactorsDefines::Return::LatentUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Return::SwingTimeUid),
                              spurtTime,
                              m_calculator->factorValue(StepOffsetFactorsDefines::Return::ReactionTimeUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Return::StatismUid),
                              m_calculator->factorValue(StepOffsetFactorsDefines::Return::RetentionDeviationUid));

    ui->wgtProcess->beginAddValues();
    for (int i = 0; i < min; ++i)
    {
        ui->wgtProcess->setCompensationValue(m_calculator->bufferCompensationValue(i));
        ui->wgtProcess->setReturnValue(m_calculator->bufferReturnValue(i));
    }
    ui->wgtProcess->endAddValues();

    wgtProcess = ui->wgtProcess;
}

void StepOffsetVisualize::showTable()
{
    auto *model = new QStandardItemModel();
    for (int i = 0; i < m_calculator->factorsCount() / 2; ++i)
    {
        auto factorCompUid = m_calculator->factorUid(i);
        auto factorCompVal = m_calculator->factorValueFormatted(factorCompUid);
        auto fiComp = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factorCompUid);
        QString fnComp = fiComp.name();
        if (fiComp.measure() != "")
            fnComp = fnComp + ", " + fiComp.measure();

        auto factorRetUid = m_calculator->factorUid(m_calculator->factorsCount() / 2 + i);
        auto factorRetVal = m_calculator->factorValueFormatted(factorRetUid);
        auto fiRet = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factorRetUid);
        QString fnRet = fiRet.name();
        if (fiRet.measure() != "")
            fnRet = fnRet + ", " + fiRet.measure();

        auto *itemName = new QStandardItem(fnComp);
        itemName->setEditable(false);
        auto *itemValueComp = new QStandardItem(factorCompVal);
        itemValueComp->setEditable(false);
        auto *itemValueRet = new QStandardItem(factorRetVal);
        itemValueRet->setEditable(false);
        model->appendRow(QList<QStandardItem*>() << itemName << itemValueComp << itemValueRet);
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель")
                                                   << tr("Компенсация воздействия")
                                                   << tr("Возврат в исходное состояние"));
    ui->tvFactors->setModel(model);
    ui->tvFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvFactors->header()->resizeSection(0, 450);

    mdlFactors = static_cast<QStandardItemModel*>(ui->tvFactors->model());
}

void StepOffsetVisualize::showParams()
{
    sForce = tr("Усилие") + " - " + QString::number(m_calculator->force()) + " " + tr("%");
    sDirection = tr("Направление") + " - " + BaseDefines::DirectionValueName.value(m_calculator->direction());
    sDeviation = tr("Отклонение") + " - " +
            QString::number(m_calculator->diap() * m_calculator->force() / 100) +
            " " + tr("мм");
    sRepeatCount = tr("Кол-во повторений") + " - " + QString::number(m_calculator->stepsCount());

    ui->lblForce->setText(sForce);
    ui->lblDirection->setText(sDirection);
    ui->lblDeviation->setText(sDeviation);
    ui->lblRepeatCount->setText(sRepeatCount);
}

void StepOffsetVisualize::showConslution()
{
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::ReactionTimeUid);
    double v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::ReactionTimeUid);
    sReactionTime = tr("Время реакции") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("сек");
    ui->lblReactionTime->setText(sReactionTime);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::StatismUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::StatismUid);
    sStatism = tr("Статизм") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblStatism->setText(sStatism);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::LatentUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::LatentUid);
    sLatent = tr("Латентный период") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("сек");
    ui->lblLatent->setText(sLatent);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::SpurtSpeedMMUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SpurtSpeedMMUid);
    sSpurtSpeed = tr("Скорость броска") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм/сек");
    ui->lblSpurtSpeed->setText(sSpurtSpeed);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::SpurtAmplUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SpurtAmplUid);
    sSpurtAmpl = tr("Амплитуда броска") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblSpurtAmpl->setText(sSpurtAmpl);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::StabilityDeviationUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::StabilityDeviationUid);
    sStabilityDeviation = tr("Разброс на этапе стабилизации") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblStabilityDeviation->setText(sStabilityDeviation);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid);
    sRetentionDeviation = tr("Разброс на этапе удержания") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblRetentionDeviation->setText(sRetentionDeviation);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::CorrectKognErrorUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::CorrectKognErrorUid);
    sPrecisionKogn = tr("Точность выполнения когнитивным сигналом") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblPrecisionKogn->setText(sPrecisionKogn);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::CorrectMotorErrorUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::CorrectMotorErrorUid);
    sPrecisionMotor = tr("Точность моторной коррекции") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblPrecisionMotor->setText(sPrecisionMotor);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::ProcessKindUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::ProcessKindUid);
    sProcessKind = tr("Тип переходного процесса") + " : " + QString::number(v, 'f', fi.format());
    ui->lblProcessKind->setText(sProcessKind);
}

void StepOffsetVisualize::showConslutionStrategy()
{
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::CorrectDominanceUid);
    double v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::CorrectDominanceUid);
    sCorrectionPredominace = tr("Преобладание коррекций") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("%");
    ui->lblCorrectionPredominace->setText(sCorrectionPredominace);

    QString resume = "";
    QString colorResume = "";
    BaseUtils::setCorrectionsDominanceResume(v, resume, colorResume);
    sCorrectionResume = resume;
    ui->lblCorrectionResume->setText(resume);
    ui->lblCorrectionResume->setStyleSheet(colorResume);
    QPalette pal = ui->lblCorrectionResume->palette();
    sCorrectionResumeColor = pal.color(QPalette::WindowText);

    ui->wgtCorrectionDiag->setValue(v);
    ui->wgtCorrectionDiag->setDescriptionLeft(tr("Быстрые коррекции"));
    ui->wgtCorrectionDiag->setDescriptionRight(tr("Медленные коррекции"));
    wgtCorrectionDiag = ui->wgtCorrectionDiag;
}
