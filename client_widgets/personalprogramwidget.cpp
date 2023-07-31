#include "personalprogramwidget.h"
#include "ui_personalprogramwidget.h"

#include <QUuid>
#include <QDebug>

#include "aanalyserapplication.h"
#include "patientsmodel.h"
#include "patientsproxymodel.h"
#include "settingsprovider.h"

PersonalProgramWidget::PersonalProgramWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::PersonalProgramWidget)
{
    ui->setupUi(this);

    restoreSplitterPosition();
    ui->tvPatients->setModel(patientsProxyModel());
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
         auto srcIndex = patientsProxyModel()->mapToSource(index);
         auto uid = patientsModel()->index(srcIndex.row(), PatientsModel::ColFio, srcIndex.parent()).
                    data(PatientsModel::PatientUidRole).toString();
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient(uid);
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
                curSelPatientUid = idx.data(PatientsModel::PatientUidRole).toString();

    //! Поиск выделенного пациента patientUid
    if (patientUid != "")
    {
        auto pmdl = patientsProxyModel();
        for (int i = 0; i < pmdl->rowCount(); ++i)
        {
            auto pindex = pmdl->index(i, 0);
            auto uid = pindex.data(PatientsModel::PatientUidRole).toString();

            //! Нашли и он не выбран
            if (uid == patientUid && uid != curSelPatientUid)
            {
                //! Выделить
                for (int i = 0; i < pmdl->columnCount(); ++i)
                {
                    auto idx = pmdl->index(pindex.row(), i);
                    ui->tvPatients->selectionModel()->select(idx, QItemSelectionModel::Select);

                    //! TODO: Здесь написать действия по показу индивидуальной программы
                }
            }
        }

    }
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

PatientsModel *PersonalProgramWidget::patientsModel() const
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->patientsModel();
}

PatientsProxyModel *PersonalProgramWidget::patientsProxyModel() const
{
    return static_cast<AAnalyserApplication*>(QApplication::instance())->patientsPPProxyModel();
}
