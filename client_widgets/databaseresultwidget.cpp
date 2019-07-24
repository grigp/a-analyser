#include "databaseresultwidget.h"
#include "ui_databaseresultwidget.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"

#include <QSettings>
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
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    set.beginGroup("DataBaseResultWidget");
    set.setValue("SplitterVPosition", ui->splitter->saveState());
    set.setValue("SplitterHPosition", ui->splitter_2->saveState());
    set.endGroup();
}

void DataBaseResultWidget::restoreSplittersPosition()
{
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    set.beginGroup("DataBaseResultWidget");
    auto valV = set.value("SplitterVPosition");
    auto valH = set.value("SplitterHPosition");
    set.endGroup();
    ui->splitter->restoreState(valV.toByteArray());
    ui->splitter_2->restoreState(valH.toByteArray());
}


