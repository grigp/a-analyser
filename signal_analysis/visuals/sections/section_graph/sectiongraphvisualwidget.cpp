#include "sectiongraphvisualwidget.h"
#include "ui_sectiongraphvisualwidget.h"

#include <QMessageBox>
#include <QDebug>

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "anysignal.h"
#include "baseutils.h"
#include "selecttransformerdialog.h"
#include "computefft.h"
#include "spectrparamsdialog.h"

SectionGraphVisualWidget::SectionGraphVisualWidget(VisualDescriptor* visual,
                                                   const QString& testUid,
                                                   const QString& probeUid,
                                                   const QString& channelId,
                                                   const QString& sectionNumber,
                                                   QWidget *parent) :
    SectionVisual(visual, testUid, probeUid, channelId, sectionNumber, parent),
    ui(new Ui::SectionGraphVisualWidget)
{
    ui->setupUi(this);
    ui->cbScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");

    ui->splHorizontal->setSizes(QList<int>() << 500 << 0);
}

SectionGraphVisualWidget::~SectionGraphVisualWidget()
{
    delete ui;
}

bool SectionGraphVisualWidget::isValid()
{
    return true;
}

void SectionGraphVisualWidget::calculate()
{
    QByteArray data;
    auto channelUid = DataProvider::getChannelUid(probeUid(), channelId());
    if (DataProvider::getSectionData(channelUid, sectionNumber(), data))
    {
        if (!m_signal)
        {
            m_signal = new AnySignal(data);
            ui->wgtGraph->setLine1Color(Qt::blue);
            ui->wgtGraph->setLine2Color(Qt::red);
            ui->wgtGraph->appendSignal(m_signal, tr(""));
            m_absMax = m_signal->absMaxValue();
            ui->wgtGraph->setDiapazone(-m_absMax, m_absMax);
            ui->cbScale->setCurrentIndex(0);
        }
    }

}

void SectionGraphVisualWidget::scaleChange(int idx)
{
    auto diap = BaseUtils::scaleMultiplier(idx);
    ui->wgtGraph->setDiapazone(-m_absMax/diap, m_absMax/diap);
}

void SectionGraphVisualWidget::btnFulSignalClicked(bool isFullSignal)
{
    ui->sbSignal->setEnabled(!isFullSignal);
    ui->btnHScalePlus->setEnabled(!isFullSignal);
    ui->btnHScaleMinus->setEnabled(!isFullSignal);
    if (isFullSignal)
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_fullSignal);
    else
        ui->wgtGraph->setXCoordSignalMode(AreaGraph::xsm_scrolling);
}

void SectionGraphVisualWidget::btnPlusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale < 8)
        ui->wgtGraph->setHScale(hScale * 2);
}

void SectionGraphVisualWidget::btnMinusClicked()
{
    auto hScale = ui->wgtGraph->hScale();
    if (hScale > 1)
        ui->wgtGraph->setHScale(hScale / 2);
}

void SectionGraphVisualWidget::signalScroll(int pos)
{
    int w = ui->wgtGraph->width() - ui->wgtGraph->leftSpace() - ui->wgtGraph->rightSpace();
    if (ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() > w)
    {
        int p = static_cast<int>((ui->wgtGraph->area(0)->signal()->size() * ui->wgtGraph->hScale() - w)
                                 * pos / 100.0 / ui->wgtGraph->hScale());
        ui->wgtGraph->setStartPoint(p);
    }
}

void SectionGraphVisualWidget::on_popupMenuRequested(QPoint pos)
{

}

void SectionGraphVisualWidget::on_createSection()
{

}

void SectionGraphVisualWidget::on_press(const int x, const int y, const Qt::MouseButtons buttons)
{

}

void SectionGraphVisualWidget::on_release(const int x, const int y, const Qt::MouseButtons buttons)
{

}

void SectionGraphVisualWidget::on_move(const int x, const int y, const Qt::MouseButtons buttons)
{

}

void SectionGraphVisualWidget::on_transform()
{
    SelectTransformerDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        QVector<double> buffer;
        for (int i = 0; i < m_signal->size(); ++i)
            buffer << m_signal->value(1, i);
        int freq = m_signal->frequency();

        auto params = dlg.params();
        params["freq_sample"] = freq;

        static_cast<AAnalyserApplication*>(QApplication::instance())->transformSignal(dlg.transformer(), buffer, params);

        auto *signal = new AnySignal(freq, 2);
        for (int i = 0; i < m_signal->size(); ++i)
        {
            QVector<double> rec;
            rec << m_signal->value(0, i) << buffer.at(i);
            signal->appendValue(rec);
        }

        ui->wgtGraph->clear();
        m_signal = signal;
        updateSectionData();
    }
}

void SectionGraphVisualWidget::on_revert()
{
    auto id = QMessageBox::question(nullptr, tr("Запрос"), tr("Отменить все преобразования?"));
    if (id == QMessageBox::Yes)
    {
        int freq = m_signal->frequency();
        auto *signal = new AnySignal(freq, 2);
        for (int i = 0; i < m_signal->size(); ++i)
        {
            QVector<double> rec;
            rec << m_signal->value(0, i) << m_signal->value(0, i);
            signal->appendValue(rec);
        }

        ui->wgtGraph->clear();
        m_signal = signal;
        updateSectionData();
    }

}

void SectionGraphVisualWidget::on_calculateSpectr()
{
    //! Диалог параметров спектра
    SpectrParamsDialog dlg(m_signal->size(), m_signal->frequency());
    if (dlg.exec() == QDialog::Accepted)
    {
        //! Буфера данных для спектра
        QVector<double> dataSrc;
        QVector<double> dataRes;

        //! Расчет спектра
        int frequency = m_signal->frequency();
        if (dlg.isAveraging())
            computeSpectrAveraging(dataSrc, dataRes, dlg.points(), dlg.averagingOffset());
        else
        {
            computeSpectrDecimation(dataSrc, dataRes, dlg.points());
            frequency = static_cast<int>(static_cast<double>(frequency) / (static_cast<double>(m_signal->size()) / static_cast<double>(dlg.points())));
        }

        //! Прорисовка спектра в одном диапазоне амплитуд
        double maxVS = 0;
        double maxVR = 0;
        showSpectr(ui->wgtSpectrSrc, dataSrc, maxVS, frequency, m_signal->size(), dlg.maxFrequency());
        showSpectr(ui->wgtSpectrRes, dataRes, maxVR, frequency, m_signal->size(), dlg.maxFrequency());
        double max = qMax(maxVS, maxVR);
        ui->wgtSpectrSrc->setVisualArea(0, dlg.maxFrequency(), 0, max);
        ui->wgtSpectrRes->setVisualArea(0, dlg.maxFrequency(), 0, max);

        //! Автоматически показать панель спектров
        ui->splHorizontal->setSizes(QList<int>() << 500 << 500);
    }
}

void SectionGraphVisualWidget::computeSpectrAveraging(QVector<double> &dataSrc, QVector<double> &dataRes,
                                                      const int points, const int offset)
{
    initData(dataSrc, points);
    initData(dataRes, points);

    int start = 0;
    int n = 0;
    do
    {
        QVector<double> ds;
        QVector<double> dr;

        for (int i = 0; i < points; ++i)
        {
            ds << m_signal->value(0, start + i);
            dr << m_signal->value(1, start + i);
        }

        ComputeFFT::baseFFT(ds);
        ComputeFFT::baseFFT(dr);

        for (int i = 0; i < points; ++i)
        {
            dataSrc[i] += ds[i];
            dataRes[i] += dr[i];
        }

        start += offset;
        ++n;
    }
    while(start + points < m_signal->size());

    if (n > 0)
        for (int i = 0; i < points; ++i)
        {
            dataSrc[i] /= n;
            dataRes[i] /= n;
        }
}

void SectionGraphVisualWidget::computeSpectrDecimation(QVector<double> &dataSrc, QVector<double> &dataRes, const int points)
{
    double prop = static_cast<double>(m_signal->size()) / static_cast<double>(points);
    double ir = 0.0;
    while (ir < m_signal->size())
    {
        int i = static_cast<int>(ir);
        dataSrc << m_signal->value(0, i);
        dataRes << m_signal->value(1, i);
        ir += prop;
    }

    ComputeFFT::baseFFT(dataSrc);
    ComputeFFT::baseFFT(dataRes);
}

void SectionGraphVisualWidget::initData(QVector<double> &data, const int size)
{
    data.clear();
    for (int i = 0; i < size; ++i)
        data << 0;
}

void SectionGraphVisualWidget::showSpectr(DiagSpectr *area, QVector<double> &data, double &maxV,
                                          const int freqSample, const int size, const double maxFreq)
{
    area->clear();
    for (int i = 0; i < data.size(); ++i)
    {
        area->addValue(data[i]);
        if (data[i] > maxV)
            maxV = data[i];
    }
    area->setFormatData(freqSample, size, maxFreq);
}

void SectionGraphVisualWidget::updateSectionData()
{
    ui->wgtGraph->appendSignal(m_signal, tr(""));
    ui->wgtGraph->setLine1Color(Qt::blue);
    ui->wgtGraph->setLine2Color(Qt::red);
    m_absMax = m_signal->absMaxValue();
    ui->wgtGraph->setDiapazone(-m_absMax, m_absMax);
    ui->cbScale->setCurrentIndex(0);

    QByteArray data;
    m_signal->toByteArray(data);
    auto channelUid = DataProvider::getChannelUid(probeUid(), channelId());
    DataProvider::updateSection(channelUid, sectionNumber(), data);
}
