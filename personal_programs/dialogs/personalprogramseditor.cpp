#include "personalprogramseditor.h"
#include "ui_personalprogramseditor.h"

#include <QMessageBox>
#include <QUuid>
#include <QDebug>

#include "aanalyserapplication.h"
#include "selectdailyprogramdialog.h"
#include "personalprogramdefines.h"

PersonalProgramsEditor::PersonalProgramsEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonalProgramsEditor)
{
    ui->setupUi(this);
    prepareParams();
}

PersonalProgramsEditor::~PersonalProgramsEditor()
{
    delete ui;
}

int PersonalProgramsEditor::exec()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->readPersonalProgramList(m_mdlPP);

    ui->tvPrograms->setModel(&m_mdlPP);
    ui->tvSchedule->setModel(&m_mdlDP);
    ui->tvPrograms->header()->setVisible(false);
    ui->tvSchedule->horizontalHeader()->setVisible(false);

    QString sSheet = QString("QTableView") +
    "{" +
        "background-color: rgb(240, 240, 245);" +
        "font-size: 11pt;" +
        "color: rgb(32,88,103);" +
    "}" +
            QString("QHeaderView::section") +
    "{" +
            "background-color: rgb(240, 240, 245);" +
            "font-size: 11pt;" +
            "color: rgb(32,88,103);" +
    "}";
    setStyleSheet(sSheet);

    return QDialog::exec();
}

void PersonalProgramsEditor::on_addDP()
{
    if (!m_dlgSelDP)
        m_dlgSelDP = new SelectDailyProgramDialog(this);

    if (m_dlgSelDP->exec() == QDialog::Accepted)
    {
        auto dp = m_dlgSelDP->dailyProgram();
        auto *item = new QStandardItem(dp["name"].toString());
        item->setEditable(false);
        item->setData(dp, PersonalProgramDefines::TableDPRoles::DPRole);
        m_mdlDP.appendRow(item);
    }
}

void PersonalProgramsEditor::on_delDP()
{
    QModelIndexList selIdxs = ui->tvSchedule->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            QString text = tr("Удалить дневную программу \"") + index.data().toString() + tr("\" из индивидуальной программы?");
            auto mr = QMessageBox::question(nullptr, tr("Запрос"), text);
            if (mr == QMessageBox::Yes)
                m_mdlDP.removeRow(index.row());
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбрана дневная программа"));
}

void PersonalProgramsEditor::on_moveDPUp()
{
    QModelIndexList selIdxs = ui->tvSchedule->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            int r = index.row();
            if (r > 0)
            {
                auto row = m_mdlDP.takeRow(r);
                m_mdlDP.insertRow(r - 1, row);
                ui->tvSchedule->selectionModel()->clearSelection();
                ui->tvSchedule->selectionModel()->select(row.at(0)->index(), QItemSelectionModel::Select);
            }
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбрана дневная программа"));
}

void PersonalProgramsEditor::on_moveDPDown()
{
    QModelIndexList selIdxs = ui->tvSchedule->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            int r = index.row();
            if (r < m_mdlDP.rowCount() - 1)
            {
                auto row = m_mdlDP.takeRow(r);
                m_mdlDP.insertRow(r + 1, row);
                ui->tvSchedule->selectionModel()->clearSelection();
                ui->tvSchedule->selectionModel()->select(row.at(0)->index(), QItemSelectionModel::Select);
            }
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбрана дневная программа"));
}

void PersonalProgramsEditor::on_ppAdd()
{
    if (ui->edName->text() != "")
    {
        if (m_mdlDP.rowCount() > 0)
        {
            QJsonObject objPP;
            QString uidPP = QUuid::createUuid().toString();
            objPP["uid"] =  uidPP;
            objPP["name"] = ui->edName->text();
            objPP["min_time_between_dp"] = ui->cbMinTimeBetweenDP->currentIndex();
            objPP["max_time_between_dp"] = ui->cbMaxTimeBetweenDP->currentIndex();

            auto *item = new QStandardItem(objPP["name"].toString());
            item->setEditable(false);
            item->setData(objPP, PersonalProgramDefines::TablePPRoles::PPRole);
            m_mdlPP.appendRow(item);

            //! Выделить добавленный итем
            QModelIndex lastIdx = m_mdlPP.index(m_mdlPP.rowCount() - 1, 0);
            ui->tvPrograms->selectionModel()->clearSelection();
            ui->tvPrograms->selectionModel()->select(lastIdx, QItemSelectionModel::Select);

            //! Внести изменения на диск
            static_cast<AAnalyserApplication*>(QApplication::instance())->savePersonalProgramList(m_mdlPP);
            QStringList uidDPs;
            for (int i = 0; i < m_mdlDP.rowCount(); ++i)
            {
                auto objDP = m_mdlDP.item(i)->data(PersonalProgramDefines::TableDPRoles::DPRole).toJsonObject();
                uidDPs << objDP["uid"].toString();

            }
            static_cast<AAnalyserApplication*>(QApplication::instance())->assignDailyProgramsForPersonal(uidPP, uidDPs);
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Необходимо добавить дневные программы в индивидуальную программу"));
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не задано название индивидуальной программы"));
}

void PersonalProgramsEditor::on_ppEdit()
{
    QModelIndexList selIdxs = ui->tvPrograms->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            auto item = m_mdlPP.itemFromIndex(index);

            auto objPP = item->data(PersonalProgramDefines::TablePPRoles::PPRole).toJsonObject();
            auto uidPP = objPP["uid"].toString();
            objPP["name"] = ui->edName->text();
            objPP["min_time_between_dp"] = ui->cbMinTimeBetweenDP->currentIndex();
            objPP["max_time_between_dp"] = ui->cbMaxTimeBetweenDP->currentIndex();

            QString pn = item->text();
            auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Внести изменения в программу") + " \"" + pn + "\"?");
            if (mr == QMessageBox::Yes)
            {
                item->setData(objPP, PersonalProgramDefines::TablePPRoles::PPRole);
                item->setText(objPP["name"].toString());
                static_cast<AAnalyserApplication*>(QApplication::instance())->savePersonalProgramList(m_mdlPP);
                QStringList uidDPs;
                for (int i = 0; i < m_mdlDP.rowCount(); ++i)
                {
                    auto objDP = m_mdlDP.item(i)->data(PersonalProgramDefines::TableDPRoles::DPRole).toJsonObject();
                    uidDPs << objDP["uid"].toString();
                }
                static_cast<AAnalyserApplication*>(QApplication::instance())->editDailyProgramsForPersonal(uidPP, uidDPs);
            }
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбрана индивидуальная программа"));
}

void PersonalProgramsEditor::on_ppDel()
{
    QModelIndexList selIdxs = ui->tvPrograms->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            QString name = index.data().toString();
            auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Удалить индивидуальную программу") + " \"" + name + "\"?");
            if (mr == QMessageBox::Yes)
            {
                auto objPP = index.data(PersonalProgramDefines::TablePPRoles::PPRole).toJsonObject();
                auto uidPP = objPP["uid"].toString();

                m_mdlPP.removeRow(index.row());
                ui->edName->setText("");
                ui->cbMinTimeBetweenDP->setCurrentIndex(0);
                ui->cbMaxTimeBetweenDP->setCurrentIndex(7);
                m_mdlDP.clear();
                ui->tvPrograms->selectionModel()->clearSelection();

                static_cast<AAnalyserApplication*>(QApplication::instance())->savePersonalProgramList(m_mdlPP);
                static_cast<AAnalyserApplication*>(QApplication::instance())->clearListDailyProgramsForPersonal(uidPP);
            }
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбрана индивидуальная программа"));
}

void PersonalProgramsEditor::on_selectPP(QModelIndex index)
{
    if (index.isValid())
    {
        auto objPP = index.data(PersonalProgramDefines::TablePPRoles::PPRole).toJsonObject();
        ui->edName->setText(objPP["name"].toString());
        ui->cbMinTimeBetweenDP->setCurrentIndex(objPP["min_time_between_dp"].toInt());
        ui->cbMaxTimeBetweenDP->setCurrentIndex(objPP["max_time_between_dp"].toInt());

        auto uid = objPP["uid"].toString();
        auto dps = static_cast<AAnalyserApplication*>(QApplication::instance())->getListDailyProgramsForPersonal(uid);
        static_cast<AAnalyserApplication*>(QApplication::instance())->readDailyProgramList(m_mdlDP, dps);
    }
}

void PersonalProgramsEditor::on_changeMinTime(int idx)
{

}

void PersonalProgramsEditor::on_changeMaxTime(int idx)
{

}

void PersonalProgramsEditor::prepareParams()
{
    ui->cbMinTimeBetweenDP->clear();
    foreach (auto v, PersonalProgramDefines::MinTimeBetweenDPList)
        ui->cbMinTimeBetweenDP->addItem(PersonalProgramDefines::MinTimeBetweenDPNames.value(v), v);
    ui->cbMinTimeBetweenDP->setCurrentIndex(0);

    ui->cbMaxTimeBetweenDP->clear();
    foreach (auto v, PersonalProgramDefines::MaxTimeBetweenDPList)
        ui->cbMaxTimeBetweenDP->addItem(PersonalProgramDefines::MaxTimeBetweenDPNames.value(v), v);
    ui->cbMaxTimeBetweenDP->setCurrentIndex(7);
}
