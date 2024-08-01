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

PatientProgramWidget *PatientProgramVisualsWidget::getCurrentPPW()
{
//    if (m_tabs)
//    {
//        if (m_tabs->currentIndex() == 0)
//            return static_cast<PatientProgramWidget*>(m_tabs->currentWidget());
//        return nullptr;
//    }
    return m_wgtPP;
}

PPVisual *PatientProgramVisualsWidget::getCurrentVisual()
{
    //! Если табулятора нет, то и визуалов тоже нет
    if (m_tabs)
    {
        //! Нулевой всегда динамика ИП. Если текущий больше нуля, то
        if (m_tabs->currentIndex() > 0)
            return static_cast<PPVisual*>(m_tabs->currentWidget());
    }
    return nullptr;
}

void PatientProgramVisualsWidget::on_currentChanged(int index)
{
    emit tabChanged(index);
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
        m_tabs = new QTabWidget(ui->frContainer);
        ui->frContainer->layout()->addWidget(m_tabs);

        //! Добавим в него виджет индивидуальной программы
        m_tabs->addTab(m_wgtPP, tr("Динамика выполнения"));
        //! И все виджеты визуализатором
        foreach (auto visual, m_visuals)
        {
            m_tabs->addTab(visual, visual->name());
            visual->calculate();
        }

        connect(m_tabs, &QTabWidget::currentChanged, this, &PatientProgramVisualsWidget::on_currentChanged);

        //! И покажем первый визуализатор
        m_tabs->setCurrentIndex(1);
    }
}
