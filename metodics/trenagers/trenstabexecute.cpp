#include "trenstabexecute.h"
#include "ui_trenstabexecute.h"

#include <QLayout>

TrenStabExecute::TrenStabExecute(QWidget *parent) :
    TrenExecute(parent),
    ui(new Ui::TrenStabExecute)
{
    ui->setupUi(this);
}

TrenStabExecute::~TrenStabExecute()
{
    delete ui;
}

void TrenStabExecute::fillGameControl(QFrame *frame)
{
    m_btnZeroing = new QPushButton(frame);
    m_btnZeroing->setText(tr("Центровка"));
    frame->layout()->addWidget(m_btnZeroing);

}

void TrenStabExecute::on_zeroing()
{
//    auto chanUid = ui->cbSelectChannel->currentData(ChannelsUtils::ChannelUidRole).toString();
//    if (m_dcControl && chanUid != "")
//        m_dcControl->zeroing(chanUid);
}
