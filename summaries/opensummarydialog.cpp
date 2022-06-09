#include "opensummarydialog.h"
#include "ui_opensummarydialog.h"

#include <QDir>
#include <QDebug>

#include "aanalyserapplication.h"
#include "datadefines.h"
#include "summary.h"
#include "metodicsfactory.h"

OpenSummaryDialog::OpenSummaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenSummaryDialog)
{
    ui->setupUi(this);
    load();
}

OpenSummaryDialog::~OpenSummaryDialog()
{
    delete ui;
}

void OpenSummaryDialog::load()
{
    QString path = DataDefines::aanalyserDocumentsPath() + "summaries/";
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(); //QDir::NoDotAndDotDot);

    auto model = new QStandardItemModel(ui->tvSummaries);

    foreach (auto fileInfo, list)
        if (fileInfo.fileName() != "." && fileInfo.fileName() != "..")
        {
            if (fileInfo.completeSuffix() == "json")
            {
                QString name;
                QString uidMethodic;
                QString nameMethodic;
                SummaryDefines::Kind kind;
                Summary::getHeader(path + fileInfo.fileName(), name, uidMethodic, nameMethodic, kind);
                auto met = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(uidMethodic);

                auto itemName = new QStandardItem(name);
                itemName->setEditable(false);

                QString mn = nameMethodic;
                if (met.name != "")
                    mn = met.name;
                auto itemMet = new QStandardItem(mn);
                itemMet->setEditable(false);
                itemMet->setData(uidMethodic, MethodicRole);
                itemMet->setIcon(QIcon(":/images/Methodics/" + met.imageName));

                QString sk = "";
                if (kind == SummaryDefines::skAll)
                    sk = tr("Все показатели");
                else
                if (kind == SummaryDefines::skPrimary)
                    sk = tr("Первичные показатели");
                auto itemKind = new QStandardItem(sk);
                itemKind->setEditable(false);
                itemKind->setData(kind, KindRole);

                model->appendRow(QList<QStandardItem*>() << itemName << itemMet << itemKind);
            }
        }

    ui->tvSummaries->setModel(model);
    model->setHorizontalHeaderLabels(QStringList() << tr("Сводка") << tr("Методика") << tr("Показатели"));
    ui->tvSummaries->header()->resizeSections(QHeaderView::ResizeToContents);
}
