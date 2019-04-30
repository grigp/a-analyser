#include "stabtestexecute.h"
#include "ui_stabtestexecute.h"

#include "executewidget.h"

#include "stabilogram.h"
#include "channelsdefines.h"

#include <QDebug>

StabTestExecute::StabTestExecute(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabTestExecute)
{
    ui->setupUi(this);
}

StabTestExecute::~StabTestExecute()
{
    delete ui;
}

void StabTestExecute::setParams(const QJsonObject &params)
{

}

void StabTestExecute::showDataBase()
{
    static_cast<ExecuteWidget*>(parent())->showDB();
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
