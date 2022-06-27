#include "opensummarydialog.h"
#include "ui_opensummarydialog.h"

#include <QDir>
#include <QMessageBox>
#include <QDebug>

#include "aanalyserapplication.h"
#include "datadefines.h"
#include "summary.h"
#include "metodicsfactory.h"
#include "listsummariesmodel.h"
#include "listsummariesproxymodel.h"
#include "settingsprovider.h"

OpenSummaryDialog::OpenSummaryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OpenSummaryDialog)
{
    ui->setupUi(this);
    ui->buttonBox->setVisible(false);
    load();
    setStyleSheet(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow()->styleSheet());

    auto val = SettingsProvider::valueFromRegAppCopy("OpenSummaryDialog", "Geometry").toByteArray();
    restoreGeometry(val);
    restoreSplitterPosition();
}

OpenSummaryDialog::~OpenSummaryDialog()
{
    delete ui;
}

QString OpenSummaryDialog::summaryFileName() const
{
    auto index = ui->tvSummaries->selectionModel()->currentIndex();

    if (index.isValid())
    {
        auto idx = index.model()->index(index.row(), 0);
        return idx.data(ListSummariesModel::FileNameRole).toString();
    }
    return "";
}

void OpenSummaryDialog::resizeEvent(QResizeEvent *)
{
    SettingsProvider::setValueToRegAppCopy("OpenSummaryDialog", "Geometry", saveGeometry());
}

void OpenSummaryDialog::on_selectMethodic(QModelIndex index)
{
    m_pmdlSummaries->selectMetodic(index.data(ListSummariesModel::MethodicRole).toString());
}

void OpenSummaryDialog::on_kindChanged()
{
    if (ui->rbAllFactors->isChecked())
        m_pmdlSummaries->selectKind(SummaryDefines::skAll);
    else
    if (ui->rbPrimaryFactors->isChecked())
        m_pmdlSummaries->selectKind(SummaryDefines::skPrimary);
    else
    if (ui->rbAll->isChecked())
        m_pmdlSummaries->selectKind(SummaryDefines::skNone);
}

void OpenSummaryDialog::on_selectSummaryWithAccept(QModelIndex index)
{
    if (index.isValid())
        accept();
}

void OpenSummaryDialog::on_openSummary()
{
    accept();
}

void OpenSummaryDialog::on_deleteSummary()
{
    auto index = ui->tvSummaries->selectionModel()->currentIndex();

    if (index.isValid())
    {
        auto idx = index.model()->index(index.row(), 0);
        auto fn = idx.data(ListSummariesModel::FileNameRole).toString();

        if (QMessageBox::question(nullptr, tr("Запрос"), tr("Удалить сводку") + " \"" + idx.data().toString() + "\"?") == QMessageBox::Yes)
        {
            if (QFile::exists(fn))
                QFile::remove(fn);

            m_mdlSummaries->removeRow(index.row());
            emit deleteSummary(fn);
        }
    }
}

void OpenSummaryDialog::on_splitterMoved(int pos,int index)
{
    Q_UNUSED(pos);
    Q_UNUSED(index);
    saveSplitterPosition();
}

void OpenSummaryDialog::load()
{
    //! Получение списка файлов
    QString path = DataDefines::aanalyserSummariesPath();
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(); //QDir::NoDotAndDotDot);

    //! Модели
    m_mdlSummaries = new ListSummariesModel(ui->tvSummaries);
    m_pmdlSummaries = new ListSummariesProxyModel(ui->tvSummaries);
    auto mdlMethodics = new QStandardItemModel(ui->tvMethodics);

    //! Список методик для исключения повторов
    QSet<QString> metList;
    metList.clear();

    //! Итем для показа всех методик
    auto itemMet = new QStandardItem(tr("<Все>"));
    itemMet->setEditable(false);
    itemMet->setData(QUuid().toString(), ListSummariesModel::MethodicRole);
    mdlMethodics->appendRow(itemMet);

    //! Цикл по сводкам в папке
    foreach (auto fileInfo, list)
        if (fileInfo.fileName() != "." && fileInfo.fileName() != "..")
        {
            if (fileInfo.completeSuffix() == "asmry")
            {
                //! Чтение заголовка сводки и названия методики
                QString name;
                QString uidMethodic;
                QString nameMethodic;
                SummaryDefines::Kind kind;
                Summary::getHeader(path + fileInfo.fileName(), name, uidMethodic, nameMethodic, kind);
                auto met = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics()->metodic(uidMethodic);

                //! Название сводки
                auto itemName = new QStandardItem(name);
                itemName->setEditable(false);
                itemName->setData(path + fileInfo.fileName(), ListSummariesModel::FileNameRole);

                //! Методика
                QString mn = nameMethodic;
                if (met.name != "")
                    mn = met.name;
                auto itemMet = new QStandardItem(mn);
                itemMet->setEditable(false);
                itemMet->setData(uidMethodic, ListSummariesModel::MethodicRole);
                itemMet->setIcon(QIcon(":/images/Methodics/" + met.imageName));
                //! Методика в списке методик для фильтрации
                if (!metList.contains(uidMethodic))
                {
                    auto itemMet = new QStandardItem(mn);
                    itemMet->setEditable(false);
                    itemMet->setData(uidMethodic, ListSummariesModel::MethodicRole);
                    itemMet->setIcon(QIcon(":/images/Methodics/" + met.imageName));
                    mdlMethodics->appendRow(itemMet);
                    metList << uidMethodic;
                }

                //! Тип сводки
                QString sk = "";
                if (kind == SummaryDefines::skAll)
                    sk = tr("Все показатели");
                else
                if (kind == SummaryDefines::skPrimary)
                    sk = tr("Первичные показатели");
                auto itemKind = new QStandardItem(sk);
                itemKind->setEditable(false);
                itemKind->setData(kind, ListSummariesModel::KindRole);

                m_mdlSummaries->appendRow(QList<QStandardItem*>() << itemName << itemMet << itemKind);
            }
        }

    //! Модель списка сводок
    m_pmdlSummaries->setSourceModel(m_mdlSummaries);
    ui->tvSummaries->setModel(m_pmdlSummaries);
    m_mdlSummaries->setHorizontalHeaderLabels(QStringList() << ListSummariesModel::columnTitle(ListSummariesModel::colName)
                                                            << ListSummariesModel::columnTitle(ListSummariesModel::colMethodic)
                                                            << ListSummariesModel::columnTitle(ListSummariesModel::colKind));
    ui->tvSummaries->header()->resizeSections(QHeaderView::ResizeToContents);
    //! Выделяем в нем запись
//    for (int i = 0; i < 3; ++i)
//        ui->tvSummaries->selectionModel()->setCurrentIndex(m_mdlSummaries->index(0, i), QItemSelectionModel::Select);
////    ui->tvSummaries->selectionModel()->setCurrentIndex(m_mdlSummaries->index(0, 0), QItemSelectionModel::Select);
//    ui->tvSummaries->scrollTo(m_mdlSummaries->index(0, 0));

    //! Модель списка методик для фильтрации
    ui->tvMethodics->setModel(mdlMethodics);
    mdlMethodics->setHorizontalHeaderLabels(QStringList() << tr("Методики"));
    ui->tvMethodics->header()->resizeSections(QHeaderView::ResizeToContents);
    ui->tvMethodics->selectionModel()->setCurrentIndex(mdlMethodics->index(0, 0), QItemSelectionModel::Select);
    ui->tvMethodics->scrollTo(mdlMethodics->index(0, 0));
}

void OpenSummaryDialog::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("OpenSummaryDialog", "SplitterPosition", ui->splitter->saveState());
}

void OpenSummaryDialog::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("OpenSummaryDialog", "SplitterPosition").toByteArray();
    if (val == "")
        val = QByteArray::fromRawData("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x01\x1B\x00\x00\x03""b\x01\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x01\x00", 31);
    ui->splitter->restoreState(val);
}
