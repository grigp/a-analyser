#include "personalprogramwidget.h"
#include "ui_personalprogramwidget.h"

#include <QUuid>
#include <QDebug>

#include "aanalyserapplication.h"
#include "patientsmodel.h"
#include "patientsproxymodel.h"
#include "settingsprovider.h"
#include "dataprovider.h"
#include "personalprogram.h"
#include "databasewigetdefines.h"
#include "patientprogramwidget.h"
#include "activepersonalprogrameditor.h"

PersonalProgramWidget::PersonalProgramWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::PersonalProgramWidget)
{
    ui->setupUi(this);

    restoreSplitterPosition();
    load();
    ui->tvPatients->setModel(m_model);
    m_wgts.clear();
}

PersonalProgramWidget::~PersonalProgramWidget()
{
    delete ui;
}

QString PersonalProgramWidget::uid()
{
    return ClientWidgets::uidPersonalProgramWidgetUid;
}

QString PersonalProgramWidget::name()
{
    return tr("Индивидуальные программы");
}

void PersonalProgramWidget::onDBConnect()
{
    ui->tvPatients->header()->resizeSections(QHeaderView::ResizeToContents);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectPatient,
            this, &PersonalProgramWidget::on_selectPatient);
}

void PersonalProgramWidget::onShow()
{
    ui->tvPatients->selectionModel()->clearSelection();
    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient("");
}

void PersonalProgramWidget::onHide()
{

}

void PersonalProgramWidget::on_splitterMoved(int, int)
{
    saveSplitterPosition();
}

void PersonalProgramWidget::on_run()
{
}

void PersonalProgramWidget::on_delete()
{

}

void PersonalProgramWidget::on_params()
{
    auto index = selectedIndex();
    if (index != QModelIndex() && index.isValid())
    {
        auto uidPP = index.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();
        auto dialog = new ActivePersonalProgramEditor(this);
        auto pp = DataProvider::getPersonalProgramByUid(uidPP);
        dialog->setPersonalProgram(pp);
        if (dialog->exec() == QDialog::Accepted)
        {
            //! Получить от диалога новую индивидуальную программу
            auto pp = dialog->personalProgram();
            //!Записать измененную индивидуальную программу в БД
            DataProvider::savePersonalProgramByUid(uidPP, pp);
            //! Обновить ее на странице
            if (m_wgts.contains(uidPP))
                m_wgts.value(uidPP)->assignPersonalProgram(uidPP);
        }
    }
}

void PersonalProgramWidget::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    selectPatient(ui->tvPatients->selectionModel()->currentIndex());
}

void PersonalProgramWidget::selectPatient(const QModelIndex index)
{
    if (index.isValid())
    {
         auto idx = index.model()->index(index.row(), 0, index.parent());
         auto uid = idx.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
         static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient(uid);

         auto uidPP = idx.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();

         //! Показать индивидуальную программу
         showPersonalProgram(uidPP);
    }
}

void PersonalProgramWidget::on_selectPatient(const QString &patientUid)
{
    //! uid выбранного в настоящий момент пациента
    QString curSelPatientUid = "";
    auto selIdxs = ui->tvPatients->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
        foreach (auto idx, selIdxs)
            if (idx.column() == 0)
                curSelPatientUid = idx.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();

    //! Поиск выделенного пациента patientUid
    if (patientUid != "")
    {
        auto pmdl = m_model;
        for (int i = 0; i < pmdl->rowCount(); ++i)
        {
            auto pindex = pmdl->index(i, 0);
            auto uid = pindex.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
            auto uidPP = pindex.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();

            //! Нашли и он не выбран
            if (uid == patientUid && uid != curSelPatientUid)
            {
                //! Выделить
                for (int i = 0; i < pmdl->columnCount(); ++i)
                {
                    auto idx = pmdl->index(pindex.row(), i);
                    ui->tvPatients->selectionModel()->select(idx, QItemSelectionModel::Select);

                }
                //! Показать индивидуальную программу
                showPersonalProgram(uidPP);
            }
        }

    }
}

QStandardItem* PersonalProgramWidget::appendLine(const QString uidPat,
                                                 const QString uidPPAssigned,
                                                 const QJsonObject& objPP,
                                                 QStandardItem* root)
{
    DataDefines::PatientKard pi;
    if (DataProvider::getPatient(uidPat, pi))
    {
        QString title = pi.fio;
        if (root)
        {
            auto db = objPP["date_begin"].toString();
            auto de = objPP["date_end"].toString();
            title = db + " - " + de;
        }
        QStandardItem *itemFIO = new QStandardItem(title);
        itemFIO->setData(pi.uid, DatabaseWidgetDefines::PatientsModel::PatientUidRole);
        itemFIO->setData(uidPPAssigned, DatabaseWidgetDefines::PatientsModel::PatientPPUidRole);
        itemFIO->setEditable(false);
        QStandardItem *itemBorn = new QStandardItem(pi.born.toString("dd.MM.yyyy"));
        itemBorn->setEditable(false);
        QStandardItem *itemSex = new QStandardItem(DataDefines::SexToText.value(
                                                       static_cast<DataDefines::Sex>(pi.sex)));
        itemSex->setData(pi.sex, DatabaseWidgetDefines::PatientsModel::PatientSexRole);
        itemSex->setEditable(false);

        QList<QStandardItem*> line;
        line << itemFIO << itemBorn << itemSex;
        if (!root)
            m_model->appendRow(line);
        else
            root->appendRow(line);
        return itemFIO;
    }
    return nullptr;
}

void PersonalProgramWidget::load()
{
    auto arrPP = DataProvider::getPersonalProgramList();
    m_model = new QStandardItemModel(ui->tvPatients);

    QMap<QString, QStandardItem*> patients;  ///< Запоминаем пациентов, уже добавленных в модель
    patients.clear();

    //! Сначала активные - они всегда в руте
    for (int i = 0; i < arrPP.size(); ++i)
    {
        auto objPP = arrPP.at(i).toObject();
        auto uidPat = objPP["patient_uid"].toString();
        auto uidPPAssigned = objPP["assigned_uid"].toString();
        auto active = objPP["active"].toBool();
        if (active && !patients.contains(uidPat))
        {
            auto item = appendLine(uidPat, uidPPAssigned);
            patients.insert(uidPat, item);
        }
    }

    for (int i = 0; i < arrPP.size(); ++i)
    {
        auto objPP = arrPP.at(i).toObject();
        auto uidPat = objPP["patient_uid"].toString();
        auto uidPPAssigned = objPP["assigned_uid"].toString();
        auto active = objPP["active"].toBool();
        if (!active)
        {
            if (patients.contains(uidPat))
            {
                appendLine(uidPat, uidPPAssigned, objPP, patients.value(uidPat));
            }
            else
            {
                auto item = appendLine(uidPat, uidPPAssigned);
                patients.insert(uidPat, item);
            }
        }
    }
    m_model->setHorizontalHeaderLabels(QStringList() << tr("ФИО") << tr("Дата рождения") << tr("Пол"));

}

void PersonalProgramWidget::showPersonalProgram(const QString& uidPPAssigned)
{
    hideAllWidgets();
    if (m_wgts.contains(uidPPAssigned))
        m_wgts.value(uidPPAssigned)->setVisible(true);
    else
    {
        auto ppw = new PatientProgramWidget(ui->frPrograms);
        ppw->assignPersonalProgram(uidPPAssigned);
        ui->frPrograms->layout()->addWidget(ppw);
        m_wgts.insert(uidPPAssigned, ppw);
    }
}


void PersonalProgramWidget::hideAllWidgets()
{
    QObjectList children = ui->frPrograms->children();
    foreach(QObject * child, children)
        if (child->isWidgetType())
            static_cast<QWidget*>(child)->setVisible(false);
}


void PersonalProgramWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("PersonalProgramWidget", "SplitterPosition", ui->splitter->saveState());
}

void PersonalProgramWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("PersonalProgramWidget", "SplitterPosition").toByteArray();
//    if (val == "")
//        val = QByteArray::fromRawData("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x01\x1B\x00\x00\x03""b\x01\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x01\x00", 31);
    ui->splitter->restoreState(val);
}

QModelIndex PersonalProgramWidget::selectedIndex() const
{
    auto selIdxs = ui->tvPatients->selectionModel()->selectedIndexes();
    for (int i = 0; i < selIdxs.size(); ++i)
    {
        if (selIdxs.at(i).column() == 0)
            return selIdxs.at(i);
    }
    return QModelIndex();
}

