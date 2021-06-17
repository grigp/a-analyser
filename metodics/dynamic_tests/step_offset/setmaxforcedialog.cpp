#include "setmaxforcedialog.h"
#include "ui_setmaxforcedialog.h"

#include <QDebug>

SetMaxForceDialog::SetMaxForceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetMaxForceDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    ui->wgtDiag->setDiap(128);
    ui->wgtDiag->setBackgroundColor(Qt::lightGray);
}

SetMaxForceDialog::~SetMaxForceDialog()
{
    delete ui;
}

void SetMaxForceDialog::setDirection(const BaseUtils::Directions direction)
{
    m_direction = direction;
    ui->wgtDiag->setIsShowValueUp(direction == BaseUtils::dirUp);
    ui->wgtDiag->setIsShowValueDown(direction == BaseUtils::dirDown);
    ui->wgtDiag->setIsShowValueRight(direction == BaseUtils::dirRight);
    ui->wgtDiag->setIsShowValueLeft(direction == BaseUtils::dirLeft);
}

void SetMaxForceDialog::getData(const double x, const double y)
{
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);

    if (m_direction == BaseUtils::dirUp)
    {
        if (iy > ui->wgtDiag->valueUp())
            ui->wgtDiag->setValueUp(iy);
    }
    else
    if (m_direction == BaseUtils::dirDown)
    {
        if (abs(iy) > ui->wgtDiag->valueDown())
            ui->wgtDiag->setValueDown(abs(iy));
    }
    else
    if (m_direction == BaseUtils::dirRight)
    {
        if (ix > ui->wgtDiag->valueRight())
            ui->wgtDiag->setValueRight(ix);
    }
    else
    if (m_direction == BaseUtils::dirLeft)
    {
        if (abs(ix) > ui->wgtDiag->valueLeft())
            ui->wgtDiag->setValueLeft(abs(ix));
    }
}

int SetMaxForceDialog::value() const
{
    if (m_direction == BaseUtils::dirUp)
        return ui->wgtDiag->valueUp();
    else
    if (m_direction == BaseUtils::dirDown)
        return ui->wgtDiag->valueDown();
    else
    if (m_direction == BaseUtils::dirRight)
        return ui->wgtDiag->valueRight();
    else
    if (m_direction == BaseUtils::dirLeft)
        return ui->wgtDiag->valueLeft();
    else
        return 0;
}

void SetMaxForceDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);
    ui->wgtDiag->setValueUp(0);
    ui->wgtDiag->setValueDown(0);
    ui->wgtDiag->setValueRight(0);
    ui->wgtDiag->setValueLeft(0);
}

void SetMaxForceDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
}

void SetMaxForceDialog::resetValue()
{
    ui->wgtDiag->setValueUp(0);
    ui->wgtDiag->setValueDown(0);
    ui->wgtDiag->setValueRight(0);
    ui->wgtDiag->setValueLeft(0);
}
