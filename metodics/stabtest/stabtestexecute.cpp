#include "stabtestexecute.h"
#include "ui_stabtestexecute.h"

#include "executewidget.h"

#include "stabilogram.h"
#include "ballistogram.h"
#include "channelsdefines.h"
#include "testresultdata.h"
#include "baseutils.h"
#include "aanalyserapplication.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

namespace
{
    const int m_frequency = 50;  ///< Частота дискретизации todo: временно, до появляения реальных драйверов
}

StabTestExecute::StabTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabTestExecute)
  , m_trd(new TestResultData())
{
    ui->setupUi(this);

    m_ti = startTimer(20);

    m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
    MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();
    m_trd->newTest(m_kard.uid, mi.uid);
}

StabTestExecute::~StabTestExecute()
{
    delete ui;
}

void StabTestExecute::setParams(const QJsonObject &params)
{
    m_params.clear();
    auto prbsArr = params["probes"].toArray();
    for (int i = 0; i < prbsArr.size(); ++i)
    {
        auto obj = prbsArr[i].toObject();
        StabTestParams::ProbeParams pp;

        pp.name = obj["name"].toString();
        pp.autoEnd = obj["autoend"].toInt() == 1;
        pp.time = obj["time"].toInt();
        pp.latentTime = obj["latent_time"].toInt();
        pp.stimulCode = obj["stimul"].toInt();
        pp.zeroingEnabled = obj["zeroing"].toInt() == 1;
        pp.scale = obj["scale"].toInt();

        m_params << pp;
    }

    ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
}

double r = 0;

void StabTestExecute::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_ti)
    {
        if (m_isRecording)
        {
            double x = 10 * sin(r);  //todo: заменить реальным чтением данных
            double y = 10 * cos(r);
            r = r + 0.1256;
            SignalsDefines::StabRec rec(std::make_tuple(x, y));
            m_stb->addValue(rec);
            m_z->addValue(x + y);

            ++m_recCount;
            if (probeParams().autoEnd)
            {
                ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_frequency) + " / " +
                                       BaseUtils::getTimeBySecCount(probeParams().time));
                double rc = m_recCount;
                double mrc = probeParams().time * m_frequency;
                ui->pbRec->setValue(rc / mrc * 100);
            }
            else
                ui->lblRecLen->setText(BaseUtils::getTimeBySecCount(m_recCount / m_frequency));

            if (probeParams().autoEnd && (m_recCount >= probeParams().time * m_frequency))
            {
                //! Следующая проба
                nextProbe();
                //! Пробы кончились - завершение
                if (m_probe >= m_params.size())
                    finishTest();
            }
        }
    }
}

void StabTestExecute::signalTest()
{
    Stabilogram stab(ChannelsDefines::chanStab, 50);

    for (int i = 0; i < 1000; ++i)
    {
        SignalsDefines::StabRec rec;
        rec.x = i;
        rec.y = 1000 - i;
        stab.addValue(rec);
    }

    QByteArray ba;
    stab.toByteArray(ba);

    qDebug() << ba;

    Stabilogram stab1(ChannelsDefines::chanStab, 50);
    stab1.fromByteArray(ba);
    for (int i = 0; i < stab1.size(); ++i)
        qDebug() << stab1.value(0, i) << stab1.value(1, i);
}

void StabTestExecute::recording()
{
    m_isRecording = ! m_isRecording;
    if (m_isRecording)
    {
        if (probeParams().autoEnd)
        {
            ui->btnRecord->setIcon(QIcon(":/images/SaveNO.png"));
            ui->btnRecord->setText("Прервать");
        }
        else
        {
            ui->btnRecord->setIcon(QIcon(":/images/SaveOK.png"));
            ui->btnRecord->setText("Завершить");
        }

        initRecSignals();
    }
    else
    {
        ui->btnRecord->setIcon(QIcon(":/images/Save.png"));
        ui->btnRecord->setText("Запись");
        if (! probeParams().autoEnd)
            finishTest();
    }
    m_recCount = 0;
}

StabTestParams::ProbeParams StabTestExecute::probeParams()
{
    if (m_probe >= 0 && m_probe < m_params.size())
        return m_params.at(m_probe);
    else
        return StabTestParams::ProbeParams();
}

void StabTestExecute::initRecSignals()
{
    ///< Запись данных в пробе
    if (m_probe < m_params.size())
    {
        m_trd->newProbe(probeParams().name);
        m_stb = new Stabilogram(ChannelsDefines::chanStab, m_frequency);
        m_trd->addSignal(m_stb);
        m_z = new Ballistogram(ChannelsDefines::chanZ, m_frequency);
        m_trd->addSignal(m_z);
    }
}

void StabTestExecute::nextProbe()
{
    ++m_probe;
    ui->lblProbeTitle->setText(probeParams().name + " - " + m_kard.fio);
    m_isRecording = false;
    m_recCount = 0;
    ui->lblRecLen->setText("00:00");
    ui->btnRecord->setIcon(QIcon(":/images/Save.png"));
    ui->btnRecord->setText("Запись");
    ui->pbRec->setValue(0);
}

void StabTestExecute::finishTest()
{
    m_trd->saveTest();
    static_cast<ExecuteWidget*>(parent())->showDB();
}
