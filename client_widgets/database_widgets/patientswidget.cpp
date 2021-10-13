#include "patientswidget.h"
#include "ui_patientswidget.h"

#include "datadefines.h"
#include "patientsmodel.h"
#include "patientsproxymodel.h"
#include "patientkarddialog.h"
#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "aanalysersettings.h"
#include "settingsprovider.h"

#include <QApplication>
#include <QMessageBox>
#include <QTimer>
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

//    ui->tvPatients->viewport()->installEventFilter(this);

    connect(ui->tvPatients->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &PatientsWidget::on_selectionChanged);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::applicationParamChanged,
            this, &PatientsWidget::on_applicationParamChanged);
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

    QTimer::singleShot(100, [=]()
    {
        m_isOnePatientMode = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_onePatientMode, static_cast<QVariant>(false)).toBool();
        m_onePatientFIO = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_onePatientFIO, static_cast<QVariant>(false)).toString();
        onePatientHandle();
    });

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

//void MethodsWidget::selectMetodic(const QModelIndex index)
//{
//    if (index.isValid())
//    {
//        auto uid = index.data(MetodicsModel::MetodicUidRole).toString();
//        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic(uid);
//    }
//    else
//        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic("");
//}


void PatientsWidget::addPatient()
{
    auto *dialog = new PatientKardDialog(this);
    connect(dialog, &PatientKardDialog::accepted, this, [=]()
    {
        DataDefines::PatientKard patient;
        patient.fio = dialog->fio();
        patient.born = dialog->born();
        patient.sex = dialog->sex();
        patient.massa = dialog->massa();
        patient.height = dialog->height();
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

    DataDefines::PatientKard pi;
    if (uid != "" && DataProvider::getPatient(uid, pi))
    {
        auto *dialog = new PatientKardDialog(this);
        dialog->setFio(fio);
        dialog->setBorn(born);
        dialog->setSex(sex);
        dialog->setMassa(pi.massa);
        dialog->setHeight(pi.height);

        connect(dialog, &PatientKardDialog::accepted, this, [=]()
        {
            DataDefines::PatientKard patient;
            patient.uid = uid;
            patient.fio = dialog->fio();
            patient.born = dialog->born();
            patient.sex = dialog->sex();
            patient.massa = dialog->massa();
            patient.height = dialog->height();
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
    ui->tvPatients->clearSelection();
    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient("");
}

void PatientsWidget::on_editSearchString(const QString &value)
{
    m_pmdlPatients->setFilterValue(value);
}

void PatientsWidget::on_applicationParamChanged(const QString &group, const QString &param, const QVariant &value)
{
    if (group == "" && param == AAnalyserSettingsParams::pn_onePatientMode)
    {
        m_isOnePatientMode = value.toBool();
        onePatientHandle();
    }
    if (group == "" && param == AAnalyserSettingsParams::pn_onePatientFIO)
    {
        m_onePatientFIO = value.toString();
        onePatientHandle();
    }
}

void PatientsWidget::onePatientHandle()
{
    setVisible(!m_isOnePatientMode);
    if (DataProvider::getPatients().size() == 0)
    {
        DataDefines::PatientKard patient;
        patient.fio = m_onePatientFIO;
        patient.born = QDate(2000, 1, 1);
        patient.sex = DataDefines::male;
        patient.massa = 80;
        patient.height = 170;
        m_mdlPatients->addPatient(patient);
    }
    else
    {
        auto idx = m_pmdlPatients->index(0, 0);
        auto uid = idx.data(PatientsModel::PatientUidRole).toString();
        auto fio = idx.data().toString();

        DataDefines::PatientKard patient;
        if (DataProvider::getPatient(uid, patient))
        {
            patient.fio = m_onePatientFIO;
            m_mdlPatients->addPatient(patient);
        }
    }
    selectPatient(m_pmdlPatients->index(0, 0));
}

//bool PatientsWidget::eventFilter(QObject *watched, QEvent *event)
//{
//    if (watched == ui->tvPatients->viewport() && event->type() == QEvent::MouseButtonRelease)
//    {
//        QMouseEvent *me = static_cast <QMouseEvent *> (event);
//        QModelIndex index = ui->tvPatients->indexAt(me->pos());

//        if (!index.isValid())
//        {
//            ui->tvPatients->clearSelection();
//            static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient("");
//        }

//        return true;
//    }

//    return QWidget::eventFilter(watched, event);
//}
