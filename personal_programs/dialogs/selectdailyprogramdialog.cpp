#include "selectdailyprogramdialog.h"
#include "ui_selectdailyprogramdialog.h"

#include <QUuid>
#include <QPushButton>
#include <QDebug>

#include "aanalyserapplication.h"
#include "personalprogramdefines.h"

SelectDailyProgramDialog::SelectDailyProgramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectDailyProgramDialog)
{
    ui->setupUi(this);
}

SelectDailyProgramDialog::~SelectDailyProgramDialog()
{
    delete ui;
}

QString SelectDailyProgramDialog::dailyProgram() const
{
    QModelIndexList selIdxs = ui->tvListDP->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
    {
        auto index = selIdxs.at(0);
        if (index.isValid())
        {
            auto item = m_mdlPrograms.itemFromIndex(index);
            auto objDP = item->data(PersonalProgramDefines::TableDPRoles::DPRole).toJsonObject();
            return objDP["uid"].toString();
        }
    }
    return QUuid().toString();
}

int SelectDailyProgramDialog::exec()
{
    ui->tvListDP->header()->setVisible(false);
    static_cast<AAnalyserApplication*>(QApplication::instance())->readDailyProgramList(m_mdlPrograms);
    ui->tvListDP->setModel(&m_mdlPrograms);

    connect(ui->tvListDP, &QTreeView::doubleClicked, this, &SelectDailyProgramDialog::on_doubleClicked);
    connect(ui->tvListDP->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SelectDailyProgramDialog::on_selectMetodicChanged);

    auto btnOK = ui->buttonBox->button(QDialogButtonBox::Ok);
    btnOK->setEnabled(false);

    return QDialog::exec();
}

void SelectDailyProgramDialog::on_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);
    accept();
}

void SelectDailyProgramDialog::on_selectMetodicChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    auto idx = ui->tvListDP->selectionModel()->currentIndex();
    auto btnOK = ui->buttonBox->button(QDialogButtonBox::Ok);
    btnOK->setEnabled(idx.isValid());
}
