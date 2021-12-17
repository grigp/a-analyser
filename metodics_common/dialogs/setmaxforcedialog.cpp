#include "setmaxforcedialog.h"
#include "ui_setmaxforcedialog.h"

#include <QDebug>

SetMaxForceDialog::SetMaxForceDialog(SetMaxForceDialog::Mode mode, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetMaxForceDialog)
{
    ui->setupUi(this);
    m_mode = mode;
    setWindowFlags(Qt::WindowStaysOnTopHint);
    if (m_mode == SetMaxForceDialog::CrossOneLine)
        ui->wgtDiagCross->setDiap(128);
    ui->wgtDiagCross->setVisible(m_mode == SetMaxForceDialog::CrossOneLine);
    ui->wgtDiagTriangle->setVisible(m_mode == SetMaxForceDialog::Triangle);
}


SetMaxForceDialog::~SetMaxForceDialog()
{
    delete ui;
}

void SetMaxForceDialog::setDirection(const BaseUtils::Directions direction)
{
    m_direction = direction;
    if (m_mode == SetMaxForceDialog::CrossOneLine)
    {
        ui->wgtDiagCross->setIsShowValueUp(direction == BaseUtils::dirUp);
        ui->wgtDiagCross->setIsShowValueDown(direction == BaseUtils::dirDown);
        ui->wgtDiagCross->setIsShowValueRight(direction == BaseUtils::dirRight);
        ui->wgtDiagCross->setIsShowValueLeft(direction == BaseUtils::dirLeft);
    }
}

void SetMaxForceDialog::getData(const double x, const double y)
{
    int ix = static_cast<int>(x);
    int iy = static_cast<int>(y);

    if (m_mode == SetMaxForceDialog::CrossOneLine)
        getDataCrossOneLine(ix, iy);
    else
    if (m_mode == SetMaxForceDialog::Triangle)
        getDataTriangle(ix, iy);

}

int SetMaxForceDialog::value() const
{
    if (m_mode == SetMaxForceDialog::CrossOneLine)
    {
        if (m_direction == BaseUtils::dirUp)
            return ui->wgtDiagCross->valueUp();
        else
        if (m_direction == BaseUtils::dirDown)
            return ui->wgtDiagCross->valueDown();
        else
        if (m_direction == BaseUtils::dirRight)
            return ui->wgtDiagCross->valueRight();
        else
        if (m_direction == BaseUtils::dirLeft)
            return ui->wgtDiagCross->valueLeft();
    }

    return 0;
}

int SetMaxForceDialog::value(const int direction) const
{
    if (m_mode == SetMaxForceDialog::CrossOneLine)
    {
        if (direction == 0)
            return ui->wgtDiagCross->valueUp();
        else
        if (direction == 1)
            return ui->wgtDiagCross->valueRight();
        else
        if (direction == 2)
            return ui->wgtDiagCross->valueDown();
        else
        if (direction == 3)
            return ui->wgtDiagCross->valueLeft();
    }
    else
    if (m_mode == SetMaxForceDialog::Triangle)
    {
        if (direction == 0)
            return ui->wgtDiagTriangle->valueUp();
        else
        if (direction == 1)
            return ui->wgtDiagTriangle->valueLeftDown();
        else
        if (direction == 2)
            return ui->wgtDiagTriangle->valueRightDown();
    }

    return 0;
}

void SetMaxForceDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    if (m_mode == SetMaxForceDialog::CrossOneLine)
    {
        ui->wgtDiagCross->setValueUp(0);
        ui->wgtDiagCross->setValueDown(0);
        ui->wgtDiagCross->setValueRight(0);
        ui->wgtDiagCross->setValueLeft(0);
    }
    ui->btnOK->setEnabled(false);
}

void SetMaxForceDialog::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
}

void SetMaxForceDialog::resetValue()
{
    if (m_mode == SetMaxForceDialog::CrossOneLine)
    {
        ui->wgtDiagCross->setValueUp(0);
        ui->wgtDiagCross->setValueDown(0);
        ui->wgtDiagCross->setValueRight(0);
        ui->wgtDiagCross->setValueLeft(0);
    }
    else
    if (m_mode == SetMaxForceDialog::Triangle)
    {
        ui->wgtDiagTriangle->setValueUp(0);
        ui->wgtDiagTriangle->setValueLeftDown(0);
        ui->wgtDiagTriangle->setValueRightDown(0);
    }
    ui->btnOK->setEnabled(false);
}

void SetMaxForceDialog::getDataCrossOneLine(const int x, const int y)
{
    if (m_direction == BaseUtils::dirUp)
    {
        if (y > ui->wgtDiagCross->valueUp())
        {
            ui->wgtDiagCross->setValueUp(y);
            if (y > m_minValueOffset)
                ui->btnOK->setEnabled(true);
        }
    }
    else
    if (m_direction == BaseUtils::dirDown)
    {
        if (abs(y) > ui->wgtDiagCross->valueDown())
        {
            ui->wgtDiagCross->setValueDown(abs(y));
            if (abs(y) > m_minValueOffset)
                ui->btnOK->setEnabled(true);
        }
    }
    else
    if (m_direction == BaseUtils::dirRight)
    {
        if (x > ui->wgtDiagCross->valueRight())
        {
            ui->wgtDiagCross->setValueRight(x);
            if (x > m_minValueOffset)
                ui->btnOK->setEnabled(true);
        }
    }
    else
    if (m_direction == BaseUtils::dirLeft)
    {
        if (abs(x) > ui->wgtDiagCross->valueLeft())
        {
            ui->wgtDiagCross->setValueLeft(abs(x));
            if (abs(x) > m_minValueOffset)
                ui->btnOK->setEnabled(true);
        }
    }
}

void SetMaxForceDialog::getDataTriangle(const int x, const int y)
{
    if (y > 0)
    {
        if (y > ui->wgtDiagTriangle->valueUp())
            ui->wgtDiagTriangle->setValueUp(y);
    }
    else
    if (y < 0 && x > 0)
    {
        int v = static_cast<int>(sqrt(x * x + y * y));
        if (v > ui->wgtDiagTriangle->valueRightDown())
            ui->wgtDiagTriangle->setValueRightDown(v);
    }
    else
    if (y < 0 && x < 0)
    {
        int v = static_cast<int>(sqrt(x * x + y * y));
        if (v > ui->wgtDiagTriangle->valueLeftDown())
            ui->wgtDiagTriangle->setValueLeftDown(v);
    }

    if (ui->wgtDiagTriangle->valueUp() > m_minValueOffset &&
            ui->wgtDiagTriangle->valueLeftDown() > m_minValueOffset &&
            ui->wgtDiagTriangle->valueRightDown() > m_minValueOffset)
        ui->btnOK->setEnabled(true);

}
