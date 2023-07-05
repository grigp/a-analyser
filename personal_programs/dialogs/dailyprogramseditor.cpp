#include "dailyprogramseditor.h"
#include "ui_dailyprogramseditor.h"

#include <QUuid>
#include <QMessageBox>
#include <QDebug>

#include "aanalyserapplication.h"
#include "selectmethodicdialog.h"
#include "metodicsfactory.h"
#include "dataprovider.h"

DailyProgramsEditor::DailyProgramsEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DailyProgramsEditor)
{
    ui->setupUi(this);

    ui->tvPrograms->setModel(&m_mdlPrograms);
    ui->tvTests->setModel(&m_mdlTests);
    ui->tvPrograms->header()->setVisible(false);
    ui->tvTests->header()->setVisible(false);
}

DailyProgramsEditor::~DailyProgramsEditor()
{
    delete ui;
}

void DailyProgramsEditor::on_addTest()
{
    SelectMethodicDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        auto metUid = dialog.methodic();
        if (metUid != QUuid().toString())
        {
            MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
            auto mi = metFactory->metodic(metUid);

            auto *item = new QStandardItem(mi.name);
            item->setEditable(false);
            item->setData(metUid, TableTestsRoles::MethodUidRole);
            item->setData(mi.params, TableTestsRoles::MethodParamsRole);
            item->setIcon(QIcon(":/images/Methodics/" + mi.imageName));
            m_mdlTests.appendRow(item);
        }
        else
            QMessageBox::information(nullptr, tr("Сообщение"), tr("Методика не выбрана"));
    }
}

void DailyProgramsEditor::on_editTest()
{
    QModelIndexList selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            auto metUid = index.data(TableTestsRoles::MethodUidRole).toString();
            auto params = index.data(TableTestsRoles::MethodParamsRole).toJsonObject();
            if (DataProvider::editMetodicParams(this, metUid, params))
            {
                m_mdlTests.itemFromIndex(index)->setData(params, TableTestsRoles::MethodParamsRole);
            }
        }
    }
}

void DailyProgramsEditor::on_delTest()
{
    QModelIndexList selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Удалить тест \"") + index.data().toString() + tr("\" из дневной программы?"));
            if (mr == QMessageBox::Yes)
                m_mdlTests.removeRow(index.row());
        }
    }
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
}

void DailyProgramsEditor::on_dpAdd()
{

}

void DailyProgramsEditor::on_dpEdit()
{

}

void DailyProgramsEditor::on_dpDel()
{

}
