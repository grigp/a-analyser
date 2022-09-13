#include "spectrparamsdialog.h"
#include "ui_spectrparamsdialog.h"

#include <QDebug>

SpectrParamsDialog::SpectrParamsDialog(const int size, const int freq, QWidget *parent) :
    QDialog(parent)
  , ui(new Ui::SpectrParamsDialog)
  , m_size(size)
  , m_freq(freq)
{
    ui->setupUi(this);

    assignPointsCount();
    m_freqPrev = ui->edMaxFrequency->value() / 2;
    ui->lblSignalLength->setText(tr("Кол-во точек сигнала") + " - " + QString::number(m_size));
    ui->lblFreqSample->setText(tr("Частота дискретизации сигнала, Гц") + " - " + QString::number(m_freq));
}

SpectrParamsDialog::~SpectrParamsDialog()
{
    delete ui;
}

int SpectrParamsDialog::points() const
{
    return ui->cbPointsCount->currentData().toInt();
}

void SpectrParamsDialog::setPoints(const int points)
{
    ui->cbPointsCount->setCurrentText(QString::number(points));
}

bool SpectrParamsDialog::isAveraging() const
{
    return ui->cbAveraging->isChecked();
}

void SpectrParamsDialog::setIsAveraging(const bool ia)
{
    ui->cbAveraging->setChecked(ia);
}

int SpectrParamsDialog::averagingOffset() const
{
    return ui->edAveragingOffset->value();
}

void SpectrParamsDialog::setAveragingOffset(const int offs)
{
    ui->edAveragingOffset->setValue(offs);
}

double SpectrParamsDialog::maxFrequency() const
{
    return ui->edMaxFrequency->value();
}

void SpectrParamsDialog::setMaxFrequency(const double maxFreq)
{
    ui->edMaxFrequency->setValue(maxFreq);
}

QJsonObject SpectrParamsDialog::params() const
{
    QJsonObject retval;
    retval["points"] = points();
    retval["is_averaging"] = isAveraging();
    retval["averaging_offset"] = averagingOffset();
    retval["max_frequency"] = maxFrequency();
    return retval;
}

void SpectrParamsDialog::setParams(const QJsonObject params)
{
    setPoints(params["points"].toInt());
    setIsAveraging(params["is_averaging"].toBool());
    setAveragingOffset(params["averaging_offset"].toInt());
    setMaxFrequency(params["max_frequency"].toDouble());
}

void SpectrParamsDialog::on_averagingClicked(bool val)
{
    ui->lblAveragingOffset->setEnabled(val);
    ui->edAveragingOffset->setEnabled(val);
}

void SpectrParamsDialog::on_changeMaxFrequencyVal(double val)
{
    if (val > m_freq / 2)
        ui->edMaxFrequency->setValue(m_freqPrev);
    else
        m_freqPrev = val;
}

void SpectrParamsDialog::assignPointsCount()
{
    int i = 128;
    while (i <= qMin(m_size, 2048))
    {

        ui->cbPointsCount->addItem(QString::number(i), i);
        i *= 2;
    }
    if (m_size >= 1024)
        ui->cbPointsCount->setCurrentText("1024");
    else
        ui->cbPointsCount->setCurrentIndex(ui->cbPointsCount->count() - 1);
}
