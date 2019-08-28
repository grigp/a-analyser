#include "jumptestvisualize.h"
#include "ui_jumptestvisualize.h"

#include "jumptestcalculator.h"
#include "aanalyserapplication.h"

#include <QDebug>

JumpTestVisualize::JumpTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::JumpTestVisualize)
{
    ui->setupUi(this);
}

JumpTestVisualize::~JumpTestVisualize()
{
    delete ui;
}

void JumpTestVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new JumpTestCalculator(testUid, this);
        m_calculator->calculate();

        auto* fctHeight = m_calculator->primaryFactor(0);
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(fctHeight->uid());
        qDebug() << fctHeight->value() << fi.format() << fctHeight->uid();
        QString fv = QString::number(fctHeight->value(), 'f', fi.format());
        qDebug() << "1";
        qDebug() << fv;
        ui->lblJumpHeight->setText(QString(tr("Высота прыжка") + " : %1 " + tr("м")).arg(fv));
//        ui->lblTime->setText(QString);
    }

    Q_UNUSED(testUid);


}
