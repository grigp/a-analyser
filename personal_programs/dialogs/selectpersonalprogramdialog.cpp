#include "selectpersonalprogramdialog.h"
#include "ui_selectpersonalprogramdialog.h"

#include <QUuid>
#include <QPushButton>
#include <QFileDialog>
#include <QDebug>

#include "aanalyserapplication.h"
#include "personalprogramdefines.h"
#include "settingsprovider.h"


SelectPersonalProgramDialog::SelectPersonalProgramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectPersonalProgramDialog)
{
    ui->setupUi(this);

    auto val = SettingsProvider::valueFromRegAppCopy("Geometry/SelectPersonalProgramDialog", "Geometry").toRect();
    if (val != QRect())
    {
        setGeometry(val);
    }
}

SelectPersonalProgramDialog::~SelectPersonalProgramDialog()
{
    delete ui;
}

SelectPersonalProgramDialog::SelectMode SelectPersonalProgramDialog::mode() const
{
    if (ui->rbListDP->isChecked())
        return smDefault;
    else
    if (ui->rbFromFile->isChecked())
        return smFromFile;
    return smNone;
}

QString SelectPersonalProgramDialog::personalProgramUid() const
{
    auto objPP = personalProgram();
    if (objPP != QJsonObject())
        return objPP["uid"].toString();
    return QUuid().toString();
}

QString SelectPersonalProgramDialog::fileName() const
{
    return ui->edFileName->text();
}

QJsonObject SelectPersonalProgramDialog::personalProgram() const
{
    QModelIndexList selIdxs = ui->tvListPP->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            auto item = m_mdlPrograms.itemFromIndex(index);
            return item->data(PersonalProgramDefines::TablePPRoles::PPRole).toJsonObject();
        }
    }
    return QJsonObject();
}

int SelectPersonalProgramDialog::exec()
{
    setStyleSheet(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow()->styleSheet());

    ui->tvListPP->header()->setVisible(false);
    static_cast<AAnalyserApplication*>(QApplication::instance())->readPersonalProgramList(m_mdlPrograms);
    ui->tvListPP->setModel(&m_mdlPrograms);

    connect(ui->tvListPP, &QTreeView::doubleClicked, this, &SelectPersonalProgramDialog::on_doubleClicked);
    connect(ui->tvListPP->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SelectPersonalProgramDialog::on_selectMetodicChanged);

    ui->btnOK->setEnabled(false);
    ui->tvListPP->setEnabled(ui->rbListDP->isChecked());
    ui->frFromFile->setEnabled(ui->rbFromFile->isChecked());

    return QDialog::exec();
}

void SelectPersonalProgramDialog::closeEvent(QCloseEvent *)
{
    SettingsProvider::setValueToRegAppCopy("Geometry/SelectPersonalProgramDialog", "Geometry", geometry());
}

void SelectPersonalProgramDialog::resizeEvent(QResizeEvent *event)
{
    SettingsProvider::setValueToRegAppCopy("Geometry/SelectPersonalProgramDialog", "Geometry", geometry());
    QDialog::resizeEvent(event);
}

void SelectPersonalProgramDialog::on_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    accept();
}

void SelectPersonalProgramDialog::on_selectMetodicChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    setEnabledOK();
}

void SelectPersonalProgramDialog::on_selectFileName()
{
    QString path = DataDefines::aanalyserDocumentsPath();
    auto fileName = QFileDialog::getOpenFileName(this, tr("Файл индивидуальной программы"), path, tr("Файлы индивидуальных программ (*.pp)"));
    if (fileName != "")
    {
        ui->edFileName->setText(fileName);
        setEnabledOK();
    }
}

void SelectPersonalProgramDialog::on_clickedMode()
{
    ui->tvListPP->setEnabled(ui->rbListDP->isChecked());
    ui->frFromFile->setEnabled(ui->rbFromFile->isChecked());
    setEnabledOK();
}

void SelectPersonalProgramDialog::on_changedFileName(QString fn)
{
    Q_UNUSED(fn);
    setEnabledOK();
}

void SelectPersonalProgramDialog::setEnabledOK()
{
    auto idx = ui->tvListPP->selectionModel()->currentIndex();
    ui->btnOK->setEnabled((ui->rbListDP->isChecked() && idx.isValid()) ||
                          (ui->rbFromFile->isChecked() && ui->edFileName->text() != ""));
}
