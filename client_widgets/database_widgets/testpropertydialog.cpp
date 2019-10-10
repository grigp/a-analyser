#include "testpropertydialog.h"
#include "ui_testpropertydialog.h"

#include <QDebug>

#include "aanalyserapplication.h"

TestPropertyDialog::TestPropertyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestPropertyDialog)
{
    ui->setupUi(this);
    ui->twCatgories->setCurrentIndex(0);
    loadConditions();
}

TestPropertyDialog::~TestPropertyDialog()
{
    delete ui;
}

QString TestPropertyDialog::comment()
{
    return ui->edComment->toPlainText();
}

void TestPropertyDialog::setComment(const QString &comment)
{
    ui->edComment->setText(comment);
}

QString TestPropertyDialog::condition()
{
    return ui->cbConditions->currentData(UidRole).toString();
}

void TestPropertyDialog::setCondition(const QString &condition)
{
    if (condition != "")
    {
        for (int i = 0; i < ui->cbConditions->count(); ++i)
            if (ui->cbConditions->itemData(i, UidRole).toString() == condition)
                ui->cbConditions->setCurrentIndex(i);
    }
    else
        if (ui->cbConditions->count() > 0)
        {
            ui->cbConditions->setCurrentIndex(0);
            conditionChanged(0);
        }
}

bool TestPropertyDialog::normContained()
{
    return ui->cbInNormContained->isChecked();
}

void TestPropertyDialog::setNormContained(const bool nc)
{
    ui->cbInNormContained->setChecked(nc);
}

void TestPropertyDialog::conditionChanged(int idx)
{
    Q_UNUSED(idx);
    QString description = ui->cbConditions->currentData(DescriptionRole).toString();
    ui->edConditionComment->setText(description);
    bool isNorm = ui->cbConditions->currentData(NormsEnabledRole).toBool();
    ui->cbInNormContained->setEnabled(isNorm);
}

void TestPropertyDialog::loadConditions()
{
    auto condUids = static_cast<AAnalyserApplication*>(QApplication::instance())->getTestConditions();
    foreach (auto condUid, condUids)
    {
        DataDefines::TestConditionInfo ci;
        if (static_cast<AAnalyserApplication*>(QApplication::instance())->getTestConditionInfo(condUid, ci))
        {
            ui->cbConditions->addItem(ci.name);
            ui->cbConditions->setItemData(ui->cbConditions->count()-1, ci.uid, UidRole);
            ui->cbConditions->setItemData(ui->cbConditions->count()-1, ci.description, DescriptionRole);
            ui->cbConditions->setItemData(ui->cbConditions->count()-1, ci.norms_enabled, NormsEnabledRole);
        }
    }
}
