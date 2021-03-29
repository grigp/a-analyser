#include "extend3dgamesexecute.h"
#include "ui_extend3dgamesexecute.h"

#include "driver.h"
#include "stabilan01.h"
#include "aanalyserapplication.h"

#include <QTimer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QFile>
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
    qDebug() << m_program;
    connect(m_process,
            static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
          [=](int exitCode, QProcess::ExitStatus exitStatus)
    {
        qDebug() << exitCode << exitStatus;
    });
    m_process->start(m_program);
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

