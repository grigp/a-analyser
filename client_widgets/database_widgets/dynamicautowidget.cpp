#include "dynamicautowidget.h"
#include "ui_dynamicautowidget.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "datadefines.h"
#include "dynamicdatamodel.h"

#include <QSettings>
#include <QDebug>

DynamicAutoWidget::DynamicAutoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DynamicAutoWidget)
  , m_mdlDynamic(new DynamicDataModel(this))
{
    ui->setupUi(this);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
            &AAnalyserApplication::selectPatient,
            this, &DynamicAutoWidget::on_selectPatient);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
            &AAnalyserApplication::selectMetodic,
            this, &DynamicAutoWidget::on_selectMetodic);
}

DynamicAutoWidget::~DynamicAutoWidget()
{
    delete ui;
}

void DynamicAutoWidget::onDbConnect()
{
    ui->tvFactors->viewport()->installEventFilter(this);
    restoreDynamicKind();
    restoreDynamicVolume();
}

bool DynamicAutoWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tvFactors->viewport())
    {
        if (event->type() == QEvent::Paint)
        {
            // Приводит к частым срабатываниям
            if (ui->tvFactors->selectionModel()
                    && ui->tvFactors->selectionModel()->currentIndex().isValid())
                showGraph(ui->tvFactors->selectionModel()->currentIndex().row());
        }
    }
    return false;
}

void DynamicAutoWidget::on_selectPatient(const QString &patientUid)
{
    if (m_selectedPatientUid != patientUid)
    {
        m_selectedPatientUid = patientUid;
        buildDynamic();
    }
}

void DynamicAutoWidget::on_selectMetodic(const QString &metodicUid)
{
    if (m_selectedMetodicUid != metodicUid)
    {
        m_selectedMetodicUid = metodicUid;
        buildDynamic();
    }
}

void DynamicAutoWidget::selectFactor(const QModelIndex index)
{
    if (index.isValid())
        showGraph(index.row());
}

void DynamicAutoWidget::dynamicAsGraph()
{
    ui->wgtDynamic->setKind(DynamicDiagram::KindGraph);
    ui->btnBar->setChecked(false);
    saveDynamicKind(static_cast<int>(DynamicDiagram::KindGraph));
}

void DynamicAutoWidget::dynamicAsBar()
{
    ui->wgtDynamic->setKind(DynamicDiagram::KindBar);
    ui->btnGraph->setChecked(false);
    saveDynamicKind(static_cast<int>(DynamicDiagram::KindBar));
}

void DynamicAutoWidget::dynamic3D(bool pressed)
{
    if (pressed)
    {
        ui->wgtDynamic->setVolume(DynamicDiagram::Volume3D);
        saveDynamicVolume(DynamicDiagram::Volume3D);
    }
    else
    {
        ui->wgtDynamic->setVolume(DynamicDiagram::Volume2D);
        saveDynamicVolume(DynamicDiagram::Volume2D);
    }
}

void DynamicAutoWidget::buildDynamic()
{
    m_mdlDynamic->clear();
    ui->wgtDynamic->clear();

    if (m_selectedPatientUid != "" && m_selectedMetodicUid != "")
    {
        //! Построение списка
        fillTable();

        //! Прорисовка
        showDynamic();
    }
}

void DynamicAutoWidget::fillTable()
{
    m_selectRow = -1;
    auto tests = DataProvider::getTests();
    foreach (auto testUid, tests)
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(testUid, ti))
        {
            if (ti.patientUid == m_selectedPatientUid &&
                ti.metodUid == m_selectedMetodicUid)
            {
                auto factors = DataProvider::getPrimaryFactors(testUid);
                m_mdlDynamic->addTestData(factors, ti.dateTime);
            }
        }
    }
}

void DynamicAutoWidget::showDynamic()
{
    ui->tvFactors->setModel(m_mdlDynamic);
    for (int i = 0; i < m_mdlDynamic->columnCount(); ++i)
        ui->tvFactors->resizeColumnToContents(i);
    if (m_mdlDynamic->rowCount() > 0 && m_mdlDynamic->columnCount() > 0)
    {
        ui->tvFactors->setCurrentIndex(m_mdlDynamic->index(0, 0));
        showGraph(0);
    }
    else
        ui->wgtDynamic->clear();
}

void DynamicAutoWidget::showGraph(const int row)
{
    if (row == m_selectRow)
        return;

    ui->wgtDynamic->clear();
    auto factorUid = m_mdlDynamic->item(row, 0)->data(DynamicDataModel::FactorUidRole).toString();
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factorUid);
    QString title = fi.name();
    if (fi.measure() != "")
        title = title + ", " + fi.measure();
    ui->wgtDynamic->setTitle(title);

    m_selectRow = row;
    for (int i = 1; i < m_mdlDynamic->columnCount(); ++i)
    {
        auto value = m_mdlDynamic->item(row, i)->data(DynamicDataModel::ValueRole).toDouble();
        auto dt = m_mdlDynamic->horizontalHeaderItem(i)->data(DynamicDataModel::DateTimeRole).toDateTime();
        auto di = new DiagItem(value, dt.toString("dd.MM.yyyy hh:mm"));
        ui->wgtDynamic->appendItem(di);
    }
}

void DynamicAutoWidget::saveDynamicKind(const int kindCode) const
{
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    set.beginGroup("DynamicWidget");
    set.setValue("DynamicKind", kindCode);
    set.endGroup();
}

void DynamicAutoWidget::restoreDynamicKind()
{
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    set.beginGroup("DynamicWidget");
    auto kindCode = set.value("DynamicKind").toInt();
    set.endGroup();

    DynamicDiagram::Kind kind = static_cast<DynamicDiagram::Kind>(kindCode);
    ui->wgtDynamic->setKind(kind);
    ui->btnGraph->setChecked(kind == DynamicDiagram::KindGraph);
    ui->btnBar->setChecked(kind == DynamicDiagram::KindBar);
}

void DynamicAutoWidget::saveDynamicVolume(const int volumeCode) const
{
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    set.beginGroup("DynamicWidget");
    set.setValue("DynamicVolume", volumeCode);
    set.endGroup();
}

void DynamicAutoWidget::restoreDynamicVolume()
{
    QSettings set(QApplication::instance()->organizationName(),
                  QApplication::instance()->applicationName());
    set.beginGroup("DynamicWidget");
    auto volumeCode = set.value("DynamicVolume").toInt();
    set.endGroup();

    DynamicDiagram::Volume volume = static_cast<DynamicDiagram::Volume>(volumeCode);
    ui->wgtDynamic->setVolume(volume);
    ui->btn3D->setChecked(volume == DynamicDiagram::Volume3D);
}
