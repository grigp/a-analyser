#include "databaseresultwidget.h"
#include "ui_databaseresultwidget.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "settingsprovider.h"

#include <QDebug>

DataBaseResultWidget::DataBaseResultWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::DataBaseResultWidget)
{
    ui->setupUi(this);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectTest,
            this, &DataBaseResultWidget::onSelectTest);

    restoreSplittersPosition();
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


