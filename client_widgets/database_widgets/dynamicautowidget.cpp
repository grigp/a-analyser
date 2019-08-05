#include "dynamicautowidget.h"
#include "ui_dynamicautowidget.h"

#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "datadefines.h"
#include "dynamicdatamodel.h"

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
    ui->treeView->viewport()->installEventFilter(this);
}

bool DynamicAutoWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->treeView->viewport())
    {
        if (event->type() == QEvent::Paint)
        {
            // Приводит к частым срабатываниям
            if (ui->treeView->selectionModel() &&
                ui->treeView->selectionModel()->currentIndex().isValid() &&
                ui->treeView->selectionModel()->currentIndex().row() != m_selectRow)
                showGraph(ui->treeView->selectionModel()->currentIndex().row());
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

void DynamicAutoWidget::buildDynamic()
{
    if (m_selectedPatientUid != "" && m_selectedMetodicUid != "")
    {
        m_mdlDynamic->clear();

        qDebug() << m_selectedPatientUid << m_selectedMetodicUid;
        //! Построение списка
        fillTable();

        //! Прорисовка
        showDynamic();
    }
    else
        hideDynamic();
}

void DynamicAutoWidget::hideDynamic()
{
    m_mdlDynamic->clear();
}

void DynamicAutoWidget::fillTable()
{
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
    ui->treeView->setModel(m_mdlDynamic);
    for (int i = 0; i < m_mdlDynamic->columnCount(); ++i)
        ui->treeView->resizeColumnToContents(i);
    if (m_mdlDynamic->rowCount() > 0 && m_mdlDynamic->columnCount() > 0)
    {
        ui->treeView->setCurrentIndex(m_mdlDynamic->index(0, 0));
        showGraph(0);
    }
}

void DynamicAutoWidget::showGraph(const int row)
{
    m_selectRow = row;
    QString line = "";
    for (int i = 0; i < m_mdlDynamic->columnCount(); ++i)
    {
        line = line + m_mdlDynamic->item(row, i)->data(Qt::DisplayRole).toString() + "   ";
    }
    qDebug() << line;
}
