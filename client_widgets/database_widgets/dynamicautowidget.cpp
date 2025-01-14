#include "dynamicautowidget.h"
#include "ui_dynamicautowidget.h"

#include "baseutils.h"
#include "aanalyserapplication.h"
#include "dataprovider.h"
#include "datadefines.h"
#include "dynamicdatamodel.h"
#include "settingsprovider.h"
#include "msexcelexporter.h"
#include "amessagebox.h"

#include <QMenu>
#include <QFileDialog>
#include <QDebug>

DynamicAutoWidget::DynamicAutoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DynamicAutoWidget)
  , m_mdlDynamic(new DynamicDataModel(this))
{
    ui->setupUi(this);

    connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
            &AAnalyserApplication::selectPatient,
            this, &DynamicAutoWidget::on_selectPatient);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
            &AAnalyserApplication::selectMetodic,
            this, &DynamicAutoWidget::on_selectMetodic);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
            &AAnalyserApplication::selectTest,
            this, &DynamicAutoWidget::on_selectTest);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()),
            &AAnalyserApplication::removeTest,
            this, &DynamicAutoWidget::on_removeTest);

    connect(ui->wgtDynamic, &DynamicDiagram::selectItem, this, &DynamicAutoWidget::on_selectItem);
    setMenuExport();
}

DynamicAutoWidget::~DynamicAutoWidget()
{
    delete ui;
}

void DynamicAutoWidget::onDbConnect()
{
    restoreDynamicKind();
    restoreDynamicVolume();
}

void DynamicAutoWidget::onDbDisconnect()
{

}

void DynamicAutoWidget::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    if (ui->tvFactors->selectionModel()
            && ui->tvFactors->selectionModel()->currentIndex().isValid())
        showGraph(ui->tvFactors->selectionModel()->currentIndex().row());
}

void DynamicAutoWidget::on_selectPatient(const QString &patientUid)
{
    if (m_selectedPatientUid != patientUid)
    {
        m_selectedPatientUid = patientUid;
        buildDynamic();
    }
}

void DynamicAutoWidget::on_selectMetodic(const QString &metodicUid)
{
    if (m_selectedMetodicUid != metodicUid)
    {
        m_selectedMetodicUid = metodicUid;
        buildDynamic();
    }
}

void DynamicAutoWidget::on_selectTest(const QString &testUid)
{
    if (!m_tests.contains(testUid) &&        ///< Тест не должен участвовать в построении текущей динамики
            m_selectedPatientUid != "" && m_selectedMetodicUid != "")
        buildDynamic();
}

void DynamicAutoWidget::on_removeTest(const QString &testUid)
{
    if (m_tests.contains(testUid) &&        ///< Тест должен участвовать в построении текущей динамики
            m_selectedPatientUid != "" && m_selectedMetodicUid != "")
        buildDynamic();
}

void DynamicAutoWidget::selectFactor(const QModelIndex index)
{
    if (index.isValid())
        showGraph(index.row());
}

void DynamicAutoWidget::dynamicAsGraph()
{
    ui->wgtDynamic->setKind(DynamicDiagramDefines::KindGraph);
    ui->btnBar->setChecked(false);
    saveDynamicKind(static_cast<int>(DynamicDiagramDefines::KindGraph));
}

void DynamicAutoWidget::dynamicAsBar()
{
    ui->wgtDynamic->setKind(DynamicDiagramDefines::KindBar);
    ui->btnGraph->setChecked(false);
    saveDynamicKind(static_cast<int>(DynamicDiagramDefines::KindBar));
}

void DynamicAutoWidget::dynamic3D(bool pressed)
{
    if (pressed)
    {
        ui->wgtDynamic->setVolume(DynamicDiagramDefines::Volume3D);
        saveDynamicVolume(DynamicDiagramDefines::Volume3D);
    }
    else
    {
        ui->wgtDynamic->setVolume(DynamicDiagramDefines::Volume2D);
        saveDynamicVolume(DynamicDiagramDefines::Volume2D);
    }
}

void DynamicAutoWidget::on_selectItem(const int idx)
{
    if (idx >= 0 && idx < m_mdlDynamic->columnCount() - 1)
    {
        auto testUid = m_mdlDynamic->headerData(idx + 1, Qt::Horizontal, DynamicDataModel::TestUidRole).toString();
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectTest(testUid);
        auto dt = m_mdlDynamic->horizontalHeaderItem(idx + 1)->data(DynamicDataModel::DateTimeRole).toDateTime();
        auto value = m_mdlDynamic->item(m_selectRow, idx + 1)->data(DynamicDataModel::ValueRole).toDouble();
        QString sdt = dt.toString("dd.MM.yyyy hh:mm");
        if (sdt != "")
            sdt = QString("(%1)").arg(sdt);
        else
            sdt = "";
        ui->wgtDynamic->setBottomText(QString(tr("Тест") + " " + sdt + " - " + QString::number(value)));
    }
}

void DynamicAutoWidget::on_exportAsText()
{
    QString path = DataDefines::aanalyserDocumentsPath();
    auto fileName = QFileDialog::getSaveFileName(this, tr("Файл экспорта динамики показателей"), path, tr("Текстовые файлы (*.txt)"));
    if (fileName != "")
        BaseUtils::modelToText(m_mdlDynamic, fileName, "\t");
}

#ifdef Q_OS_WIN32
void DynamicAutoWidget::on_openExcel()
{
    try
    {
        QString fileName = DataDefines::aanalyserTemporaryPath() + "summary.xlsx";
        fileName = fileName.replace(QString("/"), QString("\\"));

        static_cast<AAnalyserApplication*>(QApplication::instance())->initProgress(tr("Открытие динамики показателей в MS Excel"),
                                                                                   0, m_mdlDynamic->rowCount() + 2,
                                                                                   tr("Инициализация"));
        MSExcelExporter exporter;

        exporter.setCellValue(1, 1, "");
        for (int i = 0; i < m_mdlDynamic->columnCount() - 1; ++i)
        {
            auto dt = m_mdlDynamic->horizontalHeaderItem(i + 1)->data(DynamicDataModel::DateTimeRole).toDateTime();
            QString s = tr("Тест") + " " + QString::number(i + 1) + ": " + dt.toString("dd.MM.yyyy hh:mm");
            exporter.setCellValue(1, i + 2, s);
        }

        //! По строкам
        for (int i = 0; i < m_mdlDynamic->rowCount(); ++i)
        {
            static_cast<AAnalyserApplication*>(QApplication::instance())->setProgressPosition(i+1, tr("Строка") + " " + QString::number(i + 1));
            //! По столбцам для строки
            for (int j = 0; j < m_mdlDynamic->columnCount(); ++j)
            {
                //! Всегда выводим текст итема
                auto text = m_mdlDynamic->index(i, j).data().toString();
                exporter.setCellValue(i + 2, j + 1, text);
            }
        }

        static_cast<AAnalyserApplication*>(QApplication::instance())->setProgressPosition(m_mdlDynamic->rowCount() + 2, tr("Завершение"));
        exporter.saveAs(fileName);
        static_cast<AAnalyserApplication*>(QApplication::instance())->doneProgress();
    }
    catch (const std::exception& e)
    {
        AMessageBox::warning(nullptr, tr("Предупреждение"), tr("Произошла ошибка открытия сводки в MS Excel") + e.what());
    }
}
#endif

void DynamicAutoWidget::buildDynamic()
{
    m_mdlDynamic->clear();
    ui->wgtDynamic->clear();
    m_tests.clear();

    if (m_selectedPatientUid != "" && m_selectedMetodicUid != "")
    {
        //! Построение списка
        fillTable();

        //! Прорисовка
        showDynamic();
    }
}

void DynamicAutoWidget::fillTable()
{
    m_selectRow = -1;
    auto tests = DataProvider::getTests();
    foreach (auto testUid, tests)
    {
        DataDefines::TestInfo ti;
        if (DataProvider::getTestInfo(testUid, ti))
        {
            if (ti.patientUid == m_selectedPatientUid &&
                ti.metodUid == m_selectedMetodicUid)
            {
                auto factors = DataProvider::getPrimaryFactors(testUid);
                if (factors.size() > 0)
                {
                    m_mdlDynamic->addTestData(factors, ti.uid, ti.dateTime);
                    m_tests.append(ti.uid);
                }
            }
        }
    }
}

void DynamicAutoWidget::showDynamic()
{
    ui->tvFactors->setModel(m_mdlDynamic);
    for (int i = 0; i < m_mdlDynamic->columnCount(); ++i)
        ui->tvFactors->resizeColumnToContents(i);
    if (m_mdlDynamic->rowCount() > 0 && m_mdlDynamic->columnCount() > 0)
    {
        ui->tvFactors->setCurrentIndex(m_mdlDynamic->index(0, 0));
        showGraph(0);
        connect(ui->tvFactors->selectionModel(), &QItemSelectionModel::selectionChanged,
                this, &DynamicAutoWidget::on_selectionChanged);
    }
    else
        ui->wgtDynamic->clear();
}

void DynamicAutoWidget::showGraph(const int row)
{
    if (row == m_selectRow)
        return;

    ui->wgtDynamic->clear();
    auto factorUid = m_mdlDynamic->item(row, 0)->data(DynamicDataModel::FactorUidRole).toString();
    auto fi = static_cast<AAnalyserApplication*>(QApplication::instance())->getFactorInfo(factorUid);
    QString title = fi.name();
    if (fi.measure() != "")
        title = title + ", " + fi.measure();
    ui->wgtDynamic->setTitle(title);

    m_selectRow = row;
    for (int i = 1; i < m_mdlDynamic->columnCount(); ++i)
    {
        auto value = m_mdlDynamic->item(row, i)->data(DynamicDataModel::ValueRole).toDouble();
        if (m_mdlDynamic->columnCount() <= 10)
        {
            auto dt = m_mdlDynamic->horizontalHeaderItem(i)->data(DynamicDataModel::DateTimeRole).toDateTime();
            auto di = new DiagItem(value, dt.toString("dd.MM.yyyy hh:mm"));
            ui->wgtDynamic->appendItem(di);
        }
        else
        {
            auto di = new DiagItem(value, QString::number(i));
            ui->wgtDynamic->appendItem(di);
        }
    }

    ui->wgtDynamic->setBottomText(QString(tr("Тесты")));
}

void DynamicAutoWidget::saveDynamicKind(const int kindCode) const
{
    SettingsProvider::setValueToRegAppCopy("DynamicWidget", "DynamicKind", kindCode);
}

void DynamicAutoWidget::restoreDynamicKind()
{
    auto kindCode = SettingsProvider::valueFromRegAppCopy("DynamicWidget", "DynamicKind", 0).toInt();

    DynamicDiagramDefines::Kind kind = static_cast<DynamicDiagramDefines::Kind>(kindCode);
    ui->wgtDynamic->setKind(kind);
    ui->btnGraph->setChecked(kind == DynamicDiagramDefines::KindGraph);
    ui->btnBar->setChecked(kind == DynamicDiagramDefines::KindBar);
}

void DynamicAutoWidget::saveDynamicVolume(const int volumeCode) const
{
    SettingsProvider::setValueToRegAppCopy("DynamicWidget", "DynamicVolume", volumeCode);
}

void DynamicAutoWidget::restoreDynamicVolume()
{
    auto volumeCode = SettingsProvider::valueFromRegAppCopy("DynamicWidget", "DynamicVolume", 0).toInt();

    DynamicDiagramDefines::Volume volume = static_cast<DynamicDiagramDefines::Volume>(volumeCode);
    ui->wgtDynamic->setVolume(volume);
    ui->btn3D->setChecked(volume == DynamicDiagramDefines::Volume3D);
}

void DynamicAutoWidget::setMenuExport()
{
    auto menu = new QMenu(this);

    QAction *acExportAsText = new QAction(QIcon(":/images/SummaryExport.png"), tr("Сохранить в файл..."), this);
    connect(acExportAsText, &QAction::triggered, this, &DynamicAutoWidget::on_exportAsText);
    menu->addAction(acExportAsText);

#ifdef Q_OS_WIN32
    QAction *acOpenExcel = new QAction(QIcon(":/images/OpenMSExcel.png"), tr("Открыть в MS Excel"), this);
    connect(acOpenExcel, &QAction::triggered, this, &DynamicAutoWidget::on_openExcel);
    menu->addAction(acOpenExcel);
#endif

    ui->btnExport->setMenu(menu);
}
