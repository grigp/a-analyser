#include "extend3dgamesexecute.h"
#include "ui_extend3dgamesexecute.h"

#include "driver.h"
#include "stabilan01.h"
#include "aanalyserapplication.h"
#include "executewidget.h"
#include "testresultdata.h"
#include "trenresultdata.h"
#include "trenresultfactors.h"

#include <QTimer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QFile>
#include <QLocale>
#include <QDebug>

Extend3DGamesExecute::Extend3DGamesExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Extend3DGamesExecute)
{
    ui->setupUi(this);
}

Extend3DGamesExecute::~Extend3DGamesExecute()
{
    delete ui;
}

void Extend3DGamesExecute::run(const QJsonObject &params)
{
    m_program = params["program"].toString();
    m_paramStr = params["params"].toString();

    QTimer::singleShot(0, [=]()
    {
        programExecute();
    });
}

void Extend3DGamesExecute::programExecute()
{
    saveDriverParams();
    savePatientData();
    saveGameParams();

    m_process = new QProcess(this);
    connect(m_process,
            static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
          [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        Q_UNUSED(exitStatus);
        if (exitCode == 0)
        {
            getGameResult();
        }
        else
            static_cast<ExecuteWidget*>(parent())->showDB();
    });
    m_process->start("\"" + m_program + "\"");
}

void Extend3DGamesExecute::saveDriverParams() const
{
    QJsonObject params;
    Driver* driver = static_cast<AAnalyserApplication*>(QApplication::instance())->
            getDriverByFormats(QStringList() << ChannelsDefines::cfDecartCoordinates);
    int p = driver->portName();
    params["port"] = p;
    if (driver->driverUid() == uid_stabilan01)
    {
        Stabilan01* stbDriver = static_cast<Stabilan01*>(driver);
        QJsonArray td;
        QJsonObject t1;
        t1["device"] = stbDriver->tenso1Params().device;
        t1["rkp"] = stbDriver->tenso1Params().rkp;
        t1["pn"] = stbDriver->tenso1Params().pn;
        td.append(t1);
        QJsonObject t2;
        t2["device"] = stbDriver->tenso2Params().device;
        t2["rkp"] = stbDriver->tenso2Params().rkp;
        t2["pn"] = stbDriver->tenso2Params().pn;
        td.append(t2);
        QJsonObject t3;
        t3["device"] = stbDriver->tenso3Params().device;
        t3["rkp"] = stbDriver->tenso3Params().rkp;
        t3["pn"] = stbDriver->tenso3Params().pn;
        td.append(t3);
        params["tensoDevices"] = td;
    }

    QDir dir = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/A-Med/Shared/";
    QFile file(dir.absoluteFilePath("connection.json"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument doc(params);
        QByteArray ba = doc.toJson();
        file.write(ba);
        file.close();
    }
}

void Extend3DGamesExecute::savePatientData() const
{
    QDir dir = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/A-Med/Shared/";
    QFile file(dir.absoluteFilePath("patient.json"));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject patient;
        DataDefines::PatientKard kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
        patient["fio"] = kard.fio;
        patient["born"] = kard.born.toString("dd.MM.yyyy");
        QJsonDocument doc(patient);
        QByteArray ba = doc.toJson();
        file.write(ba);
        file.close();
    }
}

void Extend3DGamesExecute::saveGameParams() const
{
    if (m_paramStr != "")
    {
        QDir dir = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/A-Med/Shared/";
        QFile file(dir.absoluteFilePath("3D_games_params.txt"));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            file.write(QString(m_paramStr + "\n\n").toStdString().c_str());
            file.close();
        }
    }
}

void Extend3DGamesExecute::getGameResult() const
{
    QString fn = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/A-Med/Shared/3d_games_result.txt";
    if (QFile::exists(fn))
    {
        QList<GameResultsFactorInfo> fl = getResultsFactors(fn);

        DataDefines::PatientKard m_kard = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedPatient();
        MetodicDefines::MetodicInfo mi = static_cast<AAnalyserApplication*>(QApplication::instance())->getSelectedMetodic();

        TestResultData *m_trd = new TestResultData();
        m_trd->newTest(m_kard.uid, mi.uid);
        m_trd->newProbe(mi.name);
        auto trenRes = new TrenResultData(ChannelsDefines::chanTrenResult);
        foreach (auto factor, fl)
        {
            trenRes->addFactor(factor.uid, factor.value);
        }
        m_trd->addChannel(trenRes);

        m_trd->saveTest();
    }

    static_cast<ExecuteWidget*>(parent())->showDB();
}

QList<GameResultsFactorInfo> Extend3DGamesExecute::getResultsFactors(const QString fn) const
{
    QList<GameResultsFactorInfo> retval;

    QFile file(fn);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return retval;

    enum Mode
    {
        mdSearch,
        mdFactors,
        mdValues
    };

    Mode mode = mdSearch;

    QTextStream ts(&file);
    while (!ts.atEnd())
    {
        QString line = ts.readLine();
        if (mode == mdSearch)
        {
            if (line == "factors|")
                mode = mdFactors;
        }
        else
        if (mode == mdFactors)
        {
            if (line == "tests|")
                mode = mdValues;
            else
            {
                auto ls = line.split('|');
                if (ls.size() == 2)
                    if (ls.at(0) == "uid")
                    {
                        GameResultsFactorInfo fi;
                        fi.uid = getAAnalyserUIDByGamesUid(ls.at(1));
                        fi.value = 0;
                        retval.append(fi);
                    }
            }
        }
        else
        if (mode == mdValues)
        {
            auto ls = line.split('|');
            if (ls.at(0) == "values")
                for (int i = 1; i < ls.size(); ++i)
                {
                    auto fv = ls.at(i).split('=');
                    if (fv.size() == 2)
                        for (int i = 0; i < retval.size(); ++i)
                            if (getAAnalyserUIDByGamesUid(fv.at(0)) == retval.at(i).uid)
                            {
                                GameResultsFactorInfo fi = retval.at(i);
                                fi.value = stringToDouble(fv.at(1));
                                if (fv.at(0) == "game_time")
                                    fi.value = round(fi.value);
                                retval.replace(i, fi);
                            }
                }
        }
    }

    return retval;
}

QString Extend3DGamesExecute::getAAnalyserUIDByGamesUid(const QString fUID) const
{
    QString retval;
    if (fUID == "game_score")
        retval = TrenResultFactorsDefines::ScoreUid;
    else
    if (fUID == "game_time")
        retval = TrenResultFactorsDefines::TimeUid;
    return retval;
}

double Extend3DGamesExecute::stringToDouble(const QString strVal) const
{
    return QLocale::system().toDouble(strVal);
}

