#include "areagraph.h"
#include "ui_areagraph.h"

AreaGraph::AreaGraph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AreaGraph)
{
    ui->setupUi(this);
}

AreaGraph::~AreaGraph()
{
    delete ui;
}
