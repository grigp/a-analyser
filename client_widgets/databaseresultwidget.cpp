#include "databaseresultwidget.h"
#include "ui_databaseresultwidget.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "settingsprovider.h"
#include "aanalysersettings.h"

#include <QDebug>

DataBaseResultWidget::DataBaseResultWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::DataBaseResultWidget)
{
    ui->setupUi(this);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectTest,
            this, &DataBaseResultWidget::onSelectTest);

    restoreSplittersPosition();

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::applicationParamChanged,
            this, &DataBaseResultWidget::on_applicationParamChanged);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::methodicCount,
            this, &DataBaseResultWidget::on_methodicCount);
}

DataBaseResultWidget::~DataBaseResultWidget()
{
    delete ui;
}

QString DataBaseResultWidget::uid()
{
    return ClientWidgets::uidDatabaseResultWidgetUid;
}

QString DataBaseResultWidget::name()
{
    return tr("База данных");
}

void DataBaseResultWidget::onDBConnect()
{
    ui->wgtPatients->onDbConnect();
    ui->wgtMetods->onDbConnect();
    ui->wgtTests->onDbConnect();
}

void DataBaseResultWidget::onDBDisconnect()
{
    ui->wgtTests->onDbDisconnect();
}

void DataBaseResultWidget::onShow()
{
    ui->wgtPatients->onShow();
    ui->wgtMetods->onShow();
}

void DataBaseResultWidget::onHide()
{
    ui->wgtPatients->onHide();
}

void DataBaseResultWidget::onSelectTest(const QString &testUid)
{
    Q_UNUSED(testUid);
}

void DataBaseResultWidget::splitterVMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplittersPosition();
}

void DataBaseResultWidget::splitterHMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplittersPosition();
}

void DataBaseResultWidget::on_applicationParamChanged(const QString &group, const QString &param, const QVariant &value)
{
    if (group == "" && param == AAnalyserSettingsParams::pn_onePatientMode)
    {
        ui->wgtPatientMet->setVisible(m_metCnt > 1 || !value.toBool());
    }
}

void DataBaseResultWidget::on_methodicCount(const int count)
{
    m_metCnt = count;
    auto iop = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_onePatientMode, static_cast<QVariant>(false)).toBool();
    ui->wgtPatientMet->setVisible(m_metCnt > 1 || !iop);
}

void DataBaseResultWidget::saveSplittersPosition()
{
    SettingsProvider::setValueToRegAppCopy("DataBaseResultWidget", "SplitterVPosition", ui->splitter->saveState());
    SettingsProvider::setValueToRegAppCopy("DataBaseResultWidget", "SplitterHPosition", ui->splitter_2->saveState());
}

void DataBaseResultWidget::restoreSplittersPosition()
{
    auto valV = SettingsProvider::valueFromRegAppCopy("DataBaseResultWidget", "SplitterVPosition").toByteArray();
    auto valH = SettingsProvider::valueFromRegAppCopy("DataBaseResultWidget", "SplitterHPosition").toByteArray();
    ui->splitter->restoreState(valV);
    ui->splitter_2->restoreState(valH);
}


