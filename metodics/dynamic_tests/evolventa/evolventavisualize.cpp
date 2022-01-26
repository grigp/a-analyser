#include "evolventavisualize.h"
#include "ui_evolventavisualize.h"

#include <QStandardItemModel>
#include <QFileDialog>
#include <QDebug>

#include "aanalyserapplication.h"
#include "evolventacalculator.h"
#include "decartcoordinatessignal.h"
#include "evolventafactors.h"
#include "reportelements.h"
#include "basedefines.h"
#include "baseutils.h"

namespace
{

///< Список показателей, выводимых в таблицу основных показателей
///< Соответствие показателя для фронтали показателю для сагиттали
static QList<BaseDefines::FctTblPair> FactorsMain = {
      BaseDefines::FctTblPair(EvolventaFactorsDefines::CommonErrorsFrontal::SummErr, EvolventaFactorsDefines::CommonErrorsSagittal::SummErr)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::CommonErrorsFrontal::MidErr, EvolventaFactorsDefines::CommonErrorsSagittal::MidErr)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::MidErr, EvolventaFactorsDefines::FragmentExtremumSagittal::MidErr)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::MidErr, EvolventaFactorsDefines::FragmentBranchSagittal::MidErr)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::CrsCnt, EvolventaFactorsDefines::FragmentExtremumSagittal::CrsCnt)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::CrsCnt, EvolventaFactorsDefines::FragmentBranchSagittal::CrsCnt)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::ExtrCnt, EvolventaFactorsDefines::FragmentExtremumSagittal::ExtrCnt)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::ExtrCnt, EvolventaFactorsDefines::FragmentBranchSagittal::ExtrCnt)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::MidAmpl, EvolventaFactorsDefines::FragmentExtremumSagittal::MidAmpl)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::MidAmpl, EvolventaFactorsDefines::FragmentBranchSagittal::MidAmpl)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::CMidErr, EvolventaFactorsDefines::FragmentExtremumSagittal::CMidErr)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::CMidErr, EvolventaFactorsDefines::FragmentBranchSagittal::CMidErr)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentExtremumFrontal::CMidAmp, EvolventaFactorsDefines::FragmentExtremumSagittal::CMidAmp)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::FragmentBranchFrontal::CMidAmp, EvolventaFactorsDefines::FragmentBranchSagittal::CMidAmp)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::Frontal::InterCntR, EvolventaFactorsDefines::Sagittal::InterCntR)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::Frontal::InterLenR, EvolventaFactorsDefines::Sagittal::InterLenR)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::Frontal::SemiWavLen, EvolventaFactorsDefines::Sagittal::SemiWavLen)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::Frontal::SemiWavAmpl, EvolventaFactorsDefines::Sagittal::SemiWavAmpl)
};

static QList<BaseDefines::FctTblPair> FactorsKorrect = {
      BaseDefines::FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::Percent, EvolventaFactorsDefines::CorrectionsKognitive::Percent)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::Error, EvolventaFactorsDefines::CorrectionsKognitive::Error)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::Amplitude, EvolventaFactorsDefines::CorrectionsKognitive::Amplitude)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::TimeSumm, EvolventaFactorsDefines::CorrectionsKognitive::TimeSumm)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::TimeMid, EvolventaFactorsDefines::CorrectionsKognitive::TimeMid)
    , BaseDefines::FctTblPair(EvolventaFactorsDefines::CorrectionsMotor::Power, EvolventaFactorsDefines::CorrectionsKognitive::Power)
};

AreaGraph *wgtGraphX {nullptr};
AreaGraph *wgtGraphY {nullptr};
QStandardItemModel *mdlMain {nullptr};
QStandardItemModel *mdlKorrect {nullptr};
DualStateDiagram *wgtOutrunningDiag {nullptr};
QLabel *lblOutrunningValue {nullptr};
QLabel *lblOutrunningResume {nullptr};
DualStateDiagram *wgtCorrectionDominanceDiag {nullptr};
QLabel *lblCorrectionDominanceValue {nullptr};
QLabel *lblCorrectionDominanceResume {nullptr};

}

EvolventaVisualize::EvolventaVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EvolventaVisualize)
{
    ui->setupUi(this);
}

EvolventaVisualize::~EvolventaVisualize()
{
    delete ui;
}

void EvolventaVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new EvolventaCalculator(testUid, this);
        m_calculator->calculate();

        showEvolventa();
        showMainFactorsTable();
        showKorrectionFactorsTable();
        showWithoutTableFactors();
    }

    setTableSpecialStyle();
}

void EvolventaVisualize::print(QPrinter *printer, const QString &testUid)
{
    QPainter *painter = new QPainter(printer);
    QRect paper = printer->pageRect();

    painter->begin(printer);
    //! Заголовок
    QRect rectHeader(paper.x() + paper.width() / 20, paper.y() + paper.height() / 30, paper.width() / 20 * 18, paper.height() / 30 * 3);
    ReportElements::drawHeader(painter, testUid, rectHeader);
    //! Зона нижнего колонтитула
    QRect rectFooter(paper.x() + paper.width() / 20,
                     paper.y() + paper.height() - static_cast<int>(paper.height() / 30 * 1.5),
                     paper.width() / 20 * 18,
                     static_cast<int>(paper.height() / 30 * 1.5));

    if (printer->orientation() == QPrinter::Portrait)
    {
        //! Графики эвольвенты
        ReportElements::drawWidget(painter, wgtGraphX,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.4),
                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
        ReportElements::drawWidget(painter, wgtGraphY,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.4),
                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 10 * 2.7));


        //! Таблица основных показателей
        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10 * 4.2),
                        paper.width() / 10 * 9,
                        paper.height() / 3);
        ReportElements::drawTable(painter, mdlMain, rectTable,
                                  QList<int>() << 4 << 1 << 1, ReportElements::Table::tvsStretched,
                                  10, -1, QFont::Bold);

        //! Опережение / отставание от цели
        ReportElements::drawWidget(painter, wgtOutrunningDiag,
                                   static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.1),
                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 10 * 7.4));
        painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        QPalette pal = lblOutrunningValue->palette();
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 10 * 8.1), lblOutrunningValue->text());
        pal = lblOutrunningResume->palette();
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 10 * 8.3), lblOutrunningResume->text());

        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 2
        printer->newPage();

        //! Заголовок "Коррекции"
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->setPen(Qt::black);
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10), tr("Коррекции"));

        //! Диаграмма преобладания коррекций
        ReportElements::drawWidget(painter, wgtCorrectionDominanceDiag,
                                   static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.1),
                                   paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 1.2));
        painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        pal = lblCorrectionDominanceValue->palette();
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 1.9), lblCorrectionDominanceValue->text());
        pal = lblCorrectionDominanceResume->palette();
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 2.1), lblCorrectionDominanceResume->text());

        //! Таблица коррекций
        QRect rectTableKorrect(paper.x() + paper.width() / 10,
                               static_cast<int>(paper.y() + paper.height() / 10 * 2.7),
                               paper.width() / 10 * 9,
                               paper.height() / 3);
        ReportElements::drawTable(painter, mdlKorrect, rectTableKorrect,
                                  QList<int>() << 2 << 1 << 1, ReportElements::Table::tvsCompressed,
                                  10, -1, QFont::Bold);
    }
    else
    if (printer->orientation() == QPrinter::Landscape)
    {
        //! Графики эвольвенты
        ReportElements::drawWidget(painter, wgtGraphX,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.3),
                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 10 * 1.5));
        ReportElements::drawWidget(painter, wgtGraphY,
                                   static_cast<int>(paper.width() * 0.85), static_cast<int>(paper.height() * 0.3),
                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 10 * 3.9));

        //! Опережение / отставание от цели
        ReportElements::drawWidget(painter, wgtOutrunningDiag,
                                   static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.1),
                                   paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 10 * 6.5));
        painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        QPalette pal = lblOutrunningValue->palette();
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 10 * 7.8), lblOutrunningValue->text());
        pal = lblOutrunningResume->palette();
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(paper.x() + paper.width()/10, static_cast<int>(paper.y() + paper.height() / 10 * 8.2), lblOutrunningResume->text());


        //! Нижний колонтитул
        ReportElements::drawFooter(painter, testUid, rectFooter);

        //!------------------- Страница 2
        printer->newPage();

        //! Таблица основных показателей
        QRect rectTable(paper.x() + paper.width() / 10,
                        static_cast<int>(paper.y() + paper.height() / 10),
                        paper.width() / 10 * 9,
                        paper.height() / 3);
        ReportElements::drawTable(painter, mdlMain, rectTable,
                                  QList<int>() << 4 << 1 << 1, ReportElements::Table::tvsStretched,
                                  10, -1, QFont::Bold);

        //! Заголовок "Коррекции"
        painter->setFont(QFont("Sans", 12, QFont::Bold, false));
        painter->setPen(Qt::black);
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 5.3), tr("Коррекции"));

        //! Диаграмма преобладания коррекций
        ReportElements::drawWidget(painter, wgtCorrectionDominanceDiag,
                                   static_cast<int>(paper.width() * 0.4), static_cast<int>(paper.height() * 0.1),
                                   paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 5.6));
        painter->setFont(QFont("Sans", 10, QFont::Bold, false));
        pal = lblCorrectionDominanceValue->palette();
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 6.8), lblCorrectionDominanceValue->text());
        pal = lblCorrectionDominanceResume->palette();
        painter->setPen(pal.color(QPalette::WindowText));
        painter->drawText(paper.x() + paper.width() / 10, static_cast<int>(paper.y() + paper.height() / 10 * 7.1), lblCorrectionDominanceResume->text());

        //! Таблица коррекций
        QRect rectTableKorrect(static_cast<int>(paper.x() + paper.width() / 10 * 5.5),
                               static_cast<int>(paper.y() + paper.height() / 10 * 5.6),
                               paper.width() / 10 * 4,
                               paper.height() / 3);
        ReportElements::drawTable(painter, mdlKorrect, rectTableKorrect,
                                  QList<int>() << 2 << 1 << 1, ReportElements::Table::tvsCompressed,
                                  10, -1, QFont::Bold);
    }

    //! Нижний колонтитул
    ReportElements::drawFooter(painter, testUid, rectFooter);

    painter->end();
}

void EvolventaVisualize::saveFactorsCorrections()
{
    QString path = DataDefines::aanalyserDocumentsPath();
    auto fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Файл для экспорта таблицы"),
                                                 path,
                                                 tr("Файлы с разделенными значениями *.csv (*.csv)"));
    if (fileName != "")
        BaseUtils::modelToText(ui->tvEvolvKorrectionFactors->model(), fileName);
}

void EvolventaVisualize::saveEvolventa()
{
    QString path = DataDefines::aanalyserDocumentsPath();
    auto fileName = QFileDialog::getSaveFileName(this,
                                                 tr("Файл для экспорта эвольвенты"),
                                                 path,
                                                 tr("текстовые файлы *.txt (*.txt)"));
    if (fileName != "")
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream ts(&file);
            QStringList sl;

            sl.clear();
            sl << "Stab_X" << "Stab_Y" << "Evolv_X" << "Evolv_Y";
            ts << sl.join("             ") + "\n";

            int size = m_calculator->frontal()->size();
            if (m_calculator->sagittal()->size() < size)
                size = m_calculator->sagittal()->size();

            for (int i = 0; i < size; ++i)
            {
                double x = m_calculator->frontal()->value(0, i);
                double tx = m_calculator->frontal()->value(1, i);
                double y = m_calculator->sagittal()->value(0, i);
                double ty = m_calculator->sagittal()->value(1, i);

                sl.clear();
                sl << QString::number(x) << QString::number(y) << QString::number(tx) << QString::number(ty);
                ts << sl.join("             ") + "\n";
            }


            file.close();
        }
    }
}

void EvolventaVisualize::showEvolventa()
{
    auto *sigX = m_calculator->frontal();
    double max = sigX->absMaxValue();
    ui->wgtGraphX->appendSignal(sigX, tr("Фронталь"));
    ui->wgtGraphX->setLegend(0, QStringList() << "Траектория" << "Цель");
    ui->wgtGraphX->setDiapazone(0, -max, max);
    ui->wgtGraphX->addMarker(0, m_calculator->timeUpwinding(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgUpwinding));
    ui->wgtGraphX->addMarker(0, m_calculator->timeHold(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgHold));
    ui->wgtGraphX->addMarker(0, m_calculator->timeConvolution(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgConvolution));
    ui->wgtGraphX->setIsFillBetweenSubchans(0, true);
    wgtGraphX = ui->wgtGraphX;

    auto *sigY = m_calculator->sagittal();
    max = sigY->absMaxValue();
    ui->wgtGraphY->appendSignal(sigY, tr("Сагитталь"));
    ui->wgtGraphY->setLegend(0, QStringList() << "Траектория" << "Цель");
    ui->wgtGraphY->setDiapazone(0, -max, max);
    ui->wgtGraphY->addMarker(0, m_calculator->timeUpwinding(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgUpwinding));
    ui->wgtGraphY->addMarker(0, m_calculator->timeHold(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgHold));
    ui->wgtGraphY->addMarker(0, m_calculator->timeConvolution(), EvolventaDefines::StageValueName.value(EvolventaDefines::stgConvolution));
    ui->wgtGraphY->setIsFillBetweenSubchans(0, true);
    wgtGraphY = ui->wgtGraphY;
}

void EvolventaVisualize::showMainFactorsTable()
{
    auto *model = new QStandardItemModel(ui->tvEvolvMainFactors);

    foreach (auto uids, FactorsMain)
    {
        auto uidFctX = uids.first;
        auto uidFctY = uids.second;

        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidFctX);
        QString fn = fi.name();
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto *itemName = new QStandardItem(fn);
        itemName->setEditable(false);
        auto *itemX = new QStandardItem(m_calculator->factorValueFormatted(uidFctX));
        itemX->setEditable(false);
        auto *itemY = new QStandardItem(m_calculator->factorValueFormatted(uidFctY));
        itemY->setEditable(false);
        model->appendRow(QList<QStandardItem*>() << itemName << itemX << itemY);

    }
    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Фронталь") << tr("Сагитталь"));
    ui->tvEvolvMainFactors->setModel(model);
    ui->tvEvolvMainFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvEvolvMainFactors->header()->resizeSection(0, 370);

    mdlMain = static_cast<QStandardItemModel*>(ui->tvEvolvMainFactors->model());
}

void EvolventaVisualize::showKorrectionFactorsTable()
{
    auto *model = new QStandardItemModel(ui->tvEvolvKorrectionFactors);

    foreach (auto uids, FactorsKorrect)
    {
        auto uidFctM = uids.first;
        auto uidFctK = uids.second;

        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidFctM);
        QString fn = fi.name();
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto *itemName = new QStandardItem(fn);
        itemName->setEditable(false);
        auto *itemM = new QStandardItem(m_calculator->factorValueFormatted(uidFctM));
        itemM->setEditable(false);
        auto *itemK = new QStandardItem(m_calculator->factorValueFormatted(uidFctK));
        itemK->setEditable(false);
        model->appendRow(QList<QStandardItem*>() << itemName << itemM << itemK);

    }
    model->setHorizontalHeaderLabels(QStringList() << tr("Длительность") << tr("100-160 мс") << tr("200-280 мс"));
    ui->tvEvolvKorrectionFactors->setModel(model);
    ui->tvEvolvKorrectionFactors->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvEvolvKorrectionFactors->header()->resizeSection(0, 200);

    mdlKorrect = static_cast<QStandardItemModel*>(ui->tvEvolvKorrectionFactors->model());
}

void EvolventaVisualize::setTableSpecialStyle()
{
    QFile styleSheet(":/qss/evolv.qss");
    styleSheet.open(QFile::ReadOnly);
    QByteArray ba = styleSheet.readAll();
    ui->tvEvolvMainFactors->setStyleSheet(ba);
    ui->tvEvolvKorrectionFactors->setStyleSheet(ba);
    styleSheet.close();
}

void EvolventaVisualize::showWithoutTableFactors()
{
    auto valKorCount = m_calculator->factorValueFormatted(EvolventaFactorsDefines::KorrCount);
    ui->lblCorrectionsCount->setText(tr("Общее количество коррекций") + " - " + valKorCount);

    auto valOrvF = m_calculator->factorValueFormatted(EvolventaFactorsDefines::DAPercent);
    auto valOrv = m_calculator->factorValue(EvolventaFactorsDefines::DAPercent);
    ui->lblOutrunningValue->setText(tr("Опережение маркера цели") + " : " + valOrvF + "%");
    ui->wgtOutrunningDiag->setValue(valOrv);
    QString resume = "";
    QString resumeColor = "";
    BaseUtils::setOutrunningResume(valOrv, resume, resumeColor);
    ui->lblOutrunningResume->setText(resume);
    ui->lblOutrunningResume->setStyleSheet(resumeColor);
    wgtOutrunningDiag = ui->wgtOutrunningDiag;
    lblOutrunningValue = ui->lblOutrunningValue;
    lblOutrunningResume = ui->lblOutrunningResume;

    auto valCD = m_calculator->factorValue(EvolventaFactorsDefines::KorrDominance);
    ui->lblCorrectionDominanceValue->setText(tr("Преобладание коррекций") + " : " + QString::number(valCD, 'f', 0) + "%");
    ui->wgtCorrectionDominanceDiag->setValue(valCD);
    resume = "";
    resumeColor = "";
    BaseUtils::setCorrectionsDominanceResume(valCD, resume, resumeColor);
    ui->lblCorrectionDominanceResume->setText(resume);
    ui->lblCorrectionDominanceResume->setStyleSheet(resumeColor);
    wgtCorrectionDominanceDiag = ui->wgtCorrectionDominanceDiag;
    lblCorrectionDominanceValue = ui->lblCorrectionDominanceValue;
    lblCorrectionDominanceResume = ui->lblCorrectionDominanceResume;


    auto valSvl = m_calculator->factorValueFormatted(EvolventaFactorsDefines::SemiWavLenDAC);
    ui->lblSemiWaveLen->setText(tr("Длительность полуволны при опережении") + " " + valSvl + " " + tr("сек"));
    ui->lblSemiWaveLen->setStyleSheet("font-size: 8pt;");
    auto valSva = m_calculator->factorValueFormatted(EvolventaFactorsDefines::SemiWavAmplDAC);
    ui->lblSemiWaveAmpl->setText(tr("Амплитуда полуволны при опережении") + " " + valSva + " " + tr("мм"));
    ui->lblSemiWaveAmpl->setStyleSheet("font-size: 8pt;");
}

