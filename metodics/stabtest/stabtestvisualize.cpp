#include "stabtestvisualize.h"
#include "ui_stabtestvisualize.h"

#include "datadefines.h"
#include "dataprovider.h"

#include "channelsdefines.h"
#include "stabilogram.h"

#include <QDebug>

StabTestVisualize::StabTestVisualize(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StabTestVisualize)
{
    ui->setupUi(this);
}

StabTestVisualize::~StabTestVisualize()
{
    delete ui;
}

void StabTestVisualize::setTest(const QString &testUid)
{
    DataDefines::TestInfo ti;
    if (DataProvider::getTestInfo(testUid, ti))
    {
        for (int i = 0; i < ti.probes.size(); ++i)
        {
            QTextEdit *edit = new QTextEdit(ui->wgtMain);
            ui->wgtMain->layout()->addWidget(edit);
            showProbeResult(i, ti.probes.at(i), edit);
        }
    }
}

void StabTestVisualize::showProbeResult(const int num, const QString uid, QTextEdit *edit)
{
    Q_UNUSED(num);
    QByteArray baStab;
    if (DataProvider::getChannel(uid, ChannelsDefines::chanStab, baStab))
    {
        Stabilogram stab(baStab);
        for (int i = 0; i < stab.size(); ++i)
        {
            auto rec = stab.value(i);
            auto sx = QString::number(rec.x);
            auto sy = QString::number(rec.y);
            edit->append(sx + "     " + sy);
        }
    }
}
