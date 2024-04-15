#include "dailyprogramseditor.h"
#include "ui_dailyprogramseditor.h"

#include <QUuid>
#include <QDebug>

#include "aanalyserapplication.h"
#include "selectmethodicdialog.h"
#include "metodicsfactory.h"
#include "dataprovider.h"
#include "personalprogramdefines.h"
#include "settingsprovider.h"
#include "amessagebox.h"

DailyProgramsEditor::DailyProgramsEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DailyProgramsEditor)
{
    ui->setupUi(this);

    auto val = SettingsProvider::valueFromRegAppCopy("Geometry/DailyProgramsEditor", "Geometry").toRect();
    if (val != QRect())
    {
        setGeometry(val);

        auto valSpl = SettingsProvider::valueFromRegAppCopy("Geometry/DailyProgramsEditor", "SplitterPosition").toByteArray();
        ui->splitter->restoreState(valSpl);
    }
}

DailyProgramsEditor::~DailyProgramsEditor()
{
    if (m_dlgSelMethod)
        delete m_dlgSelMethod;
    delete ui;
}

int DailyProgramsEditor::exec()
{
    ui->tvPrograms->header()->setVisible(false);
    ui->tvTests->header()->setVisible(false);

    static_cast<AAnalyserApplication*>(QApplication::instance())->readDailyProgramList(m_mdlPrograms);

    ui->tvPrograms->setModel(&m_mdlPrograms);
    ui->tvTests->setModel(&m_mdlTests);

    return QDialog::exec();
}

void DailyProgramsEditor::closeEvent(QCloseEvent *)
{
    SettingsProvider::setValueToRegAppCopy("Geometry/DailyProgramsEditor", "Geometry", geometry());
}

void DailyProgramsEditor::resizeEvent(QResizeEvent *event)
{
    SettingsProvider::setValueToRegAppCopy("Geometry/DailyProgramsEditor", "Geometry", geometry());
    QDialog::resizeEvent(event);
}

void DailyProgramsEditor::on_addTest()
{
    if (!m_dlgSelMethod)
        m_dlgSelMethod = new SelectMethodicDialog(this);

    if (m_dlgSelMethod->exec() == QDialog::Accepted)
    {
        auto metUid = m_dlgSelMethod->methodic();
        if (metUid != QUuid().toString())
        {
            MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
            auto mi = metFactory->metodic(metUid);

            auto *item = new QStandardItem(mi.name);
            item->setEditable(false);
            item->setData(metUid, PersonalProgramDefines::TableTestsRoles::MethodUidRole);
            item->setData(mi.params, PersonalProgramDefines::TableTestsRoles::MethodParamsRole);
            item->setIcon(QIcon(":/images/Methodics/" + mi.imageName));
            m_mdlTests.appendRow(item);
        }
        else
            AMessageBox::information(nullptr, tr("Сообщение"), tr("Методика не выбрана"));
    }
}

void DailyProgramsEditor::on_doubleTest()
{
    QModelIndexList selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Дублировать тест \"") + index.data().toString() + tr("\"?"));
            if (mr == AMessageBox::Yes)
            {
                auto metUid = index.data(PersonalProgramDefines::TableTestsRoles::MethodUidRole).toString();
                auto params = index.data(PersonalProgramDefines::TableTestsRoles::MethodParamsRole).toJsonObject();

                auto *item = new QStandardItem(index.data().toString());
                item->setEditable(false);
                item->setData(metUid, PersonalProgramDefines::TableTestsRoles::MethodUidRole);
                item->setData(params, PersonalProgramDefines::TableTestsRoles::MethodParamsRole);
                item->setIcon(m_mdlTests.itemFromIndex(index)->icon());
                m_mdlTests.insertRow(index.row() + 1, item);
            }
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
}

void DailyProgramsEditor::on_editTest()
{
    QModelIndexList selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            auto metUid = index.data(PersonalProgramDefines::TableTestsRoles::MethodUidRole).toString();
            auto params = index.data(PersonalProgramDefines::TableTestsRoles::MethodParamsRole).toJsonObject();
            if (DataProvider::editMetodicParams(this, metUid, params))
            {
                m_mdlTests.itemFromIndex(index)->setData(params, PersonalProgramDefines::TableTestsRoles::MethodParamsRole);
            }
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
}

void DailyProgramsEditor::on_delTest()
{
    QModelIndexList selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Удалить тест \"") + index.data().toString() + tr("\" из дневной программы?"));
            if (mr == AMessageBox::Yes)
                m_mdlTests.removeRow(index.row());
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
}

void DailyProgramsEditor::on_moveTestUp()
{
    QModelIndexList selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            int r = index.row();
            if (r > 0)
            {
                auto row = m_mdlTests.takeRow(r);
                m_mdlTests.insertRow(r - 1, row);
                ui->tvTests->selectionModel()->clearSelection();
                ui->tvTests->selectionModel()->select(row.at(0)->index(), QItemSelectionModel::Select);
            }
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
}

void DailyProgramsEditor::on_moveTestDown()
{
    QModelIndexList selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            int r = index.row();
            if (r < m_mdlTests.rowCount() - 1)
            {
                auto row = m_mdlTests.takeRow(r);
                m_mdlTests.insertRow(r + 1, row);
                ui->tvTests->selectionModel()->clearSelection();
                ui->tvTests->selectionModel()->select(row.at(0)->index(), QItemSelectionModel::Select);
            }
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
}

void DailyProgramsEditor::on_dpAdd()
{
    if (ui->edName->text() != "")
    {
        if (m_mdlTests.rowCount() > 0)
        {
            auto objDP = compileDP();
            objDP["uid"] = QUuid::createUuid().toString();

            auto *item = new QStandardItem(objDP["name"].toString());
            item->setEditable(false);
            item->setData(objDP, PersonalProgramDefines::TableDPRoles::DPRole);
            m_mdlPrograms.appendRow(item);

            //! Выделить добавленный итем
            QModelIndex lastIdx = m_mdlPrograms.index(m_mdlPrograms.rowCount() - 1, 0);
            ui->tvPrograms->selectionModel()->clearSelection();
            ui->tvPrograms->selectionModel()->select(lastIdx, QItemSelectionModel::Select);

            //! Внести изменения на диск
            static_cast<AAnalyserApplication*>(QApplication::instance())->saveDailyProgramList(m_mdlPrograms);
        }
        else
            AMessageBox::information(nullptr, tr("Предупреждение"), tr("Необходимо добавить тесты в дневную программу"));
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Не задано название дневной программы"));
}

void DailyProgramsEditor::on_dpEdit()
{
    QModelIndexList selIdxs = ui->tvPrograms->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            auto objDP = compileDP();

            auto item = m_mdlPrograms.itemFromIndex(index);
            //! uid при редактировании надо взять из данных итема до редактирования
            objDP["uid"] = item->data(PersonalProgramDefines::TableDPRoles::DPRole).toJsonObject()["uid"].toString();

            QString pn = item->text();
            auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Внести изменения в программу") + " \"" + pn + "\"?");
            if (mr == AMessageBox::Yes)
            {
                item->setData(objDP, PersonalProgramDefines::TableDPRoles::DPRole);
                item->setText(objDP["name"].toString());
                static_cast<AAnalyserApplication*>(QApplication::instance())->saveDailyProgramList(m_mdlPrograms);
            }
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбрана дневная программа"));
}

void DailyProgramsEditor::on_dpDel()
{
    QModelIndexList selIdxs = ui->tvPrograms->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            QString name = index.data().toString();
            auto mr = AMessageBox::question(nullptr, tr("Запрос"), tr("Удалить дневную программу") + " \"" + name + "\"?");
            if (mr == AMessageBox::Yes)
            {
                m_mdlPrograms.removeRow(index.row());
                ui->edName->setText("");
                m_mdlTests.clear();
                ui->tvPrograms->selectionModel()->clearSelection();

                static_cast<AAnalyserApplication*>(QApplication::instance())->saveDailyProgramList(m_mdlPrograms);
            }
        }
    }
    else
        AMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбрана дневная программа"));
}

void DailyProgramsEditor::on_selectDP(QModelIndex index)
{
    if (index.isValid())
    {
        auto objDP = index.data(PersonalProgramDefines::TableDPRoles::DPRole).toJsonObject();
        viewDP(objDP);
    }
}

void DailyProgramsEditor::on_splitterMoved(int, int)
{
    SettingsProvider::setValueToRegAppCopy("Geometry/DailyProgramsEditor", "SplitterPosition", ui->splitter->saveState());
}

QJsonObject DailyProgramsEditor::compileDP()
{
    QJsonObject retval;

    retval["name"] = ui->edName->text();
    QJsonArray arTests;
    for (int i = 0; i < m_mdlTests.rowCount(); ++i)
    {
        QJsonObject objTest;
        objTest["uid"] = m_mdlTests.index(i, 0).data(PersonalProgramDefines::TableTestsRoles::MethodUidRole).toString();
        objTest["params"] = m_mdlTests.index(i, 0).data(PersonalProgramDefines::TableTestsRoles::MethodParamsRole).toJsonObject();
        arTests << objTest;
    }
    retval["test_list"] = arTests;

    return retval;
}

void DailyProgramsEditor::viewDP(const QJsonObject &objDP)
{
    ui->edName->setText(objDP["name"].toString());

    m_mdlTests.clear();
    auto arTests = objDP["test_list"].toArray();
    for (int i = 0; i < arTests.size(); ++i)
    {
        auto objTest = arTests.at(i).toObject();
        auto metUid = objTest["uid"].toString();
        auto params = objTest["params"].toObject();

        MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
        auto mi = metFactory->metodic(metUid);

        auto *item = new QStandardItem(mi.name);
        item->setEditable(false);
        item->setData(metUid, PersonalProgramDefines::TableTestsRoles::MethodUidRole);
        item->setData(params, PersonalProgramDefines::TableTestsRoles::MethodParamsRole);
        item->setIcon(QIcon(":/images/Methodics/" + mi.imageName));
        m_mdlTests.appendRow(item);
    }
}
