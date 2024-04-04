#include "stabtestparamsdialog.h"
#include "ui_stabtestparamsdialog.h"

#include "aanalyserapplication.h"
#include "baseutils.h"
#include "stabtestparams.h"
#include "stabtesttemplate.h"
#include "datadefines.h"
#include "settingsprovider.h"
#include "linesparamsdialog.h"
#include "stabtestdefines.h"

#include <QJsonArray>
#include <QMessageBox>
#include <QFile>
#include <QDebug>

namespace
{

///< Параметры методики
struct MetodicParams
{
    int condition;                               ///< Условия проведения
    QList<StabTestParams::ProbeParams> probes;   ///< Пробы
};

MetodicParams metParams;


}

StabTestParamsDialog::StabTestParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StabTestParamsDialog)
  , m_mdlProbes(new QStandardItemModel(this))
{
    ui->setupUi(this);
    initUi();
    assignAccessMode();

    ui->lvProbes->setModel(m_mdlProbes);
    ui->cbStimul->addItems(QStringList()
                           << StabTestDefines::PatientWindowsName.value(StabTestDefines::pwNone)
                           << StabTestDefines::PatientWindowsName.value(StabTestDefines::pwColors)
                           << StabTestDefines::PatientWindowsName.value(StabTestDefines::pwSound)
                           << StabTestDefines::PatientWindowsName.value(StabTestDefines::pwTarget)
                           << StabTestDefines::PatientWindowsName.value(StabTestDefines::pwZones)
                           << StabTestDefines::PatientWindowsName.value(StabTestDefines::pwLines));
    ui->cbScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");
    ui->cbConditions->addItems(QStringList()
                               << tr("Анализ сигналов")                ///< Код 0
                               << tr("Стань чемпионом")                ///< Код 1
                               << tr("Допусковый контроль")            ///< Код 2
                               << tr("Уровень здоровья по ИДС")        ///< Код 3
                               << tr("Мишень")                         ///< Код 4
                               << tr("Стрессовая стратегия")           ///< Код 5
                               << tr("Постуральный контроль детей"));  ///< Код 6
    fillProbeKinds();

    m_stimulParamsEditors.insert(StabTestDefines::pwNone, nullptr);
    m_stimulParamsEditors.insert(StabTestDefines::pwColors, nullptr);
    m_stimulParamsEditors.insert(StabTestDefines::pwSound, nullptr);
    m_stimulParamsEditors.insert(StabTestDefines::pwTarget, nullptr);
    m_stimulParamsEditors.insert(StabTestDefines::pwZones, nullptr);
    m_stimulParamsEditors.insert(StabTestDefines::pwLines, new LinesParamsDialog(this));

    //! Редактирование названия пробы
    connect(m_mdlProbes, &QStandardItemModel::itemChanged, [=](QStandardItem *item)
    {
        if (item->row() >= 0 && item->row() < metParams.probes.size())
        {
            auto pp = metParams.probes.at(item->row());
            pp.name = item->data(Qt::DisplayRole).toString();
            metParams.probes.replace(item->row(), pp);
        }
    });

    connect(ui->lvProbes->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &StabTestParamsDialog::on_selectionChanged);
}

StabTestParamsDialog::~StabTestParamsDialog()
{
    delete ui;
}

void StabTestParamsDialog::setParams(const QJsonObject &params)
{
    m_mdlProbes->clear();
    metParams.probes.clear();
    metParams.condition = params["condition"].toInt();
    auto prbsArr = params["probes"].toArray();
    for (int i = 0; i < prbsArr.size(); ++i)
    {
        auto obj = prbsArr[i].toObject();
        StabTestParams::ProbeParams pp;

        pp.name = obj["name"].toString();
        pp.probeKind = obj["kind"].toInt();
        pp.autoEnd = obj["autoend"].toInt() == 1;
        pp.time = obj["time"].toInt();
        pp.latentTime = obj["latent_time"].toInt();
        pp.stimulCode = obj["stimul"].toInt();
        pp.zeroingEnabled = obj["zeroing"].toInt() == 1;
        pp.scale = obj["scale"].toInt();

        auto objSL = obj["stimul_lines"].toObject();
        pp.stimLines.direction = static_cast<BaseDefines::Directions>(objSL["direction"].toInt(BaseDefines::dirRight));
        pp.stimLines.width = objSL["width"].toInt(120);
        pp.stimLines.speed = objSL["speed"].toInt(200);
        pp.stimLines.dutyCycle = objSL["duty_cycle"].toInt(1);
        pp.stimLines.color = BaseUtils::strRGBAToColor(objSL["color"].toString("00000000"));

        metParams.probes << pp;
        m_mdlProbes->appendRow(new QStandardItem(pp.name));
    }

    ui->cbConditions->setCurrentIndex(metParams.condition);

    if (metParams.probes.size() > 0)
    {
        ui->lvProbes->selectionModel()->setCurrentIndex(m_mdlProbes->index(0, 0),
                                                        QItemSelectionModel::Select | QItemSelectionModel::Rows);
        m_curProbe = 0;
        showProbeParam();
    }
}

QJsonObject StabTestParamsDialog::getParams()
{
    QJsonObject retval;
    QJsonArray prbArray;

    foreach (auto pp, metParams.probes)
    {
        QJsonObject objP;
        objP["name"] = pp.name;
        objP["kind"] = pp.probeKind;
        objP["autoend"] = static_cast<int>(pp.autoEnd);
        objP["time"] = pp.time;
        objP["latent_time"] = pp.latentTime;
        objP["stimul"] = pp.stimulCode;
        objP["zeroing"] = static_cast<int>(pp.zeroingEnabled);
        objP["scale"] = pp.scale;

        QJsonObject objSL;
        objSL["direction"] = pp.stimLines.direction;
        objSL["width"] = pp.stimLines.width;
        objSL["speed"] = pp.stimLines.speed;
        objSL["duty_cycle"] = pp.stimLines.dutyCycle;
        objSL["color"] = BaseUtils::colorToRGBAStr(pp.stimLines.color);
        objP["stimul_lines"] = objSL;

        prbArray.append(objP);
    }
    retval["probes"] = prbArray;
    retval["condition"] = metParams.condition;
    return retval;
}

void StabTestParamsDialog::on_selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    Q_UNUSED(selected);
    Q_UNUSED(deselected);
    m_curProbe = ui->lvProbes->selectionModel()->currentIndex().row();
    showProbeParam();
}

void StabTestParamsDialog::selectProbe(const int numProbe)
{
    m_curProbe = numProbe;
    showProbeParam();
}

void StabTestParamsDialog::selectProbe(const QModelIndex index)
{
    if (index.isValid())
        selectProbe(index.row());
}

void StabTestParamsDialog::addProbe()
{
    StabTestParams::ProbeParams pp;

    pp.name = tr("Новая проба");
    pp.autoEnd = false;
    pp.time = 20;
    pp.latentTime = 0;
    pp.stimulCode = 0;
    pp.zeroingEnabled = true;
    pp.scale = 1;

    if (m_curProbe < 0)
        m_curProbe = 0;
    metParams.probes.insert(m_curProbe, pp);
    m_mdlProbes->insertRow(m_curProbe, new QStandardItem(pp.name));
}

void StabTestParamsDialog::deleteProbe()
{

    if (QMessageBox::question(this, tr("Предупреждение"),
                              QString(tr("Удалить запись о пробе '%1'?")).arg(metParams.probes.at(m_curProbe).name)) ==
            QMessageBox::Yes)
    {
        metParams.probes.removeAt(m_curProbe);
        m_mdlProbes->removeRow(m_curProbe);
        if (m_curProbe >= metParams.probes.size())
            --m_curProbe;
        if (m_curProbe >= 0)
            showProbeParam();
    }
}

void StabTestParamsDialog::changeCondition(const int condition)
{
    metParams.condition = condition;
}

void StabTestParamsDialog::changeProbeKind(const int probeKind)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.probes.size())
    {
        auto pp = metParams.probes.at(m_curProbe);
        pp.probeKind = probeKind;
        metParams.probes.replace(m_curProbe, pp);
    }
}

void StabTestParamsDialog::changeAutoEnd(const bool autoEnd)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.probes.size())
    {
        auto pp = metParams.probes.at(m_curProbe);
        pp.autoEnd = autoEnd;
        metParams.probes.replace(m_curProbe, pp);
    }
}

void StabTestParamsDialog::changeTime(const QTime &time)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.probes.size())
    {
        auto pp = metParams.probes.at(m_curProbe);
        pp.time = time.hour() * 3600 + time.minute() * 60 + time.second();
        metParams.probes.replace(m_curProbe, pp);
    }
}

void StabTestParamsDialog::changeLatentTime(const QTime &time)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.probes.size())
    {
        auto pp = metParams.probes.at(m_curProbe);
        pp.latentTime = time.hour() * 3600 + time.minute() * 60 + time.second();
        metParams.probes.replace(m_curProbe, pp);
    }
}

void StabTestParamsDialog::changeStimulIndex(const int stimul)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.probes.size())
    {
        ui->btnStimulSettings->setEnabled(m_stimulParamsEditors.value(stimul));
        auto pp = metParams.probes.at(m_curProbe);
        pp.stimulCode = stimul;
        metParams.probes.replace(m_curProbe, pp);
    }
    else
        ui->btnStimulSettings->setEnabled(false);
}

void StabTestParamsDialog::changeZeroing(const bool zeroing)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.probes.size())
    {
        auto pp = metParams.probes.at(m_curProbe);
        pp.zeroingEnabled = zeroing;
        metParams.probes.replace(m_curProbe, pp);
    }
}

void StabTestParamsDialog::changeScale(const int scale)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.probes.size())
    {
        auto pp = metParams.probes.at(m_curProbe);
        pp.scale = scale;
        metParams.probes.replace(m_curProbe, pp);
    }
}

void StabTestParamsDialog::on_editStimulParams()
{
    if (m_curProbe >= 0 && m_curProbe < metParams.probes.size())
    {
        auto pp = metParams.probes.at(m_curProbe);

        QDialog* dlg = m_stimulParamsEditors.value(ui->cbStimul->currentIndex());

        if (ui->cbStimul->currentIndex() == StabTestDefines::pwLines)
        {
            static_cast<LinesParamsDialog*>(dlg)->setDirection(pp.stimLines.direction);
            static_cast<LinesParamsDialog*>(dlg)->setWidth(pp.stimLines.width);
            static_cast<LinesParamsDialog*>(dlg)->setSpeed(pp.stimLines.speed);
            static_cast<LinesParamsDialog*>(dlg)->setDutyCycle(pp.stimLines.dutyCycle);
            static_cast<LinesParamsDialog*>(dlg)->setColor(pp.stimLines.color);
        }

        if (dlg->exec() == QDialog::Accepted)
        {
            pp.stimLines.direction = static_cast<LinesParamsDialog*>(dlg)->direction();
            pp.stimLines.width = static_cast<LinesParamsDialog*>(dlg)->width();
            pp.stimLines.speed = static_cast<LinesParamsDialog*>(dlg)->speed();
            pp.stimLines.dutyCycle = static_cast<LinesParamsDialog*>(dlg)->dutyCycle();
            pp.stimLines.color = static_cast<LinesParamsDialog*>(dlg)->color();

            metParams.probes.replace(m_curProbe, pp);
        }
    }

}

void StabTestParamsDialog::on_ok()
{
    accept();
}

void StabTestParamsDialog::on_cancel()
{
    reject();
}

void StabTestParamsDialog::on_default()
{
    auto mr = QMessageBox::question(nullptr, tr("Запрос"), tr("Сбросить настройки к настройкам методики по умолчанию?"));
    if (QMessageBox::Yes == mr)
    {
        auto mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getCurrentMetodic();
        auto params = static_cast<AAnalyserApplication*>(QApplication::instance())->setMetodicParamsDefault(mi.uid);
        setParams(params);
    }
}

void StabTestParamsDialog::showProbeParam()
{
    if (m_curProbe >= 0 && m_curProbe < metParams.probes.size())
    {
        ui->cbProbeKind->setCurrentIndex(metParams.probes.at(m_curProbe).probeKind);
        ui->cbAutoEnd->setChecked(metParams.probes.at(m_curProbe).autoEnd);
        ui->edTime->setTime(QTime(0, 0, 0, 0).addSecs(metParams.probes.at(m_curProbe).time));
        ui->edTimeLatent->setTime(QTime(0, 0, 0, 0).addSecs(metParams.probes.at(m_curProbe).latentTime));
        ui->cbStimul->setCurrentIndex(metParams.probes.at(m_curProbe).stimulCode);
        ui->cbZeroing->setChecked(metParams.probes.at(m_curProbe).zeroingEnabled);
        ui->cbScale->setCurrentIndex(metParams.probes.at(m_curProbe).scale);
    }
}

void StabTestParamsDialog::initUi()
{
    QFile style( ":/qss/another_win.qss" );
    style.open( QFile::ReadOnly );
    QString stlDetail(style.readAll() );
    setStyleSheet(stlDetail);
}

void StabTestParamsDialog::assignAccessMode()
{
    // Полный доступ: HKCU/Software/A-Med/a-analyser -> Параметр "Mode" -> значение "admin"
    auto rootMode = (SettingsProvider::valueFromRegAppCopy("", "Mode").toString() == "admin");

    ui->btnAddProbe->setVisible(rootMode);
    ui->btnDeleteProbe->setVisible(rootMode);
//    ui->frAutoEnd->setVisible(rootMode);
    ui->frStimul->setVisible(rootMode);
//    ui->frZeroing->setVisible(rootMode);
    ui->frScale->setVisible(rootMode);
    ui->cbConditions->setVisible(rootMode);
    ui->lblCondition->setVisible(rootMode);
}

void StabTestParamsDialog::fillProbeKinds()
{
    QList<StabTestParams::ProbeKinds> pkl = StabTestTemplate::probeKinds();
    foreach (auto pki, pkl)
        ui->cbProbeKind->addItem(StabTestTemplate::getProbeKindName(pki));
}

