#include "bedsidescalestestvisualize.h"
#include "ui_bedsidescalestestvisualize.h"

#include <QFileDialog>
#include <QTextStream>
#include <QDebug>

#include "bedsidescalestestcalculator.h"
#include "baseutils.h"
#include "datadefines.h"

BedsideScalesTestVisualize::BedsideScalesTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BedsideScalesTestVisualize)
{
    ui->setupUi(this);

    connect(ui->wgtWeighingResults, &DynamicDiagram::selectItem, this, &BedsideScalesTestVisualize::on_selectItem);
}

BedsideScalesTestVisualize::~BedsideScalesTestVisualize()
{
    delete ui;
}

void BedsideScalesTestVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new BedsideScalesTestCalculator(testUid, this);
        m_calculator->calculate();

        int secCnt = m_calculator->signalSize() / m_calculator->signalFrequency();
        auto sSize = BaseUtils::getTimeBySecCount(secCnt, true);
        ui->lblSignalSize->setText(tr("Длительность сигнала") + " : " + sSize);

        ui->frWeighingResults->setVisible(m_calculator->isParticalWeighting());

        showDiagParticalWeighting();
    }
}

void BedsideScalesTestVisualize::print(QPrinter *printer, const QString &testUid)
{
    Q_UNUSED(printer);
    Q_UNUSED(testUid);
}

void BedsideScalesTestVisualize::signalExport()
{
    if (m_calculator)
    {
        QString path = DataDefines::aanalyserDocumentsPath();
        auto fileName = QFileDialog::getSaveFileName(this, tr("Файл для экспорта сигнала"), path, tr("Текстовые файлы (*.txt)"));
        if (fileName != "")
            saveSignal(fileName);
    }
}

void BedsideScalesTestVisualize::on_weighingResultsGraph()
{
    ui->wgtWeighingResults->setKind(DynamicDiagram::KindGraph);
    ui->btnBar->setChecked(false);
}

void BedsideScalesTestVisualize::on_weighingResultsBar()
{
    ui->wgtWeighingResults->setKind(DynamicDiagram::KindBar);
    ui->btnGraph->setChecked(false);
}

void BedsideScalesTestVisualize::on_weighingResults3D(bool is3D)
{
    if (is3D)
        ui->wgtWeighingResults->setVolume(DynamicDiagram::Volume3D);
    else
        ui->wgtWeighingResults->setVolume(DynamicDiagram::Volume2D);
}

void BedsideScalesTestVisualize::on_selectItem(const int idx)
{
    if (idx >= 0 && idx < m_calculator->size())
    {
        auto value = m_calculator->weight(idx);
        auto dt = m_calculator->dateTime(idx);
        ui->wgtWeighingResults->setBottomText(tr("Измерение") + (" (") + QString::number(idx + 1) + ") : (" +
                                              dt.toString("dd.MM.yyyy hh:mm:ss") + ") - " +
                                              QString::number(value) + " " + tr("кг"));
//        ui->wgtWeighingResults->setBottomText(QString(tr("Тест") + " " + sdt + " - " + QString::number(value)));
    }
}

void BedsideScalesTestVisualize::saveSignal(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream ts(&file);

        for (int i = 0; i < m_calculator->signalSize(); ++i)
        {
            QString line = "";
            for (int j = 0; j < m_calculator->subChansCount(); ++j)
            {
                auto s = QString::number(m_calculator->signalValue(j, i));
                QString sSp = "";
                for (int k = 0; k < 20 - s.size(); ++k)
                    sSp += " ";
                line += s + sSp;
            }

            ts << line + "\n";
        }

        file.close();
    }
}

void BedsideScalesTestVisualize::showDiagParticalWeighting()
{
    for (int i = 0; i < m_calculator->size(); ++i)
    {
        auto value = m_calculator->weight(i);
        auto di = new DiagItem(value, QString::number(i + 1)); //dt.toString("dd.MM.yyyy hh:mm"));
        ui->wgtWeighingResults->appendItem(di);
    }

}

