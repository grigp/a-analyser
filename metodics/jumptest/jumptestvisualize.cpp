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

        //! Высота прыжка
        auto* fctHeight = m_calculator->primaryFactor(0);
        auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(fctHeight->uid());
        QString fv = QString::number(fctHeight->value(), 'f', fi.format());
        ui->lblJumpHeight->setText(QString(tr("Высота прыжка") + " : %1 " + tr("м")).arg(fv));

        //! Для методики соскакивания - время задержки на платформе
        ui->lblTime->setVisible(m_calculator->metodic() == JumpTestDefines::MetHopping);
        if (m_calculator->metodic() == JumpTestDefines::MetHopping &&
                m_calculator->primaryFactorsCount() > 1)
        {
            auto* fctTime = m_calculator->primaryFactor(1);
            auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(fctTime->uid());
            QString fv = QString::number(fctTime->value(), 'f', fi.format());
            ui->lblTime->setText(QString(tr("Задержка на платформе") + " : %1 " + tr("сек")).arg(fv));
        }
    }
}
