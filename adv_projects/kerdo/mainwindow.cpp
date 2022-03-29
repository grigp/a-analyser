#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

#include "kerdoapplication.h"
#include "patientkarddialog.h"
#include "patientsmodel.h"
#include "patientsproxymodel.h"
#include "dataprovider.h"
#include "dynamicdiagram.h"
#include "settingsprovider.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
  , m_mdlPatients(new PatientsModel(this))
  , m_pmdlPatients(new PatientsProxyModel(this))
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/images/MainIcon2.ico"));

    m_pmdlPatients->setSourceModel(m_mdlPatients);
    ui->tvMans->setModel(m_pmdlPatients);
    ui->tvMans->sortByColumn(PatientsModel::ColFio, Qt::AscendingOrder);

    connect(ui->wgtKerdo, &DynamicDiagram::selectItem, this, &MainWindow::selectItem);
    connect(ui->wgtPulse, &DynamicDiagram::selectItem, this, &MainWindow::selectItem);
    connect(ui->wgtDAD, &DynamicDiagram::selectItem, this, &MainWindow::selectItem);

    ui->lblInfoBefore->setStyleSheet("font-size: 12pt; color: rgb(0, 0, 255);");
    ui->lblInfoAfter->setStyleSheet("font-size: 12pt; color: rgb(255, 0, 0);");
    ui->lblInfoSelectDate->setStyleSheet("font-size: 14pt;");

    restoreSplitterPosition();
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
    m_uidCurPatient = index.model()->index(index.row(), 0, index.parent()).data(PatientsModel::PatientUidRole).toString();
    redrawDiag();
}

void MainWindow::selectItem(const int idx)
{
    auto tests = DataProvider::getTests(m_uidCurPatient);
    if (idx >= 0 && idx < tests.size())
        m_uidCurTest = tests.at(idx);

    ui->wgtKerdo->doSelectItem(idx);
    ui->wgtPulse->doSelectItem(idx);
    ui->wgtDAD->doSelectItem(idx);

    ui->lblInfoSelectDate->setText(tr("Дата") + " : " + ui->wgtKerdo->itemName(idx));

    ui->edBeforePulse->setValue(static_cast<int>(ui->wgtPulse->value(idx, 0)));
    ui->edAfterPulse->setValue(static_cast<int>(ui->wgtPulse->value(idx, 1)));
    ui->edBeforeDAD->setValue(static_cast<int>(ui->wgtDAD->value(idx, 0)));
    ui->edAfterDAD->setValue(static_cast<int>(ui->wgtDAD->value(idx, 1)));
}

void MainWindow::btnNewClick()
{
    if (m_uidCurPatient != "")
    {
        m_uidCurTest = DataProvider::addTest(m_uidCurPatient);
        redrawDiag();
        auto tests = DataProvider::getTests(m_uidCurPatient);
        selectItem(tests.size() - 1);
    }
}

void MainWindow::btnUpdateClick()
{
    if (m_uidCurPatient != "" && m_uidCurTest != "")
    {
        DataDefines::Result result;
        result.beforePulse = ui->edBeforePulse->value();
        result.beforeDAD = ui->edBeforeDAD->value();
        if (result.beforePulse > 0)
            result.beforeKerdo = 100 * (1 - static_cast<double>(result.beforeDAD) / static_cast<double>(result.beforePulse));
        result.afterPulse = ui->edAfterPulse->value();
        result.afterDAD = ui->edAfterDAD->value();
        if (result.afterPulse > 0)
            result.afterKerdo = 100 * (1 - static_cast<double>(result.afterDAD) / static_cast<double>(result.afterPulse));

        DataProvider::setTestResult(m_uidCurPatient, m_uidCurTest, result);
        redrawDiag();
    }
}

void MainWindow::splitterMoved(int pos, int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void MainWindow::redrawDiag()
{
    auto tests = DataProvider::getTests(m_uidCurPatient);

    ui->wgtKerdo->clear();
    ui->wgtPulse->clear();
    ui->wgtDAD->clear();

    ui->wgtKerdo->setTitle(tr("Индекс Кердо"));
    ui->wgtPulse->setTitle(tr("Пульс"));
    ui->wgtDAD->setTitle(tr("Диастолическое артериальное давление"));
    ui->wgtKerdo->setDiap(-60, 60);

    foreach (auto uidTest, tests)
    {
        DataDefines::Result test;
        if (DataProvider::getTest(m_uidCurPatient, uidTest, test))
        {
            ui->wgtKerdo->appendItem(new DiagItem(QList<double>() << test.beforeKerdo << test.afterKerdo, test.dateTime.toString("dd.MM.yyyy")));
            ui->wgtPulse->appendItem(new DiagItem(QList<double>() << test.beforePulse << test.afterPulse, test.dateTime.toString("dd.MM.yyyy")));
            ui->wgtDAD->appendItem(new DiagItem(QList<double>() << test.beforeDAD << test.afterDAD, test.dateTime.toString("dd.MM.yyyy")));
        }
    }

    ui->wgtKerdo->addValuesZone(30, 60, QColor(255, 150, 150));
    ui->wgtKerdo->addValuesZone(15, 30, QColor(255, 200, 200));
    ui->wgtKerdo->addValuesZone(-15, 15, QColor(255, 255, 100));
    ui->wgtKerdo->addValuesZone(-30, -15, QColor(200, 255, 200));
    ui->wgtKerdo->addValuesZone(-60, -30, QColor(150, 255, 150));
}

void MainWindow::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("MainWindow", "SplPosMain", ui->splMain->saveState());
    SettingsProvider::setValueToRegAppCopy("MainWindow", "SplPosGraph", ui->splGraph->saveState());
}

void MainWindow::restoreSplitterPosition()
{
    auto valMain = SettingsProvider::valueFromRegAppCopy("MainWindow", "SplPosMain").toByteArray();
//    if (val == "")
//        val = QByteArray::fromRawData("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x01\x17\x00\x00\x01\xEB\x01\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x01\x00", 31);
    ui->splMain->restoreState(valMain);

    auto valGraph = SettingsProvider::valueFromRegAppCopy("MainWindow", "SplPosGraph").toByteArray();
//    if (val == "")
//        val = QByteArray::fromRawData("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x01\x17\x00\x00\x01\xEB\x01\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x01\x00", 31);
    ui->splGraph->restoreState(valGraph);
}
