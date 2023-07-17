#include "selectpersonalprogramdialog.h"
#include "ui_selectpersonalprogramdialog.h"

#include <QUuid>
#include <QPushButton>
#include <QDebug>

#include "aanalyserapplication.h"
#include "personalprogramdefines.h"


SelectPersonalProgramDialog::SelectPersonalProgramDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectPersonalProgramDialog)
{
    ui->setupUi(this);
}

SelectPersonalProgramDialog::~SelectPersonalProgramDialog()
{
    delete ui;
}

QString SelectPersonalProgramDialog::personalProgramUid() const
{

}

int SelectPersonalProgramDialog::exec()
{
    ui->tvListPP->header()->setVisible(false);
    static_cast<AAnalyserApplication*>(QApplication::instance())->readPersonalProgramList(m_mdlPrograms);
    ui->tvListPP->setModel(&m_mdlPrograms);

    connect(ui->tvListPP, &QTreeView::doubleClicked, this, &SelectPersonalProgramDialog::on_doubleClicked);
    connect(ui->tvListPP->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &SelectPersonalProgramDialog::on_selectMetodicChanged);

    auto btnOK = ui->buttonBox->button(QDialogButtonBox::Ok);
    btnOK->setEnabled(false);

    return QDialog::exec();
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
    auto idx = ui->tvListPP->selectionModel()->currentIndex();
    auto btnOK = ui->buttonBox->button(QDialogButtonBox::Ok);
    btnOK->setEnabled(idx.isValid());
}
