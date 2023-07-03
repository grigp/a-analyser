#include "personalprogramwidget.h"
#include "ui_personalprogramwidget.h"

#include <QUuid>
#include <QDebug>

#include "aanalyserapplication.h"
#include "settingsprovider.h"
#include "selectmethodicdialog.h"
#include "metodicsfactory.h"

PersonalProgramWidget::PersonalProgramWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::PersonalProgramWidget)
{
    ui->setupUi(this);

    restoreSplitterPosition();
}

PersonalProgramWidget::~PersonalProgramWidget()
{
    delete ui;
}

QString PersonalProgramWidget::uid()
{
    return ClientWidgets::uidPersonalProgramWidgetUid;
}

QString PersonalProgramWidget::name()
{
    return tr("Индивидуальные программы");
}

void PersonalProgramWidget::onShow()
{

}

void PersonalProgramWidget::onHide()
{

}

void PersonalProgramWidget::on_splitterMoved(int, int)
{
    saveSplitterPosition();
}

void PersonalProgramWidget::on_run()
{
// Пример, как работать с диалогом выбора методики
//    SelectMethodicDialog dialog(this);
//    if (dialog.exec() == QDialog::Accepted)
//    {
//        auto metUid = dialog.methodic();
//        if (metUid != QUuid().toString())
//        {
//            MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
//            auto mi = metFactory->metodic(metUid);
//            qDebug() << mi.name;
//        }
//        else
//            qDebug() << "Методика не выбрана";
//    }
}

void PersonalProgramWidget::on_delete()
{

}

void PersonalProgramWidget::on_params()
{

}

void PersonalProgramWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("PersonalProgramWidget", "SplitterPosition", ui->splitter->saveState());
}

void PersonalProgramWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("PersonalProgramWidget", "SplitterPosition").toByteArray();
//    if (val == "")
//        val = QByteArray::fromRawData("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x01\x1B\x00\x00\x03""b\x01\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x01\x00", 31);
    ui->splitter->restoreState(val);
}
