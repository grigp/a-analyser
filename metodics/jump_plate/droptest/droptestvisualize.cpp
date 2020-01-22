#include "droptestvisualize.h"
#include "ui_droptestvisualize.h"

#include "droptestcalculator.h"
#include "datadefines.h"
#include "dataprovider.h"

DropTestVisualize::DropTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DropTestVisualize)
{
    ui->setupUi(this);
}

DropTestVisualize::~DropTestVisualize()
{
    delete ui;
}

void DropTestVisualize::setTest(const QString &testUid)
{
    if (!m_calculator)
    {
        m_calculator = new DropTestCalculator(testUid, this);
        m_calculator->calculate();
    }

    if (m_calculator->primaryFactorsCount() >= 5)
    {
        auto fctJumpsCount = m_calculator->primaryFactor(0);
        auto fctTimeTest = m_calculator->primaryFactor(1);
        auto fctHeightAvg = m_calculator->primaryFactor(2);
        auto fctTimeContactAvg = m_calculator->primaryFactor(3);
        auto fctTimeNoContactAvg = m_calculator->primaryFactor(4);
        ui->lblJumpsCount->setText(QString(tr("Количество прыжков") + " %1").arg(fctJumpsCount->value()));
        ui->lblTimeTest->setText(QString(tr("Общее время прыжков") + " %1 " + tr("сек")).arg(fctTimeTest->value()));
        ui->lblHeightAvg->setText(QString(tr("Средняя высота прыжков") + " %1 " + tr("см")).arg(fctHeightAvg->value()));
        ui->lblTimeContactAvg->setText(QString(tr("Среднее время контактной фазы") + " %1 " + tr("сек")).arg(fctTimeContactAvg->value()));
        ui->lblTimeNoContactAvg->setText(QString(tr("Среднее время бесконтактной фазы") + " %1 " + tr("сек")).arg(fctTimeNoContactAvg->value()));

        auto* model = new QStandardItemModel(ui->tvJumpsDrop);

        for (int i = 0; i < m_calculator->jumpsCount(); ++i)
        {
            auto jump = m_calculator->jump(i);

            auto *itemN = new QStandardItem(QString::number(i + 1));
            itemN->setEditable(false);
            auto *itemC = new QStandardItem(QString::number(jump.timeContact));
            itemC->setEditable(false);
            auto *itemNC = new QStandardItem(QString::number(jump.timeNoContact));
            itemNC->setEditable(false);
            auto *itemM = new QStandardItem(QString::number(getPatientMassa(testUid)));
            itemM->setEditable(false);
            auto *itemFH = new QStandardItem(QString::number(jump.fallHeight));
            itemFH->setEditable(false);
            auto *itemH = new QStandardItem(QString::number(jump.height));
            itemH->setEditable(false);
            auto *itemP = new QStandardItem(QString::number(jump.power));
            itemP->setEditable(false);
            auto *itemS = new QStandardItem(QString::number(jump.stiffness));
            itemS->setEditable(false);
            auto *itemIS = new QStandardItem(QString::number(jump.initialSpeed));
            itemIS->setEditable(false);
            auto *itemRSI = new QStandardItem(QString::number(jump.rsi));
            itemRSI->setEditable(false);

            model->appendRow(QList<QStandardItem*>() << itemN << itemC << itemNC << itemM << itemFH << itemH
                             << itemP << itemS << itemIS << itemRSI);
        }

        ui->tvJumpsDrop->setModel(model);
        model->setHorizontalHeaderLabels(QStringList() << tr("N")
                                         << tr("Контактная\nфаза, сек")
                                         << tr("Бесконтактная\nфаза, сек")
                                         << tr("Масса\nпациента, кг")
                                         << tr("Высота\nспрыгивания, см")
                                         << tr("Высота\nпрыжка, см")
                                         << tr("Мощность")
                                         << tr("Жесткость")
                                         << tr("Начальная\nскорость, см/сек")
                                         << tr("Индекс\nпрыгучести"));
        ui->tvJumpsDrop->header()->resizeSections(QHeaderView::ResizeToContents);
        ui->tvJumpsDrop->header()->resizeSection(0, 50);
    }
}

int DropTestVisualize::getPatientMassa(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        DataDefines::PatientKard pk;
        if (DataProvider::getPatient(ti.patientUid, pk))
            return pk.massa;
    }
    return 0;
}
