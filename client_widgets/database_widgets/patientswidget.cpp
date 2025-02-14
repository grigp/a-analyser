#include "patientswidget.h"
#include "ui_patientswidget.h"

#include "aanalyserbuild.h"
#include "datadefines.h"
#include "patientsmodel.h"
#include "patientsproxymodel.h"
#include "patientkarddialog.h"
#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "aanalysersettings.h"
#include "settingsprovider.h"
#include "databasewigetdefines.h"
#include "personalprogramwidget.h"
#include "amessagebox.h"

#include <QApplication>
#include <QTimer>
#include <QUuid>
#include <QDebug>

PatientsWidget::PatientsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientsWidget)
{
    ui->setupUi(this);

    ui->tvPatients->setModel(patientsProxyModel());
    ui->tvPatients->sortByColumn(DatabaseWidgetDefines::PatientsModel::ColFio, Qt::AscendingOrder);

//    ui->tvPatients->viewport()->installEventFilter(this);

    connect(ui->tvPatients->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &PatientsWidget::on_selectionChanged);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::applicationParamChanged,
            this, &PatientsWidget::on_applicationParamChanged);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::assignedPPForPatient,
            this, &PatientsWidget::on_assignPPForPatient);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::canceledPPForPatient,
            this, &PatientsWidget::on_cancelPPForPatient);

    ui->tvPatients->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tvPatients, &QTreeView::customContextMenuRequested, this, &PatientsWidget::on_popupMenuRequested);
    connect(ui->tvPatients, &QTreeView::doubleClicked, this, &PatientsWidget::on_doubleClickedTable);
}

PatientsWidget::~PatientsWidget()
{
    delete ui;
}

void PatientsWidget::onDbConnect()
{
    ui->tvPatients->header()->resizeSections(QHeaderView::ResizeToContents);
//    ui->tvPatients->header()->resizeSection(PatientsModel::ColFio, 200);
//    ui->tvPatients->header()->resizeSection(PatientsModel::ColBorn, 120);
    //    ui->tvPatients->header()->resizeSection(PatientsModel::colSex, 50);

    QTimer::singleShot(100, [=]()
    {
        m_isOnePatientMode = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_onePatientMode, static_cast<QVariant>(false)).toBool();
        m_onePatientFIO = SettingsProvider::valueFromRegAppCopy("", AAnalyserSettingsParams::pn_onePatientFIO, static_cast<QVariant>(false)).toString();
        onePatientHandle();

        if (!AAnalyserBuild::isPPEnabled())
        {
            ui->btnViewModePersonalProgram->setVisible(false);
        }
    });

}

void PatientsWidget::onShow()
{
    //! uid выбранного в приложении пациента
    auto pk = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
    if (pk.uid == "")
        pk.uid = QUuid().toString();

    //! uid выбранного в таблице пациента
    QString patUid = QUuid().toString();
    auto selMod = ui->tvPatients->selectionModel();
    if (selMod)
    {
        QModelIndexList selIdxs = selMod->selectedIndexes();
        foreach (auto idx, selIdxs)
        {
            if (idx.column() == DatabaseWidgetDefines::PatientsModel::ColFio)
            {
                patUid = idx.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
            }
        }
    }

    //! Если в приложении выбран другой
    if (pk.uid != patUid)
    {
        //! При переключении к ИП надо забывать это выделение, а при вызове других окон - нет
        ui->tvPatients->selectionModel()->clearSelection();
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient("");
    }
}

void PatientsWidget::onHide()
{

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
         auto srcIndex = patientsProxyModel()->mapToSource(index);
         auto uid = patientsModel()->index(srcIndex.row(), DatabaseWidgetDefines::PatientsModel::ColFio, srcIndex.parent()).
                    data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
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
        auto uidNew = patientsModel()->addPatient(patient);
        ui->tvPatients->header()->resizeSections(QHeaderView::ResizeToContents);

        //! Выделение добавленной записи
        for (int i = 0; i < patientsModel()->rowCount(); ++i)
        {
            auto index = patientsModel()->index(i, 0);
            auto idx = patientsProxyModel()->mapFromSource(index);
            if (idx.isValid())
            {
                QString uid = idx.data(DatabaseWidgetDefines::PatientsModel::PatientsModelRoles::PatientUidRole).toString();
                if (uid == uidNew)
                {
                    ui->tvPatients->selectionModel()->select(idx, QItemSelectionModel::Select);
                    ui->tvPatients->scrollTo(idx);
                    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient(uid);
                    break;
                }
            }
        }
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
        if (idx.column() == DatabaseWidgetDefines::PatientsModel::ColFio)
        {
            uid = idx.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
            fio = idx.data().toString();
        }
        else
        if (idx.column() == DatabaseWidgetDefines::PatientsModel::ColBorn)
            born = QDate::fromString(idx.data().toString(), "dd.MM.yyyy");
        else
        if (idx.column() == DatabaseWidgetDefines::PatientsModel::colSex)
            sex = static_cast<DataDefines::Sex>(idx.data(DatabaseWidgetDefines::PatientsModel::PatientSexRole).toInt());
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

        auto objPP = DataProvider::getActivePersonalProgramForPatient(pi.uid);
        QPixmap ppPic("");
        if (objPP != QJsonObject())
        {
            auto ppName = objPP["pp"].toObject()["name"].toString();
            ppPic = QPixmap(objPP["pp"].toObject()["logo_file_name"].toString());
            dialog->setPersonalProgram(ppName, ppPic);
        }
        else
            dialog->setPersonalProgram(tr("Не задана"), ppPic);

        connect(dialog, &PatientKardDialog::accepted, this, [=]()
        {
            DataDefines::PatientKard patient;
            patient.uid = uid;
            patient.fio = dialog->fio();
            patient.born = dialog->born();
            patient.sex = dialog->sex();
            patient.massa = dialog->massa();
            patient.height = dialog->height();
            patientsModel()->addPatient(patient);
            static_cast<AAnalyserApplication*>(QApplication::instance())->doUpdatePatientData(uid);
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
        if (idx.column() == DatabaseWidgetDefines::PatientsModel::ColFio)
        {
            uid = idx.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
            fio = idx.data().toString();
        }

    if (uid != "" &&
        AMessageBox::question(this, tr("Запрос"), QString(tr("Удалить запись о пациенте") + " %1?").arg(fio))
            == AMessageBox::Yes)
    {
        patientsModel()->removePatient(uid);
        ui->tvPatients->header()->resizeSections(QHeaderView::ResizeToContents);
    }
}

void PatientsWidget::assignPersonalProgram()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->assignPPForPatient();
}

void PatientsWidget::cancelPersonalProgram()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->cancelPPForPatient();
}

void PatientsWidget::unselect()
{
    ui->tvPatients->clearSelection();
    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient("");
}

void PatientsWidget::on_editSearchString(const QString &value)
{
    patientsProxyModel()->setFilterValue(value);
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

void PatientsWidget::on_assignPPForPatient(const QString &patientUid, const QString& ppUid)
{
    for (int i = 0; i < patientsModel()->rowCount(); ++i)
    {
        auto item = patientsModel()->item(i);
        if (item->data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString() == patientUid)
            item->setData(ppUid, DatabaseWidgetDefines::PatientsModel::PatientPPUidRole);
    }
}

void PatientsWidget::on_cancelPPForPatient(const QString &patientUid)
{
    for (int i = 0; i < patientsModel()->rowCount(); ++i)
    {
        auto item = patientsModel()->item(i);
        if (item->data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString() == patientUid)
            item->setData("", DatabaseWidgetDefines::PatientsModel::PatientPPUidRole);
    }
}

void PatientsWidget::on_viewModePP()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->showClientPage(ClientWidgets::uidPersonalProgramWidgetUid);
    SettingsProvider::setValueToRegAppCopy("MainWindow", "MainClientWidget", ClientWidgets::uidPersonalProgramWidgetUid);
}

void PatientsWidget::on_popupMenuRequested(QPoint pos)
{
    if (!m_menu)
    {
        m_menu = new QMenu(this);

        QAction *addPatient = new QAction(QIcon(":/images/PatientAdd.png"), tr("Новый пациент..."), this);
        connect(addPatient, &QAction::triggered, this, &PatientsWidget::addPatient);
        m_menu->addAction(addPatient);

        QAction *editPatient = new QAction(QIcon(":/images/PatientsCard.png"), tr("Карточка пациента..."), this);
        connect(editPatient, &QAction::triggered, this, &PatientsWidget::editPatient);
        m_menu->addAction(editPatient);

        QAction *delPatient = new QAction(QIcon(":/images/PatientsDel.png"), tr("Удалить запись о пациенте..."), this);
        connect(delPatient, &QAction::triggered, this, &PatientsWidget::removePatient);
        m_menu->addAction(delPatient);

        if (AAnalyserBuild::isPPEnabled())
        {
            m_menu->addSeparator();

            QAction *ppAssign = new QAction(QIcon(":/images/PP_assign.png"), tr("Назначить индивидуальную программу..."), this);
            connect(ppAssign, &QAction::triggered, this, &PatientsWidget::assignPersonalProgram);
            m_menu->addAction(ppAssign);

            QAction *ppCancel = new QAction(QIcon(":/images/PP_cancel.png"), tr("Отменить индивидуальную программу..."), this);
            connect(ppCancel, &QAction::triggered, this, &PatientsWidget::cancelPersonalProgram);
            m_menu->addAction(ppCancel);
        }
    }
    m_menu->popup(ui->tvPatients->mapToGlobal(pos));
}

void PatientsWidget::on_doubleClickedTable(QModelIndex index)
{

    Q_UNUSED(index);
    editPatient();
}

void PatientsWidget::onePatientHandle()
{
    setVisible(!m_isOnePatientMode);

    if (m_isOnePatientMode)
    {
        if (DataProvider::getPatients().size() == 0)
        {
            DataDefines::PatientKard patient;
            patient.fio = m_onePatientFIO;
            patient.born = QDate(2000, 1, 1);
            patient.sex = DataDefines::male;
            patient.massa = 80;
            patient.height = 170;
            patientsModel()->addPatient(patient);
        }
        else
        {
            auto idx = patientsProxyModel()->index(0, 0);
            auto uid = idx.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
            auto fio = idx.data().toString();

            DataDefines::PatientKard patient;
            if (DataProvider::getPatient(uid, patient))
            {
                patient.fio = m_onePatientFIO;
                patientsModel()->addPatient(patient);
            }
        }
        selectPatient(patientsProxyModel()->index(0, 0));
//        selectPatient(m_pmdlPatients->index(0, 0));
    }
}

PatientsModel *PatientsWidget::patientsModel() const
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->patientsModel();
}

PatientsProxyModel *PatientsWidget::patientsProxyModel() const
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->patientsProxyModel();
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
