#include "signalanalysiswidget.h"
#include "ui_signalanalysiswidget.h"

#include <QDebug>

#include "aanalyserapplication.h"
#include "settingsprovider.h"

SignalAnalysisWidget::SignalAnalysisWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::SignalAnalysisWidget)
{
    ui->setupUi(this);

    restoreSplitterPosition();
}

SignalAnalysisWidget::~SignalAnalysisWidget()
{
    delete ui;
}

QString SignalAnalysisWidget::uid()
{
    return ClientWidgets::uidSignalAnalysisWidgetUid;
}

QString SignalAnalysisWidget::name()
{
    return tr("Анализ сигналов");
}

void SignalAnalysisWidget::onShow()
{
    auto ti = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedTest();
    if (ti.uid != "")
        openTest(ti.uid);
}

void SignalAnalysisWidget::onHide()
{
}

void SignalAnalysisWidget::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void SignalAnalysisWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("SignalAnalysisWidget", "SplitterPosition", ui->splitter->saveState());
}

void SignalAnalysisWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("SignalAnalysisWidget", "SplitterPosition").toByteArray();
    if (val == "")
        val = QByteArray::fromRawData("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x01\x1B\x00\x00\x03""b\x01\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x01\x00", 31);
    ui->splitter->restoreState(val);
}

void SignalAnalysisWidget::openTest(const QString uid)
{

}

void SignalAnalysisWidget::closeTest(const QString uid)
{

}
