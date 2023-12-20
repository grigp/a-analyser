#include "patientprogramvisualswidget.h"
#include "ui_patientprogramvisualswidget.h"

#include <QDebug>

#include "patientprogramwidget.h"
#include "patientprogramvisualswidget.h"
#include "ppvisual.h"

PatientProgramVisualsWidget::PatientProgramVisualsWidget(PatientProgramWidget* wgtPP,
                                                         QList<PPVisual*> visuals,
                                                         QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PatientProgramVisualsWidget)
  , m_wgtPP(wgtPP)
  , m_visuals(visuals)
{
    ui->setupUi(this);
    assign();
}

PatientProgramVisualsWidget::~PatientProgramVisualsWidget()
{
    delete ui;
}

void PatientProgramVisualsWidget::assignPersonalProgram(const QString &uidPP)
{
    if (m_wgtPP)
        m_wgtPP->assignPersonalProgram(uidPP);
}

QModelIndex PatientProgramVisualsWidget::selectedIndex() const
{
    if (m_wgtPP)
        return m_wgtPP->selectedIndex();
    return QModelIndex();
}

QString PatientProgramVisualsWidget::namePP() const
{
    if (m_wgtPP)
        return m_wgtPP->namePP();
    return "";
}

void PatientProgramVisualsWidget::assign()
{
    //! Если визуализаторов нет, то покажем только виджет индивидуальной программы
    if (m_visuals.size() == 0)
    {
        m_wgtPP->setParent(ui->frContainer);
        ui->frContainer->layout()->addWidget(m_wgtPP);
    }
    else
    //! Если визуализаторы есть, то
    {
        //! Создадим табулятор
        auto tw = new QTabWidget(ui->frContainer);
        ui->frContainer->layout()->addWidget(tw);

        //! Добавим в него виджет индивидуальной программы
        tw->addTab(m_wgtPP, tr("Индивидуальная программа"));
        //! И все виджеты визуализатором
        foreach (auto visual, m_visuals)
            tw->addTab(visual, visual->name());
        //! И покажем первый визуализатор
        tw->setCurrentIndex(1);
    }
}
