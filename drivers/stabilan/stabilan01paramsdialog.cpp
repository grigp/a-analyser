#include "stabilan01paramsdialog.h"
#include "ui_stabilan01paramsdialog.h"

#include "stabilan01.h"

Stabilan01ParamsDialog::Stabilan01ParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Stabilan01ParamsDialog)
{
    ui->setupUi(this);

    ui->tabPages->setCurrentIndex(0);

    foreach (auto modelCode, Stabilan01::models())
        ui->cbModel->addItem(Stabilan01::modelName(modelCode), modelCode);

    foreach (auto ztCode, Stabilan01::zeroingTypes())
        ui->cbZeroingType->addItem(Stabilan01::zeroingTypeName(ztCode), ztCode);

    ui->cbTensoChan1->addItems(QStringList() << "Кистевой силомер" << "Становой силомер" << "Дыхание");
    ui->cbTensoChan2->addItems(QStringList() << "Кистевой силомер" << "Становой силомер" << "Дыхание");
    ui->cbTensoChan3->addItems(QStringList() << "Кистевой силомер" << "Становой силомер" << "Дыхание");
}

Stabilan01ParamsDialog::~Stabilan01ParamsDialog()
{
    delete ui;
}

Stabilan01Defines::Model Stabilan01ParamsDialog::model() const
{
    return static_cast<Stabilan01Defines::Model>(ui->cbModel->currentData().toInt());
}

void Stabilan01ParamsDialog::setModel(const Stabilan01Defines::Model mdl)
{
    ui->cbModel->setCurrentText(Stabilan01::modelName(mdl));
}

Stabilan01Defines::ZeroingType Stabilan01ParamsDialog::zeroingType() const
{
    return static_cast<Stabilan01Defines::ZeroingType>(ui->cbZeroingType->currentData().toInt());
}

void Stabilan01ParamsDialog::setZeroingType(const Stabilan01Defines::ZeroingType zt)
{
    ui->cbZeroingType->setCurrentText(Stabilan01::zeroingTypeName(zt));
}

void Stabilan01ParamsDialog::setRecording(const QMap<QString, bool> &recMap)
{
    ui->cbRecStab->setChecked(recMap.value(ChannelsDefines::chanStab));
    ui->cbRecZ->setChecked(recMap.value(ChannelsDefines::chanZ));
    ui->cbRecMyo->setChecked(recMap.value(ChannelsDefines::chanMyogram));
    ui->cbRecPulse->setChecked(recMap.value(ChannelsDefines::chanRitmogram));
    ui->cbRecTenso1->setChecked(recMap.value(ChannelsDefines::chanTenso1));
    ui->cbRecTenso2->setChecked(recMap.value(ChannelsDefines::chanTenso2));
    ui->cbRecTenso3->setChecked(recMap.value(ChannelsDefines::chanTenso3));
    ui->cbRecMyoChan1->setChecked(recMap.value("MyoChan0"));
    ui->cbRecMyoChan2->setChecked(recMap.value("MyoChan1"));
    ui->cbRecMyoChan3->setChecked(recMap.value("MyoChan2"));
    ui->cbRecMyoChan4->setChecked(recMap.value("MyoChan3"));
}

QMap<QString, bool> Stabilan01ParamsDialog::getRecording() const
{
    QMap<QString, bool> retval;
    retval.insert(ChannelsDefines::chanStab, ui->cbRecStab->isChecked());
    retval.insert(ChannelsDefines::chanZ, ui->cbRecZ->isChecked());
    retval.insert(ChannelsDefines::chanMyogram, ui->cbRecMyo->isChecked());
    retval.insert(ChannelsDefines::chanRitmogram, ui->cbRecPulse->isChecked());
    retval.insert(ChannelsDefines::chanTenso1, ui->cbRecTenso1->isChecked());
    retval.insert(ChannelsDefines::chanTenso2, ui->cbRecTenso2->isChecked());
    retval.insert(ChannelsDefines::chanTenso3, ui->cbRecTenso3->isChecked());
    retval.insert("MyoChan0", ui->cbRecMyoChan1->isChecked());
    retval.insert("MyoChan1", ui->cbRecMyoChan2->isChecked());
    retval.insert("MyoChan2", ui->cbRecMyoChan3->isChecked());
    retval.insert("MyoChan3", ui->cbRecMyoChan4->isChecked());
    return retval;
}

void Stabilan01ParamsDialog::on_selectModel(int modelIdx)
{
    Q_UNUSED(modelIdx);
    ui->cbRecPulse->setVisible(Stabilan01Defines::ModelsWithPulse.contains(model()));
    ui->cbRecTenso1->setVisible(Stabilan01Defines::ModelsWithTenso.contains(model()));
    ui->cbRecTenso2->setVisible(Stabilan01Defines::ModelsWithTenso.contains(model()));
    ui->cbRecTenso3->setVisible(Stabilan01Defines::ModelsWithTenso.contains(model()));
    ui->cbRecMyo->setVisible(Stabilan01Defines::ModelsWithMyo.contains(model()));
    ui->gbRecMyoChans->setVisible(Stabilan01Defines::ModelsWithMyo.contains(model()));

    ui->tabPages->setTabEnabled(2, Stabilan01Defines::ModelsWithTenso.contains(model()));
}

void Stabilan01ParamsDialog::on_selectZeroingType(int zcIdx)
{

}

