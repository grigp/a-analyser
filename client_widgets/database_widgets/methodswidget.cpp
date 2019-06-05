#include "methodswidget.h"
#include "ui_methodswidget.h"

#include "metodicdefines.h"
#include "metodicsmodel.h"
#include "aanalyserapplication.h"

#include "channelsdefines.h"
#include "channelsutils.h"

#include <QApplication>
#include <QDebug>

MethodsWidget::MethodsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MethodsWidget)
  , m_model(new MetodicsModel(this))
{
    ui->setupUi(this);
}

MethodsWidget::~MethodsWidget()
{
    delete ui;
}

void MethodsWidget::onDbConnect()
{
    if (m_model)
    {
        m_model->load();
        ui->tvMetods->setModel(m_model);
        ui->tvMetods->viewport()->installEventFilter(this);
    }
}

bool MethodsWidget::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->tvMetods->viewport())
    {
        if (event->type() == QEvent::Paint)
        {
            // Приводит к частым срабатываниям
            selectMetodic(ui->tvMetods->selectionModel()->currentIndex());
        }
    }
    return false;
}

void MethodsWidget::selectMetodic(const QModelIndex index)
{
    if (index.isValid())
    {
        auto uid = m_model->index(index.row(), MetodicsModel::ColName, index.parent()).
                data(MetodicsModel::MetodicUidRole).toString();
        static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic(uid);
    }
}

void MethodsWidget::editMetodParams()
{
    QModelIndexList selIdxs = ui->tvMetods->selectionModel()->selectedIndexes();
    if (m_model && selIdxs.size() > 0)
        m_model->editMetodicParams(this, selIdxs.at(0).row());
}

void MethodsWidget::unselect()
{
    ui->tvMetods->selectionModel()->clear();
    static_cast<AAnalyserApplication*>(QApplication::instance())->doSelectMetodic("");
}
