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
    ui->tvPatients->sortByColumn(PatientsModel::ColFio, Qt::AscendingOrder);

    connect(ui->tvPatients->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &PatientsWidget::on_selectionChanged);
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

void PatientsWidget::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    selectPatient(ui->tvPatients->selectionModel()->currentIndex());
}

void PatientsWidget::selectPatient(const QModelIndex index)
{
    if (index.isValid())
    {
        auto srcIndex = m_pmdlPatients->mapToSource(index);
        auto uid = m_mdlPatients->index(srcIndex.row(), PatientsModel::ColFio, srcIndex.parent()).
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
        QMessageBox::question(this, tr("Запрос"), QString(tr("Удалить запись о пациенте") + " %1?").arg(fio))
            == QMessageBox::Yes)
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
