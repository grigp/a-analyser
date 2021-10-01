#include "boxerdodgingpatientwindow.h"
#include "ui_boxerdodgingpatientwindow.h"

#include "boxerdodgingdefines.h"

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
    });
}

BoxerDodgingPatientWindow::~BoxerDodgingPatientWindow()
{
    delete ui;
}
