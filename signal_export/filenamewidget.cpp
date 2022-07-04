#include "filenamewidget.h"
#include "ui_filenamewidget.h"

#include <QFileDialog>

#include "datadefines.h"

FileNameWidget::FileNameWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileNameWidget)
{
    ui->setupUi(this);
}

FileNameWidget::~FileNameWidget()
{
    delete ui;
}

void FileNameWidget::setTitle(const QString &title)
{
    ui->lblFileName->setText(title);
}

QString FileNameWidget::fileName() const
{
    return ui->edFileName->text();
}

void FileNameWidget::setFileName(const QString &fn)
{
    ui->edFileName->setText(fn);
}

void FileNameWidget::browseFile()
{
    QString path = DataDefines::aanalyserDocumentsPath();
    ui->edFileName->setText(QFileDialog::getSaveFileName(nullptr,
                                                         tr("Файл для экспорта сигнала"),
                                                         path,
                                                         tr("Текстовые файлы") + " *.txt (*.txt)"));
}
