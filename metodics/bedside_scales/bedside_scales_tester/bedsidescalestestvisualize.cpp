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

