#include "signalexportdialog.h"
#include "ui_signalexportdialog.h"

#include <QMessageBox>
#include <QDebug>

#include "aanalyserapplication.h"
#include "filenamewidget.h"

SignalExportDialog::SignalExportDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SignalExportDialog)
{
    ui->setupUi(this);

    setStyleSheet(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow()->styleSheet());

    auto model = new QStandardItemModel(ui->tvFilters);
    ui->tvFilters->setModel(model);
}

SignalExportDialog::~SignalExportDialog()
{
    delete ui;
}

void SignalExportDialog::setFileNameSelectors(QStringList &titles)
{
    clearFNSelectors();

    for (int i = 0; i < titles.size(); ++i)
    {
        auto wgt = new FileNameWidget(FileNameWidget::mdFile, ui->frFileName);
        if (titles.size() > 1 && titles.at(i) != "")
            wgt->setTitle(tr("Выбор файла для подканала") + " \"" + titles.at(i) + "\"");
        else
            wgt->setTitle(tr("Выбор файла для экспорта"));
        ui->frFileName->layout()->addWidget(wgt);

        if (ui->rbToDifferentFiles->isChecked() && i == 0)
            wgt->setActive(false);  //->setVisible(false);
        if (ui->rbToSingleFile->isChecked() && i != 0)
            wgt->setActive(false);  //->setVisible(false);
    }
}

void SignalExportDialog::setFolderSelector()
{
    clearFNSelectors();

    auto wgt = new FileNameWidget(FileNameWidget::mdFolder, ui->frFileName);
    wgt->setTitle(tr("Выбор папки"));
    ui->frFileName->layout()->addWidget(wgt);
}

int SignalExportDialog::fileNameSelectorCount() const
{
    int cnt = 0;
    QObjectList children = ui->frFileName->children();
    foreach(QObject * child, children)
    {
        if (child && child->isWidgetType() && static_cast<FileNameWidget*>(child)->active())
            ++cnt;
    }
    return cnt;
}

QString SignalExportDialog::fileName(const int idx) const
{
    int n = 0;
    QObjectList children = ui->frFileName->children();
    for (int i = 0; i < children.size(); ++i)
    {
        if (children.at(i)->isWidgetType() && static_cast<FileNameWidget*>(children.at(i))->active())
        {
            if (n == idx)
                return static_cast<FileNameWidget*>(children.at(i))->fileName();
            ++n;
        }
    }
    return "";
}

void SignalExportDialog::addFilter(const QString &uid, const QString &name)
{
    auto item = new QStandardItem(name);
    item->setData(uid, FilterUidRole);
    static_cast<QStandardItemModel*>(ui->tvFilters->model())->appendRow(item);
}

void SignalExportDialog::accept()
{
    if (m_filterUid != "")
        QDialog::accept();
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран фильтр экспорта"));
}

void SignalExportDialog::on_selectFilter(QModelIndex index)
{
    m_filterUid = index.data(FilterUidRole).toString();
}

void SignalExportDialog::on_isSeparateChanged(bool)
{
    QObjectList children = ui->frFileName->children();
    int n = 0;
    for (int i = 0; i < children.size(); ++i)
        if (children.at(i)->isWidgetType())
            ++n;

    if (n > 1)
    {
        int n = 0;
        for (int i = 0; i < children.size(); ++i)
        {
            if (children.at(i)->isWidgetType())
            {
                bool vis = (ui->rbToDifferentFiles->isChecked() && n != 0) ||
                           (ui->rbToSingleFile->isChecked() && n == 0);
                static_cast<FileNameWidget*>(children.at(i))->setActive(vis);
//                static_cast<FileNameWidget*>(children.at(i))->setVisible(vis);
                ++n;
            }
        }
    }
}

void SignalExportDialog::clearFNSelectors()
{
    QObjectList children = ui->frFileName->children();
    foreach(QObject * child, children)
        if (child->isWidgetType())
            delete child;
}
