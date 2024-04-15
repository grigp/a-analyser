#include "filenamewidget.h"
#include "ui_filenamewidget.h"

#include <QFileDialog>

#include "datadefines.h"
#include "amessagebox.h"

FileNameWidget::FileNameWidget(const Mode mode, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FileNameWidget)
{
    m_mode = mode;
    ui->setupUi(this);
    ui->edFileName->setEnabled(false);
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
    {
        if (m_filter != "")
        {
            auto fn = QFileDialog::getSaveFileName(nullptr,
                                                   tr("Файл для экспорта сигнала"),
                                                   path,
                                                   m_filter);  // tr("Текстовые файлы") + " *.txt (*.txt)"));
            if (fn != "")
            {
                ui->edFileName->setText(fn);
//                ui->edFileName->setEnabled(true);
            }
        }
        else
            AMessageBox::information(nullptr, tr("Информация"), tr("Не выбран фильтр экспорта"));
    }
    else
    {
        auto fn = QFileDialog::getExistingDirectory(nullptr,
                                                    tr("Выбор пустой папки для экспорта"),
                                                    path);
        if (fn != "")
            ui->edFileName->setText(fn);
    }
}
