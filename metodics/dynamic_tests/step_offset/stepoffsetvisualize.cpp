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
#include "transientspainter.h"


StepOffsetVisualize::StepOffsetVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StepOffsetVisualize)
{

    ui->setupUi(this);

    restoreSplitterPosition();
}

StepOffsetVisualize::~StepOffsetVisualize()
{
    delete ui;
}

void StepOffsetVisualize::setTest(const QString &testUid)
{
    auto val = SettingsProvider::valueFromRegAppCopy("StepOffsetTestWidget", "CurrentPage").toInt();
    ui->tabWidget->setCurrentIndex(val);
    m_wgtTab = ui->tabWidget;

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
    //! Получаем указатель на экземпляр визуализатора
    auto vis = static_cast<AAnalyserApplication*>(QCoreApplication::instance())->getOpenedTest(testUid);
    StepOffsetVisualize* visual = static_cast<StepOffsetVisualize*>(vis);

    int tab = visual->m_wgtTab->currentIndex();
    for (int i = 0; i < visual->m_wgtTab->count(); ++i)
        visual->m_wgtTab->setCurrentIndex(i);
    visual->m_wgtTab->setCurrentIndex(tab);

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
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sReactionTime);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sStatism);
        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        pos += (offset + 1);
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, tr("Эффективность"));
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sLatent);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sSpurtSpeed);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sSpurtAmpl);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sStabilityDeviation);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sRetentionDeviation);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sPrecisionKogn);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sPrecisionMotor);
        painter->setFont(QFont("Sans", 14, QFont::Bold, false));
        pos += (offset + 1);
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, tr("Стратегия"));
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sProcessKind);

        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sCorrectionPredominace);
        painter->setPen(visual->m_sCorrectionResumeColorTrain);
        pos += offset;
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 90 * pos, visual->m_sCorrectionResume);
    };

    //! Параметры воздействия
    auto drawParams = [&]()
    {
        painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        painter->drawText(paper.x() + paper.width() / 10, paper.y() + paper.height() / 10 * 8, visual->m_sForce);
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 8.2), visual->m_sDirection);
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 8.4), visual->m_sDeviation);
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 8.6), visual->m_sRepeatCount);
    };

    if (printer->orientation() == QPrinter::Portrait)
    {
        //! Диаграмма
        auto rectDiag = QRect(paper.x() + paper.width()/10, paper.y() + paper.height()/7,
                              static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.27));
        double ratio = ReportElements::ratio(paper, visual->m_wgtProcess, 3);
        TransientsPainter tp(painter, rectDiag);
        printTransistents(&tp, visual);
        tp.doPaint(ratio);

        //! Таблица показателей. Берется модель таблицы из визуализатора
        QRect rectTable(paper.x() + paper.width() / 10,
                        paper.y() + paper.height() / 7 * 3,
                        paper.width() / 10 * 9,
                        paper.height() / 3);
        ReportElements::drawTable(painter, visual->m_mdlFactors, rectTable, QList<int>() << 3 << 2 << 2,
                                  false, ReportElements::Table::tvsStretched, 8, -1, QFont::Bold);

        //! Параметры воздействия
        drawParams();

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 2
        printer->newPage();

        drawConslutionFactors(7, 2);

        //! Диаграмма преобладания коррекций.
        auto rectDSDDomCorr = QRect(static_cast<int>(paper.x() + paper.width() * 0.1),
                                    static_cast<int>(paper.y() + paper.height() * 0.43),
                                    static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.1));
        ratio = ReportElements::ratio(paper, visual->m_wgtCorrectionDiagTrain, 4);
        auto dsdDomCorr = DualStateDiagramPainter(painter, rectDSDDomCorr);
        auto valCD = visual->m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::CorrectDominanceUid);
        dsdDomCorr.setValue(valCD);
        dsdDomCorr.setDescriptionLeft(tr("Быстрые коррекции"));
        dsdDomCorr.setDescriptionRight(tr("Медленные коррекции"));
        dsdDomCorr.doPaint(ratio);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! Диаграмма
        auto rectDiag = QRect(paper.x() + paper.width()/20, paper.y() + paper.height()/7,
                              static_cast<int>(paper.width() * 0.9), static_cast<int>(paper.height() * 0.6));
        double ratio = ReportElements::ratio(paper, visual->m_wgtProcess, 3);
        TransientsPainter tp(painter, rectDiag);
        printTransistents(&tp, visual);
        tp.doPaint(ratio);

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
        ReportElements::drawTable(painter, visual->m_mdlFactors, rectTable, QList<int>() << 3 << 2 << 2,
                                  false, ReportElements::Table::tvsStretched, 8, -1, QFont::Bold);

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 3
        printer->newPage();

        drawConslutionFactors(7, 3);

        //! Диаграмма преобладания коррекций.
        auto rectDSDDomCorr = QRect(static_cast<int>(paper.x() + paper.width() * 0.1),
                                    static_cast<int>(paper.y() + paper.height() * 0.6),
                                    static_cast<int>(paper.width() * 0.8), static_cast<int>(paper.height() * 0.1));
        ratio = ReportElements::ratio(paper, visual->m_wgtCorrectionDiagTrain, 4);
        auto dsdDomCorr = DualStateDiagramPainter(painter, rectDSDDomCorr);
        auto valCD = visual->m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::CorrectDominanceUid);
        dsdDomCorr.setValue(valCD);
        dsdDomCorr.setDescriptionLeft(tr("Быстрые коррекции"));
        dsdDomCorr.setDescriptionRight(tr("Медленные коррекции"));
        dsdDomCorr.doPaint(ratio);
    }

    //! Нижний колонтитул
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void StepOffsetVisualize::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, StepOffsetCalculator *calculator)
{
    Q_UNUSED(testUid);

    painter->save();

    int size = 0;
    int z = 6;
    if (rect.width() > rect.height())
        size = (rect.height() - z) / 2;
    else
        size = (rect.width() - z) / 2;

    //! Значения показателей
    painter->setFont(QFont("Sans", 7, QFont::Bold, false));
    painter->setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));

    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::ReactionTimeUid);
    double v = calculator->factorValue(StepOffsetFactorsDefines::Compensation::ReactionTimeUid);
    painter->drawText(rect.x() + 4, rect.y() + 7, fi.shortName() + ", " + fi.measure());
    painter->drawText(rect.x() + 100, rect.y() + 7, QString::number(v, 'f', fi.format()));

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::StatismUid);
    v = calculator->factorValue(StepOffsetFactorsDefines::Compensation::StatismUid);
    painter->drawText(rect.x() + 4, rect.y() + 17, fi.shortName() + ", " + fi.measure());
    painter->drawText(rect.x() + 100, rect.y() + 17, QString::number(v, 'f', fi.format()));

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::ProcessKindUid);
    v = calculator->factorValue(StepOffsetFactorsDefines::Compensation::ProcessKindUid);
    painter->drawText(rect.x() + 4, rect.y() + 27, fi.shortName() + ", " + fi.measure());
    painter->drawText(rect.x() + 100, rect.y() + 27, QString::number(v, 'f', fi.format()));

    painter->restore();
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

    m_wgtProcess = ui->wgtProcess;
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

    m_mdlFactors = static_cast<QStandardItemModel*>(ui->tvFactors->model());
}

void StepOffsetVisualize::showParams()
{
    m_sForce = tr("Усилие") + " - " + QString::number(m_calculator->force()) + " " + tr("%");
    m_sDirection = tr("Направление") + " - " + BaseDefines::DirectionValueFBName.value(m_calculator->direction());
    m_sDeviation = tr("Отклонение") + " - " +
            QString::number(m_calculator->diap() * m_calculator->force() / 100) +
            " " + tr("мм");
    m_sRepeatCount = tr("Кол-во повторений") + " - " + QString::number(m_calculator->stepsCount());

    ui->lblForce->setText(m_sForce);
    ui->lblDirection->setText(m_sDirection);
    ui->lblDeviation->setText(m_sDeviation);
    ui->lblRepeatCount->setText(m_sRepeatCount);
}

void StepOffsetVisualize::showConslution()
{
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::ReactionTimeUid);
    double v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::ReactionTimeUid);
    m_sReactionTime = tr("Время реакции") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("сек");
    ui->lblReactionTime->setText(m_sReactionTime);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::StatismUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::StatismUid);
    m_sStatism = tr("Статизм") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblStatism->setText(m_sStatism);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::LatentUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::LatentUid);
    m_sLatent = tr("Латентный период") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("сек");
    ui->lblLatent->setText(m_sLatent);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::SpurtSpeedMMUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SpurtSpeedMMUid);
    m_sSpurtSpeed = tr("Скорость броска") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм/сек");
    ui->lblSpurtSpeed->setText(m_sSpurtSpeed);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::SpurtAmplUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SpurtAmplUid);
    m_sSpurtAmpl = tr("Амплитуда броска") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblSpurtAmpl->setText(m_sSpurtAmpl);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::StabilityDeviationUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::StabilityDeviationUid);
    m_sStabilityDeviation = tr("Разброс на этапе стабилизации") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblStabilityDeviation->setText(m_sStabilityDeviation);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid);
    m_sRetentionDeviation = tr("Разброс на этапе удержания") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblRetentionDeviation->setText(m_sRetentionDeviation);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::CorrectKognErrorUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::CorrectKognErrorUid);
    m_sPrecisionKogn = tr("Точность выполнения когнитивным сигналом") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblPrecisionKogn->setText(m_sPrecisionKogn);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::CorrectMotorErrorUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::CorrectMotorErrorUid);
    m_sPrecisionMotor = tr("Точность моторной коррекции") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("мм");
    ui->lblPrecisionMotor->setText(m_sPrecisionMotor);

    fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::ProcessKindUid);
    v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::ProcessKindUid);
    m_sProcessKind = tr("Тип переходного процесса") + " : " + QString::number(v, 'f', fi.format());
    ui->lblProcessKind->setText(m_sProcessKind);
}

void StepOffsetVisualize::showConslutionStrategy()
{
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(StepOffsetFactorsDefines::Compensation::CorrectDominanceUid);
    double v = m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::CorrectDominanceUid);
    m_sCorrectionPredominace = tr("Преобладание коррекций") + " : " + QString::number(v, 'f', fi.format()) + " " + tr("%");
    ui->lblCorrectionPredominace->setText(m_sCorrectionPredominace);

    QString resume = "";
    QString colorResume = "";
    BaseUtils::setCorrectionsDominanceResume(v, resume, colorResume);
    m_sCorrectionResume = resume;
    ui->lblCorrectionResume->setText(resume);
    ui->lblCorrectionResume->setStyleSheet(colorResume);
    QPalette pal = ui->lblCorrectionResume->palette();
    m_sCorrectionResumeColorTrain = pal.color(QPalette::WindowText);

    ui->wgtCorrectionDiag->setValue(v);
    ui->wgtCorrectionDiag->setDescriptionLeft(tr("Быстрые коррекции"));
    ui->wgtCorrectionDiag->setDescriptionRight(tr("Медленные коррекции"));
    m_wgtCorrectionDiagTrain = ui->wgtCorrectionDiag;
}

void StepOffsetVisualize::printTransistents(TransientsPainter* tp, StepOffsetVisualize* visual)
{
    int min = qMin(visual->m_calculator->bufferCompensationCount(), visual->m_calculator->bufferReturnCount());
    tp->setFreq(visual->m_calculator->freq());

    //! Время броска от стимула = время броска + время размаха.
    //! Если время размаха == 0, то время броска + латентный период.
    double spurtTime = visual->m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SpurtTimeUid) +
                       visual->m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SwingTimeUid);
    if (visual->m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SwingTimeUid) - 0 < 1e-10)
        spurtTime = spurtTime + visual->m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::LatentUid);
    tp->setParams(StepOffsetDefines::stgCompensaton,
                              visual->m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::LatentUid),
                              visual->m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::SwingTimeUid),
                              spurtTime,
                              visual->m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::ReactionTimeUid),
                              visual->m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::StatismUid),
                              visual->m_calculator->factorValue(StepOffsetFactorsDefines::Compensation::RetentionDeviationUid));

    spurtTime = visual->m_calculator->factorValue(StepOffsetFactorsDefines::Return::SpurtTimeUid) +
                           visual->m_calculator->factorValue(StepOffsetFactorsDefines::Return::SwingTimeUid);
        if (visual->m_calculator->factorValue(StepOffsetFactorsDefines::Return::SwingTimeUid) - 0 < 1e-10)
            spurtTime = spurtTime + visual->m_calculator->factorValue(StepOffsetFactorsDefines::Return::LatentUid);
    tp->setParams(StepOffsetDefines::stgReturn,
                              visual->m_calculator->factorValue(StepOffsetFactorsDefines::Return::LatentUid),
                              visual->m_calculator->factorValue(StepOffsetFactorsDefines::Return::SwingTimeUid),
                              spurtTime,
                              visual->m_calculator->factorValue(StepOffsetFactorsDefines::Return::ReactionTimeUid),
                              visual->m_calculator->factorValue(StepOffsetFactorsDefines::Return::StatismUid),
                              visual->m_calculator->factorValue(StepOffsetFactorsDefines::Return::RetentionDeviationUid));

    tp->beginAddValues();
    for (int i = 0; i < min; ++i)
    {
        tp->setCompensationValue(visual->m_calculator->bufferCompensationValue(i));
        tp->setReturnValue(visual->m_calculator->bufferReturnValue(i));
    }
    tp->endAddValues();
}
