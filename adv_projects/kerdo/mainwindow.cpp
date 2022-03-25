#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>

#include "kerdoapplication.h"
#include "patientkarddialog.h"
#include "patientsmodel.h"
#include "patientsproxymodel.h"
#include "dataprovider.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  , m_mdlPatients(new PatientsModel(this))
  , m_pmdlPatients(new PatientsProxyModel(this))
{
    ui->setupUi(this);

    m_pmdlPatients->setSourceModel(m_mdlPatients);
    ui->tvMans->setModel(m_pmdlPatients);
    ui->tvMans->sortByColumn(PatientsModel::ColFio, Qt::AscendingOrder);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onDbConnected()
{
    if (m_mdlPatients)
        m_mdlPatients->load();

}

void MainWindow::obDbDisconnected()
{

}

void MainWindow::addMan()
{
    auto *dialog = new PatientKardDialog(this);
    connect(dialog, &PatientKardDialog::accepted, this, [=]()
    {
        DataDefines::PatientKard patient;
        patient.fio = dialog->fio();
        patient.born = dialog->born();
        patient.sex = dialog->sex();
        auto uidNew = m_mdlPatients->addPatient(patient);
        ui->tvMans->header()->resizeSections(QHeaderView::ResizeToContents);

        //! Выделение добавленной записи
        for (int i = 0; i < m_mdlPatients->rowCount(); ++i)
        {
            auto index = m_mdlPatients->index(i, 0);
            auto idx = m_pmdlPatients->mapFromSource(index);
            if (idx.isValid())
            {
                QString uid = idx.data(PatientsModel::PatientsModelRoles::PatientUidRole).toString();
                if (uid == uidNew)
                {
                    ui->tvMans->selectionModel()->select(idx, QItemSelectionModel::Select);
                    ui->tvMans->scrollTo(idx);
                    static_cast<KerdoApplication*>(QApplication::instance())->doSelectPatient(uid);
                    break;
                }
            }
        }
    });
    dialog->show();
}

void MainWindow::delMan()
{
    QString uid = "";
    QString fio = "";
    QModelIndexList selIdxs = ui->tvMans->selectionModel()->selectedIndexes();
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
        ui->tvMans->header()->resizeSections(QHeaderView::ResizeToContents);
    }
}

void MainWindow::editMan()
{
    QString uid = "";
    QString fio = "";
    QDate born = QDate();
    DataDefines::Sex sex = DataDefines::male;
    QModelIndexList selIdxs = ui->tvMans->selectionModel()->selectedIndexes();
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

    DataDefines::PatientKard pi;
    if (uid != "" && DataProvider::getPatient(uid, pi))
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
            ui->tvMans->header()->resizeSections(QHeaderView::ResizeToContents);
        });
        dialog->show();
    }
}

void MainWindow::selectPatient(QModelIndex index)
{
    auto uid = index.model()->index(index.row(), 0, index.parent()).data(PatientsModel::PatientUidRole).toString();
    auto tests = DataProvider::getTests(uid);
    qDebug() << tests;
}
