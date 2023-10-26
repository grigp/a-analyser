#include "activepersonalprogrameditor.h"
#include "ui_activepersonalprogrameditor.h"

#include <QMessageBox>
#include <QDebug>

#include "aanalyserapplication.h"
#include "datadefines.h"
#include "dataprovider.h"
#include "personalprogramdefines.h"
#include "personalprogram.h"
#include "metodicsfactory.h"
#include "selectdailyprogramdialog.h"
#include "selectmethodicdialog.h"
#include "dailyprogramcompleteddelegate.h"
#include "testcompleteddelegate.h"
#include "settingsprovider.h"


ActivePersonalProgramEditor::ActivePersonalProgramEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActivePersonalProgramEditor)
{
    ui->setupUi(this);
    prepareParams();

    ui->tvSchedule->setModel(&m_mdlDP);
    ui->tvTests->setModel(&m_mdlT);
    ui->tvSchedule->setItemDelegateForColumn(0, new DailyProgramCompletedDelegate(ui->tvSchedule));
    ui->tvTests->setItemDelegateForColumn(0, new TestCompletedDelegate(ui->tvTests));

    auto val = SettingsProvider::valueFromRegAppCopy("ActivePersonalProgramEditor", "Geometry").toRect();
    if (val != QRect())
    {
        setGeometry(val);
    }
}

ActivePersonalProgramEditor::~ActivePersonalProgramEditor()
{
    if (m_mdlPP)
        delete m_mdlPP;
    delete ui;
}

void ActivePersonalProgramEditor::setPersonalProgram(const QJsonObject &objPPAll)
{
    m_mdlPP = new PersonalProgram(this);
    m_mdlPP->load(objPPAll);

    DataDefines::PatientKard pi;
    if (DataProvider::getPatient(m_mdlPP->uidPatient(), pi))
    {
        ui->edPatient->setText(pi.fio);
        ui->edName->setText(m_mdlPP->name());
        ui->cbMinTimeBetweenDP->setCurrentIndex(m_mdlPP->minTimeBetweenDP());
        ui->cbMaxTimeBetweenDP->setCurrentIndex(m_mdlPP->maxTimeBetweenDP());
        ui->lblLogotip->setPixmap(QPixmap(m_mdlPP->logoFileName()));
    }

    initListDP();
    m_mdlT.clear();

    QString sSheet = QString("QTableView") +
    "{" +
//!        "background-color: rgb(240, 240, 245);" +
        "font-size: 11pt;" +
        "color: rgb(32,88,103);" +
    "}" +
            QString("QHeaderView::section") +
    "{" +
            "background-color: rgb(240, 240, 245);" +
            "font-size: 11pt;" +
            "color: rgb(32,88,103);" +
    "}";
    setStyleSheet(sSheet);
}

QJsonObject ActivePersonalProgramEditor::personalProgram() const
{
    return m_mdlPP->save();
}

void ActivePersonalProgramEditor::showEvent(QShowEvent *event)
{
    ui->tvSchedule->horizontalHeader()->resizeSection(0, ui->tvSchedule->geometry().width());
    QDialog::showEvent(event);
}

void ActivePersonalProgramEditor::resizeEvent(QResizeEvent *event)
{
    SettingsProvider::setValueToRegAppCopy("ActivePersonalProgramEditor", "Geometry", geometry());
    ui->tvSchedule->horizontalHeader()->resizeSection(0, ui->tvSchedule->geometry().width());
    QDialog::resizeEvent(event);
}

void ActivePersonalProgramEditor::on_selectDP(QModelIndex index)
{
    if (index.isValid())
    {
        PersonalProgramDefines::DPCompletedValue complNext = PersonalProgramDefines::dpcvNotStarted;
        auto idxNext = m_mdlDP.index(index.row() + 1, index.column());
        if  (idxNext.isValid())
            complNext = static_cast<PersonalProgramDefines::DPCompletedValue>(idxNext.data(PersonalProgramDefines::DPCompletedRole).toInt());
        viewDP(index.row(), complNext);
    }
}

void ActivePersonalProgramEditor::on_selectT(QModelIndex index)
{
    Q_UNUSED(index);
}

void ActivePersonalProgramEditor::on_dpAdd()
{
    if (!m_dlgSelDP)
        m_dlgSelDP = new SelectDailyProgramDialog(this);

    if (m_dlgSelDP->exec() == QDialog::Accepted)
    {
        auto dp = m_dlgSelDP->dailyProgram();
        m_mdlPP->addDailyProgram(dp);

        auto *item = new QStandardItem(dp["name"].toString());
        item->setEditable(false);
        item->setData(dp["uid"].toString(), PersonalProgramDefines::PersonalProgram::DPUidRole);
        item->setData(dp["name"].toString(), PersonalProgramDefines::PersonalProgram::DPNameRole);
        m_mdlDP.appendRow(item);
    }
}

void ActivePersonalProgramEditor::on_dpDel()
{
    auto selIdx = selectedDPIndex();

    if (selIdx.data(PersonalProgramDefines::DPCompletedRole).toInt() == PersonalProgramDefines::dpcvNotStarted)
    {
        if (selIdx != QModelIndex() && selIdx.isValid())
        {
            QString name = selIdx.data().toString();
            int num = selIdx.row();
            auto mr = QMessageBox::question(nullptr,
                                            tr("Запрос"),
                                            tr("Удалить дневную программу?") + "\n" +
                                            tr("Программа №") + QString::number(num + 1) + " : " + name);
            if (mr == QMessageBox::Yes)
            {
                m_mdlPP->removeRow(num);
                m_mdlDP.removeRow(num);
            }
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя удалять выполненную дневную программу"));
}

void ActivePersonalProgramEditor::on_dpMoveUp()
{
    auto selIdx = selectedDPIndex();
    if (selIdx.data(PersonalProgramDefines::DPCompletedRole).toInt() == PersonalProgramDefines::dpcvNotStarted)
    {
        int num = selIdx.row();

        //! Нельзя поднимать выше, чем последняя проведенная дневная программа
        bool upCompleted = false;
        if (num > 0)
        {
            auto upIdx = m_mdlDP.index(num - 1, 0);
            upCompleted = upIdx.data(PersonalProgramDefines::DPCompletedRole).toBool();
        }

        if (selIdx != QModelIndex() && selIdx.isValid() && num > 0 && !upCompleted)
        {
            //! Перемещаем в модели DP
            auto row = m_mdlDP.takeRow(num);
            m_mdlDP.insertRow(num - 1, row);
            //! Играем с выделением
            ui->tvSchedule->selectionModel()->clearSelection();
            ui->tvSchedule->selectionModel()->select(row.at(0)->index(), QItemSelectionModel::Select);

            //! Перемещаем в модели общейы
            row = m_mdlPP->takeRow(num);
            m_mdlPP->insertRow(num - 1, row);
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя перемещать выполненную дневную программу"));
}

void ActivePersonalProgramEditor::on_dpMoveDown()
{
    auto selIdx = selectedDPIndex();
    if (selIdx.data(PersonalProgramDefines::DPCompletedRole).toInt() == PersonalProgramDefines::dpcvNotStarted)
    {
        int num = selIdx.row();
        if (selIdx != QModelIndex() && selIdx.isValid() && num < m_mdlDP.rowCount() - 1)
        {
            //! Перемещаем в модели DP
            auto row = m_mdlDP.takeRow(num);
            m_mdlDP.insertRow(num + 1, row);
            //! Играем с выделением
            ui->tvSchedule->selectionModel()->clearSelection();
            ui->tvSchedule->selectionModel()->select(row.at(0)->index(), QItemSelectionModel::Select);

            //! Перемещаем в модели общейы
            row = m_mdlPP->takeRow(num);
            m_mdlPP->insertRow(num + 1, row);
        }
    }
    else
        QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя перемещать выполненную дневную программу"));
}

void ActivePersonalProgramEditor::on_testAdd()
{
    auto dpIdx = selectedDPIndex();
    if (dpIdx != QModelIndex())
    {
        if (dpIdx.data(PersonalProgramDefines::DPCompletedRole).toInt() != PersonalProgramDefines::dpcvCompleted)
        {
            if (!m_dlgSelMethod)
                m_dlgSelMethod = new SelectMethodicDialog(this);

            if (m_dlgSelMethod->exec() == QDialog::Accepted)
            {
                auto metUid = m_dlgSelMethod->methodic();
                if (metUid != QUuid().toString())
                {
                    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
                    auto mi = metFactory->metodic(metUid);

                    auto *item = new QStandardItem(mi.name);
                    item->setEditable(false);
                    item->setData(metUid, PersonalProgramDefines::PersonalProgram::MethodUidRole);
                    item->setData(mi.params, PersonalProgramDefines::PersonalProgram::ParamsRole);
                    item->setIcon(QIcon(":/images/Methodics/" + mi.imageName));
                    m_mdlT.appendRow(item);

                    m_mdlPP->addTest(dpIdx.row(), metUid, mi.params);
                }
                else
                    QMessageBox::information(nullptr, tr("Сообщение"), tr("Методика не выбрана"));
            }
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя добавлять тест в выполненную дневную программу"));
    }
    else
        QMessageBox::information(nullptr, tr("Сообщение"), tr("Не выбрана дневная программа"));
}

void ActivePersonalProgramEditor::on_testEdit()
{
    auto dpIdx = selectedDPIndex();
    if (dpIdx != QModelIndex())
    {
        if (dpIdx.data(PersonalProgramDefines::DPCompletedRole).toInt() != PersonalProgramDefines::dpcvCompleted)
        {
            auto testIdx = selectedTestIndex();
            if (testIdx.isValid())
            {
                auto testUid = testIdx.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();
                if (testUid == "")
                {
                    auto metUid = testIdx.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString();
                    auto params = testIdx.data(PersonalProgramDefines::PersonalProgram::ParamsRole).toJsonObject();
                    if (DataProvider::editMetodicParams(this, metUid, params))
                    {
                        m_mdlT.itemFromIndex(testIdx)->setData(params, PersonalProgramDefines::PersonalProgram::ParamsRole);
                        m_mdlPP->item(dpIdx.row(), testIdx.row() + 1)->setData(params, PersonalProgramDefines::PersonalProgram::ParamsRole);
                    }
                }
                else
                    QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя редактировать проведенный тест"));
            }
            else
                QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя редактировать тест в выполненной дневной программе"));
    }
    else
        QMessageBox::information(nullptr, tr("Сообщение"), tr("Не выбрана дневная программа"));
}

void ActivePersonalProgramEditor::on_testDel()
{
    auto dpIdx = selectedDPIndex();
    if (dpIdx != QModelIndex())
    {
        if (dpIdx.data(PersonalProgramDefines::DPCompletedRole).toInt() != PersonalProgramDefines::dpcvCompleted)
        {
            auto testIdx = selectedTestIndex();
            if (testIdx.isValid())
            {
                auto testUid = testIdx.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();
                if (testUid == "")
                {
                    auto metName = testIdx.data().toString();
                    auto mr = QMessageBox::question(nullptr,
                                                    tr("Запрос"),
                                                    tr("Удалить тест из дневной программы?") + "\n" +
                                                    tr("Тест") + " : " + metName);
                    if (mr == QMessageBox::Yes)
                    {
                        //! Удаляем из виджета
                        m_mdlT.removeRow(testIdx.row());

                        //! Удаляем из модели
                        //! Поскольку надо удалить итем из строки, сформируем новую строку без этого итема и заменим ею существующую
                        QList<QStandardItem*> items;
                        for (int i = 0; i < m_mdlPP->columnCount(); ++i)
                        {
                            auto item = m_mdlPP->item(dpIdx.row(), i);
                            if (item)
                            {
                                auto uidMethod = item->data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString();
                                if ((i == 0) || (i > 0 && uidMethod != "" && i != testIdx.row() + 1))
                                    items << item->clone();
                            }
                        }
                        m_mdlPP->removeRow(dpIdx.row());
                        m_mdlPP->insertRow(dpIdx.row(), items);
                    }
                }
                else
                    QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя удалять проведенный тест"));
            }
            else
                QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя удалять тест из выполненной дневной программы"));
    }
    else
        QMessageBox::information(nullptr, tr("Сообщение"), tr("Не выбрана дневная программа"));
}

void ActivePersonalProgramEditor::on_testMoveUp()
{
    auto dpIdx = selectedDPIndex();
    if (dpIdx != QModelIndex())
    {
        if (dpIdx.data(PersonalProgramDefines::DPCompletedRole).toInt() != PersonalProgramDefines::dpcvCompleted)
        {
            auto testIdx = selectedTestIndex();
            if (testIdx.isValid())
            {
                auto testUid = testIdx.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();
                if (testUid == "")
                {
                    if (testIdx.row() > 0)
                    {
                        auto row = m_mdlT.takeRow(testIdx.row());
                        m_mdlT.insertRow(testIdx.row() - 1, row);
                        ui->tvTests->selectionModel()->clearSelection();
                        ui->tvTests->selectionModel()->select(row.at(0)->index(), QItemSelectionModel::Select);
                        row = m_mdlPP->takeRow(dpIdx.row());
                        row.move(testIdx.row() + 1, testIdx.row());
                        m_mdlPP->insertRow(dpIdx.row(), row);
                    }
                }
                else
                    QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя перемещать проведенный тест"));
            }
            else
                QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя перемещать тест в выполненной дневной программе"));
    }
    else
        QMessageBox::information(nullptr, tr("Сообщение"), tr("Не выбрана дневная программа"));
}

void ActivePersonalProgramEditor::on_testMoveDown()
{
    auto dpIdx = selectedDPIndex();
    if (dpIdx != QModelIndex())
    {
        if (dpIdx.data(PersonalProgramDefines::DPCompletedRole).toInt() != PersonalProgramDefines::dpcvCompleted)
        {
            auto testIdx = selectedTestIndex();
            if (testIdx.isValid())
            {
                auto testUid = testIdx.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();
                if (testUid == "")
                {
                    if (testIdx.row() < m_mdlT.rowCount() - 1)
                    {
                        auto row = m_mdlT.takeRow(testIdx.row());
                        m_mdlT.insertRow(testIdx.row() + 1, row);
                        ui->tvTests->selectionModel()->clearSelection();
                        ui->tvTests->selectionModel()->select(row.at(0)->index(), QItemSelectionModel::Select);
                        row = m_mdlPP->takeRow(dpIdx.row());
                        row.move(testIdx.row() + 1, testIdx.row() + 2);
                        m_mdlPP->insertRow(dpIdx.row(), row);
                    }
                }
                else
                    QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя перемещать проведенный тест"));
            }
            else
                QMessageBox::information(nullptr, tr("Предупреждение"), tr("Не выбран тест"));
        }
        else
            QMessageBox::information(nullptr, tr("Предупреждение"), tr("Нельзя перемещать тест в выполненной дневной программе"));
    }
    else
        QMessageBox::information(nullptr, tr("Сообщение"), tr("Не выбрана дневная программа"));
}

void ActivePersonalProgramEditor::on_minTimeChanged(int idx)
{
    if (m_mdlPP)
    {
        Q_UNUSED(idx);
        auto valMin = ui->cbMinTimeBetweenDP->currentData().toInt();
        auto valMax = ui->cbMaxTimeBetweenDP->currentData().toInt();
        if (valMin > valMax)
        {
            while (valMin > valMax)
            {
                int idx = ui->cbMaxTimeBetweenDP->currentIndex();
                if (idx < ui->cbMaxTimeBetweenDP->count() - 1)
                    ui->cbMaxTimeBetweenDP->setCurrentIndex(idx + 1);
                else
                    break;
                valMax = ui->cbMaxTimeBetweenDP->currentData().toInt();
            }
        }
        m_mdlPP->setMinTimeBetweenDP(idx);
    }
}

void ActivePersonalProgramEditor::on_maxTimeChanged(int idx)
{
    if (m_mdlPP)
    {
        Q_UNUSED(idx);
        auto valMin = ui->cbMinTimeBetweenDP->currentData().toInt();
        auto valMax = ui->cbMaxTimeBetweenDP->currentData().toInt();
        if (valMax < valMin)
        {
            while (valMax < valMin)
            {
                int idx = ui->cbMinTimeBetweenDP->currentIndex();
                if (idx > 0)
                    ui->cbMinTimeBetweenDP->setCurrentIndex(idx - 1);
                else
                    break;
                valMin = ui->cbMinTimeBetweenDP->currentData().toInt();
            }
        }
        m_mdlPP->setMaxTimeBetweenDP(idx);
    }
}

void ActivePersonalProgramEditor::initListDP()
{
    m_mdlDP.clear();

    for (int i = 0; i < m_mdlPP->rowCount(); ++i)
    {
        auto dpName = m_mdlPP->index(i, 0).data(PersonalProgramDefines::PersonalProgram::DPNameRole).toString();
        auto dpUid = m_mdlPP->index(i, 0).data(PersonalProgramDefines::PersonalProgram::DPUidRole).toString();
        auto dpDT = m_mdlPP->index(i, 0).data(PersonalProgramDefines::PersonalProgram::DPDateTimeRole).toString();

        PersonalProgramDefines::DPCompletedValue dpCompleted = PersonalProgramDefines::dpcvNotStarted;
        bool isCompleted = true;
        for (int j = 1; j < m_mdlPP->columnCount(); ++j)
        {
            auto idxTest = m_mdlPP->index(i, j);
            if (idxTest.isValid() &&
                idxTest.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString() != "")
            {
                if (idxTest.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString() != "")
                    dpCompleted = PersonalProgramDefines::dpcvInterrupted;
                else
                    isCompleted = false;
            }
        }
        if (isCompleted)
                dpCompleted = PersonalProgramDefines::dpcvCompleted;

        auto item = new QStandardItem(dpName);
        item->setEditable(false);
        item->setData(dpUid, PersonalProgramDefines::PersonalProgram::DPUidRole);
        item->setData(dpDT, PersonalProgramDefines::PersonalProgram::DPDateTimeRole);
        item->setData(dpCompleted, PersonalProgramDefines::DPCompletedRole);

        m_mdlDP.appendRow(item);
    }
}

void ActivePersonalProgramEditor::viewDP(const int numDP, const PersonalProgramDefines::DPCompletedValue complNext)
{
    m_mdlT.clear();

    if (numDP >= 0 && numDP < m_mdlPP->rowCount())
    {
        for (int i = 0; i < m_mdlPP->columnCount(); ++i)
        {
            auto index = m_mdlPP->index(numDP, i);
            if (index.isValid())
            {
                auto uidMethod = index.data(PersonalProgramDefines::PersonalProgram::MethodUidRole).toString();
                if (uidMethod != "")
                {
                    MetodicsFactory *metFactory = static_cast<AAnalyserApplication*>(QApplication::instance())->getMetodics();
                    auto mi = metFactory->metodic(uidMethod);

                    auto uidTest = index.data(PersonalProgramDefines::PersonalProgram::TestUidRole).toString();
                    //! Если следующая ДП начата, то все тесты пометить проведенными вне зависимости от реального статуса
                    if (complNext != PersonalProgramDefines::dpcvNotStarted)
                        uidTest = "completed";
                    auto params = index.data(PersonalProgramDefines::PersonalProgram::ParamsRole).toJsonObject();

                    auto item = new QStandardItem(mi.name);
                    item->setEditable(false);
                    item->setData(uidMethod, PersonalProgramDefines::PersonalProgram::MethodUidRole);
                    item->setData(uidTest, PersonalProgramDefines::PersonalProgram::TestUidRole);
                    item->setData(params, PersonalProgramDefines::PersonalProgram::ParamsRole);
                    item->setIcon(QIcon(":/images/Methodics/" + mi.imageName));

                    m_mdlT.appendRow(item);
                }
            }
        }
    }
}

void ActivePersonalProgramEditor::prepareParams()
{
    ui->cbMinTimeBetweenDP->clear();
    foreach (auto v, PersonalProgramDefines::MinTimeBetweenDPList)
        ui->cbMinTimeBetweenDP->addItem(PersonalProgramDefines::MinTimeBetweenDPNames.value(v), v);
    ui->cbMinTimeBetweenDP->setCurrentIndex(0);

    ui->cbMaxTimeBetweenDP->clear();
    foreach (auto v, PersonalProgramDefines::MaxTimeBetweenDPList)
        ui->cbMaxTimeBetweenDP->addItem(PersonalProgramDefines::MaxTimeBetweenDPNames.value(v), v);
    ui->cbMaxTimeBetweenDP->setCurrentIndex(7);
}

QModelIndex ActivePersonalProgramEditor::selectedDPIndex()
{
    auto selIdxs = ui->tvSchedule->selectionModel()->selectedIndexes();
    for (int i = 0; i < selIdxs.size(); ++i)
        if (selIdxs.at(i).column() == 0)
            return selIdxs.at(i);
    return QModelIndex();
}

QModelIndex ActivePersonalProgramEditor::selectedTestIndex()
{
    auto selIdxs = ui->tvTests->selectionModel()->selectedIndexes();
    for (int i = 0; i < selIdxs.size(); ++i)
        if (selIdxs.at(i).column() == 0)
            return selIdxs.at(i);
    return QModelIndex();
}
