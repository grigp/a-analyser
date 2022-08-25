#include "sectiongraphvisualwidget.h"
#include "ui_sectiongraphvisualwidget.h"

SectionGraphVisualWidget::SectionGraphVisualWidget(VisualDescriptor* visual,
                                                   const QString& testUid,
                                                   const QString& probeUid,
                                                   const QString& channelId,
                                                   const QString& sectionNumber,
                                                   QWidget *parent) :
    SectionVisual(visual, testUid, probeUid, channelId, sectionNumber, parent),
    ui(new Ui::SectionGraphVisualWidget)
{
    ui->setupUi(this);
}

SectionGraphVisualWidget::~SectionGraphVisualWidget()
{
    delete ui;
}

bool SectionGraphVisualWidget::isValid()
{
    return true;
}

void SectionGraphVisualWidget::calculate()
{

}
