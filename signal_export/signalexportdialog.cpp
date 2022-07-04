#include "signalexportdialog.h"
#include "ui_signalexportdialog.h"

#include <QStandardItemModel>
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

void SignalExportDialog::setFileNameCount(const int count)
{
    QObjectList children = ui->frFileName->children();
    foreach(QObject * child, children)
        if (child->isWidgetType())
            delete child;

    for (int i = 0; i < count; ++i)
    {
        auto wgt = new FileNameWidget(ui->frFileName);
        ui->frFileName->layout()->addWidget(wgt);
    }
}

int SignalExportDialog::fileNameCount() const
{
    int cnt = 0;
    QObjectList children = ui->frFileName->children();
    foreach(QObject * child, children)
        if (child->isWidgetType())
            ++cnt;
    return cnt;
}

QString SignalExportDialog::fileName(const int idx) const
{
    int n = 0;
    QObjectList children = ui->frFileName->children();
    for (int i = 0; i < children.size(); ++i)
    {
        if (children.at(i)->isWidgetType())
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
