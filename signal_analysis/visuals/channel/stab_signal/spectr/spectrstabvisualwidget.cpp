#include "spectrstabvisualwidget.h"
#include "ui_spectrstabvisualwidget.h"

#include <QStandardItemModel>
#include <QDebug>

#include "aanalyserapplication.h"
#include "channelsdefines.h"
#include "channelsutils.h"
#include "spectrstabfactors.h"
#include "settingsprovider.h"
#include "dataprovider.h"
#include "stabilogram.h"

SpectrStabVisualWidget::SpectrStabVisualWidget(VisualDescriptor* visual,
                                               const QString& testUid, const QString& probeUid, const QString& channelId,
                                               QWidget *parent) :
    ChannelVisual(visual, testUid, probeUid, channelId, parent),
    ui(new Ui::SpectrStabVisualWidget)
{
    ui->setupUi(this);

    connect(ui->wgtSpectrX, &DiagSpectr::press, this, &SpectrStabVisualWidget::on_press);
    connect(ui->wgtSpectrX, &DiagSpectr::release, this, &SpectrStabVisualWidget::on_release);
    connect(ui->wgtSpectrX, &DiagSpectr::move, this, &SpectrStabVisualWidget::on_move);
    connect(ui->wgtSpectrY, &DiagSpectr::press, this, &SpectrStabVisualWidget::on_press);
    connect(ui->wgtSpectrY, &DiagSpectr::release, this, &SpectrStabVisualWidget::on_release);
    connect(ui->wgtSpectrY, &DiagSpectr::move, this, &SpectrStabVisualWidget::on_move);

    QCursor cursor = QCursor(QPixmap(":/images/CursorArea.png"));
    ui->wgtSpectrX->setCursor(cursor);
    ui->wgtSpectrY->setCursor(cursor);
}

SpectrStabVisualWidget::~SpectrStabVisualWidget()
{
    if (m_factors)
        delete m_factors;
    delete ui;
}

bool SpectrStabVisualWidget::isValid()
{
    return ChannelsUtils::instance().channelType(channelId()) == ChannelsDefines::ctStabilogram;
}

void SpectrStabVisualWidget::calculate()
{
    m_factors = new SpectrStabFactors(testUid(), probeUid(), channelId());
    showTable();
    showSpectrs();
    restoreSplitterPosition();
}

void SpectrStabVisualWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void SpectrStabVisualWidget::on_press(const int x, const int y)
{
    m_selectionProcess = true;
    m_selFrom = QPoint(x, y);

    m_selectAreaWidget = static_cast<DiagSpectr*>(sender());
    m_selAreaX = x;
    m_selAreaY = y;
}

void SpectrStabVisualWidget::on_release(const int x, const int y)
{
    Q_UNUSED(x);
    Q_UNUSED(y);
    if (m_selectionProcess)
    {
        if (m_selTo.x() > m_selFrom.x() && m_selTo.y() > m_selFrom.y())
        {
            auto from = static_cast<DiagSpectr*>(sender())->getValues(m_selFrom);
            auto to = static_cast<DiagSpectr*>(sender())->getValues(m_selTo);
            static_cast<DiagSpectr*>(sender())->setVisualArea(from.x(), to.x(), to.y(), from.y());
        }
        else
        if (m_selTo.x() < m_selFrom.x() && m_selTo.y() < m_selFrom.y())
            static_cast<DiagSpectr*>(sender())->resetVisualArea();

        selectionReset();
    }
}

void SpectrStabVisualWidget::on_move(const int x, const int y)
{
    if (m_selectionProcess)
    {
        m_selTo = QPoint(x, y);
        if (m_selTo.x() > m_selFrom.x() && m_selTo.y() > m_selFrom.y())
            m_selectAreaWidget->selectArea(QRect(m_selFrom, m_selTo));
    }
}

void SpectrStabVisualWidget::showTable()
{
    auto *model = new QStandardItemModel();

    int cnt = m_factors->size() / 2;
    for (int i = 0; i < cnt; ++i)
    {
        auto uidX = m_factors->factorUid(i);
        auto uidY = m_factors->factorUid(cnt + i);

        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(uidX);
        QString fn = fi.name();
        auto sl = fn.split('(');
        if (sl.size() > 0)
            fn = sl.at(0);
        if (fi.measure() != "")
            fn = fn + ", " + fi.measure();
        auto* itemName = new QStandardItem(fn);
        itemName->setEditable(false);

        auto fvfX = m_factors->factorValueFormatted(uidX);
        auto* itemValueX = new QStandardItem(fvfX);
        itemValueX->setEditable(false);

        auto fvfY = m_factors->factorValueFormatted(uidY);
        auto* itemValueY = new QStandardItem(fvfY);
        itemValueY->setEditable(false);

        model->appendRow(QList<QStandardItem*>() << itemName << itemValueX << itemValueY);
    }

    model->setHorizontalHeaderLabels(QStringList() << tr("Показатель") << tr("Фронталь") << tr("Сагитталь"));
    ui->tvFactors->setModel(model);
    ui->tvFactors->header()->resizeSection(0, 500);
    ui->tvFactors->header()->resizeSection(1, 200);
    ui->tvFactors->header()->resizeSection(2, 200);
}

void SpectrStabVisualWidget::showSpectrs()
{   
    ui->wgtSpectrX->setTitle(tr("Фронталь X"));
    if (m_factors->channelsCount() == 2)
    {
        for (int i = 0; i < m_factors->points(); ++i)
            ui->wgtSpectrX->addValue(m_factors->value(0, i));
        ui->wgtSpectrX->setFormatData(computeMaxFreq(), 6.1);
//        ui->wgtSpectrX->setFormatData(m_factors->frequency(), 6.1);

        ui->wgtSpectrY->setTitle(tr("Сагитталь Y"));
        for (int i = 0; i < m_factors->points(); ++i)
            ui->wgtSpectrY->addValue(m_factors->value(1, i));
        ui->wgtSpectrY->setFormatData(computeMaxFreq(), 6.1);
//        ui->wgtSpectrY->setFormatData(m_factors->frequency(), 6.1);
    }
    ui->lblNoCalculated->setVisible(m_factors->channelsCount() != 2);
    ui->frControl->setVisible(m_factors->channelsCount() == 2);
    ui->splVertical->setVisible(m_factors->channelsCount() == 2);

    ui->wgtSpectrX->addFreqArea(0, 0.2, "1", QColor(200, 210, 210), QColor(30, 120, 120));
    ui->wgtSpectrX->addFreqArea(0.2, 2, "2", QColor(220, 230, 230), QColor(50, 140, 140));
    ui->wgtSpectrX->addFreqArea(2, 6, "3", QColor(240, 250, 250), QColor(80, 160, 160));
    ui->wgtSpectrX->addFreqLabel(m_factors->factorValue(SpectrStabFactorsDefines::Frontal::Level60Uid), "60%");
    ui->wgtSpectrY->addFreqArea(0, 0.2, "1", QColor(200, 210, 210), QColor(30, 120, 120));
    ui->wgtSpectrY->addFreqArea(0.2, 2, "2", QColor(220, 230, 230), QColor(50, 140, 140));
    ui->wgtSpectrY->addFreqArea(2, 6, "3", QColor(240, 250, 250), QColor(80, 160, 160));
    ui->wgtSpectrY->addFreqLabel(m_factors->factorValue(SpectrStabFactorsDefines::Sagittal::Level60Uid), "60%");
}

int SpectrStabVisualWidget::computeMaxFreq()
{
    int frequency = m_factors->frequency();
    QByteArray baStab;
    if (DataProvider::getChannel(probeUid(), channelId(), baStab))
    {
        Stabilogram stab(baStab);
        frequency = static_cast<int>((1 / (static_cast<double>(stab.size()) / static_cast<double>(stab.frequency()))) *
                                     (static_cast<double>(m_factors->points())));
    }
    return frequency;
}

void SpectrStabVisualWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("Geometry/SpectrStabVisualWidget", "HSplitterPosition", ui->splHorizontal->saveState());
    SettingsProvider::setValueToRegAppCopy("Geometry/SpectrStabVisualWidget", "VSplitterPosition", ui->splVertical->saveState());
}

void SpectrStabVisualWidget::restoreSplitterPosition()
{
    ui->splVertical->setSizes(QList<int>() << geometry().height() / 10 * 7 << geometry().height() / 10 * 3);

    auto val = SettingsProvider::valueFromRegAppCopy("Geometry/SpectrStabVisualWidget", "HSplitterPosition").toByteArray();
    ui->splHorizontal->restoreState(val);
    val = SettingsProvider::valueFromRegAppCopy("Geometry/SpectrStabVisualWidget", "VSplitterPosition").toByteArray();
    ui->splVertical->restoreState(val);
}

void SpectrStabVisualWidget::selectionReset()
{
    m_selectionProcess = false;
    m_selFrom = QPoint(-1, -1);
    m_selTo = QPoint(-1, -1);
    m_selectAreaWidget->selectArea(QRect());
}
