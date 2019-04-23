#include "stabtestparamsdialog.h"
#include "ui_stabtestparamsdialog.h"

#include <QJsonArray>
#include <QMessageBox>
#include <QDebug>

namespace
{
/*!
 * \brief Параметры пробы в тесте ProbeParams struct
 */
struct ProbeParams
{
    QString name;          ///< Название пробы
    int time;              ///< Длительность
    int latentTime;        ///< Задержка привыкания
    int stimulCode;        ///< Код видеостимуляции 0 - нет, 1 - цветные круги, 2 - звуковые сигналы, 3 - мишень
    bool zeroingEnabled;   ///< Разрешена ли центровка
    int scale;             ///< Номер масштаба 0 - 1, 1 - 2, 2 - 4, 3 - 8, 4 - 16, 5 - 32, 6 - 64, 7 -128
    ProbeParams() {}
};

QList<ProbeParams> metParams;  ///< Параметры методики

}

StabTestParamsDialog::StabTestParamsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StabTestParamsDialog)
  , m_mdlProbes(new QStandardItemModel(this))
{
    ui->setupUi(this);
    ui->lvProbes->setModel(m_mdlProbes);
    ui->cbStimul->addItems(QStringList() << "нет" << "Открытые глаза" << "Закрытые глаза" << "Мишень");
    ui->cbScale->addItems(QStringList() << "1" << "2" << "4" << "8" << "16" << "32" << "64" << "128");

    ui->lvProbes->viewport()->installEventFilter(this);
}

StabTestParamsDialog::~StabTestParamsDialog()
{
    delete ui;
}

void StabTestParamsDialog::setParams(const QJsonObject &params)
{
    m_mdlProbes->clear();
    metParams.clear();
    auto prbsArr = params["probes"].toArray();
    for (int i = 0; i < prbsArr.size(); ++i)
    {
        auto obj = prbsArr[i].toObject();
        ProbeParams pp;

        pp.name = obj["name"].toString();
        pp.time = obj["time"].toInt();
        pp.latentTime = obj["latent_time"].toInt();
        pp.stimulCode = obj["stimul"].toInt();
        pp.zeroingEnabled = obj["zeroing"].toInt() == 1;
        pp.scale = obj["scale"].toInt();

        metParams << pp;
        m_mdlProbes->appendRow(new QStandardItem(pp.name));
    }

    if (metParams.size() > 0)
    {
        ui->lvProbes->selectionModel()->select(m_mdlProbes->index(0, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
        m_curProbe = 0;
        showProbeParam();
    }
}

QJsonObject StabTestParamsDialog::getParams()
{
    QJsonObject retval;
    QJsonArray prbArray;

    foreach (auto pp, metParams)
    {
        QJsonObject objP;
        objP["name"] = pp.name;
        objP["time"] = pp.time;
        objP["latent_time"] = pp.latentTime;
        objP["stimul"] = pp.stimulCode;
        objP["zeroing"] = static_cast<int>(pp.zeroingEnabled);
        objP["scale"] = pp.scale;
        prbArray.append(objP);
    }
    retval["probes"] = prbArray;
    return retval;
}

bool StabTestParamsDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lvProbes->viewport())
    {
        if (event->type() == QEvent::Paint)
        {
            m_curProbe = ui->lvProbes->selectionModel()->currentIndex().row();
            showProbeParam();

        }
    }
    return false;
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
    ProbeParams pp;

    pp.name = "Новая проба";
    pp.time = 20;
    pp.latentTime = 0;
    pp.stimulCode = 0;
    pp.zeroingEnabled = true;
    pp.scale = 1;

    if (m_curProbe < 0)
        m_curProbe = 0;
    metParams.insert(m_curProbe, pp);
    m_mdlProbes->insertRow(m_curProbe, new QStandardItem(pp.name));
}

void StabTestParamsDialog::deleteProbe()
{

    if (QMessageBox::question(this, tr("Предупреждение"),
                              QString(tr("Удалить запись о пробе '%1'?")).arg(metParams.at(m_curProbe).name)) ==
            QMessageBox::Yes)
    {
        metParams.removeAt(m_curProbe);
        m_mdlProbes->removeRow(m_curProbe);
        if (m_curProbe >= metParams.size())
            --m_curProbe;
        if (m_curProbe >= 0)
            showProbeParam();
    }
}

void StabTestParamsDialog::changeTime(const QTime &time)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.size())
    {
        auto pp = metParams.at(m_curProbe);
        pp.time = time.hour() * 3600 + time.minute() * 60 + time.second();
        metParams.replace(m_curProbe, pp);
    }
}

void StabTestParamsDialog::changeLatentTime(const QTime &time)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.size())
    {
        auto pp = metParams.at(m_curProbe);
        pp.latentTime = time.hour() * 3600 + time.minute() * 60 + time.second();
        metParams.replace(m_curProbe, pp);
    }
}

void StabTestParamsDialog::changeStimulIndex(const int stimul)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.size())
    {
        auto pp = metParams.at(m_curProbe);
        pp.stimulCode = stimul;
        metParams.replace(m_curProbe, pp);
    }
}

void StabTestParamsDialog::changeZeroing(const bool zeroing)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.size())
    {
        auto pp = metParams.at(m_curProbe);
        pp.zeroingEnabled = zeroing;
        metParams.replace(m_curProbe, pp);
    }
}

void StabTestParamsDialog::changeScale(const int scale)
{
    if (m_curProbe >= 0 && m_curProbe < metParams.size())
    {
        auto pp = metParams.at(m_curProbe);
        pp.scale = scale;
        metParams.replace(m_curProbe, pp);
    }
}

void StabTestParamsDialog::showProbeParam()
{
    if (m_curProbe >= 0 && m_curProbe < metParams.size())
    {
        ui->edTime->setTime(QTime(0, 0, 0, 0).addSecs(metParams.at(m_curProbe).time));
        ui->edTimeLatent->setTime(QTime(0, 0, 0, 0).addSecs(metParams.at(m_curProbe).latentTime));
        ui->cbStimul->setCurrentIndex(metParams.at(m_curProbe).stimulCode);
        ui->cbZeroing->setChecked(metParams.at(m_curProbe).zeroingEnabled);
        ui->cbScale->setCurrentIndex(metParams.at(m_curProbe).scale);
    }
}
