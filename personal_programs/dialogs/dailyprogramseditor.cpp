#include "dailyprogramseditor.h"
#include "ui_dailyprogramseditor.h"

#include <QUuid>
#include <QDebug>

#include "aanalyserapplication.h"
#include "selectmethodicdialog.h"
#include "metodicsfactory.h"

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
            qDebug() << mi.name;
        }
        else
            qDebug() << "Методика не выбрана";
    }
}

void DailyProgramsEditor::on_editTest()
{

}

void DailyProgramsEditor::on_delTest()
{

}

void DailyProgramsEditor::on_moveTestUp()
{

}

void DailyProgramsEditor::on_moveTestDown()
{

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
