#include "personalprogramseditor.h"
#include "ui_personalprogramseditor.h"

#include <QDebug>

#include "selectdailyprogramdialog.h"

PersonalProgramsEditor::PersonalProgramsEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersonalProgramsEditor)
{
    ui->setupUi(this);
}

PersonalProgramsEditor::~PersonalProgramsEditor()
{
    delete ui;
}

void PersonalProgramsEditor::on_addDP()
{
    if (!m_dlgSelDP)
        m_dlgSelDP = new SelectDailyProgramDialog(this);

    if (m_dlgSelDP->exec() == QDialog::Accepted)
    {
        auto dpUid = m_dlgSelDP->dailyProgram();
//        auto metUid = m_dlgSelMethod->methodic();
//        if (metUid != QUuid().toString())
//        {
//            MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
//            auto mi = metFactory->metodic(metUid);

//            auto *item = new QStandardItem(mi.name);
//            item->setEditable(false);
//            item->setData(metUid, PersonalProgramDefines::TableTestsRoles::MethodUidRole);
//            item->setData(mi.params, PersonalProgramDefines::TableTestsRoles::MethodParamsRole);
//            item->setIcon(QIcon(":/images/Methodics/" + mi.imageName));
//            m_mdlTests.appendRow(item);
//        }
//        else
//            QMessageBox::information(nullptr, tr("Сообщение"), tr("Методика не выбрана"));
    }
}
