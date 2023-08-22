#include "personalprogramwidget.h"
#include "ui_personalprogramwidget.h"

#include <QUuid>
#include <QMessageBox>
#include <QDebug>

#include "aanalyserapplication.h"
#include "patientsmodel.h"
#include "patientsproxymodel.h"
#include "settingsprovider.h"
#include "dataprovider.h"
#include "personalprogram.h"
#include "databasewigetdefines.h"
#include "patientprogramwidget.h"
#include "activepersonalprogrameditor.h"
#include "metodicsfactory.h"
#include "mainwindow.h"
#include "executewidget.h"

PersonalProgramWidget::PersonalProgramWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::PersonalProgramWidget)
{
    ui->setupUi(this);

    restoreSplitterPosition();
    load();
    ui->tvPatients->setModel(m_model);
    m_wgts.clear();
}

PersonalProgramWidget::~PersonalProgramWidget()
{
    delete ui;
}

QString PersonalProgramWidget::uid()
{
    return ClientWidgets::uidPersonalProgramWidgetUid;
}

QString PersonalProgramWidget::name()
{
    return tr("Индивидуальные программы");
}

void PersonalProgramWidget::onDBConnect()
{
    ui->tvPatients->header()->resizeSections(QHeaderView::ResizeToContents);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectPatient,
            this, &PersonalProgramWidget::on_selectPatient);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::newTest,
            this, &PersonalProgramWidget::on_newTest);
}

void PersonalProgramWidget::onShow()
{
    ui->tvPatients->selectionModel()->clearSelection();
    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient("");

    if (m_finishedTestUid != "")
    {
        //! Записать в ИП uid теста и времени начала ДП

        //! Запустить следующий тест
        doRunTest();
    }
}

void PersonalProgramWidget::onHide()
{

}

void PersonalProgramWidget::on_splitterMoved(int, int)
{
    saveSplitterPosition();
}

void PersonalProgramWidget::on_run()
{
    auto index = selectedIndex();
    if (index != QModelIndex())
    {
        if (index.parent() == QModelIndex())
        {
            //! Проверка ИП на завершенность.
            //! Завершенные всегда находятся в выпадающем списке у пациента, а активные в корне
            //! То есть, просто проверяем, имеется ли корневой узел
            if (index != QModelIndex() && index.isValid())
            {
                auto uidPP = index.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();
                m_activePatientUid = index.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
                m_objPPExecuted = DataProvider::getPersonalProgramByUid(uidPP);
                doRunTest();
            }
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Выбранная индивидуальная программа завершена"));
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Индивидуальная программа не выбрана"));
}

void PersonalProgramWidget::on_delete()
{
    auto index = selectedIndex();
    if (index != QModelIndex())
    {
        if (index.parent() == QModelIndex())
        {
            //! Проверка ИП на завершенность.
            //! Завершенные всегда находятся в выпадающем списке у пациента, а активные в корне
            //! То есть, просто проверяем, имеется ли корневой узел
            if (index != QModelIndex() && index.isValid())
            {
                auto uidPP = index.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();
                auto pp = DataProvider::getPersonalProgramByUid(uidPP);
                //TODO: прерывание ИП
            }
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Выбранная индивидуальная программа уже завершена"));
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Индивидуальная программа не выбрана"));
}

void PersonalProgramWidget::on_params()
{
    auto index = selectedIndex();
    if (index != QModelIndex())
    {
        if (index.parent() == QModelIndex())
        {
            //! Проверка ИП на завершенность.
            //! Завершенные всегда находятся в выпадающем списке у пациента, а активные в корне
            //! То есть, просто проверяем, имеется ли корневой узел
            if (index != QModelIndex() && index.isValid())
            {
                auto uidPP = index.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();
                auto dialog = new ActivePersonalProgramEditor(this);
                auto pp = DataProvider::getPersonalProgramByUid(uidPP);
                dialog->setPersonalProgram(pp);
                if (dialog->exec() == QDialog::Accepted)
                {
                    //! Получить от диалога новую индивидуальную программу
                    auto pp = dialog->personalProgram();
                    //!Записать измененную индивидуальную программу в БД
                    DataProvider::savePersonalProgramByUid(uidPP, pp);
                    //! Обновить ее на странице
                    if (m_wgts.contains(uidPP))
                        m_wgts.value(uidPP)->assignPersonalProgram(uidPP);
                }
            }
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя редактировать завершенные индивидуальные программы"));
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Индивидуальная программа не выбрана"));
}

void PersonalProgramWidget::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    selectPatient(ui->tvPatients->selectionModel()->currentIndex());
}

void PersonalProgramWidget::selectPatient(const QModelIndex index)
{
    if (index.isValid())
    {
         auto idx = index.model()->index(index.row(), 0, index.parent());
         auto uid = idx.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
         static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient(uid);

         auto uidPP = idx.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();

         //! Показать индивидуальную программу
         showPersonalProgram(uidPP);
    }
}

void PersonalProgramWidget::on_selectPatient(const QString &patientUid)
{
    //! uid выбранного в настоящий момент пациента
    QString curSelPatientUid = "";
    auto selIdxs = ui->tvPatients->selectionModel()->selectedIndexes();
    if (selIdxs.size() > 0)
        foreach (auto idx, selIdxs)
            if (idx.column() == 0)
                curSelPatientUid = idx.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();

    //! Поиск выделенного пациента patientUid
    if (patientUid != "")
    {
        auto pmdl = m_model;
        for (int i = 0; i < pmdl->rowCount(); ++i)
        {
            auto pindex = pmdl->index(i, 0);
            auto uid = pindex.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
            auto uidPP = pindex.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();

            //! Нашли и он не выбран
            if (uid == patientUid && uid != curSelPatientUid)
            {
                //! Выделить
                for (int i = 0; i < pmdl->columnCount(); ++i)
                {
                    auto idx = pmdl->index(pindex.row(), i);
                    ui->tvPatients->selectionModel()->select(idx, QItemSelectionModel::Select);

                }
                //! Показать индивидуальную программу
                showPersonalProgram(uidPP);
            }
        }

    }
}

void PersonalProgramWidget::on_newTest(const QString &testUid)
{
    m_finishedTestUid = "";
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        if (ti.patientUid == m_activePatientUid && ti.metodUid == m_activeMethodicUid)
        {
            m_finishedTestUid = testUid;
        }
    }
}

QStandardItem* PersonalProgramWidget::appendLine(const QString uidPat,
                                                 const QString uidPPAssigned,
                                                 const QJsonObject& objPP,
                                                 QStandardItem* root)
{
    DataDefines::PatientKard pi;
    if (DataProvider::getPatient(uidPat, pi))
    {
        QString title = pi.fio;
        if (root)
        {
            auto db = objPP["date_begin"].toString();
            auto de = objPP["date_end"].toString();
            title = db + " - " + de;
        }
        QStandardItem *itemFIO = new QStandardItem(title);
        itemFIO->setData(pi.uid, DatabaseWidgetDefines::PatientsModel::PatientUidRole);
        itemFIO->setData(uidPPAssigned, DatabaseWidgetDefines::PatientsModel::PatientPPUidRole);
        itemFIO->setEditable(false);
        QStandardItem *itemBorn = new QStandardItem(pi.born.toString("dd.MM.yyyy"));
        itemBorn->setEditable(false);
        QStandardItem *itemSex = new QStandardItem(DataDefines::SexToText.value(
                                                       static_cast<DataDefines::Sex>(pi.sex)));
        itemSex->setData(pi.sex, DatabaseWidgetDefines::PatientsModel::PatientSexRole);
        itemSex->setEditable(false);

        QList<QStandardItem*> line;
        line << itemFIO << itemBorn << itemSex;
        if (!root)
            m_model->appendRow(line);
        else
            root->appendRow(line);
        return itemFIO;
    }
    return nullptr;
}

void PersonalProgramWidget::load()
{
    auto arrPP = DataProvider::getPersonalProgramList();
    m_model = new QStandardItemModel(ui->tvPatients);

    QMap<QString, QStandardItem*> patients;  ///< Запоминаем пациентов, уже добавленных в модель
    patients.clear();

    //! Сначала активные - они всегда в руте
    for (int i = 0; i < arrPP.size(); ++i)
    {
        auto objPP = arrPP.at(i).toObject();
        auto uidPat = objPP["patient_uid"].toString();
        auto uidPPAssigned = objPP["assigned_uid"].toString();
        auto active = objPP["active"].toBool();
        if (active && !patients.contains(uidPat))
        {
            auto item = appendLine(uidPat, uidPPAssigned);
            patients.insert(uidPat, item);
        }
    }

    for (int i = 0; i < arrPP.size(); ++i)
    {
        auto objPP = arrPP.at(i).toObject();
        auto uidPat = objPP["patient_uid"].toString();
        auto uidPPAssigned = objPP["assigned_uid"].toString();
        auto active = objPP["active"].toBool();
        if (!active)
        {
            if (patients.contains(uidPat))
            {
                appendLine(uidPat, uidPPAssigned, objPP, patients.value(uidPat));
            }
            else
            {
                auto item = appendLine(uidPat, uidPPAssigned);
                patients.insert(uidPat, item);
            }
        }
    }
    m_model->setHorizontalHeaderLabels(QStringList() << tr("ФИО") << tr("Дата рождения") << tr("Пол"));

}

void PersonalProgramWidget::showPersonalProgram(const QString& uidPPAssigned)
{
    hideAllWidgets();
    if (m_wgts.contains(uidPPAssigned))
        m_wgts.value(uidPPAssigned)->setVisible(true);
    else
    {
        auto ppw = new PatientProgramWidget(ui->frPrograms);
        ppw->assignPersonalProgram(uidPPAssigned);
        ui->frPrograms->layout()->addWidget(ppw);
        m_wgts.insert(uidPPAssigned, ppw);
    }
}


void PersonalProgramWidget::hideAllWidgets()
{
    QObjectList children = ui->frPrograms->children();
    foreach(QObject * child, children)
        if (child->isWidgetType())
            static_cast<QWidget*>(child)->setVisible(false);
}


void PersonalProgramWidget::saveSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("PersonalProgramWidget", "SplitterPosition", ui->splitter->saveState());
}

void PersonalProgramWidget::restoreSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("PersonalProgramWidget", "SplitterPosition").toByteArray();
//    if (val == "")
//        val = QByteArray::fromRawData("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x01\x1B\x00\x00\x03""b\x01\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x01\x00", 31);
    ui->splitter->restoreState(val);
}

QModelIndex PersonalProgramWidget::selectedIndex() const
{
    auto selIdxs = ui->tvPatients->selectionModel()->selectedIndexes();
    for (int i = 0; i < selIdxs.size(); ++i)
    {
        if (selIdxs.at(i).column() == 0)
            return selIdxs.at(i);
    }
    return QModelIndex();
}

void PersonalProgramWidget::doRunTest()
{
    QJsonObject objTest = QJsonObject();
    if (getNextTestInfo(m_objPPExecuted, objTest))
    {
        if (objTest != QJsonObject())
            runTest(objTest);
    }
    else
    {
        // TODO: Завершить ИП
    }
}

QDateTime PersonalProgramWidget::getDateTimeByString(const QString &s) const
{
    if (s != "")
        return QDateTime::fromString(s, "dd.MM.yyyy hh:mm");
    return QDateTime();
}

QJsonObject PersonalProgramWidget::findEmptyTestInfo(const QJsonArray &arr) const
{
    for (int i = 0; i < arr.size(); ++i)
    {
        auto obj = arr.at(i).toObject();
        auto uidT = obj["test_uid"].toString("");
        if (uidT == "")
            return obj;
    }
    return QJsonObject();
}

bool PersonalProgramWidget::getNextTestInfo(const QJsonObject &objPPAll, QJsonObject& objTest) const
{
    auto objPP = objPPAll["pp"].toObject();

    auto minTimeDP = objPP["min_time_between_dp"].toInt();                   ///< В индексах 0, 1, 2, ...
    minTimeDP = PersonalProgramDefines::MinTimeBetweenDPList.at(minTimeDP);  ///< В часах
    auto minTimeDPSec = minTimeDP * 3600;                                    ///< В секундах
    auto maxTimeDP = objPP["max_time_between_dp"].toInt();                   ///< В индексах 0, 1, 2, ...
    maxTimeDP = PersonalProgramDefines::MaxTimeBetweenDPList.at(maxTimeDP);  ///< В часах
    auto maxTimeDPSec = maxTimeDP * 3600;                                    ///< В секундах

    QDateTime dtLast = QDateTime();
    auto arrDP = objPP["dp_list"].toArray();
    for (int i = 0; i < arrDP.size(); ++i)
    {
        auto objDP = arrDP.at(i).toObject();
        auto dtDP = getDateTimeByString(objDP["date_time"].toString(""));
        if (dtDP != QDateTime())
            dtLast = dtDP;

        auto arrTests = objDP["test_list"].toArray();
        auto objT = findEmptyTestInfo(arrTests);

        //! Варианты:
        //! 1. dtLast != QDateTime() && QDateTime::currentDateTime() - dtLast > maxTimeDPSec Превысили максимальное время - завершаем ИП
        //! 2. objTest != QJsonObject() В ДП есть непроведенный тест
        //!    2.1. dtDP == QDateTime() ДП не проводилась вообще. тест должен быть первый. Вернуть objTest
        //!    2.2. dtDP != QDateTime() ДП проводилась, но была прервана
        //!       2.2.1. QDateTime::currentDateTime() - dtDP < minTimeDPSec Прошло меньше минимума. Вернуть objTest
        //!       2.2.2. QDateTime::currentDateTime() - dtDP > minTimeDPSec Прошло больше минимума. Следующая ДП
        //! 3. objTest == QJsonObject() В ДП нет непроведенных тестов. Следующая ДП

        //! Превысили максимальное время - завершаем ИП
        if (maxTimeDPSec > 0 &&
            dtLast != QDateTime() &&
            QDateTime::currentDateTime().secsTo(dtLast) > maxTimeDPSec)
        {
            objTest = QJsonObject();
            return false;
        }

        //! В ДП есть непроведенный тест
        if (objT != QJsonObject())
        {
            //! ДП не проводилась вообще
            if (dtDP == QDateTime())
            {
                objTest = objT;
                return true;
            }
            else
            //! ДП проводилась, но была прервана
            {
                //! Прошло меньше минимума
                if (QDateTime::currentDateTime().secsTo(dtDP) > minTimeDPSec)
                {
                    objTest = objT;
                    return true;
                }
            }
        }
    }

    objTest = QJsonObject();
    return false;
}

void PersonalProgramWidget::runTest(const QJsonObject& objTest)
{
    //! uid методики и параметры проведения
    auto uidM = objTest["uid"].toString();
    auto params = objTest["params"].toObject();

    //! Виджет выполнения
    auto widget = static_cast<MainWindow*>(static_cast<AAnalyserApplication*>(QApplication::instance())->mainWindow())->getExecuteWidget();
    if (widget)
    {
        //! Удаляем предыдущее, если есть...
        while (QLayoutItem* item = widget->layout()->takeAt(0))
        {
            delete item->widget();
            delete item;
        }
        //! Вызываем методику
        m_activeMethodicUid = uidM;
        m_finishedTestUid = "";
        MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
        metFactory->execute(widget, uidM, params);
        //! И показываем страницу
        static_cast<AAnalyserApplication*>(QApplication::instance())->showClientPage(ClientWidgets::uidExecuteWidgetUid);
    }

}

