#include "histogramvisualwidget.h"
#include "ui_histogramvisualwidget.h"

#include <QDir>
#include <QDebug>

#include "baseutils.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "dynamicdiagram.h"
#include "dataprovider.h"
#include "stabilogram.h"

#include "computefft.h"

HistogramVisualWidget::HistogramVisualWidget(VisualDescriptor* visual,
                                             const QString& testUid, const QString& probeUid, const QString& channelUid,
                                             QWidget *parent) :
    ChannelVisual (visual, testUid, probeUid, channelUid, parent),
    ui(new Ui::HistogramVisualWidget)
{
    ui->setupUi(this);

    connect(ui->wgtHistX, &DynamicDiagram::selectItem, this, &HistogramVisualWidget::on_selectItem);
    connect(ui->wgtHistY, &DynamicDiagram::selectItem, this, &HistogramVisualWidget::on_selectItem);
}

HistogramVisualWidget::~HistogramVisualWidget()
{
    delete ui;
}

bool HistogramVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelUid()) == ChannelsDefines::ctStabilogram;
}

void HistogramVisualWidget::calculate()
{
    computeHist();
    showHist();
}

void HistogramVisualWidget::on_selectItem(const int idx)
{
    auto value = static_cast<DynamicDiagram*>(sender())->value(idx);
    static_cast<DynamicDiagram*>(sender())->setBottomText(QString::number(value) + " %");
}

void HistogramVisualWidget::computeHist()
{
    spectrTest();

    QByteArray data;
    if (DataProvider::getChannel(probeUid(), channelUid(), data))
    {
        auto stab = new Stabilogram(data);

        //! Минимум и максимум
        for (int i = 0; i < stab->size(); ++i)
        {
            auto rec = stab->value(i);
            if (rec.x < m_minX) m_minX = rec.x;
            if (rec.x > m_maxX) m_maxX = rec.x;
            if (rec.y < m_minY) m_minY = rec.y;
            if (rec.y > m_maxY) m_maxY = rec.y;
        }
        //! Размер зон по X и Y
        m_sizeOneX = (m_maxX - m_minX) / static_cast<double>(HistBarsCount);
        m_sizeOneY = (m_maxY - m_minY) / static_cast<double>(HistBarsCount);

        //! Инициализация гистограммы
        for (int i = 0; i < HistBarsCount; ++i)
        {
            m_dataX[i] = 0;
            m_dataY[i] = 0;
        }
        //! Расчет гистограммы
        for (int i = 0; i < stab->size(); ++i)
        {
            auto rec = stab->value(i);

            int idx = static_cast<int>((rec.x - m_minX) / m_sizeOneX);
            if (idx < 0) idx = 0;
            if (idx >= HistBarsCount) idx = HistBarsCount - 1;
            ++m_dataX[idx];

            idx = static_cast<int>((rec.y - m_minY) / m_sizeOneY);
            if (idx < 0) idx = 0;
            if (idx >= HistBarsCount) idx = HistBarsCount - 1;
            ++m_dataY[idx];
        }

        for (int i = 0; i < HistBarsCount; ++i)
        {
            m_dataX[i] = m_dataX[i] / stab->size() * 100;
            m_dataY[i] = m_dataY[i] / stab->size() * 100;
        }
    }
}

void HistogramVisualWidget::showHist()
{
    ui->wgtHistX->setKind(DynamicDiagram::KindBar);
    ui->wgtHistX->setVolume(DynamicDiagram::Volume3D);
    ui->wgtHistX->setTitle(tr("Фронталь"));
    ui->wgtHistX->setAxisSpaceLeft(30);
    ui->wgtHistX->setSizeDivider(2);

    ui->wgtHistY->setKind(DynamicDiagram::KindBar);
    ui->wgtHistY->setVolume(DynamicDiagram::Volume3D);
    ui->wgtHistY->setTitle(tr("Сагитталь"));
    ui->wgtHistY->setAxisSpaceLeft(30);
    ui->wgtHistY->setSizeDivider(2);

    for (int i = 0; i < HistBarsCount; ++i)
    {
        auto itemX = new DiagItem(m_dataX[i], QString::number(m_minX + i * m_sizeOneX, 'f', 2));
        ui->wgtHistX->appendItem(itemX);

        auto itemY = new DiagItem(m_dataY[i], QString::number(m_minY + i * m_sizeOneY, 'f', 2));
        ui->wgtHistY->appendItem(itemY);
    }
}

void HistogramVisualWidget::spectrTest()
{

    QVector<double> data;
    double r = 0;
    double r1 = 0;
    for (int i = 0; i < ComputeFFT::FFT_COUNT * 2; ++i)
    {
//        data << static_cast<double>(100/100);
//        data << static_cast<double>(qrand() % 1000) / 100.0 - 5;
//        data << 10 * sin(r); // + 5 * sin(r1);

        if (i < ComputeFFT::FFT_COUNT)
            data << 10 * sin(r); // + 5 * sin(r1);
        else
            data << 5 * sin(r1);

        r += (2 * M_PI) / 51.2;
        r1 += (2 * M_PI) / 25.6;
    }
//    QString path = DataDefines::aanalyserDocumentsPath() + "Export/";
//    QDir dir(path);
//    if (!dir.exists())
//        dir.mkpath(path);
//    BaseUtils::vectorToText(data, path + "fft_source.txt");

//    ComputeFFT::baseFFT(data);
    QVector<double> spectr;
    ComputeFFT::extendFFT(data, spectr, ComputeFFT::FFT_COUNT, ComputeFFT::FFT_COUNT - 1);

//    BaseUtils::vectorToText(spectr, path + "fft_result.txt");
//    BaseUtils::vectorToText(data, DataDefines::aanalyserDocumentsPath() + "Export/fft_result.txt");
}

