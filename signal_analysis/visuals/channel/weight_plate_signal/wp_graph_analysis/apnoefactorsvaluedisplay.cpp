#include "apnoefactorsvaluedisplay.h"
#include "ui_apnoefactorsvaluedisplay.h"

#include <QMenu>
#include <QFileDialog>
#include <QTextStream>

#include "datadefines.h"

ApnoeFactorsValueDisplay::ApnoeFactorsValueDisplay(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApnoeFactorsValueDisplay)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    ui->frDetail->setVisible(false);
    adjustSize();
    setFixedSize(size());

    auto mnWriteToFile = new QMenu(ui->btnWriteToFile);
    auto acWTFApnoe = new QAction(tr("Приступы апноэ..."), ui->btnWriteToFile);
    auto acWTFLog = new QAction(tr("Лог..."), ui->btnWriteToFile);
    connect(acWTFApnoe, &QAction::triggered, this, &ApnoeFactorsValueDisplay::on_wtfApnoe);
    connect(acWTFLog, &QAction::triggered, this, &ApnoeFactorsValueDisplay::on_wtfLog);
    mnWriteToFile->addAction(acWTFApnoe);
    mnWriteToFile->addAction(acWTFLog);
    ui->btnWriteToFile->setMenu(mnWriteToFile);
}

ApnoeFactorsValueDisplay::~ApnoeFactorsValueDisplay()
{
    delete ui;
}

void ApnoeFactorsValueDisplay::assignValues(const int count, const double timeAverage, const double timeMax)
{
    ui->lblApnoeFactsCount->setText(tr("Количество приступов") + " - " + QString::number(count));
    ui->lblApnoeFactTimeAverage->setText(tr("Средняя длительность приступа") + " - " + QString::number(timeAverage) + " " + tr("сек"));
    ui->lblApnoeFactTimeMax->setText(tr("Максимальная длительность приступа") + " - " + QString::number(timeMax) + " " + tr("сек"));
}

void ApnoeFactorsValueDisplay::setApnoeList(const QStringList &list)
{
    foreach (auto line, list)
        ui->teApnoeList->append(line);
}

void ApnoeFactorsValueDisplay::setLog(const QStringList &log)
{
    foreach (auto line, log)
        ui->teLog->append(line);
}

void ApnoeFactorsValueDisplay::on_showDetail()
{
    ui->frDetail->setVisible(!ui->frDetail->isVisible());
    adjustSize();
    if (!ui->frDetail->isVisible())
        setFixedSize(QSize(800, 120)); //size().height());
    else
        setFixedSize(QSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX));
}

void ApnoeFactorsValueDisplay::on_wtfApnoe()
{
    writeToFile(ui->teApnoeList, tr("списка приступов апноэ"));
}

void ApnoeFactorsValueDisplay::on_wtfLog()
{
    writeToFile(ui->teLog, tr("лога"));

}

void ApnoeFactorsValueDisplay::writeToFile(QTextEdit* text, const QString &subjName)
{
    auto fn = QFileDialog::getSaveFileName(this,
                                           tr("Выберите файл для сохранения") + " " + subjName,
                                           DataDefines::aanalyserDocumentsPath(),
                                           tr("Текстовые файлы (*.txt)"));
    if (fn != "")
    {
        QFile file(fn);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
                 return;
        QTextStream out(&file);
        out << text->toPlainText();
        file.close();
    }
}

