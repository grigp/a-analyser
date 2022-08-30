#include "sectiongraphvisualwidget.h"
#include "ui_sectiongraphvisualwidget.h"

#include <QMessageBox>
#include <QDebug>

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "anysignal.h"
#include "baseutils.h"
#include "selecttransformerdialog.h"

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
