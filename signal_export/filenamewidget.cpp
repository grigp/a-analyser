#include "filenamewidget.h"
#include "ui_filenamewidget.h"

#include <QFileDialog>

#include "datadefines.h"

FileNameWidget::FileNameWidget(const Mode mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileNameWidget)
{
    m_mode = mode;
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

void FileNameWidget::setActive(const bool active)
{
    m_active = active;
    setVisible(active);
}

void FileNameWidget::browseFile()
{
    QString path = DataDefines::aanalyserDocumentsPath();
    if (m_mode == mdFile)
        ui->edFileName->setText(QFileDialog::getSaveFileName(nullptr,
                                                             tr("Файл для экспорта сигнала"),
                                                             path,
                                                             tr("Текстовые файлы") + " *.txt (*.txt)"));
    else
        ui->edFileName->setText(QFileDialog::getExistingDirectory(nullptr,
                                                                  tr("Выбор пустой папки для экспорта"),
                                                                  path));
}
