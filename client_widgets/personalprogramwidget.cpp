#include "personalprogramwidget.h"
#include "ui_personalprogramwidget.h"

#include <QUuid>
#include <QMessageBox>
#include <QThread>
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
#include "runningmodedialog.h"
#include "databaseresultwidget.h"
#include "patientkarddialog.h"
#include "ppvisualdescriptor.h"
#include "dataprovider.h"
#include "ppvisual.h"

PersonalProgramWidget::PersonalProgramWidget(QWidget *parent) :
    ClientWidget(parent),
    ui(new Ui::PersonalProgramWidget)
{
    ui->setupUi(this);

    ui->frPPOpenTest->setVisible(false);
    restoreMainSplitterPosition();
    ui->btnPPOpenTest->setVisible(false);
//    load();
//    ui->tvPatients->setModel(m_model);
//    m_wgts.clear();
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
    load();
    ui->tvPatients->setModel(m_model);
    m_wgts.clear();

    ui->tvPatients->header()->resizeSections(QHeaderView::ResizeToContents);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::selectPatient,
            this, &PersonalProgramWidget::on_selectPatient);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::newTest,
            this, &PersonalProgramWidget::on_newTest);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::assignedPPForPatient,
            this, &PersonalProgramWidget::on_assignPPForPatient);
    connect(static_cast<AAnalyserApplication*>(QApplication::instance()), &AAnalyserApplication::canceledPPForPatient,
            this, &PersonalProgramWidget::on_cancelPPForPatient);
}

void PersonalProgramWidget::onShow()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic("");

    if (m_finishedTestUid != "")
    {
        bool isDPComplette = false;
        bool isLastDP = false;

        //! Записать в ИП uid теста и времени начала ДП
        if (m_currentDP != -1 && m_curTestNumber != -1)
        {
            isDPComplette = appendTestCompletionInfoToPP(isLastDP);
            auto uidPP = m_objPPExecuted["assigned_uid"].toString();
            DataProvider::savePersonalProgramByUid(uidPP, m_objPPExecuted);

            //! Обновить ее на странице
            if (m_wgts.contains(uidPP))
                m_wgts.value(uidPP)->assignPersonalProgram(uidPP);
        }

        //! Запустить следующий тест через одну секунду
        if (!isDPComplette)
            m_tmNextStep = startTimer(1000);
        else
        {
            //! Восстановим режим проведения проб из параметров
            static_cast<AAnalyserApplication*>(QApplication::instance())->setRunningMode();

            if (isLastDP)
            {
                QMessageBox::information(nullptr, tr("Сообщение"), tr("Индивидуальная программа завершена"));
                doFinishPP();
            }
        }
    }
    else
    {
        //! Восстановим режим проведения проб из параметров
        static_cast<AAnalyserApplication*>(QApplication::instance())->setRunningMode();

        if (m_model)
        {
            //! Если uid текущего пациента не совпадает с uid пациента, выбранного в дереве назначенных ИП,
            //! то снимем выделение и покажем исходную панель
            auto pk = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
            auto index = selectedIndex();
            if (index != QModelIndex())
            {
                auto uidPat = index.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
                if (pk.uid != uidPat)
                {
                    ui->tvPatients->selectionModel()->clearSelection();
                    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectPatient("");
                    hideAllWidgets();
                    ui->lblPicture->setVisible(true);
                }
            }
        }
   }
}

void PersonalProgramWidget::onHide()
{

}

void PersonalProgramWidget::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_tmNextStep)
    {
        killTimer(m_tmNextStep);
        //! Запустить следующий тест
        doRunTest(false);
    }
    ClientWidget::timerEvent(event);
}

void PersonalProgramWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        on_closeTest();
    ClientWidget::keyPressEvent(event);
}

void PersonalProgramWidget::on_splitterMoved(int, int)
{
    saveMainSplitterPosition();
}

void PersonalProgramWidget::on_splTestMoved(int, int)
{
    SettingsProvider::setValueToRegAppCopy("PersonalProgramWidget", "SplitterTestPosition", ui->splTableTest->saveState());
}

void PersonalProgramWidget::on_run()
{
    auto index = selectedIndex();
    if (index != QModelIndex())
    {
        auto uidPP = index.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();
        if (uidPP != "")
        {
            if (index.parent() == QModelIndex())
            {
                //! Проверка ИП на завершенность.
                //! Завершенные всегда находятся в выпадающем списке у пациента, а активные в корне
                //! То есть, просто проверяем, имеется ли корневой узел
                if (index != QModelIndex() && index.isValid())
                {
                    bool isRun = true;
                    auto rm = static_cast<AAnalyserApplication*>(QApplication::instance())->runningMode();
                    if (rm == BaseDefines::rmOperator)
                    {
                        RunningModeDialog dlg(this);
                        dlg.setRunningMode(rm);
                        if (dlg.exec() == QDialog::Accepted)
                        {
                            static_cast<AAnalyserApplication*>(QApplication::instance())->setRunningMode(dlg.runningMode());
                        }
                        else
                            isRun = false;

                    }

                    if (isRun)
                    {
                        m_activePatientUid = index.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
                        m_objPPExecuted = DataProvider::getPersonalProgramByUid(uidPP);
                        doRunTest(true);
                    }
                }
            }
            else
                QMessageBox::information(nullptr, tr("Предупреждение"), tr("Выбранная индивидуальная программа завершена"));
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Пациенту не назначена индивидуальная программа"));
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран пациент с назначенной индивидуальной программой"));
}

void PersonalProgramWidget::on_append()
{
    auto index = selectedIndex();
    if (index != QModelIndex())
    {
        if (index.parent() == QModelIndex())
        {
            auto uidPP = index.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();
            if (uidPP == "")
            {
                //! Проверка ИП на завершенность.
                //! Завершенные всегда находятся в выпадающем списке у пациента, а активные в корне
                //! То есть, просто проверяем, имеется ли корневой узел
                if (index != QModelIndex() && index.isValid())
                {
                    static_cast<AAnalyserApplication*>(QApplication::instance())->assignPPForPatient();
                }
            }
            else
                QMessageBox::information(nullptr, tr("Предупреждение"), tr("Пациенту уже назначена индивидуальная программа"));
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Выберите запись пациента"));
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Пациент не выбран"));
}

void PersonalProgramWidget::on_delete()
{
    auto index = selectedIndex();
    if (index != QModelIndex())
    {
        auto uidPP = index.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();
        if (uidPP != "")
        {
            if (index.parent() == QModelIndex())
            {
                //! Проверка ИП на завершенность.
                //! Завершенные всегда находятся в выпадающем списке у пациента, а активные в корне
                //! То есть, просто проверяем, имеется ли корневой узел
                if (index != QModelIndex() && index.isValid())
                {
                    static_cast<AAnalyserApplication*>(QApplication::instance())->cancelPPForPatient();
                }
            }
            else
                QMessageBox::information(nullptr, tr("Предупреждение"), tr("Выбранная индивидуальная программа уже завершена"));
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Пациенту не назначена индивидуальная программа"));
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Индивидуальная программа не выбрана"));
}

void PersonalProgramWidget::on_params()
{
    auto index = selectedIndex();
    if (index != QModelIndex())
    {
        auto uidPP = index.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();
        if (uidPP != "")
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
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Пациенту не назначена индивидуальная программа"));
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Индивидуальная программа не выбрана"));
}

void PersonalProgramWidget::on_openTest()
{
    if (m_wgtResult)
        delete m_wgtResult;
    m_wgtResult = nullptr;

    auto index = selectedIndex();
    if (index != QModelIndex())
    {
        auto uidPP = index.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();
        if (uidPP != "")
        {
            if (m_wgts.contains(uidPP))
            {
                auto idx = m_wgts.value(uidPP)->selectedIndex();
                if (idx != QModelIndex())
                {
                    auto uidTest = idx.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();
                    if (uidTest != "")
                    {
                        ui->splTableTest->setSizes(QList<int>()
                                                   << static_cast<int>(ui->frPPTable->geometry().width()/2)
                                                   << static_cast<int>(ui->frPPTable->geometry().width()/2));
                        auto splPos = SettingsProvider::valueFromRegAppCopy("PersonalProgramWidget", "SplitterTestPosition").toByteArray();
                        ui->splTableTest->restoreState(splPos);
                        ui->frPPOpenTest->setVisible(true);

                        auto methodics = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
                        m_wgtResult = methodics->visualize(ui->wgtPPTest, uidTest);

                        return;
                    }
                }
            }
        }
    }
}

void PersonalProgramWidget::on_closeTest()
{
    ui->frPPOpenTest->setVisible(false);
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

void PersonalProgramWidget::on_assignPPForPatient(const QString &patientUid, const QString &ppUid)
{
    //! Ищем в модели. Для пациента уже были программы
    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        auto idx = m_model->index(i, 0);
        auto uidP = idx.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
        if (uidP == patientUid)
        {
            m_model->itemFromIndex(idx)->setData(ppUid, DatabaseWidgetDefines::PatientsModel::PatientPPUidRole);
            return;
        }
    }

    //! Не нашли, у пациента не было завершенных программ
    appendLine(patientUid, ppUid);
}

void PersonalProgramWidget::on_cancelPPForPatient(const QString &patientUid)
{
    for (int i = 0; i < m_model->rowCount(); ++i)
    {
        auto idx = m_model->index(i, 0);
        auto uidP = idx.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();
        auto uidPP = idx.data(DatabaseWidgetDefines::PatientsModel::PatientPPUidRole).toString();
        auto pp = DataProvider::getPersonalProgramByUid(uidPP);

        if (uidP == patientUid)
        {
            //! У пациента не было ранее проведенных программ
            if (m_model->rowCount(idx) == 0)
            {
                //! Тесты были по этой программе
                if (PersonalProgram::isTestByPPExists(pp))
                {
                    appendLine(patientUid, uidPP, pp, m_model->itemFromIndex(idx));
                    m_model->itemFromIndex(idx)->setData("", DatabaseWidgetDefines::PatientsModel::PatientPPUidRole);
                }
                else
                    //! Тестов не было
                    m_model->removeRow(i);
            }
            else
            {
                //! Тесты были по этой программе - добавить строку во вложенный список
                if (PersonalProgram::isTestByPPExists(pp))
                    appendLine(patientUid, uidPP, pp, m_model->itemFromIndex(idx));
                //! Тестов не было - просто снять пометку
                m_model->itemFromIndex(idx)->setData("", DatabaseWidgetDefines::PatientsModel::PatientPPUidRole);
            }

            hideAllWidgets();
            m_wgts.remove(uidPP);
        }
    }
}

void PersonalProgramWidget::on_selectTest(QModelIndex idx)
{
    if (idx.isValid())
    {
        //! Если результаты открыты, покажем новые
//        if (ui->frPPOpenTest->isVisible())
            on_openTest();
    }
}

void PersonalProgramWidget::on_editPatientCard()
{
    auto index = selectedIndex();
    if (index != QModelIndex())
    {
        auto uid = index.data(DatabaseWidgetDefines::PatientsModel::PatientUidRole).toString();

        DataDefines::PatientKard pi;
        if (uid != "" && DataProvider::getPatient(uid, pi))
        {
            auto *dialog = new PatientKardDialog(this);
            dialog->setFio(pi.fio);
            dialog->setBorn(pi.born);
            dialog->setSex(pi.sex);
            dialog->setMassa(pi.massa);
            dialog->setHeight(pi.height);

            auto objPP = DataProvider::getActivePersonalProgramForPatient(pi.uid);
            QPixmap ppPic("");
            if (objPP != QJsonObject())
            {
                auto ppName = objPP["pp"].toObject()["name"].toString();
                ppPic = QPixmap(objPP["pp"].toObject()["logo_file_name"].toString());
                dialog->setPersonalProgram(ppName, ppPic);
            }
            else
                dialog->setPersonalProgram(tr("Не задана"), ppPic);

            connect(dialog, &PatientKardDialog::accepted, this, [=]()
            {
                DataDefines::PatientKard patient;
                patient.uid = uid;
                patient.fio = dialog->fio();
                patient.born = dialog->born();
                patient.sex = dialog->sex();
                patient.massa = dialog->massa();
                patient.height = dialog->height();
                DataProvider::updatePatient(patient);
            });
            dialog->show();
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Необходимо выбрать пациента"));
}

void PersonalProgramWidget::on_viewModeDatabase()
{
    static_cast<AAnalyserApplication*>(QApplication::instance())->showClientPage(ClientWidgets::uidDatabaseResultWidgetUid);
    SettingsProvider::setValueToRegAppCopy("MainWindow", "MainClientWidget", ClientWidgets::uidDatabaseResultWidgetUid);
}

QStandardItem* PersonalProgramWidget::appendLine(const QString uidPat,
                                                 const QString uidPPAssigned,
                                                 const QJsonObject& objPP,
                                                 QStandardItem* root)
{
    DataDefines::PatientKard pi;
    if (DataProvider::getPatient(uidPat, pi))
    {
        QList<QStandardItem*> line;

        QString title = pi.fio;
        QString imgName = ":/images/tree/man.png";
        if (root)
        {
            auto db = objPP["date_begin"].toString();
            auto de = objPP["date_end"].toString();
            title = db + " - " + de;
            imgName = ":/images/tree/probe.png";
        }
        QStandardItem *itemFIO = new QStandardItem(title);
        itemFIO->setData(pi.uid, DatabaseWidgetDefines::PatientsModel::PatientUidRole);
        itemFIO->setData(uidPPAssigned, DatabaseWidgetDefines::PatientsModel::PatientPPUidRole);
        itemFIO->setIcon(QIcon(imgName));
        itemFIO->setEditable(false);
        if (!root)
        {
            QStandardItem *itemBorn = new QStandardItem(pi.born.toString("dd.MM.yyyy"));
            itemBorn->setEditable(false);
            QStandardItem *itemSex = new QStandardItem(DataDefines::SexToText.value(
                                                           static_cast<DataDefines::Sex>(pi.sex)));
            itemSex->setData(pi.sex, DatabaseWidgetDefines::PatientsModel::PatientSexRole);
            itemSex->setEditable(false);
            line << itemFIO << itemBorn << itemSex;
        }
        else
            line << itemFIO;

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
                auto item = appendLine(uidPat, ""); // uidPPAssigned);
                patients.insert(uidPat, item);
                appendLine(uidPat, uidPPAssigned, objPP, patients.value(uidPat));
            }
        }
    }
    m_model->setHorizontalHeaderLabels(QStringList() << tr("ФИО") << tr("Дата рождения") << tr("Пол"));
}

void PersonalProgramWidget::showPersonalProgram(const QString& uidPPAssigned)
{
    hideAllWidgets();
    if (m_wgts.contains(uidPPAssigned))
    {
        m_wgts.value(uidPPAssigned)->setVisible(true);
        ui->lblPPTitle->setText("Индивидуальная программа: \"" + m_wgts.value(uidPPAssigned)->namePP() + "\"");
    }
    else
    {
        auto ppw = new PatientProgramWidget(ui->frPrograms);
        ppw->assignPersonalProgram(uidPPAssigned);
        connect(ppw, &PatientProgramWidget::selectItem, this, &PersonalProgramWidget::on_selectTest);
        ui->lblPPTitle->setText("Индивидуальная программа: \"" + ppw->namePP() + "\"");
        ui->frPrograms->layout()->addWidget(ppw);
        m_wgts.insert(uidPPAssigned, ppw);

        //! Пример получения списка визуализаторов ИП
        auto objPP = DataProvider::getPersonalProgramByUid(uidPPAssigned);
        int n = static_cast<AAnalyserApplication*>(QApplication::instance())->ppVisualCount();
        qDebug() << Q_FUNC_INFO << "----------" << n;
        for (int i = 0; i < n; ++i)
        {
            auto vd = static_cast<AAnalyserApplication*>(QApplication::instance())->getPPVisual(i);

            auto wgtVD = vd->getVisualWidget(objPP, nullptr);
            qDebug() << Q_FUNC_INFO << "----------" << i << vd->name() << wgtVD->isValid();
            delete wgtVD;
        }
    }
}


void PersonalProgramWidget::hideAllWidgets()
{
    QObjectList children = ui->frPrograms->children();
    foreach(QObject * child, children)
        if (child->isWidgetType())
            static_cast<QWidget*>(child)->setVisible(false);
}


void PersonalProgramWidget::saveMainSplitterPosition()
{
    SettingsProvider::setValueToRegAppCopy("PersonalProgramWidget", "SplitterPosition", ui->splitter->saveState());
}

void PersonalProgramWidget::restoreMainSplitterPosition()
{
    auto val = SettingsProvider::valueFromRegAppCopy("PersonalProgramWidget", "SplitterPosition").toByteArray();
//    if (val == "")
//        val = QByteArray::fromRawData("\x00\x00\x00\xFF\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x01\x1B\x00\x00\x03""b\x01\xFF\xFF\xFF\xFF\x01\x00\x00\x00\x01\x00", 31);
    ui->splitter->restoreState(val);
}

QModelIndex PersonalProgramWidget::selectedIndex() const
{
    auto selModel = ui->tvPatients->selectionModel();
    if (selModel)
    {
        auto selIdxs = selModel->selectedIndexes();
        for (int i = 0; i < selIdxs.size(); ++i)
        {
            if (selIdxs.at(i).column() == 0)
                return selIdxs.at(i);
        }
    }
    return QModelIndex();
}

void PersonalProgramWidget::doRunTest(bool isFirstRun)
{
    QJsonObject objTest = QJsonObject();
    if (getNextTestInfo(m_objPPExecuted, objTest, isFirstRun))
    {
        if (objTest != QJsonObject())
            runTest(objTest);
        else
            QMessageBox::information(nullptr,
                                     tr("Сообщение"),
                                     tr("Запуск теста по индивидуальной программе в настоящий момент невозможен") + "\n" +
                                     tr("Прошло мало времени с момента окончания предыдущего теста"));
    }
    else
    {
        if (m_currentDP == -1)
            QMessageBox::information(nullptr, tr("Сообщение"), tr("Индивидуальная программа завершена"));
        else
            QMessageBox::information(nullptr,
                                     tr("Сообщение"),
                                     tr("Превышено максимальное время между дневными программами. Индивидуальная программа завершена"));

        //! Завершить ИП
        doFinishPP();
    }
}

void PersonalProgramWidget::doFinishPP()
{
    auto pi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentPatient();
    if (pi.uid != "")
    {
        auto pp = DataProvider::getActivePersonalProgramForPatient(pi.uid);
        if (pp != QJsonObject())
        {
            DataProvider::deactivatePersonalProgramForPatient(pi.pp_uid);
            //! Удалить программу у пациента
            pi.pp_uid = "";
            DataProvider::updatePatient(pi);

            //! Извещаем мир об отмене индивидуальной программы для пациента
            emit static_cast<AAnalyserApplication*>(QApplication::instance())->canceledPPForPatient(pi.uid);
        }
    }
}

QDateTime PersonalProgramWidget::getDateTimeByString(const QString &s) const
{
    if (s != "")
        return QDateTime::fromString(s, "dd.MM.yyyy hh:mm");
    return QDateTime();
}

int PersonalProgramWidget::findEmptyTestInfo(const QJsonArray &arr, QJsonObject &objTest)
{
    for (int i = 0; i < arr.size(); ++i)
    {
        auto obj = arr.at(i).toObject();
        auto uidT = obj["test_uid"].toString("");
        if (uidT == "")
        {
            objTest = obj;
            return i;
        }
    }
    objTest = QJsonObject();
    return -1;
}

bool isNowLessMin(const QDateTime& dtLast, const int minTimeDPSec)
{
    return (((minTimeDPSec > 0 &&
              dtLast != QDateTime() &&
              dtLast.secsTo(QDateTime::currentDateTime()) < minTimeDPSec)
                 ||
              (minTimeDPSec < 0 && (QDateTime::currentDateTime().date().day() == dtLast.date().day()))));
}

bool isNowAboveMax(const QDateTime& dtLast, const int maxTimeDPSec)
{
    return (maxTimeDPSec > 0 &&
            dtLast != QDateTime() &&
            dtLast.secsTo(QDateTime::currentDateTime()) > maxTimeDPSec);
}

bool isNowInBounds(const QDateTime& dtLast, const int minTimeDPSec, const int maxTimeDPSec)
{
    if (dtLast == QDateTime())
        return true;
    else
        return ((((minTimeDPSec >= 0 &&
                   dtLast != QDateTime() &&
                   dtLast.secsTo(QDateTime::currentDateTime()) > minTimeDPSec)
                      ||
                   (minTimeDPSec < 0 && (QDateTime::currentDateTime().date().day() != dtLast.date().day()))))
                  &&
                (maxTimeDPSec > 0 &&
                 dtLast != QDateTime() &&
                 dtLast.secsTo(QDateTime::currentDateTime()) < maxTimeDPSec));
}

bool PersonalProgramWidget::getNextTestInfo(const QJsonObject &objPPAll, QJsonObject& objTest, bool isFirstRun)
{
    auto objPP = objPPAll["pp"].toObject();

    m_currentDP = -1;
    m_curTestNumber = -1;

    auto minTimeDP = objPP["min_time_between_dp"].toInt();                   ///< В индексах 0, 1, 2, ...
    minTimeDP = PersonalProgramDefines::MinTimeBetweenDPList.at(minTimeDP);  ///< В часах
    auto minTimeDPSec = minTimeDP * 3600;                                    ///< В секундах
    auto maxTimeDP = objPP["max_time_between_dp"].toInt();                   ///< В индексах 0, 1, 2, ...
    maxTimeDP = PersonalProgramDefines::MaxTimeBetweenDPList.at(maxTimeDP);  ///< В часах
    auto maxTimeDPSec = maxTimeDP * 3600;                                    ///< В секундах

    QDateTime dtLast = QDateTime();
    auto arrDP = objPP["dp_list"].toArray();

    //! Поиск первой ДП, имеющей проведенные тесты или не имеющей их вообще
    int startDPIdx = findStartDP(arrDP, dtLast);

    for (int i = startDPIdx; i < arrDP.size(); ++i)
    {
        auto objDP = arrDP.at(i).toObject();
        auto dtDP = getDateTimeByString(objDP["date_time"].toString(""));
        if (dtDP != QDateTime())
            dtLast = dtDP;

        auto arrTests = objDP["test_list"].toArray();
        QJsonObject objT = QJsonObject();
        m_curTestNumber = findEmptyTestInfo(arrTests, objT);

        if (m_curTestNumber > -1 && objT != QJsonObject())
        {
            //! Первый тест в серии
            if (isFirstRun)
            {
                //! Первый тест в дневной программе
                if (m_curTestNumber == 0)
                {
                    //! Превысили максимальное время - завершаем ИП
                    if (isNowAboveMax(dtLast, maxTimeDPSec))
                    {
                        objTest = QJsonObject();
                        m_currentDP = i;
                        return false;
                    }
                    else
                    //! Меньше минимального времени - запускать нельзя
                    if (isNowLessMin(dtLast, minTimeDPSec))
                    {
                        objTest = QJsonObject();
                        m_currentDP = -1;
                        return true;
                    }
                    else
                    //! В допустимом временном диапазоне - проводим
                    if (isNowInBounds(dtLast, minTimeDPSec, maxTimeDPSec))
                    {
                        objTest = objT;
                        m_currentDP = i;
                        return true;
                    }
                }
                else
                //! Не первый тест в дневной программе
                {
                    //! Меньше минимального времени - проводим, иначе - следующая ДП
                    if (isNowLessMin(dtLast, minTimeDPSec))
                    {
                        objTest = objT;
                        m_currentDP = i;
                        return true;
                    }
                }
            }
            else
            //! Не первый тест в серии
            {
                //! Первый тест в дневной программе - завершаем, проводить нельзя
                if (m_curTestNumber == 0)
                {
                    objTest = QJsonObject();
                    m_currentDP = -1;
                    return true;
                }
                else
                //! Не первый тест в дневной программе - проводим
                {
                    objTest = objT;
                    m_currentDP = i;
                    return true;
                }
            }
        }
    }

    objTest = QJsonObject();
    m_currentDP = -1;
    return false;
}

int PersonalProgramWidget::findStartDP(QJsonArray arrDP, QDateTime& dtLast)
{
    for (int i = arrDP.size() - 1; i >= 0; --i)
    {
        auto objDP = arrDP.at(i).toObject();
        auto dtDP = getDateTimeByString(objDP["date_time"].toString(""));
        if (dtDP != QDateTime() && dtLast == QDateTime())
            dtLast = dtDP;

        auto arrTests = objDP["test_list"].toArray();
        QJsonObject objT = QJsonObject();
        int emptyTestIdx = findEmptyTestInfo(arrTests, objT);
        if (emptyTestIdx == -1)
            return i + 1;
        if (emptyTestIdx > 0)
            return i;
    }
    return 0;
}

//bool PersonalProgramWidget::getNextTestInfo(const QJsonObject &objPPAll, QJsonObject& objTest, bool isFirstRun)
//{
//    auto objPP = objPPAll["pp"].toObject();

//    m_currentDP = -1;
//    m_curTestNumber = -1;

//    auto minTimeDP = objPP["min_time_between_dp"].toInt();                   ///< В индексах 0, 1, 2, ...
//    minTimeDP = PersonalProgramDefines::MinTimeBetweenDPList.at(minTimeDP);  ///< В часах
//    auto minTimeDPSec = minTimeDP * 3600;                                    ///< В секундах
//    auto maxTimeDP = objPP["max_time_between_dp"].toInt();                   ///< В индексах 0, 1, 2, ...
//    maxTimeDP = PersonalProgramDefines::MaxTimeBetweenDPList.at(maxTimeDP);  ///< В часах
//    auto maxTimeDPSec = maxTimeDP * 3600;                                    ///< В секундах

//    QDateTime dtLast = QDateTime();
//    auto arrDP = objPP["dp_list"].toArray();
//    for (int i = 0; i < arrDP.size(); ++i)
//    {
//        auto objDP = arrDP.at(i).toObject();
//        auto dtDP = getDateTimeByString(objDP["date_time"].toString(""));
//        if (dtDP != QDateTime())
//            dtLast = dtDP;

//        auto arrTests = objDP["test_list"].toArray();
//        QJsonObject objT = QJsonObject();
//        m_curTestNumber = findEmptyTestInfo(arrTests, objT);

//        //! Варианты:
//        //! 1. dtLast != QDateTime() && QDateTime::currentDateTime() - dtLast > maxTimeDPSec Превысили максимальное время - завершаем ИП
//        //! 2. objTest != QJsonObject() В ДП есть непроведенный тест
//        //!    2.1. dtDP == QDateTime() ДП не проводилась вообще. тест должен быть первый. Вернуть objTest
//        //!    2.2. dtDP != QDateTime() ДП проводилась, но была прервана
//        //!       2.2.1. QDateTime::currentDateTime() - dtDP < minTimeDPSec Прошло меньше минимума. Вернуть objTest
//        //!       2.2.2. QDateTime::currentDateTime() - dtDP > minTimeDPSec Прошло больше минимума. Следующая ДП
//        //! 3. objTest == QJsonObject() В ДП нет непроведенных тестов. Следующая ДП

//        //! Превысили максимальное время - завершаем ИП
//        if (isFirstRun &&
//            maxTimeDPSec > 0 &&
//            dtLast != QDateTime() &&
//            dtLast.secsTo(QDateTime::currentDateTime()) > maxTimeDPSec)
//        {
//            objTest = QJsonObject();
//            m_currentDP = i;
//            return false;
//        }

//        //! Меньше минимального времени - запускать нельзя
//        if (isFirstRun &&
//            ((minTimeDPSec > 0 &&
//             dtLast != QDateTime() &&
//             dtLast.secsTo(QDateTime::currentDateTime()) < minTimeDPSec)
//                ||
//             (minTimeDPSec < 0 && (QDateTime::currentDateTime().date().day() == dtLast.date().day()))))
//        {
//            objTest = QJsonObject();
//            m_currentDP = -1;
//            return true;
//        }

//        //! В ДП есть непроведенный тест
//        if (objT != QJsonObject())
//        {
//            //! ДП не проводилась вообще
//            if (dtDP == QDateTime())
//            {
//                objTest = objT;
//                m_currentDP = i;
//                return true;
//            }
//            else
//            //! ДП проводилась, но была прервана
//            {
//                //! Прошло меньше минимума
//                if ((isFirstRun && dtDP.secsTo(QDateTime::currentDateTime()) < minTimeDPSec)
//                        ||
//                     !isFirstRun)
//                {
//                    objTest = objT;
//                    m_currentDP = i;
//                    return true;
//                }
//            }
//        }
//    }

//    objTest = QJsonObject();
//    return false;
//}

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

        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic(uidM);

        //! Вызываем методику
        m_activeMethodicUid = uidM;
        m_finishedTestUid = "";
        MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
        metFactory->execute(widget, uidM, params);
        //! И показываем страницу
        static_cast<AAnalyserApplication*>(QApplication::instance())->showClientPage(ClientWidgets::uidExecuteWidgetUid);
    }

}

bool PersonalProgramWidget::appendTestCompletionInfoToPP(bool& isLastDP)
{
    bool isDPComplete = false;

    auto objPP = m_objPPExecuted["pp"].toObject();
    auto arrDP = objPP["dp_list"].toArray();

    if (m_currentDP < arrDP.size())
    {
        auto objDP = arrDP.at(m_currentDP).toObject();

        //! Если тест первый в DP, то записать дату и время проведения
        if (m_curTestNumber == 0)
            objDP["date_time"] = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm");

        auto arrTests = objDP["test_list"].toArray();

        if (m_curTestNumber < arrTests.size())
        {
            auto objTest = arrTests.at(m_curTestNumber).toObject();
            objTest["test_uid"] = m_finishedTestUid;
            arrTests.replace(m_curTestNumber, objTest);
        }
        objDP["test_list"] = arrTests;
        if (m_curTestNumber == (arrTests.size() - 1))
            isDPComplete = true;

        arrDP.replace(m_currentDP, objDP);
    }

    objPP["dp_list"] = arrDP;
    m_objPPExecuted["pp"] = objPP;

    isLastDP = (m_currentDP == (arrDP.size() - 1));

    return isDPComplete;
}

