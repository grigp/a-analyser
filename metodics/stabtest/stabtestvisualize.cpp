#include "stabtestvisualize.h"
#include "ui_stabtestvisualize.h"

#include "datadefines.h"
#include "dataprovider.h"

#include "channelsdefines.h"
#include "stabilogram.h"
#include "ballistogram.h"

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
    QByteArray baZ;
    if (DataProvider::getChannel(uid, ChannelsDefines::chanStab, baStab))
    {
        auto isZ = DataProvider::getChannel(uid, ChannelsDefines::chanZ, baZ);
        Ballistogram bsg(baZ);

        Stabilogram stab(baStab);
        for (int i = 0; i < stab.size(); ++i)
        {
            auto rec = stab.value(i);
            auto sx = QString::number(rec.x);
            auto sy = QString::number(rec.y);
            QString line = sx + "     " + sy;

            if (isZ && i < baZ.size())
            {
                auto sz = QString::number(bsg.value(i));
                line = line + "     " + sz;
            }

            edit->append(line);
        }
    }
}
