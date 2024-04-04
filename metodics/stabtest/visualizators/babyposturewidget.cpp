#include "babyposturewidget.h"
#include "ui_babyposturewidget.h"

#include "babyposturecalculator.h"

BabyPostureWidget::BabyPostureWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BabyPostureWidget)
{
    ui->setupUi(this);
}

BabyPostureWidget::~BabyPostureWidget()
{
    delete ui;
}

void BabyPostureWidget::calculate(BabyPostureCalculator *calculator, const QString &testUid)
{
    Q_UNUSED(testUid);
    ui->teResume->setStyleSheet("font-size: 14pt; color: rgb(32,88,103);");
    ui->teResume->append(tr("Площадь статокинезиграммы (s95, кв.мм)") + "  " + calculator->square());
    ui->teResume->append(tr("Длина эллипса статокинезиграммы (Le95, мм)") + "  " + calculator->ellLen());
    ui->teResume->append(tr("Отношение длины статокинезиграммы к ее площади (LFS95, 1/мм)") + "  " + calculator->lfs());
    ui->teResume->append(tr("Отношение длины эллипса статокинезиграммы к его ширине (Le-We, ед)") + "  " + calculator->ellE());
 //   ui->teResume->append(tr("Показатель стабильности") + "  " + calculator->stab());
}

void BabyPostureWidget::print(QPrinter *printer, const QString &testUid)
{

}

void BabyPostureWidget::paintPreview(QPainter *painter, QRect &rect, const QString &testUid, TestCalculator *calculator)
{

}
