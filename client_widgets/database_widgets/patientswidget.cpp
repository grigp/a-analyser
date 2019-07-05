#include "patientswidget.h"
#include "ui_patientswidget.h"

#include "datadefines.h"
#include "patientsmodel.h"
#include "patientsproxymodel.h"
#include "patientkarddialog.h"
#include "aanalyserapplication.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>

PatientsWidget::PatientsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientsWidget)
  , m_mdlPatients(new PatientsModel(this))
  , m_pmdlPatients(new PatientsProxyModel(this))
{
    ui->setupUi(this);

    m_pmdlPatients->setSourceModel(m_mdlPatients);
    ui->tvPatients->setModel(m_pmdlPatients);
    ui->tvPatients->viewport()->installEventFilter(this);
    ui->tvPatients->sortByColumn(PatientsModel::ColFio, Qt::AscendingOrder);
}

PatientsWidget::~PatientsWidget()
{
    delete ui;
}

void PatientsWidget::onDbConnect()
{
    if (m_mdlPatients)
        m_mdlPatients->load();
    ui->tvPatients->header()->resizeSections(QHeaderView::ResizeToContents);
//    ui->tvPatients->header()->resizeSection(PatientsModel::ColFio, 200);
//    ui->tvPatients->header()->resizeSection(PatientsModel::ColBorn, 120);
    //    ui->tvPatients->header()->resizeSection(PatientsModel::colSex, 50);
}

bool PatientsWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tvPatients->viewport())
    {
        if (event->type() == QEvent::Paint)
        {
            // Приводит к частым срабатываниям
            selectPatient(ui->tvPatients->selectionModel()->currentIndex());
        }
    }
    return false;
}

void PatientsWidget::selectPatient(const QModelIndex index)
{
    if (index.isValid())
    {
        auto uid = m_mdlPatients->index(index.row(), PatientsModel::ColFio, index.parent()).
                data(PatientsModel::PatientUidRole).toString();
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient(uid);
    }
}

void PatientsWidget::addPatient()
{
    auto *dialog = new PatientKardDialog(this);
    connect(dialog, &PatientKardDialog::accepted, this, [=]()
    {
        DataDefines::PatientKard patient;
        patient.fio = dialog->fio();
        patient.born = dialog->born();
        patient.sex = dialog->sex();
        m_mdlPatients->addPatient(patient);
        ui->tvPatients->header()->resizeSections(QHeaderView::ResizeToContents);
    });
    dialog->show();
}

void PatientsWidget::editPatient()
{
    QString uid = "";
    QString fio = "";
    QDate born = QDate();
    DataDefines::Sex sex = DataDefines::male;
    QModelIndexList selIdxs = ui->tvPatients->selectionModel()->selectedIndexes();
    foreach (auto idx, selIdxs)
    {
        if (idx.column() == PatientsModel::ColFio)
        {
            uid = idx.data(PatientsModel::PatientUidRole).toString();
            fio = idx.data().toString();
        }
        else
        if (idx.column() == PatientsModel::ColBorn)
            born = QDate::fromString(idx.data().toString(), "dd.MM.yyyy");
        else
        if (idx.column() == PatientsModel::colSex)
            sex = static_cast<DataDefines::Sex>(idx.data(PatientsModel::PatientSexRole).toInt());
    }

    if (uid != "")
    {
        auto *dialog = new PatientKardDialog(this);
        dialog->setFio(fio);
        dialog->setBorn(born);
        dialog->setSex(sex);

        connect(dialog, &PatientKardDialog::accepted, this, [=]()
        {
            DataDefines::PatientKard patient;
            patient.uid = uid;
            patient.fio = dialog->fio();
            patient.born = dialog->born();
            patient.sex = dialog->sex();
            m_mdlPatients->addPatient(patient);
            ui->tvPatients->header()->resizeSections(QHeaderView::ResizeToContents);
        });
        dialog->show();
    }
}

void PatientsWidget::removePatient()
{
    QString uid = "";
    QString fio = "";
    QModelIndexList selIdxs = ui->tvPatients->selectionModel()->selectedIndexes();
    foreach (auto idx, selIdxs)
        if (idx.column() == PatientsModel::ColFio)
        {
            uid = idx.data(PatientsModel::PatientUidRole).toString();
            fio = idx.data().toString();
        }

    if (uid != "" &&
        QMessageBox::question(this, "Запрос", QString("Удалить запись о пациенте %1?").arg(fio)) == QMessageBox::Yes)
    {
        m_mdlPatients->removePatient(uid);
        ui->tvPatients->header()->resizeSections(QHeaderView::ResizeToContents);
    }
}

void PatientsWidget::unselect()
{
    ui->tvPatients->selectionModel()->clear();
    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient("");
}
