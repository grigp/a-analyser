#include "boxerdodgingpatientwindow.h"
#include "ui_boxerdodgingpatientwindow.h"

#include <QTimer>

BoxerDodgingPatientWindow::BoxerDodgingPatientWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BoxerDodgingPatientWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint);


    QTimer::singleShot(100, [=]()
    {
        ui->wgtBoxing->setBackground(":/images/Boxing/Ring.png");
        ui->wgtBoxing->addPicture(BoxerDodgingDefines::bdsBase, ":/images/Boxing/BoxerBase.png",
                                  QRectF(ui->wgtBoxing->geometry().width() / 2 - 700 / 2,
                                         ui->wgtBoxing->geometry().height() - 700, 700, 700),
                                  true);
        ui->wgtBoxing->addPicture(BoxerDodgingDefines::bdsLeftDodging, ":/images/Boxing/BoxerLeftDodging.png",
                                  QRectF(ui->wgtBoxing->geometry().width() / 2 - 700 / 2,
                                         ui->wgtBoxing->geometry().height() - 700, 700, 700),
                                  false);
        ui->wgtBoxing->addPicture(BoxerDodgingDefines::bdsRightDodging, ":/images/Boxing/BoxerRightDodging.png",
                                  QRectF(ui->wgtBoxing->geometry().width() / 2 - 700 / 2,
                                         ui->wgtBoxing->geometry().height() - 700, 700, 700),
                                  false);
        ui->wgtBoxing->addPicture(BoxerDodgingDefines::bdsAheadBend, ":/images/Boxing/BoxerAheadBend.png",
                                  QRectF(ui->wgtBoxing->geometry().width() / 2 - 700 / 2,
                                         ui->wgtBoxing->geometry().height() - 700, 700, 700),
                                  false);
        ui->wgtBoxing->addPicture(BoxerDodgingDefines::bdsBackBend, ":/images/Boxing/BoxerBackBend.png",
                                  QRectF(ui->wgtBoxing->geometry().width() / 2 - 700 / 2,
                                         ui->wgtBoxing->geometry().height() - 700, 700, 700),
                                  false);
    });
}

BoxerDodgingPatientWindow::~BoxerDodgingPatientWindow()
{
    delete ui;
}

void BoxerDodgingPatientWindow::setVisibleStage(const BoxerDodgingDefines::Stages stage, const bool visible)
{
    ui->wgtBoxing->setIsVisible(stage, visible);
}